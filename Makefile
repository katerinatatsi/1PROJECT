# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -O2 -w -pthread

# Directories
SRC_DIR = src
CPP_DIR = $(SRC_DIR)/cpp
TEST_DIR = $(SRC_DIR)/tests
HPP_DIR = $(SRC_DIR)/hpp

# Main executables
MAIN_TARGET = main
VAMANA_TARGETS = filtered_vamana_main stitched_vamana_main

# All executables
ALL_TARGETS = $(MAIN_TARGET) $(VAMANA_TARGETS)

# Common source files
COMMON_SRCS = $(CPP_DIR)/io.cpp \
              $(CPP_DIR)/medoid.cpp \
              $(CPP_DIR)/parallelized_vamana_indexing.cpp \
              $(CPP_DIR)/greedy_search.cpp \
              $(CPP_DIR)/robust_prune.cpp \
              $(CPP_DIR)/Point.cpp \
              $(CPP_DIR)/Node.cpp \
              $(CPP_DIR)/evaluate.cpp \
              $(CPP_DIR)/shared_vamana.cpp

# Main program sources
MAIN_SRCS = $(SRC_DIR)/main.cpp $(COMMON_SRCS)

# Test files
TEST_SRCS = $(TEST_DIR)/test_medoid.cpp \
            $(TEST_DIR)/test_greedy_search.cpp \
            $(TEST_DIR)/test_robust_prune.cpp

# Test executables
TEST_EXECUTABLES = $(TEST_DIR)/test_medoid \
                  $(TEST_DIR)/test_greedy_search \
                  $(TEST_DIR)/test_robust_prune

# Object files
COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)

# Include directories
INCLUDES = -I$(SRC_DIR) -I$(HPP_DIR) -I.

# Default target
all: $(ALL_TARGETS)

# Rule for main program
$(MAIN_TARGET): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(MAIN_OBJS)

# Rule for filtered_vamana
filtered_vamana_main: $(SRC_DIR)/filtered_vamana_main.cpp $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# Rule for stitched_vamana
stitched_vamana_main: $(SRC_DIR)/stitched_vamana_main.cpp $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# Rule to compile .cpp files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rule to build each test executable
$(TEST_DIR)/test_%: $(TEST_DIR)/test_%.cpp $(COMMON_SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# Target to build all tests
tests: $(TEST_EXECUTABLES)

# Target to run all tests
run_tests: tests
	@echo "Running test_medoid"
	./$(TEST_DIR)/test_medoid
	@echo "Running test_greedy_search"
	./$(TEST_DIR)/test_greedy_search
	@echo "Running test_robust_prune"
	./$(TEST_DIR)/test_robust_prune

# Run Vamana executables in parallel
run_vamana: filtered_vamana_main stitched_vamana_main
	./filtered_vamana_main & \
	./stitched_vamana_main 

# Clean up build files
clean:
	rm -f $(COMMON_OBJS) $(MAIN_OBJS) $(ALL_TARGETS) $(TEST_EXECUTABLES)

.PHONY: all clean run_vamana run_tests tests