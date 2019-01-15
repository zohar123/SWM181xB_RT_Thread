# SWM181xB 芯片 说明

标签： SYNWIT、Cortex-M0、SWM181xB、国产MCU

---

## 1. 简介


### 1.2  MCU 简介

SWM320VET7 是 SYNWIT 公司的一款面向工业控制、白色家电、电机驱动等领域的芯片。包括如下硬件特性：

| 硬件 | 描述 |
| -- | -- |
|芯片型号| SWM181xB |
|CPU| ARM Cortex-M0 |
|主频| 48MHz |
|片内SRAM| 16KB |
|片内Flash|  120KB |

## 2. 编译说明

本 BSP 为开发者提供 MDK5 工程。下面以 MDK5 开发环境为例，介绍如何将系统运行起来。

双击 project.uvprojx 文件，打开 MDK5 工程，编译并下载程序到开发板。

> 工程默认配置使用 Jlink 仿真器下载程序，在通过 Jlink 连接开发板到 PC 的基础上，点击下载按钮即可下载程序到开发板

推荐熟悉 RT_Thread 的用户使用[env工具][1]，可以在console下进入到 `bsp/swm320vet7` 目录中，运行以下命令：

`scons --target=mdk5`

来编译这个板级支持包。如果编译正确无误，会产生rtthread.elf、rtthread.bin文件。其中 rtthread.bin 可以烧写到设备中进行运行。

## 3. 烧写及执行

### 3.1 硬件连接

- 使用 USB B-Type 数据线连接开发板到 PC（注意：需要下载安装串口驱动支持CH340芯片，使用 MDK5 需要安装 SWM320 相关的 pack）。

  >  USB B-Type 数据线用于串口通讯，同时供电

- 使用 Jlink 连接开发板到 PC （需要 Jlink 驱动）

连接好串口，使用115200-N-8-1的配置方式连接到设备上。串口引脚是：`[PA2/PA3]`

当使用 [env工具][1] 正确编译产生出rtthread.bin映像文件后，可以使用 ISP 的方式来烧写到设备中。

**建议使用 keil 软件直接下载**。ISP 下载较复杂。

### 3.2 运行结果

如果编译 & 烧写无误，当复位设备后，会在串口上看到板子上的蓝色LED闪烁。串口打印RT-Thread的启动logo信息：

```
 \ | /
- RT -     Thread Operating System
 / | \     4.0.0 build Dec 11 2018
 2006 - 2018 Copyright by rt-thread team
msh />
```

## 4. 驱动支持情况及计划



## 5. 联系人信息

维护人：

-[Zohar_Lee](https://github.com/zohar123) email: lizhh@synwit.cn

## 6. 参考

- 芯片[SWM320系列 数据手册][4]

  [1]: https://www.rt-thread.org/page/download.html
  [2]: http://www.synwit.cn/Public/Uploads/2018-11-05/5bdfea74d5712.pdf
  [3]: http://www.synwit.cn/Public/Uploads/2018-11-01/5bdab8ad2e5b9.pdf
  [4]: http://www.synwit.cn/Public/Uploads/2018-11-05/5bdff49b396d1.pdf
  [5]: http://www.synwit.cn/support.html
