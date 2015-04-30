CC       = g++
CC_FLAGS = -O3 -Wall

MAIN     = main.cpp

SRC_DIR  = src/
OBJ_DIR  = obj/
SRC      = $(MAIN) $(wildcard $(SRC_DIR)*.cpp)
OBJ      = $(addprefix $(OBJ_DIR),$(SRC:.cpp=.o))
DEP      = $(addprefix $(OBJ_DIR),$(SRC:.cpp=.d))


flipgraph: $(OBJ)
	$(CC) $(CC_FLAGS) $^ -o $@

$(OBJ_DIR)%.o: %.cpp | $(OBJ_DIR)$(SRC_DIR)
	$(CC) $(CC_FLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)$(SRC_DIR): | $(OBJ_DIR)
	mkdir $(OBJ_DIR)$(SRC_DIR)

-include $(DEP)

.PHONY:clean default

clean:
	rm -f -r $(OBJ_DIR)

