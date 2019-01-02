# Tool Congiuration
ARM_GCC := arm-none-eabi-gcc
ARM_GPP := arm-none-eabi-g++
ARM_OBJ_CPY := arm-none-eabi-objcopy
ARM_OBJ_SIZE := arm-none-eabi-size
RM := rm -rf
###

# Directory Configuration
ROOT_DIR ?= .
BUILD_DIR ?= $(ROOT_DIR)/build
CONFIG_DIR ?= $(ROOT_DIR)/config
LIB_DIR ?= $(ROOT_DIR)/lib
SRC_DIR ?= $(ROOT_DIR)/src
SDK_DIR := $(LIB_DIR)/sdk
###

# General Variable Configuration
IC_REV := A
IC_STEP := E
###

# Compiler/Link Flags
ARMFLAGS := -mcpu=cortex-m0 -mthumb
DFLAGS := \
	-Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_$(IC_REV) \
	-Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_$(IC_STEP)
FFLAGS := \
	-fmessage-length=0 \
	-fsigned-char \
	-ffunction-sections \
	-fdata-sections
GPPFFLAGS := $(FFLAGS) \
	-fpermissive # This is to allow casting done in the SDK
MFLAGS = -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"
OPTFLAGS := -Og -g3
WARNFLAGS := -Wall

LIB_DIRS := -L$(SDK_DIR)/bsp/misc

CONFIG_INCS := -include$(CONFIG_DIR)/system_config.h

ifdef RELEASE
DFLAGS += -DRELEASE_BUILD -DNDEBUG
OPT_FLAGS := -Os -g0
LIB_DIRS += -L$(SDK_DIR)/interfaces/ble_stack/DA14681-01-Release
else
DFLAGS += -DDEBUG
LIB_DIRS += -L$(SDK_DIR)/interfaces/ble_stack/DA14681-01-Debug
endif
###

# Dependency Variables
LINKER_SCRIPTS := $(BUILD_DIR)/linker_scripts/mem.ld $(BUILD_DIR)/linker_scripts/sections.ld
SECONDARY_FLASH := $(BUILD_DIR)/$(OUTPUT_NAME).bin
SECONDARY_SIZE := $(BUILD_DIR)/$(OUTPUT_NAME).siz

LINKER_DEPS := ${foreach linker_file,$(LINKER_SCRIPTS),-T$(linker_file)}
###

# List of all necessary SDK Source Directories
SDK_SRC_DIRS := \
	$(SDK_DIR)/bsp/startup \
	$(SDK_DIR)/bsp/system/sys_man \
	$(SDK_DIR)/bsp/peripherals/src \
	$(SDK_DIR)/bsp/osal \
	$(SDK_DIR)/bsp/memory/src \
	$(SDK_DIR)/bsp/free_rtos \
	$(SDK_DIR)/bsp/free_rtos/portable/MemMang \
	$(SDK_DIR)/bsp/free_rtos/portable/GCC/ARM_CM0 \
	$(SDK_DIR)/bsp/adapters/src \
	$(SDK_DIR)/interfaces/ble/src \
	$(SDK_DIR)/interfaces/ble/src/adapter \
	$(SDK_DIR)/interfaces/ble/src/manager \
	$(SDK_DIR)/interfaces/ble/src/util \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/ll/src/rwble \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/profiles \
	$(SDK_DIR)/interfaces/ble/src/stack/modules/nvds/src \
	$(SDK_DIR)/interfaces/ble/src/stack/modules/rwip/src \
	$(SDK_DIR)/interfaces/ble/src/stack/plf/black_orca/src/arch/main/ble \
	$(SDK_DIR)/interfaces/ble/src/stack/plf/black_orca/src/driver/rf/src \
	$(SDK_DIR)/interfaces/ble_services/src

