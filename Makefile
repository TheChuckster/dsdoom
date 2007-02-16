#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM)
endif

include $(DEVKITARM)/ds_rules

export TARGET		:=	dsdoom
export TOPDIR		:=	$(CURDIR)

export DSDOOM_VERSION	:=	1.1.2SVN


.PHONY: $(TARGET).arm7 $(TARGET).arm9

#---------------------------------------------------------------------------------
#all: $(TARGET).fcsr.nds

#---------------------------------------------------------------------------------
# targets for building FCSR images
#---------------------------------------------------------------------------------
#$(TARGET).fcsr.nds : $(TARGET).gba.nds
#	padbin 512 $<
#	cat -B $< doom.fcsr > $@
#	dlditool.exe fcsr.dldi $@

all : $(TARGET).gba.nds

#---------------------------------------------------------------------------------
# nds target with gba header for devices which need rom startup code
#---------------------------------------------------------------------------------
$(TARGET).gba.nds	: $(TARGET).nds
	dsbuild $< -o $@

#---------------------------------------------------------------------------------
# standard nds target
#---------------------------------------------------------------------------------
$(TARGET).nds	:	$(TARGET).arm7 $(TARGET).arm9
	ndstool	-c $(TARGET).nds -7 $(TARGET).arm7 -9 $(TARGET).arm9 -o banner.bmp -b icon.bmp "DS DOOM;prBoom DS Port v$(DSDOOM_VERSION)"

#---------------------------------------------------------------------------------
$(TARGET).arm7	: arm7/$(TARGET).elf
$(TARGET).arm9	: arm9/$(TARGET).elf

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
	rm -f $(TARGET).ds.gba $(TARGET).nds $(TARGET).arm7 $(TARGET).arm9
