#pragma once

//猜测:在kernel中包含global.h中的xcl.hpp头文件，会让vitis_hls出问题（thread-local storage is not supported for the
// current target）
// #include "global.h"

#define N 1000

typedef int dt;

extern "C" void vadd(dt* in1, dt* in2, dt* res);