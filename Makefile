# Directory Configuration
ROOT_DIR := .
SRC_DIR := $(ROOT_DIR)/src
###

VERSION ?= 0.1.0

# Setup output name IFF OUTPUT_NAME not provided
ifeq ($(origin OUTPUT_NAME), undefined)

OUTPUT_NAME := climbalytics_$(VERSION)

ifdef RELEASE
OUTPUT_NAME := ${join $(OUTPUT_NAME),_release}
else
OUTPUT_NAME := ${join $(OUTPUT_NAME),_debug}
endif

ifdef RAM
OUTPUT_NAME := ${join $(OUTPUT_NAME),_ram}
else
OUTPUT_NAME := ${join $(OUTPUT_NAME),_qspi}
endif

endif

BOOTLOADER ?= uartboot.bin

-include source.mk
-include defines.mk
-include $(DEPS)

# All Target
all: pre-build main-build

# Pre-build Target
pre-build: $(LINKER_SCRIPTS)

# Main-build Target
main-build: $(BUILD_DIR)/$(OUTPUT_NAME).elf $(SECONDARY_FLASH) $(SECONDARY_SIZE)

# Main ELF Rule
$(BUILD_DIR)/$(OUTPUT_NAME).elf: $(OBJS) $(LINKER_SCRIPTS)
	@echo 'Building target: $@'
	@$(ARM_GPP) \
		$(ARMFLAGS) $(WARNFLAGS) $(FFLAGS) $(OPTFLAGS) $(DFLAGS) \
		$(LINKER_DEPS) -Xlinker --gc-sections \
		$(OBJS) \
		$(LIB_DIRS) -lble_stack_da14681_01 \
		-Wl,-Map,$(BUILD_DIR)/$(OUTPUT_NAME).map \
		--specs=nano.specs --specs=nosys.specs \
		-o $@

# Rule on how to build binary file
$(SECONDARY_FLASH): $(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	@echo 'Building Flash binary: $@'
	@$(ARM_OBJ_CPY) -O binary $^ $@

# Rule on how to output binary size
$(SECONDARY_SIZE): $(BUILD_DIR)/%.siz: $(BUILD_DIR)/%.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	@$(ARM_OBJ_SIZE) --format=berkeley $^ > $@
	-@cat $@

# Rule on how to build all C++ object files
$(USER_OBJS): $(BUILD_DIR)/%.o: %.cpp
	@echo 'Compiling: $< --> $@'
	-@mkdir -p $(@D)
	@$(ARM_GPP) $(ARMFLAGS) $(WARNFLAGS) $(GPPFFLAGS) $(OPTFLAGS) $(DFLAGS) $(INC_DIRS) $(CONFIG_INCS) $(MFLAGS) -c -o $@ $<

# Rule on how to build all C object files
$(C_OBJS): $(BUILD_DIR)/%.o: %.c
	@echo 'Compiling: $< --> $@'
	-@mkdir -p $(@D)
	@$(ARM_GCC) $(ARMFLAGS) $(WARNFLAGS) $(FFLAGS) $(OPTFLAGS) $(DFLAGS) $(INC_DIRS) $(CONFIG_INCS) $(MFLAGS) -c -o $@ $<

# Rule on how to build all S object files
$(S_OBJS): $(BUILD_DIR)/%.o: %.S
	@echo 'Assembling: $< --> $@'
	-@mkdir -p $(@D)
	@$(ARM_GCC) $(ARMFLAGS) $(WARNFLAGS) $(FFLAGS) $(OPTFLAGS) -x assembler-with-cpp $(DFLAGS) $(INC_DIRS) $(CONFIG_INCS) $(MFLAGS) -c -o $@ $<

# Rule on how to build linker script files
$(LINKER_SCRIPTS): $(BUILD_DIR)/%.ld : $(SRC_DIR)/%.ld.h
	@echo 'Building: $< --> $@'
	-@mkdir -p $(@D)
	@$(ARM_GCC) $(DFLAGS) -I$(SDK_DIR)/bsp/config $(CONFIG_INCS) -E -P -c $< -o $@

# Rule to copy the firmware binary into the install directory for the flash burning
.PRECIOUS: install/bin/$(OUTPUT_NAME).bin
install/bin/$(OUTPUT_NAME).bin: $(BUILD_DIR)/$(OUTPUT_NAME).bin
	@cp -f $< $@

clean:
	$(RM) $(BUILD_DIR)

flash: install/bin/$(OUTPUT_NAME).bin startup_vm
	@VBoxManage guestcontrol climbalytics-win run \
	  --username climbalytics \
	  --password climbalytics \
	  --wait-stdout \
	  --wait-stderr \
	  --dos2unix \
	  --exe "C:/DiaSemi/SmartSnippetsStudio/Toolbox/SmartSnippetsToolbox.exe" \
	  -- \
	  arg0 -type qspi -chip DA14680-01 -com_port $(COM) \
	    -firmware "E:/$(BOOTLOADER)" \
	    -cmd write -mode cached -file "E:/$(OUTPUT_NAME).bin"

flash_erase: startup_vm
	@VBoxManage guestcontrol climbalytics-win run \
	  --username climbalytics \
	  --password climbalytics \
	  --wait-stdout \
	  --wait-stderr \
	  --dos2unix \
	  --exe "C:/DiaSemi/SmartSnippetsStudio/Toolbox/SmartSnippetsToolbox.exe" \
	  -- \
	  arg0 -type qspi -chip DA14680-01 -com_port $(COM) \
	    -cmd erase

serial_out: startup_vm
	@VBoxManage guestcontrol climbalytics-win run \
	  --username climbalytics \
	  --password climbalytics \
	  --wait-stdout \
	  --wait-stderr \
	  --dos2unix \
	  --exe "C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe" \
	  -- \
	  arg0 -File E:\serial_read.ps1 -com COM$(COM)

debug: all
	make flash
	make serial_out

startup_vm:
	@install/startup_vm.sh

shutdown_vm:
	@VBoxManage controlvm climbalytics-win poweroff

.PHONY: all clean debug startup_vm shutdown_vm flash flash_erase serial_out
.SECONDARY: main-build pre-build
