#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM)
endif

include $(DEVKITARM)/ds_rules

export TARGET		:=	dsdoom
export TOPDIR		:=	$(CURDIR)

export DSDOOM_VERSION	:=	1.2.0


.PHONY: arm7/$(TARGET).elf arm9/$(TARGET).elf

#---------------------------------------------------------------------------------
#all: $(TARGET).fcsr.nds

#---------------------------------------------------------------------------------
# targets for building FCSR images
#---------------------------------------------------------------------------------
#$(TARGET).fcsr.nds : $(TARGET).nds
#	padbin 512 $<
#	cat -B $< doom.fcsr > $@
#	dlditool.exe fcsr.dldi $@

all : $(TARGET).nds

#---------------------------------------------------------------------------------
# standard nds target
#---------------------------------------------------------------------------------
$(TARGET).nds	:	arm7/$(TARGET).elf arm9/$(TARGET).elf
	ndstool	-c $(TARGET).nds -7 arm7/$(TARGET).elf -9 arm9/$(TARGET).elf -b icon.bmp "DS DOOM;prBoom DS Port v$(DSDOOM_VERSION)"

#---------------------------------------------------------------------------------
arm7/$(TARGET).elf:
	$(MAKE) -C arm7
	
#---------------------------------------------------------------------------------
arm9/$(TARGET).elf:
	$(MAKE) -C arm9

#---------------------------------------------------------------------------------
clean:
	$(MAKE) -C arm9 clean
	$(MAKE) -C arm7 clean
	rm -f $(TARGET).nds
