#ifndef LOG_H
#define LOG_H

#include "stdio.h"
#include "../Npos_cpu/systick.h"
#if NPOS_logInfoPrintf_EN
#define pNONE                  "\e[0m"           //清除颜色，即之后的打印为正常输出，之前的不受影响
#define pBLACK                 "\e[0;30m"         //深黑
#define pL_BLACK             "\e[1;30m"            //亮黑，偏灰褐
#define pRED                     "\e[0;31m"      //深红，暗红
#define pL_RED                 "\e[1;31m"        //鲜红
#define pGREEN                "\e[0;32m"         //深绿，暗绿
#define pL_GREEN            "\e[1;32m"           //鲜绿
#define pBROWN               "\e[0;33m"          //深黄，暗黄
#define pYELLOW              "\e[1;33m"          //鲜黄
#define pBLUE                    "\e[0;34m"      //深蓝，暗蓝
#define pL_BLUE                "\e[1;34m"        //亮蓝，偏白灰
#define pPURPLE               "\e[0;35m"         //深粉，暗粉，偏暗紫
#define pL_PURPLE           "\e[1;35m"           //亮粉，偏白灰
#define pCYAN                   "\e[0;36m"       //暗青色
#define pL_CYAN               "\e[1;36m"         //鲜亮青色
#define pGRAY                   "\e[0;37m"       //灰色
#define pWHITE                  "\e[1;37m"       //白色，字体粗一点，比正常大，比bold小
#define pBOLD                    "\e[1m"         //白色，粗体
#define pUNDERLINE         "\e[4m"               //下划线，白色，正常大小
#define pBLINK                   "\e[5m"         //闪烁，白色，正常大小
#define pREVERSE            "\e[7m"              //反转，即字体背景为白色，字体为黑色
#define pHIDE                     "\e[8m"        //隐藏
#define pCLEAR                  "\e[2J"          //清除
#define pCLRLINE               "\r\e[K"          //清除行

#define _TIME_  get_sys_runtime()

#define LOG_ERR(who,content)    printf("[%ld]  [%s]["pRED "ERROR"pNONE"]: %s. \n",_TIME_,who,content)
#define LOG_OK(who,content)    printf("[%ld]  [%s]["pGREEN "OK"pNONE"]: %s. \n",_TIME_,who,content)
#define LOG_WARING(who,content)    printf("[%ld]  [%s]["pBROWN "WARING"pNONE"]: %s. \n",_TIME_,who,content)
#define LOG_INFO(who,content)    printf("[%ld]  [%s]["pNONE "INFO"pNONE"]: %s. \n",_TIME_,who,content)
#else
#define LOG_ERR(who,content)   
#define LOG_OK(who,content)   
#define LOG_WARING(who,content)   
#define LOG_INFO(who,content)  
#endif



#endif // !LOG_H



