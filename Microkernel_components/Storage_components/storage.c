
#include "storage.h"

#include <W25Q64/spiFlash.h>
/// @brief 存储初始化
void storage_init()
{
    #ifdef SPIFLASH_H
    // 初始化flash
    uint8_t MID;
    uint16_t  DID;
    
    W25Q64_Init();
    W25Q64_ReadID(&MID, &DID);
    if(MID&&DID)
    {
        printf("Flash Init successful\n");
    }
    #endif
}


