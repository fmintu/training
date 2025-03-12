SRC_DIR = .
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
CC = g++
CFLAGS = -Wall -Wextra -std=c++11

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	# echo $< $@
	$(CC) $(CFLAGS) -c $< -o $@

server: $(OBJ)
	$(CC) $(OBJ) -o $@
