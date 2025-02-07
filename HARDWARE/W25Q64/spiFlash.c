#include <stm32f4xx.h> // Device header
#include <gpio/stm32f4xx_gpio.h>
#include "spiFlash.h"
#include <stdlib.h>

/**
 * 函    数：W25Q64初始化
 * 参    数：无
 * 返 回 值：无
 */
void W25Q64_Init(void)
{
	Soft_SPI_Init(); // 先初始化基本的SPI引脚
}

/**
 * 函    数：W25Q64读取ID号
 * 参    数：MID 工厂ID，使用输出参数的形式返回
 * 参    数：DID 设备ID，使用输出参数的形式返回
 * 返 回 值：无
 */
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	Soft_SPI_Start();							  // SPI起始
	Soft_SPI_SwapByte(W25Q64_JEDEC_ID);			  // 交换发送读取ID的指令
	*MID = Soft_SPI_SwapByte(W25Q64_DUMMY_BYTE);  // 交换接收MID，通过输出参数返回
	*DID = Soft_SPI_SwapByte(W25Q64_DUMMY_BYTE);  // 交换接收DID高8位
	*DID <<= 8;									  // 高8位移到高位
	*DID |= Soft_SPI_SwapByte(W25Q64_DUMMY_BYTE); // 或上交换接收DID的低8位，通过输出参数返回
	Soft_SPI_Stop();							  // SPI终止
}

/**
 * 函    数：W25Q64写使能
 * 参    数：无
 * 返 回 值：无
 */
void W25Q64_WriteEnable(void)
{
	Soft_SPI_Start();						// SPI起始
	Soft_SPI_SwapByte(W25Q64_WRITE_ENABLE); // 交换发送写使能的指令
	Soft_SPI_Stop();						// SPI终止
}

/**
 * 函    数：W25Q64等待忙
 * 参    数：无
 * 返 回 值：无
 */
void W25Q64_WaitBusy(void)
{
	uint32_t Timeout;
	Soft_SPI_Start();											  // SPI起始
	Soft_SPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);			  // 交换发送读状态寄存器1的指令
	Timeout = 100000;											  // 给定超时计数时间
	while ((Soft_SPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01) // 循环等待忙标志位
	{
		Timeout--;		  // 等待时，计数值自减
		if (Timeout == 0) // 自减到0后，等待超时
		{
			/*超时的错误处理代码，可以添加到此处*/
			break; // 跳出等待，不等了
		}
	}
	Soft_SPI_Stop(); // SPI终止
}

/**
 * 函    数：W25Q64页编程
 * 参    数：Address 页编程的起始地址，范围：0x000000~0x7FFFFF
 * 参    数：DataArray	用于写入数据的数组
 * 参    数：Count 要写入数据的数量，范围：0~256
 * 返 回 值：无
 * 注意事项：写入的地址范围不能跨页
 */
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;

	W25Q64_WriteEnable(); // 写使能

	Soft_SPI_Start();						// SPI起始
	Soft_SPI_SwapByte(W25Q64_PAGE_PROGRAM); // 交换发送页编程的指令
	Soft_SPI_SwapByte(Address >> 16);		// 交换发送地址23~16位
	Soft_SPI_SwapByte(Address >> 8);		// 交换发送地址15~8位
	Soft_SPI_SwapByte(Address);				// 交换发送地址7~0位
	for (i = 0; i < Count; i++)				// 循环Count次
	{
		Soft_SPI_SwapByte(DataArray[i]); // 依次在起始地址后写入数据
	}
	Soft_SPI_Stop(); // SPI终止

	W25Q64_WaitBusy(); // 等待忙
}

/**
 * 函    数：W25Q64扇区擦除（4KB）
 * 参    数：Address 指定扇区的地址，范围：0x000000~0x7FFFFF
 * 返 回 值：无
 */
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable(); // 写使能

	Soft_SPI_Start();							// SPI起始
	Soft_SPI_SwapByte(W25Q64_SECTOR_ERASE_4KB); // 交换发送扇区擦除的指令
	Soft_SPI_SwapByte(Address >> 16);			// 交换发送地址23~16位
	Soft_SPI_SwapByte(Address >> 8);			// 交换发送地址15~8位
	Soft_SPI_SwapByte(Address);					// 交换发送地址7~0位
	Soft_SPI_Stop();							// SPI终止

	W25Q64_WaitBusy(); // 等待忙
}

