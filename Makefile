# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -Wall -Wextra -O2
# Target executable
TARGET = main
# Source and include directories
SRC_DIR = src
CPP_DIR = $(SRC_DIR)/cpp
TEST_DIR = $(SRC_DIR)/tests
# Source files
SRCS = $(SRC_DIR)/main.cpp $(CPP_DIR)/io.cpp $(CPP_DIR)/medoid.cpp $(CPP_DIR)/vamana_indexing.cpp $(CPP_DIR)/greedy_search.cpp $(CPP_DIR)/robust_prune.cpp $(CPP_DIR)/Point.cpp $(CPP_DIR)/Node.cpp
# Test files
TEST_SRCS = $(TEST_DIR)/test_medoid.cpp $(TEST_DIR)/test_greedy_search.cpp $(TEST_DIR)/test_robust_prune.cpp
# Test executables
TEST_EXECUTABLES = $(TEST_DIR)/test_medoid $(TEST_DIR)/test_greedy_search $(TEST_DIR)/test_robust_prune
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
# Rule to build each test executable
$(TEST_DIR)/test_%: $(TEST_DIR)/test_%.cpp $(CPP_DIR)/io.cpp $(CPP_DIR)/medoid.cpp $(CPP_DIR)/vamana_indexing.cpp $(CPP_DIR)/greedy_search.cpp $(CPP_DIR)/robust_prune.cpp $(CPP_DIR)/Point.cpp $(CPP_DIR)/Node.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(CPP_DIR)/io.cpp $(CPP_DIR)/medoid.cpp $(CPP_DIR)/vamana_indexing.cpp $(CPP_DIR)/greedy_search.cpp $(CPP_DIR)/robust_prune.cpp $(CPP_DIR)/Point.cpp $(CPP_DIR)/Node.cpp -I.
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
# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET) $(TEST_EXECUTABLES)