CC       = g++
CC_FLAGS = -O3 -Wall

MAIN     = main.cpp
TEST     = test.cpp

SRC_DIR  = src/
OBJ_DIR  = obj/

INC_SRC  = $(wildcard $(SRC_DIR)*.cpp)
MAIN_SRC = $(MAIN) $(INC_SRC)
TEST_SRC = $(TEST) $(INC_SRC)
ALL_SRC  = $(MAIN) $(TEST) $(INC_SRC)

MAIN_OBJ = $(addprefix $(OBJ_DIR),$(MAIN_SRC:.cpp=.o))
TEST_OBJ = $(addprefix $(OBJ_DIR),$(TEST_SRC:.cpp=.o))
ALL_DEP  = $(addprefix $(OBJ_DIR),$(ALL_SRC:.cpp=.d))

flipgraph: $(MAIN_OBJ)
	$(CC) $(CC_FLAGS) $^ -o $@

test: $(TEST_OBJ)
	$(CC) $(CC_FLAGS) $^ -o $@

$(OBJ_DIR)%.o: %.cpp | $(OBJ_DIR)$(SRC_DIR)
	$(CC) $(CC_FLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)$(SRC_DIR): | $(OBJ_DIR)
	mkdir $(OBJ_DIR)$(SRC_DIR)

-include $(ALL_DEP)

.PHONY:clean 
clean:
	rm -f -r $(OBJ_DIR)
