#include "main.h"
#include "libmorse/morse.h"

#define BUTTON GPIO_Pin_0
#define LED1 GPIO_Pin_12
#define LED2 GPIO_Pin_13
#define LED3 GPIO_Pin_14
#define LED4 GPIO_Pin_15
#define LEDS LED1 | LED2 | LED3 | LED4

#define DEBOUNCE_DELAY 200
#define PRESCALER 8400
#define PERIOD 20000

int button_delay;
int count_of_pressing;
morse_symbol_t current_symbol;

void init_NVIC(void);
void init_EXTI(void);
void init_TIM(void);
void init_button(void);
void init_leds(void);

void set_led(void);

void EXTI0_IRQHandler(void);
void TIM2_IRQHandler(void);
void SysTick_Handler(void);

int main(void) {
    button_delay = 0;
    count_of_pressing = 0;

    init_symbol(&current_symbol);

    init_button();
    init_leds();
    init_TIM();
    init_EXTI();
    init_NVIC();

    while (1) {}
}

void init_button(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); /* GPIOA */

    GPIO_InitTypeDef gpio_struct;
    gpio_struct.GPIO_Pin = BUTTON;
    gpio_struct.GPIO_Mode = GPIO_Mode_IN;
    gpio_struct.GPIO_OType = GPIO_OType_PP;
    gpio_struct.GPIO_Speed = GPIO_Speed_100MHz;
    gpio_struct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &gpio_struct); /* GPIOA */

    SysTick_Config(SystemCoreClock / 1000);
}

void init_leds(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = LEDS;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_ResetBits(GPIOD, LED1);
    GPIO_ResetBits(GPIOD, LED2);
    GPIO_ResetBits(GPIOD, LED3);
    GPIO_ResetBits(GPIOD, LED4);
}

void init_TIM(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef tim_struct;
    tim_struct.TIM_Period = PERIOD - 1;
    tim_struct.TIM_Prescaler = PRESCALER - 1;
    tim_struct.TIM_ClockDivision = 0;
    tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &tim_struct);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

void init_NVIC(void)
{
    NVIC_InitTypeDef nvic_struct;

    // Init NVIC for a external interrupt
    nvic_struct.NVIC_IRQChannel = EXTI0_IRQn;
    nvic_struct.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_struct.NVIC_IRQChannelSubPriority = 0;
    nvic_struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_struct);

    // Init NVIC for a Timer interrupt
    nvic_struct.NVIC_IRQChannel = TIM2_IRQn;
    nvic_struct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&nvic_struct);
}

void init_EXTI(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0); /* GPIOA */
    
    EXTI_InitTypeDef exti_struct;
    exti_struct.EXTI_Line = EXTI_Line0;
    exti_struct.EXTI_LineCmd = ENABLE;
    exti_struct.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_struct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_Init(&exti_struct);
}

void set_led(void)
{
    switch (current_symbol.symbol)
    {
        case '4':
            GPIO_SetBits(GPIOD, LED1);
            GPIO_SetBits(GPIOD, LED2);
            GPIO_SetBits(GPIOD, LED3);
            break;
        case 'B':
            GPIO_ResetBits(GPIOD, LED1);
            GPIO_SetBits(GPIOD, LED2);
            GPIO_ResetBits(GPIOD, LED3);
            break;
        case 'C':
            GPIO_ResetBits(GPIOD, LED1);
            GPIO_ResetBits(GPIOD, LED2);
            GPIO_SetBits(GPIOD, LED3);
            break;
        default:
            break;
    }
}

void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        if (button_delay > DEBOUNCE_DELAY) {
            button_delay = 0;

            count_of_pressing++;
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line0);
}

void TIM2_IRQHandler(void)
{
    int old_count_of_pressing = count_of_pressing;
    count_of_pressing = 0;

    GPIO_ToggleBits(GPIOD, LED4);

    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        switch (old_count_of_pressing) {
            case 0:
                set_char(&current_symbol);
                set_led();
                init_symbol(&current_symbol);
                break;
            case 1:
                add_dot(&current_symbol);
                break;
            case 3:
                add_dash(&current_symbol);
                break;
        }

        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

void SysTick_Handler(void)
{
    button_delay++;
}
