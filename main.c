/**
 ******************************************************************************
 * @file    main.c
 * @author  Nazim SAIBI
 * @version V1.0
 * @date    20-January-2018
 * @brief   Bit Banding
 ******************************************************************************
 */


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

/*
 * See programming manual :
 * bit_word_offset = (byte_offset x 32) + (bit_number x 4)
 * bit_word_addr = bit_band_base + bit_word_offset
 */

/*
 * GPIOD
 * 0x4002 0C00 - 0x4002 0FFF (ref manual, page 65)
 */
#define GPIOD_IN_MEMORY_MAP 0x40020C00

// GPIOx_BSRR - page 284 (ref manual)
#define OFFSET_ODR 0x14

//GPIO register map

/* Bit band PERIPHERAL definitions */
#define BITBANDING_PERI_REF   0x40000000
#define BITBANDING_PERI_BASE  0x42000000

/* set */
#define BITBANDING_PERI(a,b) ((BITBANDING_PERI_BASE + ((a-BITBANDING_PERI_REF)<<5) + (b<<2)))  // Convert PERI address

#define PERI_ClearBit(a,b)      (*(uint32_t *) (BITBANDING_PERI(a,b)) = 0)
#define PERI_SetBit(a,b)        (*(uint32_t *) (BITBANDING_PERI(a,b)) = 1)
#define PERI_GetBit(a,b)        (*(uint32_t *) (BITBANDING_PERI(a,b)))

/* Peripheral address
 * should be in range: 0x40000000 - 0x40100000
 */

// GPIOD Control Register
#define PERI_ADDRESS    GPIOD_IN_MEMORY_MAP+OFFSET_ODR

//bit 12 for set
#define PERI_BIT        12

int main(void)
{
    HAL_Init();
    
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    /* Enable the GPIO_LED Clock */
    LEDx_GPIO_CLK_ENABLE(LED3);
    
    /* Configure the GPIO_LED pin */
    GPIO_InitStruct.Pin = LED3_PIN | LED4_PIN | LED5_PIN | LED6_PIN ;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    //*(uint32_t *) (((0x42000000 + ((0x40020C18-0x40000000)*32) + (12*4)))) = 1;
    uint32_t var;
    for(;;)
    {
        PERI_SetBit(PERI_ADDRESS,PERI_BIT);
        HAL_Delay(200);
        var = PERI_GetBit(PERI_ADDRESS,PERI_BIT);
        
        PERI_ClearBit(PERI_ADDRESS,PERI_BIT);
        HAL_Delay(200);
        var = PERI_GetBit(PERI_ADDRESS,PERI_BIT);
        
    }
}

