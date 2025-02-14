//
// Created by 34575 on 25-2-5.
//

/*
        在stm23上进行SD卡的读写,以及文件系统的移植
                1. 移植SD卡驱动
                2. 移植文件系统
        创建文件,生成文件,写入文件,读取文件,创建设备树


*/
#include <BSP/usart/Serial.h>
#include <W25Q64/spiFlash.h>
#include <stdio.h>

#define INRAM __attribute__((section(".ramfunc")))

void INRAM test_func() {
    while (1) {
        printf("test_func addr:%x\n", test_func);
    }

    // __ASM volatile ("");
}

void Test() {
    W25Q64_Init();

    Stde_DataTypeDef Test = {.Res_note = 0

    };
    uint8_t MID;
    uint16_t DID;
    uint8_t DataArry[5] = {0x78, 0x59, 0x12, 0x34, 0x56};
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
    // printf("\033[2J\033[H");
    // W25Q64_WriteData(0x00000000, DataArry, 5);
    // W25Q64_ReadData(0x00000000, Data, 5);
    // printf("Data:%x,%x,%x,%x,%x,%x,%x,%x\n",
    // Data[0],Data[1],Data[2],Data[3],Data[4],Data[5],Data[6],Data[7]);
    // __ASM volatile ("ldr pc, =0x20000000");
    test_func();
    while (1) {
        // test_func();
        bsp_systick_delay_ms(1000);
    }
}
