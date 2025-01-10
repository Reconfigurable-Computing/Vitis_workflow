#
# Copyright 2019-2021 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# makefile-generator v1.0.4
#

# ####################################### Help Section #####################################
.PHONY: help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all TARGET=<sw_emu/hw_emu/hw> DEVICE=<FPGA platform> HOST_ARCH=<x86>"
	$(ECHO) "      Command to generate the design for specified Target and Shell."
	$(ECHO) ""
	$(ECHO) "  make clean "
	$(ECHO) "      Command to remove the generated non-hardware files."
	$(ECHO) ""
	$(ECHO) "  make cleanall"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""
	$(ECHO) "  make run TARGET=<sw_emu/hw_emu/hw> DEVICE=<FPGA platform> HOST_ARCH=<x86>"
	$(ECHO) "      Command to run application in emulation or on board."
	$(ECHO) ""
	$(ECHO) "  make build TARGET=<sw_emu/hw_emu/hw> DEVICE=<FPGA platform> HOST_ARCH=<x86>"
	$(ECHO) "      Command to build xclbin application."
	$(ECHO) ""
	$(ECHO) "  make host HOST_ARCH=<x86>"
	$(ECHO) "      Command to build host application."
	$(ECHO) ""

# NOTE: 本Makefile仅支持生成单个xclbin(统一命名为fused.xclbin)，不支持切换xclbin
# ##################### Setting up default value of TARGET ##########################
# TODO: 设置TARGET
TARGET ?= sw_emu

# ################### Setting up default value of DEVICE ##############################
# TODO: 设置板卡（可选值：u280 u50）
BOARD ?= u280

# 以下是几个板卡型号：
# u50：xilinx_u50_gen3x16_xdma_201920_3
# U280:xilinx_u280_gen3x16_xdma_1_202211_1
# U280(用于最后实际上板时，即TARGET=HW时): xilinx_u280_gen3x16_xdma_base_1
DEVICE ?= xilinx_u280_gen3x16_xdma_1_202211_1

# 根据板卡设置 DEVICE
ifeq ($(BOARD), u50)
DEVICE := xilinx_u50_gen3x16_xdma_201920_3
else ifeq ($(BOARD), u280)
DEVICE := xilinx_u280_gen3x16_xdma_1_202211_1
else
$(error [ERROR]: Unsupported BOARD=$(BOARD). Please use 'u50' or 'u280'.)
endif

# 打印当前板卡设置
$(info [INFO]: Using BOARD=$(BOARD), DEVICE=$(DEVICE))

# ###################### Setting up default value of HOST_ARCH ####################### 
HOST_ARCH ?= x86

CXXFLAGS := -I${PWD}

# TODO: 设置kernel名称（kernel文件夹下的cpp文件名.top function必须和cpp名称一样，见"Setting Rules for Binary Containers Building Kernels"） 
KERNEL_LIST := vmul vadd

# TODO: 设置频率，默认值为300
FREQUENCY ?= 300

# TODO: 设置线程并行度，默认值为8
THREADS ?= 32

# TODO: 设置优化等级，默认值为3
OPT_LEVEL ?= 3

# TODO: 设置报告级别
REPORT_LEVEL ?= 2

# #################### Checking if DEVICE in blacklist #############################
ifeq ($(findstring zc, $(DEVICE)), zc)
$(error [ERROR]: This project is not supported for $(DEVICE).)
endif

# #################### Checking if DEVICE in whitelist ############################
# ifneq ($(findstring u280, $(DEVICE)), u280)
# $(warning [WARNING]: This project has not been tested for $(DEVICE). It may or may not work.)
# endif

ifneq ($(findstring $(BOARD), $(DEVICE)),$(BOARD))
$(warning [WARNING]: This project has not been tested for $(DEVICE). It may or may not work.)
endif

# ######################## Setting up Project Variables #################################
MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CUR_DIR := $(patsubst %/,%,$(dir $(MK_PATH)))

# ######################### Include environment variables in utils.mk ####################
include ./utils.mk
XDEVICE := $(call device2xsa, $(DEVICE))
TEMP_DIR := _x_temp.$(TARGET).$(XDEVICE)
TEMP_REPORT_DIR := $(CUR_DIR)/reports/_x.$(TARGET).$(XDEVICE)
BUILD_DIR := build_dir.$(TARGET).$(XDEVICE)
BUILD_REPORT_DIR := $(CUR_DIR)/reports/_build.$(TARGET).$(XDEVICE)
EMCONFIG_DIR := $(BUILD_DIR)
XCLBIN_DIR := $(CUR_DIR)/$(BUILD_DIR)
export XCL_BINDIR = $(XCLBIN_DIR)

