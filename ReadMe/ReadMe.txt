2017.5.9   上下移动完成
2017.5.15   15:44    可以用 AT24Cxx 保存数据
2017.5.17   14:56    完成 float 转 BCD  和 BCD 转 float (存在 0.001误差   用 VS2012 也一样)
2017.5.18   15:50    注释掉了 os_cfg.c 的 24 行 __root 。没改前:#define  OS_COMPILER_OPT  __root      改后 : #define  OS_COMPILER_OPT  //__root
2017.5.22   20:16    uart.c和uart.h完成485的UART0串口中断接收程序发送
2017.5.23   16:11    创建了一个开始任务,在开始任务里面创建了一个运行任务,将键码值移到keyfuntion.c中并设置了ReadKeyValue(void),WriteKeyValue(uint32 value)对这个值进行封装.去掉了timer0键盘实现,timer0只扫描键盘并给
                     键码赋值,在运行任务Opera_Task()中读取键码值并执行.
					 
2017.5.27   13:55    在bsp_timer.c中创建控制风扇转速的程序. A/D片上采样温度程序解决    		15:30:解决获取温度采样值并控制风扇转速