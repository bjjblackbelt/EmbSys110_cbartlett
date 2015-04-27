CC = g++

# List command line functions
RM = rm -fr

# List all default C defines here, like -D_DEBUG=1
WARNINGS = -Wall -Wextra
DDEFS = -g3 $(WARNINGS)

# Define project name
PROJECT = RunAllTests

# Define CppUTest home
CPPUTEST_HOME = ../../CppUTest

# List all user C define here, like -D_DEBUG=1
UDEFS =

# List Application source files here
SRC  = \
	../prj/CriticalSection.cpp

# List Test source files here
TSRC  = \
	RunAllTests.cpp \
	TestTemplate.cpp \
	TestCriticalSectionAPI.cpp

# List all user directories here
UINCDIR = \
	. \
	${CPPUTEST_HOME}/include/CppUTest \
	../prj

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
	$(CC) $(SRC) $(TSRC) $(LIBDIR) $(LIBS) $(CPFLAGS) $(INCDIR) -o $(PROJECT).sh
	./$(PROJECT).sh

