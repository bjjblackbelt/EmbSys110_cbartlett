TEMPLATEROOT = .

vpath %.cpp $(TEMPLATEROOT)/App
vpath %.cpp $(TEMPLATEROOT)/Lib

# object files

LIBOBJS = \
	stm32f10x_gpio.o \
	stm32f10x_rcc.o \
	stm32f10x_usart.o \
	CriticalSection.o

SRCOBJS= \
	main.o

OBJS = $(LIBOBJS) $(SRCOBJS)

# compilation flags for gdb

CFLAGS  = -O0 -g

CPPFLAGS = \
	-fno-rtti \
	-fno-exceptions \
	-fmessage-length=0 \
	-mthumb-interwork \
	-mlong-calls \
	-fomit-frame-pointer

ASFLAGS = -g

# warning flags

CWARNINGS   =-Wall \
		 	 -Wcast-align \
    	 	 -Wdisabled-optimization \
    	 	 -Wfloat-equal -Wformat=2 \
    	 	 -Wformat-nonliteral -Wformat-security \
    	 	 -Wformat-y2k \
		 	 -Wimport -Winit-self -Winline \
    	 	 -Winvalid-pch \
    	 	 -Wunsafe-loop-optimizations -Wlong-long -Wmissing-braces \
    	 	 -Wmissing-field-initializers -Wmissing-format-attribute \
    	 	 -Wmissing-include-dirs \
    	 	 -Wpacked -Wpointer-arith \
    	 	 -Wredundant-decls -Wreturn-type \
    	 	 -Wshadow -Wstack-protector \
    	 	 -Wstrict-aliasing=2 -Wswitch-default \
    	 	 -Wswitch-enum \
    	 	 -Wunreachable-code \
    	 	 -Wvariadic-macros \
    	 	 -Wwrite-strings \
    	 	 -Wunused -Wunused-parameter \
			 -Wno-padded
			 # -Werror
		 	 #-Wconversion \
			 #-Wsign-conversion \
    	 	 #-Wcast-qual \
		 	 #-Wextra \

CPPWARNINGS	=-Weffc++ -Woverloaded-virtual


# include common make file

include ./Makefile.common


