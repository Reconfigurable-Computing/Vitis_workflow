# Overall introduction


## 教程目录

1. [ZCU104](./start_ZCU104.md)：针对嵌入式板卡的加速器设计简单样例，以ZCU104作为参考。[跳转链接](./start_ZCU104.md)

2. [Alveo U50](./start_U50.md)：针对数据中心板卡的加速器设计简单样例，以Alveo U50作为参考。[跳转链接](./start_U50.md)

## 源代码结构

+ host : 上层代码
    - event_timer: 记录器
    - host: 主程序
    - vadd_host: 加速器上层程序
    - xcl2: xilinx魔改opencl库

+ kernel: 加速器IP核代码
    - vadd: 向量逐元素加法IP
    