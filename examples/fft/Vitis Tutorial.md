# Vitis使用教程

与Vitis_hls一样，Vitis也可以通过GUI和命令行两种方式运行。命令行较为方便快捷，所以这里介绍命令行的使用方法。

相关脚本文件的含义如下：

conn_u280.cfg：全局配置文件， --connectivity.XXX 选项可支持定义 FPGA 二进制文件的拓扑结构、指定 CU 数量、将其分配给 SLR、将内核端口连接到全局存储器并建立串流端口连接。这些命令是构建进程中不可或缺的一部分, 对于应用的定义和构造都至关重要。

make_kernel.sh：编译kernel的sw_emu、hw_emu和hw的目标

make_sw.sh：跑sw_emu流程（编译kernel的sw_emu目标，并跑sw_emu的主机代码）

make_hwe.sh：跑hw_emu流程（编译kernel的hw_emu目标，并跑hw_emu的主机代码）

make_hw.sh：跑 hw流程（编译kernel的hw目标，并跑hw的主机代码）

make_run.sh：跑sw_emu、hw_emu、hw的主机代码

![image-20250106194714626](C:\Users\hanggu\AppData\Roaming\Typora\typora-user-images\image-20250106194714626.png)



makefile中关于清除文件的规则如下：

cleanh：清除所有关于host的文件。
	 cleank：清除所有关于kernel的文件。
	 cleanall：清除所有关于host和kernel的文件。

makeflie中所有写了TODO注释的都是需要配置的参数，例如：

![image-20250108170352415](C:\Users\hanggu\AppData\Roaming\Typora\typora-user-images\image-20250108170352415.png)



## 附录

connectivity写法的详细文档：ug1393的第32章：v++命令，--connectivity选项