# ######################### Setting up Host Variables #########################
# TODO: 在这里include需要的头文件和链接需要的库
#Include Required Host Source Files
HOST_SRCS += $(CUR_DIR)/common/include/xcl2.cpp
HOST_SRCS += $(CUR_DIR)/common/include/event_timer.cpp
HOST_SRCS += $(CUR_DIR)/host/host.cpp

CXXFLAGS += -I$(CUR_DIR)
CXXFLAGS += -I$(CUR_DIR)/host
CXXFLAGS += -I$(CUR_DIR)/common/include

ifeq ($(TARGET),sw_emu)
CXXFLAGS += -D SW_EMU_TEST
endif

ifeq ($(TARGET),hw_emu)
CXXFLAGS += -D HW_EMU_TEST
endif

# ######################### Host compiler global settings ############################
CXXFLAGS += -I$(XILINX_XRT)/include -I$(XILINX_HLS)/include -std=c++11 -O3 -Wall -Wno-unknown-pragmas -Wno-unused-label
LDFLAGS += -L$(XILINX_XRT)/lib -lOpenCL -lpthread -lrt -Wno-unused-label -Wno-narrowing -DVERBOSE
CXXFLAGS += -fmessage-length=0 -O3
CXXFLAGS += -I$(CUR_DIR)/src

# lIp_floating_point_v7_0_bitacc_cmodel 只存在于2022.1版本的Vitis HLS中
# 2023版本的Vitis HLS中使用lIp_floating_point_v7_1_bitacc_cmodel
ifeq ($(HOST_ARCH), x86)
LDFLAGS += -L$(XILINX_HLS)/lnx64/tools/fpo_v7_1 -Wl,--as-needed -lgmp -lmpfr -lIp_floating_point_v7_1_bitacc_cmodel
endif

# ################### Setting package and image directory #######################
EXE_NAME := host.exe
EXE_FILE := $(BUILD_DIR)/$(EXE_NAME)
# TODO: 设置host程序参数(xclbin文件路径)
HOST_ARGS := --xclbin $(BUILD_DIR)/fused.xclbin
#HOST_ARGS := --xclbin $(BUILD_DIR)/fused.xclbin --offset $(CUR_DIR)/data/data-csr-offset.mtx --index $(CUR_DIR)/data/data-csr-indicesweights.mtx

# ##################### Kernel compiler global settings ##########################
VPP_FLAGS += -t $(TARGET) --platform $(XPLATFORM) --save-temps --optimize $(OPT_LEVEL)
VPP_FLAGS += --hls.jobs $(THREADS)
VPP_LDFLAGS += --vivado.synth.jobs $(THREADS) --vivado.impl.jobs $(THREADS)

# ---------------------
# !!! 在编译阶段不使用 .cfg !!!
# ---------------------
# 设置cfg文件
# ifneq (,$(shell echo $(XPLATFORM) | awk '/$(BOARD)/'))
# VPP_FLAGS += --config $(CUR_DIR)/conn_$(BOARD).cfg
# endif

VPP_FLAGS += -I$(CUR_DIR)/kernel

# 对于KERNEL_LIST中的每一个kernel定义一个VPP_FLAGS 和 VPP_LDFLAGS
define SET_KERNEL_VPP_FLAGS
$(1)_VPP_FLAGS += -D KERNEL_NAME=$(1)
$(1)_VPP_FLAGS += --hls.clock $(FREQUENCY)000000:$(1)
endef

# 只在链接时一次性指定频率
VPP_LDFLAGS += --kernel_frequency $(FREQUENCY)

# Apply to each kernel
$(foreach KERNEL, $(KERNEL_LIST), $(eval $(call SET_KERNEL_VPP_FLAGS,$(KERNEL))))

# 设置report level
ifeq ($(REPORT_LEVEL), estimate)
VPP_FLAGS += --report_level estimate
else ifneq ($(filter $(REPORT_LEVEL), 0 1 2),)
VPP_FLAGS += --report_level $(REPORT_LEVEL)
else
$(error [ERROR]: Unsupported REPORT_LEVEL=$(REPORT_LEVEL). Please use 0, 1, 2, or estimate.)
endif

# ############################ Declaring Binary Containers ##########################
# Initialize BINARY_CONTAINERS and their OBJS
# 一个BINARY_CONTAINER对应生成一个xclbin文件。本makefile只支持生成一个fused.xclbin文件
BINARY_CONTAINERS += $(BUILD_DIR)/fused.xclbin

# 该xclbin文件内含的kernel object(.xo)文件
#TODO: 关于kernel在FPGA上的连接关系，请修改.cfg文件
# BINARY_CONTAINER_$(KERNEL)_OBJS += $(TEMP_DIR)/$(KERNEL).xo
BINARY_CONTAINER_fused_OBJS := $(foreach KERNEL, $(KERNEL_LIST), $(TEMP_DIR)/$(KERNEL).xo)

