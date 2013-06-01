TOOLCHAIN=arm-2010q1
export INSTALL_ROOT=`pwd`
TOOLS_PATH=$INSTALL_ROOT/tools
CROSS_PATH=$TOOLS_PATH/$TOOLCHAIN/bin
#NOR
export MACH_STM32_INCLUDE=$INSTALL_ROOT/linux/arch/arm/mach-stm32/include
export LINUX_INCLUDE=$INSTALL_ROOT/linux/include
export PATH=$TOOLS_PATH/bin:$CROSS_PATH:$MACH_STM32_INCLUDE:$LINUX_INCLUDE:$PATH

# Path to cross-tools
export CROSS_COMPILE=arm-uclinuxeabi-
export CROSS_COMPILE_APPS=arm-uclinuxeabi-

# Define the MCU architecture
export MCU=STM

