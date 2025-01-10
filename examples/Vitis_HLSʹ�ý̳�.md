# Vitis_HLS使用教程

Vitis_HLS可以通过tcl脚本和GUI两种方式操作。下面以快速傅里叶变换(FFT)为例，简要介绍两种流程。

## 1. 使用tcl脚本

首先，将相关代码解压并放入文件夹中，如下图所示：

![image-20250106154407864](C:\Users\hanggu\AppData\Roaming\Typora\typora-user-images\image-20250106154407864.png)

其中，hls_prj文件夹用于存储vitis_hls工程。script.tcl脚本用于在这个目录下创建相应的vitis_hls工程并执行hls的仿真、综合、联合仿真、导出设计四个步骤。详细注释已在tcl脚本中给出。src文件夹中存储hls设计代码和testbench。testbench会在csim和cosim中用到。在csim中，用于验证代码的功能是否正确；在cosim中，用于验证vitis_hls生成的RTL代码是否与软件代码的行为一致。out.gold.dat是testbench中会用到的真值文件(ground truth)。

然后，终端进入hls_prj文件夹，运行tcl脚本（假设这里tcl脚本中只运行csim）：

```tcl
vitis_hls -f script.tcl
```

![image-20250106161357212](C:\Users\hanggu\AppData\Roaming\Typora\typora-user-images\image-20250106161357212.png)

可以看到csim通过，说明代码功能没有问题。另外，在hls_prj文件夹下也可以看到tcl脚本生成的hls工程项目fft：

![2bde4ea848e911dc307525dd359e70e4](D:\Tencent\QQ\HistoryMessage\981651741\nt_qq\nt_data\Pic\2025-01\Ori\2bde4ea848e911dc307525dd359e70e4.png)

如果要运行综合、联合仿真、导出ip等步骤，在tcl脚本中打开对应注释即可。例如，想要一次性运行csim、综合、cosim，将前三行注释打开：

![image-20250106161814852](C:\Users\hanggu\AppData\Roaming\Typora\typora-user-images\image-20250106161814852.png)

然后再次通过命令行运行tcl脚本，等待结果即可。

接下来，在命令行使用vitis_hls命令打开刚刚创建的fft工程，在左下角的Flow Navigator中找到C SYNTHESIS的report，点击即可打开综合报告。在这里，可以查看到顶层函数的所有信息，包括latency、interval、资源使用量等。

![abaeeb9056d37a72a2598b4490bd3c41](D:\Tencent\QQ\HistoryMessage\981651741\nt_qq\nt_data\Pic\2025-01\Ori\abaeeb9056d37a72a2598b4490bd3c41.png)

另外，要查看每一个module的详细综合报告，在相应module上右键点击"Open Synthhesis Details Report"：

![image-20250106164155120](C:\Users\hanggu\AppData\Roaming\Typora\typora-user-images\image-20250106164155120.png)

在详细综合报告中，可以看到该module的性能评估和资源使用量。分析这些信息有助于优化HLS代码。

## 2.使用GUI

首先，使用命令vitis_hls打开GUI界面：

![image-20250106164530672](C:\Users\hanggu\AppData\Roaming\Typora\typora-user-images\image-20250106164530672.png)

点击 create project创建新hls工程（刚刚使用tcl脚本创建的工程叫做fft，这里起名为fft2）：

![image-20250106164624970](C:\Users\hanggu\AppData\Roaming\Typora\typora-user-images\image-20250106164624970.png)

接下来，向工程项目中加入设计文件并设置top function：

![image-20250106164843096](C:\Users\hanggu\AppData\Roaming\Typora\typora-user-images\image-20250106164843096.png)

设置testbench文件：

![image-20250106164909915](C:\Users\hanggu\AppData\Roaming\Typora\typora-user-images\image-20250106164909915.png)

最后一步，将目标板卡改为U280，点击finish：

![image-20250106165003683](C:\Users\hanggu\AppData\Roaming\Typora\typora-user-images\image-20250106165003683.png)

目前为止，hls工程创建完毕。接下来，在Flow Navigator中依次点击C SIMLULATION、C SYNTHESIS、COSIMULATION、IMPLEMENTATION即可。

![image-20250106165321358](C:\Users\hanggu\AppData\Roaming\Typora\typora-user-images\image-20250106165321358.png)

运行csim，显示通过：

![image-20250106165306262](C:\Users\hanggu\AppData\Roaming\Typora\typora-user-images\image-20250106165306262.png)

运行C综合：

![image-20250106165438505](C:\Users\hanggu\AppData\Roaming\Typora\typora-user-images\image-20250106165438505.png)

这一步完成后flow navigator中的cosimulation和IMPLEMENTATION变绿，表示可以运行最后两步了，依次点击即可，后面不再赘述。

在C综合中，可以查看Schedule Viewer和Dataflow Viewer。

Schedule Viewer：用于显示硬件执行流程中每个操作的调度情况的工具，它以时间轴的方式可视化每个操作或任务的执行顺序及其并行性。

Dataflow Viewer：用于显示数据流设计中各模块之间的数据传递情况的工具。它以模块化的图形形式直观地展示设计中数据流动的结构和性能瓶颈。



## 附录

关于HLS的详细信息，请参考：UG1399-Vitis 高层次综合用户指南 https://docs.amd.com/r/en-US/ug1399-vitis-hls/Introduction

关于vitis_hls软件的详细用法，请参考：UG1393-Vitis 统一软件平台文档 https://docs.amd.com/v/u/zh-CN/ug1393-vitis-application-acceleration
