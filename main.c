
#ifdef __cplusplus
extern "C"
{
#endif

#include <sys.h>
#include "usart/Serial.h"
#include "usart/usart.h"
#include "OLED/OLED.h"
#include "MPU6050/inv_mpu.h"
#include <W25Q64/spiFlash.h>

	// 定义串口数据结构
	Stde_DataTypeDef USART2_DataBuff;
	Stde_DataTypeDef USART3_DataBuff;
	Stde_DataTypeDef UART5_DataBuff;
	Stde_DataTypeDef UART4_DataBuff;

	extern float pitch, roll, yaw;

	void Test();
	int main();
	/// @brief 主函数运行完了自动复位
	void BSP_Init()
	{
	Init_BSP: // 初始化基本外设
		bsp_usart_1_inti(115200);

		// main();
		Test();
	}

	int main()
	{
	}

	void TIM3_IRQHandler()
	{
		if ((TIM3->SR & TIM_SR_UIF) && (TIM3->DIER & TIM_DIER_UIE))
		{
			TIM3->SR &= ~TIM_SR_UIF; // 清除中断标志
		}
	}

	void USART2_IRQHandler()
	{
		if (USART2->SR & USART_SR_RXNE)
		{
		}
	}

	void USART3_IRQHandler()
	{
		if (USART3->SR & USART_SR_RXNE)
		{
		}
	}

	void UART4_IRQHandler()
	{
		if (UART4->SR & USART_SR_RXNE)
		{
		}
	}

	void UART5_IRQHandler()
	{
		if (UART5->SR & USART_SR_RXNE)
		{
		}
	}

#ifdef __cplusplus
}
#endif
