SRC_DIR = .
SRC = main.cpp status.cpp
OBJ = $(SRC:.cpp=.o)
CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -lrt

LDFLAGS = -L/home/fminhtu/dev/github/training/cpp/lib/math -lmath
RPATH = -Wl,-rpath=/home/fminhtu/dev/github/training/cpp/lib/math

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	echo $< $@
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS) $(RPATH)

client: $(OBJ)
	$(CC) $(OBJ) -o $@
