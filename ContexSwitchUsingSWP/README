This directory contains Chad Bartlett's version of HW 08 for EMBSYS100.

AUTHOR
------
20141216 - Chad Bartlett

Project Tree
------------
./  -
    |- readme.txt
inc -
    |- init.h
    |- lpc2378.h
    |- print.h
    |- pins.h
src -
    |- crt.S
    |- debugger.c
    |- init.c
    |- main.c
    |- print.c

Design
------
Within the Prefetch Abort exception handler I save the state of the CPSR while
changing modes. That is, instead of simply setting the CPSR to ARM_MODE_SVC and
ARM_MODE_ABORT, I first cleared the CPSR mode bits and then ORR'd the CPSR with
the corresponding ARM_MODE_*** mask.

Problems
--------
After building the Data Abort exception handler for Lab 08 the homework
was relatively straight forward. The biggest issue I had was deducing whether or
not it was necessary to leave the bits of the CPSR register intact when
switching modes. I reviewed the ARM7TDMI Reference Manual and David Seal's ARM
ARM, but could not any indication if this was necessary. Therefore, I decided to
play it save and preserve the other bits of the CPSR when changing modes.

Observations
------------
It is still a bit confusing why we have to subtract a word from the LR to get
the address of where the exception actually occurred. Also, it is unclear why
Prefetch and Data exceptions are different in regards to returning from the
exceptions. I believe I just need to spend more time with the documentation and
code.
