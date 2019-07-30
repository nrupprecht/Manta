# Makefile snatched from <https://ubuntuforums.org/showthread.php?t=1204739>

COMPILER ?= 1
USE_MPI  ?= 0
DEBUG    ?= 0
NO_SIMD  ?= 0
DO_OPTIMIZE ?= 0

# Add flags to this variable as neccessary
FLAGS = 

# Compiler options
ifeq ($(COMPILER), 0)
CC = icpc
FLAGS += -D _INTEL_=1
else ifeq ($(COMPILER), 1)
CC = g++
FLAGS += -D _CLANG_=1
else
CC = clang++
FLAGS += -D _CLANG_=1
endif 

ifeq ($(DEBUG), 1)
FLAGS += -D DEBUG=1
endif

ifeq ($(NO_SIMD), 1)
FLAGS += -D SIMD_TYPE=0
endif

ifeq ($(CPARSE), 1)
FLAGS += -D CPARSE=1
endif

# Name of the application to compile
APP   = compile
# Directories for source, object, bin files
SRC   = src
OBJ   = obj
BIN   = bin

EXCLUDE = 

SRCS1   := $(shell find $(SRC) -name '*.cpp')
SRCS    := $(filter-out $(EXCLUDE), $(SRCS1))
SRCDIRS := $(shell find . -name '*.cpp' -exec dirname {} \; | uniq)
OBJS    := $(patsubst %.cpp,$(OBJ)/%.o,$(SRCS))

CSTD     = -std=c++11
DEBUG    = -g
ifeq ($(COMPILER), 0) 
ifeq ($(DO_OPTIMIZE), 1)
OPTIMIZE := -O3 -funroll-loops -no-prec-div -restrict -no-inline-max-size -ffast-math
endif
else
ifeq ($(DO_OPTIMIZE), 1)
OPTIMIZE := -O3
endif
endif
CFLAGS   := -c $(CSTD) $(DEBUG) $(OPTIMIZE) $(FLAGS)

all: $(BIN)/$(APP)

# Executable

$(BIN)/$(APP): obj/src/$(APP).o $(OBJS)
	@mkdir -p `dirname $@`
	@echo "Linking $@..."
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# General object files

$(OBJ)/%.o: %.cpp
	@mkdir -p `dirname $@`
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $< -o $@

.PHONY : clean
clean:
	rm -r -f $(OBJ) $(BIN)/*
