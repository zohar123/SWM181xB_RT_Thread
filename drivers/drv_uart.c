/*
 * Copyright (c) 2006-2018, Synwit Technology Co.,Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-31     ZYH          first version
 * 2018-12-10     Zohar_Lee    format file
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

struct swm260_uart
{
    UART_TypeDef *uart;
    IRQn_Type irq;
};

static rt_err_t swm260_uart_configure(struct rt_serial_device *serial,
                                      struct serial_configure *cfg)
{
    struct swm260_uart *uart;
    UART_InitStructure UART_initStruct;
    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);
    uart = (struct swm260_uart *)serial->parent.user_data;
    NVIC_DisableIRQ(uart->irq);
    UART_initStruct.Baudrate = cfg->baud_rate;
    UART_initStruct.RXThreshold = 0;
    UART_initStruct.RXThresholdIEn = 1;
    UART_initStruct.TXThresholdIEn = 0;
    UART_initStruct.TimeoutTime = 10;
    UART_initStruct.TimeoutIEn = 0;
    switch (cfg->data_bits)
    {
    case DATA_BITS_9:
        UART_initStruct.DataBits = UART_DATA_9BIT;
        break;
    default:
        UART_initStruct.DataBits = UART_DATA_8BIT;
        break;
    }
    switch (cfg->stop_bits)
    {
    case STOP_BITS_2:
        UART_initStruct.StopBits = UART_STOP_2BIT;
        break;
    default:
        UART_initStruct.StopBits = UART_STOP_1BIT;
        break;
    }
    switch (cfg->parity)
    {
    case PARITY_ODD:
        UART_initStruct.Parity = UART_PARITY_ODD;
        break;
    case PARITY_EVEN:
        UART_initStruct.Parity = UART_PARITY_EVEN;
        break;
    default:
        UART_initStruct.Parity = UART_PARITY_NONE;
        break;
    }
    UART_Init(uart->uart, &UART_initStruct);
    IRQ_Connect(IRQ0_15_UART0, uart->irq, 1);
    UART_Open(uart->uart);
    return RT_EOK;
}

static rt_err_t swm260_uart_control(struct rt_serial_device *serial,
                                    int cmd, void *arg)
{
    struct swm260_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    uart = (struct swm260_uart *)serial->parent.user_data;
    switch (cmd)
    {
    case RT_DEVICE_CTRL_CLR_INT:
        /* disable rx irq */
        NVIC_DisableIRQ(uart->irq);
        break;
    case RT_DEVICE_CTRL_SET_INT:
        /* enable rx irq */
        NVIC_EnableIRQ(uart->irq);
        break;
    }
    return RT_EOK;
}

static int swm260_uart_putc(struct rt_serial_device *serial, char c)
{
    struct swm260_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    uart = (struct swm260_uart *)serial->parent.user_data;
    while (UART_IsTXBusy(uart->uart))
        ;
    uart->uart->DATA = c;
    return 1;
}

static int swm260_uart_getc(struct rt_serial_device *serial)
{
    int ch;
    struct swm260_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    uart = (struct swm260_uart *)serial->parent.user_data;
    ch = -1;
    if (UART_IsRXFIFOEmpty(uart->uart) == 0)
        UART_ReadByte(uart->uart, (uint32_t *)&ch);
    return ch;
}

static const struct rt_uart_ops swm260_uart_ops =
{
    swm260_uart_configure,
    swm260_uart_control,
    swm260_uart_putc,
    swm260_uart_getc,
};

#if defined(BSP_USING_UART0)
/* UART0 device driver structure */
static struct swm260_uart uart0;
struct rt_serial_device serial0;
void IRQ0_Handler(void)
{
    struct swm260_uart *uart;
    /* enter interrupt */
    rt_interrupt_enter();

    uart = &uart0;
    /* UART in mode Receiver */
    if (UART_INTRXThresholdStat(uart->uart) || UART_INTTimeoutStat(uart->uart))
    {
        rt_hw_serial_isr(&serial0, RT_SERIAL_EVENT_RX_IND);
    }
    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* BSP_USING_UART0 */

int rt_hw_uart_init(void)
{
    struct swm260_uart *uart;
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
#ifdef BSP_USING_UART0
    PORT_Init(PORTA, PIN0, FUNMUX_UART0_RXD, 1); //GPIOA.0配置为UART0输入引脚
    PORT_Init(PORTA, PIN1, FUNMUX_UART0_TXD, 0); //GPIOA.1配置为UART0输出引脚
    uart = &uart0;
    uart->uart = UART0;
    uart->irq = IRQ0_IRQ;
    serial0.ops = &swm260_uart_ops;
    serial0.config = config;
    /* register UART0 device */
    rt_hw_serial_register(&serial0, "uart0",
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX,
                          uart);
#endif /* BSP_USING_UART0 */
    return 0;
}
INIT_BOARD_EXPORT(rt_hw_uart_init);