# List of all necessary SDK Header Directories
SDK_INC_DIRS := \
  $(CONFIG_DIR) \
	$(SDK_DIR)/bsp/adapters/include \
	$(SDK_DIR)/bsp/memory/include \
	$(SDK_DIR)/bsp/config \
	$(SDK_DIR)/bsp/include \
	$(SDK_DIR)/bsp/osal \
	$(SDK_DIR)/bsp/peripherals/include \
	$(SDK_DIR)/bsp/free_rtos/include \
	$(SDK_DIR)/bsp/free_rtos/portable/GCC/ARM_CM0 \
	$(SDK_DIR)/bsp/system/sys_man/include \
	$(SDK_DIR)/interfaces/ble/config \
	$(SDK_DIR)/interfaces/ble/include \
	$(SDK_DIR)/interfaces/ble/include/adapter \
	$(SDK_DIR)/interfaces/ble/include/manager \
	$(SDK_DIR)/interfaces/ble/include/util \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ea/api \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/em/api \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/hci/api \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/ll/src/controller \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/ll/src/controller/em \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/ll/src/controller/llc \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/ll/src/controller/lld \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/ll/src/controller/llm \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/ll/src/rwble \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/hl/src/rwble_hl \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/hl/src/host/l2c/l2cc \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/hl/src/host/smp/smpc \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/hl/src/host/gatt \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/hl/src/host/gatt/gattm \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/hl/src/host/gatt/gattc \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/hl/src/host/gap/gapc \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/hl/src/host/gap/gapm \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/hl/src/host/gap \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/hl/src/host/smp/smpm \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/hl/src/host/smp \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/hl/src/host/att \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/hl/src/host/att/attc \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/hl/src/host/att/attm \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/hl/src/host/att/atts \
	$(SDK_DIR)/interfaces/ble/src/stack/ip/ble/profiles \
	$(SDK_DIR)/interfaces/ble/src/stack/config \
	$(SDK_DIR)/interfaces/ble/src/stack/modules/ke/src \
	$(SDK_DIR)/interfaces/ble/src/stack/modules/gtl/api \
	$(SDK_DIR)/interfaces/ble/src/stack/modules/h4tl/api \
	$(SDK_DIR)/interfaces/ble/src/stack/modules/dbg/api \
	$(SDK_DIR)/interfaces/ble/src/stack/modules/ke/api \
	$(SDK_DIR)/interfaces/ble/src/stack/modules/nvds/api \
	$(SDK_DIR)/interfaces/ble/src/stack/modules/rwip/api \
	$(SDK_DIR)/interfaces/ble/src/stack/modules/common/api \
	$(SDK_DIR)/interfaces/ble/src/stack/plf/black_orca/src/arch \
	$(SDK_DIR)/interfaces/ble/src/stack/plf/black_orca/src/driver/flash \
	$(SDK_DIR)/interfaces/ble/src/stack/plf/black_orca/src/driver/reg \
	$(SDK_DIR)/interfaces/ble/src/stack/plf/black_orca/src/build/ble-full/reg/fw \
	$(SDK_DIR)/interfaces/ble/src/stack/plf/black_orca/src/arch/boot/armgcc_4_8 \
	$(SDK_DIR)/interfaces/ble/src/stack/plf/black_orca/src/arch/compiler/armgcc_4_8 \
	$(SDK_DIR)/interfaces/ble/src/stack/plf/black_orca/src/driver/rf/api \
	$(SDK_DIR)/interfaces/ble_services/include

# Combine the SDK directories with the User Source Directories
SRC_DIRS := $(SDK_SRC_DIRS) $(USER_SRC_DIRS)
INC_DIRS := ${foreach dir,$(SDK_INC_DIRS) $(USER_INC_DIRS),-I$(dir)}

# All of the individual c/S files that will be compiled
SRC_FILES := ${shell find $(SRC_DIRS) -maxdepth 1 -type f -name "*.c" -or -name "*.S" -or -name "*.cpp"}
C_SRC_FILES := ${filter %.c,$(SRC_FILES)}
S_SRC_FILES := ${filter %.S,$(SRC_FILES)}
USER_SRC_FILES := ${filter $(SRC_DIR)/%,$(SRC_FILES)}

#  All of the Object files that will be generated
# OBJS := ${foreach src,$(SRC_FILES),$(BUILD_DIR)/${subst .c,.o,$(src)}}
C_OBJS := ${foreach src,$(C_SRC_FILES),${patsubst $(ROOT_DIR)/%.c,$(BUILD_DIR)/%.o,$(src)}}
S_OBJS := ${foreach src,$(S_SRC_FILES),${patsubst $(ROOT_DIR)/%.S,$(BUILD_DIR)/%.o,$(src)}}
USER_OBJS := ${foreach src,$(USER_SRC_FILES),${patsubst $(ROOT_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(src)}}

# All of the Object files combined into one variable
OBJS := $(S_OBJS) $(C_OBJS) $(USER_OBJS)

DEPS := ${foreach obj,$(OBJS),${subst .o,.d,$(obj)}}
