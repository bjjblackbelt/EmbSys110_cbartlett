CC = g++

# List command line functions
RM = rm -fr

# List all default C defines here, like -D_DEBUG=1
DDEFS = -g3 $(WARNINGS)

# Define project name
PROJECT = RunAllTests

include ../Makefile.Includes

# Define CppUTest home
CPPUTEST_HOME = $(WORKSPACEROOT)/CppUTest

# List all user C define here, like -D_DEBUG=1
UDEFS = \
	-DSTM32F10X_MD_VL \
	-fno-permissive \
	-Wno-attributes \
	-Wno-int-to-pointer-cast

# List Application source files here
SRC  = \
	../Lib/CriticalSection.cpp \
	../Lib/IUart.cpp \
	../App/Threads.cpp \
	../App/OS.cpp

# List Test source files here
TSRC  = \
	Stubs/StubBsp.cpp \
	Stubs/StubDTimer.cpp \
	RunAllTests.cpp \
	TestTemplate.cpp \
	TestIUart.cpp \
	TestCriticalSectionAPI.cpp \
	TestOS.cpp

# List all user directories here
UINCDIR = \
	${CPPUTEST_HOME}/include/CppUTest \
	$(STDINCDIR) \
	./Stubs \
	./ \
	../Lib \
	../App \
	../Bsp

# List the user directory to look for the libraries here
ULIBDIR = ${CPPUTEST_HOME}/lib

# List all user libraries here
ULIBS = CppUTest

# Define optimization level here -O0 -O1 -O2 -Os -03
OPT = -O0

INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))
LIBDIR  = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))
DEFS    = $(DDEFS) $(UDEFS)
OBJS    = $(SRC:.cpp=.o) $(TSRC:.cpp=.o)
LIBS    = $(patsubst %,-l%,$(ULIBS))

CPFLAGS = $(OPT) -fno-rtti -fno-exceptions -fmessage-length=0 -fomit-frame-pointer $(DEFS)

all:
	@$(CC) $(SRC) $(TSRC) $(LIBDIR) $(LIBS) $(CPFLAGS) $(INCDIR) -o $(PROJECT).sh
	@./$(PROJECT).sh

.PHONY: clean
clean:
	@$(RM) ./$(PROJECT).sh

.PHONY: print
print:
	@echo "INCDIR: "
	@echo $(INCDIR)

