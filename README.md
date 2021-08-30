# NpOS ![license: GPLv3 (shields.io)](https://img.shields.io/badge/license-GPLv3-brightgreen)![version: 1.0.0 (shields.io)](https://img.shields.io/badge/version-1.0.0-brightgreen)
a simple Embedded Real-Time Operating System

## 简介

`NpOS`以运行在RISC-V架构芯片上的完整的嵌入式实时操作系统为目标。目前可以进行基于抢占优先级的调度，实现多任务。课余时间的小实践。

## NpOS

* 抢占式调度以保证实时性

* 目前最高支持八优先级
  * 最低优先级保留
* 可裁剪式内核，可以减少至仅剩一个任务调度核心

## 使用

### 获取源码

```shell
git clone https://github.com/Janey7695/NpOS.git
```

### 文件目录说明

| 目录     | 说明                             |
| -------- | -------------------------------- |
| Npos_app | 存放Npos应用程序                 |
| Npos_bsp | 存放不同具体开发板的板级支持文件 |
| Npos_cpu | 针对不同cpu的移植文件            |
| Npos_inc | Npos内核文件的头文件             |
| Npos_src | Npos内核文件的源文件             |

### 使用方法

#### 使用platformIO

使用platformIO创建一个基于gd32vfSDK的工程后，将上述的文件夹都加入platformIO工程中的`src`文件夹中。然后编译下载即可。应用层写在`main.c`中，声明为任务，交由调度系统来管理。

#### 系统启动

整体流程与`ucos`类似

```c
#define task1_StackSize 2048 //首先需要都要任务堆栈大小

TASK_STACK_TYPE task1_Stack[task1_StackSize]; //创建任务堆栈

Np_TCB task1_Tcb;//创建任务PCB

void task1(void); //声明任务

//任务具体逻辑
void task1(){
    while(1){
    LOG_INFO("task1","task1 run ...");
    gpio_bit_reset(GPIOA, GPIO_PIN_1);
    NpOS_task_pendDelayTicks(100);
    gpio_bit_set(GPIOA, GPIO_PIN_1);
    NpOS_task_pendDelayTicks(100);
    }
}

int main(void)
{	
    //系统底层初始化
    gpio_config();
    usart0_config();
    eclic_global_interrupt_enable();
    eclic_set_nlbits(ECLIC_GROUP_LEVEL3_PRIO1);
	
    //初始化任务链表
    NpOS_task_tcblistInit();    	
    
    //创建任务
   NpOS_task_createTask(&task1_Tcb,task1,7,task1_Stack,task1_StackSize,TASK_READY);
    
    //启动Npos 此时cpu的管理权交给操作系统
    NpOS_Start();

    while (1){
    }
}
```

## 框架 todo

## TODO

### [1.0.0]

- [ ] 系统框架稳定性测试，测试gd的固件库能否在Npos上正常使用
- [ ] 增加优先级数量
- [ ] 加入基于时间片调度的调度算法
- [ ] 加入一些零零散散的系统API

## ChangeLog

### [1.4.6] - 2021-08-30

#### Changed
- 优化就绪列表相关函数，使这些函数对链表结点的操作更加准确
- 修改格式，添加最低优先级宏

#### Fixed
- - 修复[1.4.1]新增的3个API的bug

### [1.4.1] - 2021-08-27

#### Added

- new api for pending task
- new api for deleting task
- new api for ready a task

### [1.0.0] - 2021-08-25

#### Added

- README.md



## LICENSE

* GPLv3

