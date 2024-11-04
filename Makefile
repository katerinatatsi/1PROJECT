# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -O2
#LDFLAGS = -lstdc++

# Target executable
TARGET = main

# Source and include directories
SRC_DIR = src
CPP_DIR = $(SRC_DIR)/cpp

# Source files
SRCS = $(SRC_DIR)/main.cpp $(CPP_DIR)/read_dataset.cpp $(CPP_DIR)/vamana_indexing.cpp $(CPP_DIR)/greedy_search.cpp $(CPP_DIR)/robust_prune.cpp $(CPP_DIR)/Point.cpp $(CPP_DIR)/Node.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Rule to compile .cpp files in the src directory into object files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)