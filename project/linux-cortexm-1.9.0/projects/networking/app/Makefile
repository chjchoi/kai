# Makefile for a custom application comprised of
# a loable kernel module and a user space program
# (or a set of related modules and user space programs).
# If your application doesn't need to have a kernel module or
# a user space program, edit the two goals below to
# exclude one or the other.
all             : apps modules
clean           : clean_apps clean_modules

# Edit the line below to modify a set of user-space programs
# you need to build
APPS            = diaK ttyK 
apps            : $(APPS)
diaK            : diaK.o stm32f2_gpio.o
# These are flags/tools used to build user-space programs
CFLAGS          := -Os -mcpu=cortex-m3 -mthumb -I${MACH_STM32_INCLUDE} -I${LINUX_INCLUDE}
LDFLAGS         := -mcpu=cortex-m3 -mthumb
CC              = $(CROSS_COMPILE_APPS)gcc

# Clean-up after user-space programs
clean_apps      :
	-rm -f $(APPS) *.gdb *.o


#Edit the line below to modify a set of loadable modules
# you need to build
obj-m           += timer120.o
timer120-objs := tickJob.o stm32f2_gpio_drv.o tick.o

#obj-m		+= kernel_timer.o
#timer120-objs := kernel_timer.o 
#timer120-objs := kernel_timer.o timer120HzJob.o stm32f2_gpio_drv.o
#timer120HzJob-objs :=timer120HzJob.o stm32f2_gpio_drv.o

# Path to the kernel modules directory in context of which
# these loadable modules are built
KERNELDIR       :=  $(INSTALL_ROOT)/linux

# Build the modules in this directory
modules         :
	make -C $(KERNELDIR) M=`pwd` modules CFLAGS="" LDFLAGS=""

# Clean-up after modules
clean_modules:
	make -C $(KERNELDIR) M=`pwd` clean
	rm -f modules.order

