
TARGET = main


CXX = g++


BUILD_DIR = .build
BIN1_DIR = ./bin1

SCR = $(shell find $(BIN1_DIR) -type f -name *.cpp)
OBJ = $(BIN1_DIR)/%.o: $(BIN1_DIR)/%.cpp

bin1: $(OBJ)
	@mkdir -p $(BUILD_DIR)/bin1
	$(CXX) $(OBJ) -o $(BUILD_DIR)$(BIN1_DIR)


static: main
	g++ main.cpp -Llib -l:staticlib.a -o main

dynamic: main
	g++ main.cpp  -o main

