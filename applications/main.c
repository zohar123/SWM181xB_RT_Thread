/*
 * Copyright (c) 2006-2018, Synwit Technology Co.,Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-10     Zohar_Lee    first version
 */

#include <rtthread.h>
#include <rtdevice.h>

/*
* 程序清单：systemview 演示代码
*
* 这个例子中将创建一个动态信号量(初始值为0)及两个动态线程，在这个两个动态线程中
* 线程2将试图采用永远等待方式去持有信号量，持有成功之后发送运行标志。
* 线程1将先发送正在运行标志，然后释放一次信号量，因线程2的优先级较高，线程2持有到信号量将线程1抢断。
* 然后线程2发送运行标志之后，获取不到信号量，被挂起，线程1继续运行
*/

#define THREAD_PRIORITY 25
#define THREAD_STACK_SIZE 512
#define THREAD_TIMESLICE 5
/* 指向信号量的指针 */
rt_sem_t sem_food;
/* 线程1入口 */
void thread1_entry(void *parameter)
{
    while (1)
    {
        /* 线程1第一次运行 */
        rt_kprintf("thread1 is run!\n");
        /* 释放一次信号量 */
        rt_sem_release(sem_food);
        /* 线程1第二次运行 */
        rt_kprintf("thread1 run again!\n");
        /* 线程1延时1秒 */
        rt_thread_delay(RT_TICK_PER_SECOND);
    }
}
/* 线程2入口 */
void thread2_entry(void *parameter)
{
    while (1)
    {
        /* 试图持有信号量，并永远等待直到持有到信号量 */
        rt_sem_take(sem_food, RT_WAITING_FOREVER);
        /* 线程2正在运行 */
        rt_kprintf("thread2 is run!\n");
    }
}
/* DEMO初始化函数 */
void demo_init(void)
{
    /* 指向线程控制块的指针 */
    rt_thread_t thread1_id, thread2_id;
    /* 创建一个信号量，初始值是0 */
    sem_food = rt_sem_create("sem_food", 0, RT_IPC_FLAG_FIFO);
    if (sem_food == RT_NULL)
    {
        rt_kprintf("sem created fail!\n");
        return;
    }
    /* 创建线程1 */
    thread1_id = rt_thread_create("thread1",
                                  thread1_entry, RT_NULL, /* 线程入口是thread1_entry, 参数RT_NULL */
                                  THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (thread1_id != RT_NULL)
        rt_thread_startup(thread1_id);
    /* 创建线程2 */
    thread2_id = rt_thread_create("thread2",
                                  thread2_entry, RT_NULL, /* 线程入口是thread2_entry, 参数RT_NULL */
                                  THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (thread2_id != RT_NULL)
        rt_thread_startup(thread2_id);
}

int main(void)
{
    /* user app entry */
    demo_init();
    while (1)
    {
        //        rt_kprintf("synwit uart test!\r\n");
        rt_thread_mdelay(500);
    }
}
