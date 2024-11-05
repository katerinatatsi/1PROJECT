# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -O2

# Target executable for main
TARGET = main

# Source and include directories
SRC_DIR = src
CPP_DIR = $(SRC_DIR)/cpp
TEST_DIR = $(SRC_DIR)/tests

# Source files for main
SRCS = $(SRC_DIR)/main.cpp $(CPP_DIR)/read_dataset.cpp $(CPP_DIR)/vamana_indexing.cpp $(CPP_DIR)/greedy_search.cpp $(CPP_DIR)/robust_prune.cpp $(CPP_DIR)/Point.cpp $(CPP_DIR)/Node.cpp

# Object files for main
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Rule to build the main target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Rule to compile .cpp files in the src directory into object files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target for the test_graph executable
test_graph: $(TEST_DIR)/test_graph.cpp $(CPP_DIR)/Point.cpp $(CPP_DIR)/vamana_indexing.cpp $(CPP_DIR)/Node.cpp $(CPP_DIR)/robust_prune.cpp $(CPP_DIR)/greedy_search.cpp
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_graph.cpp $(CPP_DIR)/Point.cpp $(CPP_DIR)/vamana_indexing.cpp $(CPP_DIR)/Node.cpp $(CPP_DIR)/robust_prune.cpp $(CPP_DIR)/greedy_search.cpp -I. -o $(TEST_DIR)/test_graph

# Target for the medoid executable
test_medoid: $(TEST_DIR)/test_medoid.cpp $(CPP_DIR)/Point.cpp $(CPP_DIR)/vamana_indexing.cpp $(CPP_DIR)/Node.cpp $(CPP_DIR)/robust_prune.cpp $(CPP_DIR)/greedy_search.cpp
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_medoid.cpp $(CPP_DIR)/Point.cpp $(CPP_DIR)/vamana_indexing.cpp $(CPP_DIR)/Node.cpp $(CPP_DIR)/robust_prune.cpp $(CPP_DIR)/greedy_search.cpp -I. -o $(TEST_DIR)/test_medoid

# Target for the robust prune executable
test_robustprune: $(TEST_DIR)/test_robustprune.cpp $(CPP_DIR)/Point.cpp $(CPP_DIR)/vamana_indexing.cpp $(CPP_DIR)/Node.cpp $(CPP_DIR)/robust_prune.cpp $(CPP_DIR)/greedy_search.cpp
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_robustprune.cpp $(CPP_DIR)/Point.cpp $(CPP_DIR)/vamana_indexing.cpp $(CPP_DIR)/Node.cpp $(CPP_DIR)/robust_prune.cpp $(CPP_DIR)/greedy_search.cpp -I. -o $(TEST_DIR)/test_robustprune

# Target for the greedy_search executable
test_greedy_search: $(TEST_DIR)/test_greedy_search.cpp $(CPP_DIR)/Point.cpp $(CPP_DIR)/Node.cpp $(CPP_DIR)/greedy_search.cpp
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_greedy_search.cpp $(CPP_DIR)/Point.cpp $(CPP_DIR)/Node.cpp $(CPP_DIR)/greedy_search.cpp -I. -o $(TEST_DIR)/test_greedy_search

# Target to run the test_graph executable
run_test_graph: test_graph
	./$(TEST_DIR)/test_graph

# Target to run the test_medoid executable
run_test_medoid: test_medoid
	./$(TEST_DIR)/test_medoid

# Target to run the test_robustprune executable
run_test_robustprune: test_robustprune
	./$(TEST_DIR)/test_robustprune

# Target to run the test_greedy_search executable
run_test_greedy_search: test_greedy_search
	./$(TEST_DIR)/test_greedy_search

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET) $(TEST_DIR)/test_graph $(TEST_DIR)/test_medoid $(TEST_DIR)/test_robustprune $(TEST_DIR)/test_greedy_search