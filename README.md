<img src=".\readme_img\Npos图标.png" alt="Npos图标" style="zoom:75%;" />

# NpOS ![license: GPLv3 (shields.io)](https://img.shields.io/badge/license-GPLv3-brightgreen)![version: 1.7.11 (shields.io)](https://img.shields.io/badge/version-1.7.11-brightgreen)

a simple Embedded Real-Time Operating System

## 简介

`NpOS`以运行在RISC-V架构芯片上的完整的嵌入式实时操作系统为目标。目前可以进行基于抢占优先级的调度，实现多任务。课余时间的小实践。

## NpOS

* 抢占式调度以尽量实现实时性

* 目前最高支持64优先级
  * 最低优先级保留
* 可裁剪式内核，可以减少至仅剩一个任务调度核心
  * 抢占式调度内核
  * OBJ_message

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
#### 详细的函数说明
请见[user_guide](./npos_user_guide.md)(用这个链接查看的话目录无法显示)  
__推荐下载代码后，打开`npos_user_guide.pdf`以获得更好的阅读体验~__

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

## 框架

<img src=".\readme_img\Npos系统框架.png" alt="Npos系统框架" style="zoom:75%;" />

## TODO

- [ ] 系统框架稳定性测试，测试gd的固件库能否在Npos上正常使用
- [x] 增加优先级数量
- [x] 加入基于时间片调度的调度算法
- [ ] 加入一些零零散散的系统API
- [ ] 添加各类组件
  - [ ] 文件系统

## ChangeLog

### [1.7.9] - 2021-10-2

#### Fixed
- 修复[1.7.3]之后使用消息object会导致阻塞的bug

### [1.7.7] - 2021-9-24

#### Added
 - 添加新的临界区进入方式
#### Changed
 - 系统内核与硬件底层进一步解耦

### [1.7.5] - 2021-9-20

#### Fixed
- 修复时间片轮转调度的bug,该bug导致同优先级的任务，如果是头结点被拆离链表后，无法正确找到后续任务继续执行
#### Changed
- 减少了需要分配给空闲任务的堆栈大小，从本来的至多1K降到了256Byte

### [1.7.3] - 2021-9-19 

#### Added
- 在原调度系统的基础上新增“同优先级基于时间片的轮转调度”功能
#### Fixed
- 修复任务拆离就绪列表时的分离残留导致的问题


### [1.7.0] - 2021-9-11

#### Added
- 新增任务runtime统计功能，以及空闲任务可以用以粗略计算cpu利用率
- 现在任务的一般最小延时还是一个系统Tick，但是对单个任务的运行时间可以计算到us级别
#### Fixed
- 修复 NpOS_obj_receiveMsgFromTask 的 bug ，该bug导致同一条消息，会被错误的接收2次

### [1.6.0] - 2021-9-6

#### Added
- 新增object之 消息 message

### [1.5.0] - 2021-9-5

#### Added
- 增加任务优先级到最多64个

### [1.4.6] - 2021-08-30

#### Changed
- 优化就绪列表相关函数，使这些函数对链表结点的操作更加准确
- 修改格式，添加最低优先级宏

#### Fixed
- 修复[1.4.1]新增的3个API的bug

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