# ######################### Setting Targets of Makefile ################################
DATA_DIR += $(CUR_DIR)/data

.PHONY: all clean cleanall docs emconfig
ifeq ($(HOST_ARCH), x86)
all:  check_version check_vpp check_platform check_xrt $(EXE_FILE) $(BINARY_CONTAINERS) emconfig
else
all:  check_version check_vpp check_platform check_sysroot $(EXE_FILE) $(BINARY_CONTAINERS) emconfig  sd_card
endif

.PHONY: host
ifeq ($(HOST_ARCH), x86)
host:   check_xrt $(EXE_FILE)
else
host:   check_sysroot $(EXE_FILE)
endif

.PHONY: xclbin
ifeq ($(HOST_ARCH), x86)
xclbin: check_vpp  check_xrt  $(BINARY_CONTAINERS)
else
xclbin: check_vpp  check_sysroot  $(BINARY_CONTAINERS)
endif

.PHONY: build
build: xclbin

# ################ Setting Rules for Binary Containers (Building Kernels) ################
# 编译阶段：仅 -c，不带 config
$(TEMP_DIR)/%.xo: $(CUR_DIR)/kernel/%.cpp
	$(ECHO) "Compiling Kernel: $*"
	mkdir -p $(TEMP_DIR)
	$(VPP) -c $($*_VPP_FLAGS) $(VPP_FLAGS) -k $* \
	       -I'$(<D)' --temp_dir $(TEMP_DIR) --report_dir $(TEMP_REPORT_DIR) \
	       -o '$@' $^

# 链接阶段：统一使用 --config conn_$(BOARD).cfg
$(BUILD_DIR)/fused.xclbin: $(BINARY_CONTAINER_fused_OBJS)
	mkdir -p $(BUILD_DIR)
	$(VPP) -l $(VPP_FLAGS) \
	       --config $(CUR_DIR)/conn_$(BOARD).cfg \
	       --temp_dir $(TEMP_DIR) --report_dir $(BUILD_REPORT_DIR)/fused \
	       $(VPP_LDFLAGS) $(foreach KERNEL, $(KERNEL_LIST), $(VPP_LDFLAGS_$(KERNEL))) \
	       -o '$@' $+


# ################# Setting Rules for Host (Building Host Executable) ################
ifeq ($(HOST_ARCH), x86)
$(EXE_FILE):  $(HOST_SRCS) | check_xrt 
else
$(EXE_FILE):  $(HOST_SRCS) | check_sysroot 
endif

	mkdir -p $(BUILD_DIR)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

# emconfig是一个伪目标，不是实际生成的文件
emconfig:$(EMCONFIG_DIR)/emconfig.json
# $(EMCONFIG_DIR)/emconfig.json是一个真实目标
$(EMCONFIG_DIR)/emconfig.json:
	emconfigutil --platform $(XPLATFORM) --od $(EMCONFIG_DIR)


# ###############Setting Essential Checks And Running Rules For Vitis Flow #############
# run目标依赖于all目标
run: all
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	$(CP) $(EMCONFIG_DIR)/emconfig.json .
	XCL_EMULATION_MODE=$(TARGET) $(EXE_FILE) $(HOST_ARGS)
else
	$(EXE_FILE) $(HOST_ARGS)
endif

# ################################# Cleaning Rules ##################################
cleanh:
	-$(RMDIR) $(EXE_FILE) vitis_* TempConfig system_estimate.xtxt *.rpt .run/
	-$(RMDIR) src/*.ll _xocc_* .Xil dltmp* xmltmp* *.log *.jou *.wcfg *.wdb sample_link.ini sample_compile.ini obj* bin* *.csv *.jpg *.jpeg *.png

cleank:
	-$(RMDIR) $(BUILD_DIR)/*.xclbin _vimage *xclbin.run_summary qemu-memory-_* emulation/ _vimage/ start_simulation.sh *.xclbin
	-$(RMDIR) _x_temp.*/_x.* _x_temp.*/.Xil _x_temp.*/profile_summary.* xo_* _x*
	-$(RMDIR) _x_temp.*/dltmp* _x_temp.*/kernel_info.dat _x_temp.*/*.log 
	-$(RMDIR) _x_temp.* 

cleanall: cleanh cleank
	-$(RMDIR) $(BUILD_DIR)  build_dir.* emconfig.json *.html $(TEMP_DIR) $(CUR_DIR)/reports *.csv *.run_summary $(CUR_DIR)/*.raw package_* run_script.sh .ipcache *.str
	-$(RMDIR) $(AIE_CONTAINERS) $(CUR_DIR)/Work $(CUR_DIR)/*.xpe $(CUR_DIR)/hw.o $(CUR_DIR)/*.xsa $(CUR_DIR)/xnwOut aiesimulator_output .AIE_SIM_CMD_LINE_OPTIONS

clean: cleanh
