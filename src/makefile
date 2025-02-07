# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++17 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Target executable
TARGET = main

# Source files
SRC = main.cpp Particle.cpp CircleBorder.cpp RectBorder.cpp Solver.cpp Spawner.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Rules
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(TARGET) $(OBJ)

