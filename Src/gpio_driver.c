#define STM32F405xx
#include "stm32f4xx.h"
#include "gpio_driver.h"


void TFT_ctrl_gpio_init(void)
{
    // 1. Habilitar reloj de GPIOB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    /* 2. MODER: PB6, PB7, PB8 como OUTPUT (01)
       PB6 -> bits [13:12]
       PB7 -> bits [15:14]
       PB8 -> bits [17:16]
    */
    GPIOB->MODER &= ~0x0003F000U;   // limpia PB6–PB8
    GPIOB->MODER |=  0x00015000U;   // OUTPUT en PB6,7,8

    /* 3. OTYPER: push-pull (0) */
    GPIOB->OTYPER &= ~0x000001C0U;  // bits 6,7,8 = 0

    /* 4. OSPEEDR: alta velocidad (11) */
    GPIOB->OSPEEDR |=  0x0003F000U; // PB6–PB8 high speed

    /* 5. PUPDR: sin pull-up / pull-down (00) */
    GPIOB->PUPDR &= ~0x0003F000U;

    /* 6. Estado inicial seguro */
    GPIOB->BSRR =  (1U << 6);              // CS = 1 (inactivo)
    GPIOB->BSRR =  (1U << 7);              // DC = 1 (data por defecto)
    GPIOB->BSRR =  (1U << 8);              // RST = 1 (no reset)
}
void GPIOB_Init_PB12_13_14_Output(void)
{
    /* 1) Habilitar reloj de GPIOB (AHB1) */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    (void)RCC->AHB1ENR;   // lectura dummy para asegurar el habilitado

    /* 2) Modo: Output (01) para PB12, PB13, PB14 */
    // Limpiar bits MODER: (2 bits por pin)
    GPIOB->MODER &= ~((3U << (12U*2U)) |
                      (3U << (13U*2U)) |
                      (3U << (14U*2U)));
    // Poner 01
    GPIOB->MODER |=  ((1U << (12U*2U)) |
                      (1U << (13U*2U)) |
                      (1U << (14U*2U)));

    /* 3) Tipo de salida: Push-Pull (0) */
    GPIOB->OTYPER &= ~((1U << 12U) |
                       (1U << 13U) |
                       (1U << 14U));

    /* 4) Velocidad: Medium (01) (ajusta si quieres High/VeryHigh) */
    GPIOB->OSPEEDR &= ~((3U << (12U*2U)) |
                        (3U << (13U*2U)) |
                        (3U << (14U*2U)));
    GPIOB->OSPEEDR |=  ((1U << (12U*2U)) |
                        (1U << (13U*2U)) |
                        (1U << (14U*2U)));

    /* 5) Pull-up/Pull-down: None (00) */
    GPIOB->PUPDR &= ~((3U << (12U*2U)) |
                      (3U << (13U*2U)) |
                      (3U << (14U*2U)));
}

void GPIOB_Init_RGB_PWM_Output(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    (void)RCC->AHB1ENR;   // lectura dummy para asegurar el habilitado

    // 1. Limpiar MODER para PB12, PB13, PB14
    GPIOB->MODER &= ~((3U << (12*2)) | (3U << (13*2)) | (3U << (14*2)));

    // 2. PB12 y PB14 siguen como Salida (01)
    // 3. PB13 cambia a Función Alterna (10)
    GPIOB->MODER |=  ((1U << (12*2)) | (2U << (13*2)) | (1U << (14*2)));

    // 4. Configurar PB13 específicamente para AF1 (TIM1_CH1N)
    // AFR[1] controla los pines 8-15. PB13 es el índice 5 en AFR[1]
    GPIOB->AFR[1] &= ~(0xFU << (5*4));
    GPIOB->AFR[1] |=  (0x1U << (5*4)); // AF1 = TIM1

    // 5. El resto (Push-Pull, Speed) se mantiene igual para los 3 pines
    GPIOB->OSPEEDR |= ((1U << (12*2)) | (1U << (13*2)) | (1U << (14*2)));
    GPIOB->OTYPER  &= ~((1U << 12) | (1U << 13) | (1U << 14));
}

void gpio_pa2_pa3_output_init(void)
{
    /* 1) Habilitar reloj GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* 2) Modo salida (MODER = 01) */
    GPIOA->MODER &= ~((3U << (2*2)) | (3U << (3*2)));
    GPIOA->MODER |=  ((1U << (2*2)) | (1U << (3*2)));

    /* 3) Tipo de salida: Push-Pull */
    GPIOA->OTYPER &= ~((1U << 2) | (1U << 3));

    /* 4) Velocidad: Media / Alta (elige una) */
    GPIOA->OSPEEDR &= ~((3U << (2*2)) | (3U << (3*2)));
    GPIOA->OSPEEDR |=  ((2U << (2*2)) | (2U << (3*2))); // medium speed

    /* 5) Sin pull-up / pull-down */
    GPIOA->PUPDR &= ~((3U << (2*2)) | (3U << (3*2)));
}

void gpio_pc0_pc1_pc2_pc3_pc4_init(void)
{
    /* 1) Habilitar reloj de GPIOC (AHC1) */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    (void)RCC->AHB1ENR;   // lectura dummy para asegurar el habilitado

    /* 2) Modo: Input (00) para PC0, PC1, PC2, PC3, PC4 */
    // Limpiar bits MODER: (2 bits por pin)
    GPIOC->MODER &= ~((3U << (0U*2U)) |
                      (3U << (1U*2U)) |
                      (3U << (2U*2U)) |
					  (3U << (3U*2U)) |
					  (3U << (4U*2U)));

    /* 3) Tipo de salida: Push-Pull (0) */
    GPIOC->OTYPER &= ~((1U << 0U) |
                       (1U << 1U) |
                       (1U << 2U) |
					   (1U << 3U) |
					   (1U << 4U));

    /* 4) Pull-up/Pull-down: None (00) */
    GPIOC->PUPDR &= ~((3U << (0U*2U)) |
                      (3U << (1U*2U)) |
					  (3U << (2U*2U)) |
					  (3U << (3U*2U)) |
                      (3U << (4U*2U)));
}
