cmd_/home/choi/new_work/test1/projects/networking/app/stm32f2_gpio.o := arm-uclinuxeabi-gcc -Wp,-MD,/home/choi/new_work/test1/projects/networking/app/.stm32f2_gpio.o.d  -nostdinc -isystem /home/choi/new_work/test1/tools/arm-2010q1/bin/../lib/gcc/arm-uclinuxeabi/4.4.1/include -I/home/choi/new_work/test1/linux/arch/arm/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-stm32/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -marm -mabi=aapcs-linux -mno-thumb-interwork -mthumb -Wa,-mimplicit-it=always -Wa,-mno-warn-deprecated -D__LINUX_ARM_ARCH__=7 -mcpu=cortex-m3 -Wa,-mcpu=cortex-m3 -msoft-float -Uarm -Wframe-larger-than=1024 -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fno-dwarf2-cfi-asm -fconserve-stack  -DMODULE -mlong-calls -fno-optimize-sibling-calls -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(stm32f2_gpio)"   -c -o /home/choi/new_work/test1/projects/networking/app/stm32f2_gpio.o /home/choi/new_work/test1/projects/networking/app/stm32f2_gpio.c

deps_/home/choi/new_work/test1/projects/networking/app/stm32f2_gpio.o := \
  /home/choi/new_work/test1/projects/networking/app/stm32f2_gpio.c \
  include/linux/errno.h \
  /home/choi/new_work/test1/linux/arch/arm/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  include/linux/kernel.h \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /home/choi/new_work/test1/tools/arm-2010q1/bin/../lib/gcc/arm-uclinuxeabi/4.4.1/include/stdarg.h \
  include/linux/linkage.h \
  /home/choi/new_work/test1/linux/arch/arm/include/asm/linkage.h \
  include/linux/stddef.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/choi/new_work/test1/linux/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  /home/choi/new_work/test1/linux/arch/arm/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  /home/choi/new_work/test1/linux/arch/arm/include/asm/posix_types.h \
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /home/choi/new_work/test1/linux/arch/arm/include/asm/bitops.h \
    $(wildcard include/config/smp.h) \
  /home/choi/new_work/test1/linux/arch/arm/include/asm/system.h \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/fa526.h) \
    $(wildcard include/config/arch/has/barriers.h) \
    $(wildcard include/config/cpu/v7m.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
    $(wildcard include/config/arch/a2f.h) \
    $(wildcard include/config/cpu/32v6k.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /home/choi/new_work/test1/linux/arch/arm/include/asm/irqflags.h \
  /home/choi/new_work/test1/linux/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /home/choi/new_work/test1/linux/arch/arm/include/asm/hwcap.h \
  /home/choi/new_work/test1/linux/arch/arm/include/asm/outercache.h \
    $(wildcard include/config/outer/cache/sync.h) \
    $(wildcard include/config/outer/cache.h) \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/lock.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/dynamic_debug.h \
  /home/choi/new_work/test1/linux/arch/arm/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /home/choi/new_work/test1/linux/arch/arm/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  /home/choi/new_work/test1/linux/arch/arm/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /home/choi/new_work/test1/linux/arch/arm/include/asm/div64.h \
  arch/arm/mach-stm32/include/mach/platform.h \
    $(wildcard include/config/arch/stm32f1.h) \
  arch/arm/mach-stm32/include/mach/stm32.h \
  /home/choi/new_work/test1/projects/networking/app/stm32f2_gpio.h \

/home/choi/new_work/test1/projects/networking/app/stm32f2_gpio.o: $(deps_/home/choi/new_work/test1/projects/networking/app/stm32f2_gpio.o)

$(deps_/home/choi/new_work/test1/projects/networking/app/stm32f2_gpio.o):
