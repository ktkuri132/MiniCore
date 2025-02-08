//
// Created by 34575 on 25-2-5.
//
#include <stdio.h>
#include <W25Q64/spiFlash.h>
void Test()
{

	W25Q64_Init();

	uint8_t MID;
	uint16_t DID;
	uint8_t DataArry[5] = {0x78,0x59,0x12,0x34,0x56};
	uint8_t Data[5];
	// W25Q64_ReadID(&MID, &DID);
	// // W25Q64_ReadData(0x00000000,&MID,1);
	// printf("MID:%d,DID:%d\n", MID, DID);

	// // W25Q64_PageProgram(0x00000000,DataArry,1);
	// W25Q64_ReadData(0x00000000, &MID, 1);
	// printf("MID:%d,DID:%d\n", MID, DID);

	// W25Q64_SectorErase(0x00000000);

	// W25Q64_ReadData(0x00000000, &DID, 1);
	// printf("MID:%d,DID:%d\n", MID, DID);
	W25Q64_WriteData(0x00000000, DataArry, 5);
	W25Q64_ReadData(0x00000000, Data, 5);
	printf("Data:%x,%x,%x,%x,%x\n", Data[0],Data[1],Data[2],Data[3],Data[4]);
	while (1)
	{
	}
}
