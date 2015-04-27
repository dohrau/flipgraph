CC       = g++
CC_FLAGS = -O3
MAIN     = main.cpp
SRC_DIR  = src/
OBJ_DIR  = obj/
SRC      = $(wildcard $(SRC_DIR)*.cpp)
OBJ      = $(addprefix $(OBJ_DIR),$(notdir $(SRC:.cpp=.o)))

flipgraph: $(MAIN) $(OBJ)
	$(CC) $(CC_FLAGS) $^ -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp | $(OBJ_DIR)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm -f obj/*.o