/// @brief 扇区编程
/// @param Address 扇区的起始地址
/// @param DataArray 4KB的数组
void W25Q64_SectorProgram(uint32_t Address, uint8_t *DataArray)
{
	for (uint32_t i = 0; i < 4 * 1024; i += 256)
	{
		W25Q64_PageProgram(Address + i, DataArray + i, 256);
	}
}

/**
 * 函    数：W25Q64读取数据
 * 参    数：Address 读取数据的起始地址，范围：0x000000~0x7FFFFF
 * 参    数：DataArray 用于接收读取数据的数组，通过输出参数返回
 * 参    数：Count 要读取数据的数量，范围：0~0x800000
 * 返 回 值：无
 */
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	Soft_SPI_Start();					 // SPI起始
	Soft_SPI_SwapByte(W25Q64_READ_DATA); // 交换发送读取数据的指令
	Soft_SPI_SwapByte(Address >> 16);	 // 交换发送地址23~16位
	Soft_SPI_SwapByte(Address >> 8);	 // 交换发送地址15~8位
	Soft_SPI_SwapByte(Address);			 // 交换发送地址7~0位
	for (i = 0; i < Count; i++)			 // 循环Count次
	{
		DataArray[i] = Soft_SPI_SwapByte(W25Q64_DUMMY_BYTE); // 依次在起始地址后读取数据
	}
	Soft_SPI_Stop(); // SPI终止
}

/// @brief 得到扇区的起始地址
/// @param Address
static uint32_t *GetSectorStartAddress(uint32_t Address, uint32_t Count)
{
	uint32_t *StartAddressArry = (uint32_t *)malloc(256); // 最大跨越256个扇区的起始地址
	const uint32_t SectorSize = 4 * 1024;				  // 每个扇4KB
	uint32_t crossing = 0;								  // 跨越的扇区数量
	uint32_t i = 0;
AddrCheck:
	for (i; i <= 0x7FFFFF; i += SectorSize) // 从i开始循环遍历所有扇区
	{
		if ((Address >= i) && (Address < i + SectorSize))
		{
			if (crossing)
			{
				if (StartAddressArry[crossing - 1] == i)	// 检查是否已经存在
				{
					goto Out;	// 相同,说明写入长度并未超过一个扇区大小
				}
				else
				{
					uint32_t j = i - StartAddressArry[crossing - 1];
					j = j / 4*1024;
					while(j--)
					{
						StartAddressArry[crossing] = StartAddressArry[crossing - 1] + 4*1024;
						crossing++;
					}
					goto Out;
				}
			}
			else
			{
				StartAddressArry[crossing] = i;
				crossing++;
			}
		}
	}
	Address += Count;
	goto AddrCheck;
Out:
	return StartAddressArry;
}

void W25Q64_WriteData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint8_t *W25Q64_Sector_Cache = (uint8_t *)malloc(4 * 1024);
	uint32_t StartAddress[] 
	= GetSectorStartAddress(Address, Count);
	W25Q64_ReadData(StartAddress, W25Q64_Sector_Cache, 4 * 1024);
	uint32_t OffsetAddress = Address - StartAddress;
	for (uint32_t i = 0; i < Count; i++)
	{
		W25Q64_Sector_Cache[OffsetAddress + i] = DataArray[i];
	}
	W25Q64_SectorErase(StartAddress);
	W25Q64_SectorProgram(StartAddress, W25Q64_Sector_Cache);
	free(W25Q64_Sector_Cache);
	free(StartAddress);
}

/// @brief 覆盖写入数据
/// @param Address 起始地址
/// @param DataArray 数据数组
/// @param Count 大小
void W25Q64_OverWriteData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	static uint8_t Temp[256];
	W25Q64_ReadData(Address, Temp, 1);
	if (Temp[0] == 0xff)
	{
		W25Q64_SectorErase(Address);
	}
}