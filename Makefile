# Compiler flags for C++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -Iinclude/glad -I.

# Linker flags
LDFLAGS = -Llib -lglfw3 -framework OpenGL -framework CoreFoundation -framework CoreGraphics -framework Cocoa -framework IOKit

# Output target
TARGET = a.out

# Source and object files
SRC := $(shell find src -name "*.cpp") $(wildcard *.cpp)  # Find .cpp files in src and the root folder
OBJ := $(SRC:.cpp=.o) src/glad.o  # Include glad.o explicitly

# Default target
$(TARGET): $(OBJ)
	g++ $(OBJ) -o $(TARGET) $(LDFLAGS)

# Rule for compiling .cpp to .o (using g++)
%.o: %.cpp
	g++ $(CXXFLAGS) -c $< -o $@

# Rule for compiling glad.c to glad.o (using gcc, no C++ flags)
src/glad.o: src/glad.c
	gcc -Wall -Wextra -Iinclude -Iinclude/glad -I. -c src/glad.c -o src/glad.o

.PHONY: test clean

# Test target (runs the program)
test: $(TARGET)
	./$(TARGET)

# Clean target (removes compiled files)
clean:
	rm -f $(TARGET) $(OBJ)
