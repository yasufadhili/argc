
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++23
LDFLAGS = -lfl


TARGET = arc


FLEX_SRC = lexer.l
FLEX_CPP = lexer.cc
MAIN_SRC = main.cc
OBJECTS = $(FLEX_CPP:.cc=.o) $(MAIN_SRC:.cc=.o)


all: $(TARGET)

# Generate lexer.cc from lexer.l using Flex
$(FLEX_CPP): $(FLEX_SRC)
	flex++ $(FLEX_SRC)

# Compile lexer.cc
$(FLEX_CPP:.cc=.o): $(FLEX_CPP)
	$(CXX) $(CXXFLAGS) -c $(FLEX_CPP) -o $@

# Compile main.cc
$(MAIN_SRC:.cc=.o): $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -c $(MAIN_SRC) -o $@

# Link object files into the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Test the compiler (requires a test file)
test: $(TARGET)
	@echo "Running compiler on test.c..."
	@./$(TARGET) test.c

# Create a sample test file if it doesn't exist
test.c:
	@echo "Creating sample test file..."
	@echo '// Sample test code\n' > test.c
	@echo 'int main() {\n' >> test.c
	@echo '    float x = 3.14;\n' >> test.c
	@echo '    int count = 42;\n' >> test.c
	@echo '    /* This is a multi-line\n' >> test.c
	@echo '       comment block */\n' >> test.c
	@echo '    string message = "Hello, world!";\n' >> test.c
	@echo '    return 0;\n' >> test.c
	@echo '}\n' >> test.c

# Clean up generated files
clean:
	rm -f $(TARGET) $(OBJECTS) $(FLEX_CPP) test.c

# Clean everything and rebuild
rebuild: clean all

# Show help
help:
	@echo "Makefile for argon compiler project"
	@echo ""
	@echo "Available targets:"
	@echo "  all       - Build the compiler (default)"
	@echo "  clean     - Remove generated files"
	@echo "  rebuild   - Clean and rebuild everything"
	@echo "  test      - Run the compiler on test.c"
	@echo "  test.c    - Generate a sample test file"
	@echo "  help      - Show this help message"

.PHONY: all clean rebuild test help