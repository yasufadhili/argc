###############################################################################
# Makefile for the Argon Compiler
###############################################################################

ROOT_DIR = $(shell pwd)

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++23 -I$(ROOT_DIR)

# Flex and Bison
FLEX = flex
BISON = bison

# Directories
SRC_DIR = $(ROOT_DIR)/src
INCLUDE_DIR = include
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Make sure build directories exist
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

TARGET = $(BIN_DIR)/argc

# Source files
BISON_SRC = $(SRC_DIR)/parser.yy
FLEX_SRC = $(SRC_DIR)/lexer.ll
BISON_CC = $(SRC_DIR)/parser.cc
BISON_HH = $(INCLUDE_DIR)/parser.hh
FLEX_CC = $(SRC_DIR)/lexer.cc

# Find all C++ source files (excluding generated files)
CC_SRC = $(filter-out $(BISON_CC) $(FLEX_CC), $(wildcard $(SRC_DIR)/*.cc))

# Object files
BISON_OBJ = $(OBJ_DIR)/parser.o
FLEX_OBJ = $(OBJ_DIR)/lexer.o
CC_OBJ = $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(CC_SRC))

# All object files
OBJ_FILES = $(BISON_OBJ) $(FLEX_OBJ) $(CC_OBJ)

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJ_FILES)
	@echo "Linking $(TARGET)"
	@$(CXX) $(CXXFLAGS) -o $@ $^

# Bison rules
$(BISON_CC) $(BISON_HH): $(BISON_SRC)
	@echo "Generating parser with Bison"
	@$(BISON) -d -o $(BISON_CC) $<
	#@$(BISON) -Wcounterexamples -d -o $(BISON_CC) $<

# Flex rules (depends on Bison output)
$(FLEX_CC): $(FLEX_SRC) $(BISON_HH)
	@echo "Generating lexer with Flex"
	@$(FLEX) --outfile=$@ $<

# Compile Bison output
$(BISON_OBJ): $(BISON_CC)
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

# Compile Flex output
$(FLEX_OBJ): $(FLEX_CC)
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

# Compile regular C++ files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean target
clean:
	@echo "Cleaning build artifacts"
	@rm -f $(TARGET) $(OBJ_FILES) $(BISON_CC) $(BISON_HH) $(FLEX_CC)

# Very clean target - remove all build directories
distclean: clean
	@echo "Removing build directories"
	@rm -rf $(BUILD_DIR)
	@rm -f ./src/parser.cc ./src/parser.hh ./src/location.hh

# Rebuild target
rebuild: clean all

# Generate dependencies
depend: $(BISON_CC) $(FLEX_CC) $(CC_SRC)
	@echo "Generating dependencies"
	@$(CXX) $(CXXFLAGS) -MM $^ > .depend

# Include dependency file if it exists
-include .depend

# Run the compiler
run: $(TARGET)
	@echo "Running $(TARGET)"
	@$(TARGET)

# Phony targets
.PHONY: all clean distclean rebuild depend run
