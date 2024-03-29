#################################
##### Linux Makefile
#################################

##### Compiler
COMPILER = g++
##### Remove directories
RMDIR = rm -rdf
##### Remove files
RM = rm -f

##### Automatic dependency creation
DEP_FLAGS = -M -MT $@ -MT $(BIN_PATH)/$(*F).o -MP -MF $@
##### Linked libraries
LIBS = -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_imgcodecs
##### Include path
OPENCV_PATH = /usr/local/include/opencv4
INC_PATHS = -I$(INC_PATH) -I$(OPENCV_PATH)

##### Compile directives
FLAGS = -std=c++17 -Wall -pedantic -Wextra -Werror=init-self
##### Extra debugging directives
DFLAGS = -ggdb -O0 -DDEBUG -g
##### Extra release directives
RFLAGS = -O3 -mtune=native

INC_PATH = include
SRC_PATH = src
BIN_PATH = bin
DEP_PATH = dep

##### File list by type
CPP_FILES = $(wildcard $(SRC_PATH)/*.cpp)
INC_FILES = $(wildcard $(INC_PATH)/*.h)
FILE_NAMES = $(sort $(notdir $(CPP_FILES:.cpp=)) $(notdir $(INC_FILES:.h=)))
DEP_FILES = $(addprefix $(DEP_PATH)/,$(addsuffix .d,$(FILE_NAMES)))
OBJ_FILES = $(addprefix $(BIN_PATH)/,$(notdir $(CPP_FILES:.cpp=.o)))

##### Executable name
EXEC = program.out

.PRECIOUS: $(DEP_FILES)
.PHONY: release debug cppcheck clean folders help execute valgrind

##### Generates executable
$(EXEC): $(OBJ_FILES)
	$(COMPILER) -o $@ $^ $(LINK_PATH) $(LIBS) $(FLAGS)

##### Generates object files
$(BIN_PATH)/%.o: $(DEP_PATH)/%.d | folders
	$(COMPILER) $(INC_PATHS) $(addprefix $(SRC_PATH)/,$(notdir $(<:.d=.cpp))) -c $(FLAGS) -o $@

##### Generates dependency files
$(DEP_PATH)/%.d: $(SRC_PATH)/%.cpp | folders
	$(COMPILER) $(INC_PATHS) $< $(DEP_FLAGS) $(FLAGS)

clean:
	-$(RMDIR) $(DEP_PATH)
	-$(RMDIR) $(BIN_PATH)
	-$(RM) $(EXEC)

execute:
	./$(EXEC)

##### Call for cppcheck
cppcheck:
	cppcheck --enable=all . -I./include --suppress=missingIncludeSystem

##### Call for valgrind
valgrind:
	valgrind -s --suppressions=/usr/local/share/opencv4/valgrind.supp --suppressions=/usr/local/share/opencv4/valgrind_3rdparty.supp --leak-check=full --show-leak-kinds=all ./$(EXEC)

release: FLAGS += $(RFLAGS)
release: $(EXEC)

debug: FLAGS += $(DFLAGS)
debug: $(EXEC)

folders:
	@mkdir -p $(DEP_PATH) $(BIN_PATH) $(INC_PATH) $(SRC_PATH)

##### Debugging rules
print-% : ; @echo $* = $($*)

help:
	@echo Available commands
	@echo - release:  Builds release version
	@echo - debug:    Builds debug version
	@echo - clean:    Cleans generated files
	@echo - cppcheck: Uses cppcheck on compiled objects
	@echo - valgrind: Uses valgrind on linked programs
	@echo - execute:  Executes the game
	@echo - help:     Shows help
	@echo - folders:  Generates project directories

.SECONDEXPANSION:
-include $$(DEP_FILES)
