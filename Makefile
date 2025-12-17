# Makefile for Optimized Dino Game

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
INCLUDES = -I"E:/CLion 2025.2.2/bin/mingw/include"
LIBS = -L"E:/CLion 2025.2.2/bin/mingw/lib" -lgraphics -lgdi32 -luser32 -lkernel32 -lgdiplus

# Target executable
TARGET = dino_game.exe

# Source files
SRCS = src/OptimizedMain.cpp src/OptimizedDinoGame.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build files
clean:
	del *.exe *.o 2>nul || true

# Rebuild everything
rebuild: clean all

.PHONY: all clean rebuild