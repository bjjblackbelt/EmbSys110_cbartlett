TEMPLATEROOT = .

vpath %.cpp $(TEMPLATEROOT)/App
vpath %.cpp $(TEMPLATEROOT)/Lib

# object files

LIBOBJS = \
    CriticalSection.o \
	Interrupts.o \
    IUart.o \
	mini_cpp.o \
	no_heap.o \
	Threads.o \
	DUart.o \
	Bsp.o \
	OS.o


SRCOBJS= \
	main.o

OBJS = $(LIBOBJS) $(SRCOBJS)

# compilation flags for gdb

CFLAGS  = \
	-O0 -g \
	-fno-exceptions

CPPFLAGS = \
	-fno-rtti \
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


