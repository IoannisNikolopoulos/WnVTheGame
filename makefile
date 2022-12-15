# Global definitions
PROG := wnv
BIN := bin
BUILD := build
SRC := src
INCLUDE := include
LIB := lib
RES := res

ifeq ($(OS),Windows_NT)
	EXE := $(PROG).exe
	SRCDIRS := $(SRC)
	INCDIRS := $(INCLUDE)
	LIBDIRS := $(LIB)
else
	EXE := $(PROG)
	SRCDIRS := $(shell find $(SRC) -type d)
	INCDIRS := $(shell find $(INCLUDE) -type d)
	LIBDIRS := $(shell find $(LIB) -type d)
endif

vpath %.cpp $(SRCDIRS)
vpath %.h $(INCDIRS)

# Compiler options
WFLAGS := -Wall

# make sure to compile with x64 on windows and no console window
ifeq ($(OS), Windows_NT)
	CXXFLAGS += -m64 -Wl, -subsystem windows
endif

CXXFLAGS += $(WFLAGS) $(addprefix -I, $(INCDIRS)) -std=c++20

# Linker options
ifeq ($(OS), Windows_NT)
	LDFLAGS += -lmingw -lSDL2main
endif

LDFLAGS += $(addprefix -L, $(LIBDIRS)) -lSDL2 -lSDL2_ttf -lm -Wl,-rpath,'$$ORIGIN/../../lib'

# Source, Object and Dependency definitions
SRCS := $(shell find $(SRC) -name *.cpp -or -name *.c)
OBJS := $(notdir $(patsubst %.cpp, %.o, $(SRCS)))
DEPS := main.cpp

# Debug build settings
DBGDIR := $(BIN)/debug
DBGEXE := $(DBGDIR)/$(EXE)
DBGOBJDIR := $(BUILD)/debug
DBGOBJS := $(addprefix $(DBGOBJDIR)/, $(OBJS))
DBGFLAGS := -g3 -O0

# Release build settings
RELDIR := $(BIN)/release
RELEXE := $(RELDIR)/$(EXE)
RELOBJDIR := $(BUILD)/release
RELOBJS := $(addprefix $(RELOBJDIR)/, $(OBJS))
RELFLAGS := -O3

.PHONY: all relrun run clean debug release remake

# Default build
all: dbgrun

dbgrun: debug
	@./$(DBGEXE)

relrun: release
	@./$(RELEXE)

# Debug rules
debug: $(DBGEXE) $(RES)/arial.ttf

$(DBGEXE): $(DBGOBJS)
	@mkdir -p $(DBGDIR)
	$(CXX) $^ $(LDFLAGS) -o $@

$(DBGOBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DBGFLAGS) -c $< -o $@

# Release rules
release: $(RELEXE) $(RES)/arial.ttf

$(RELEXE): $(RELOBJS)
	@mkdir -p $(RELDIR)
	$(CC) $^ $(LDFLAGS) -o $@

$(RELOBJDIR)/%.o: %.cpp
	@mkdir -p $(RELOBJDIR)
	$(CC) $(CFLAGS) $(RELFLAGS) -c $< -o $@

# Clean and rebulid
remake: clean all

# Clean build files
clean:
	$(RM) -r $(BIN) $(BUILD)
