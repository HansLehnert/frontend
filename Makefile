CC		:= g++
CFLAGS	:= -std=c++11 -Wall -DGLEW_STATIC
LFLAGS	:= -lGL -lSDL2 -lGLEW -lSOIL -lcurl

MKDIR	= mkdir
RM		= rm

BIN_NAME := frontend

INC_DIR := include
BIN_DIR := bin
SRC_DIR := src
LIB_DIR := lib
OUT_DIR := output

PLATFORM := $(shell uname -s)

CPP_FILES	:= $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)
O_FILES		:= $(addprefix $(BIN_DIR)/, $(CPP_FILES:.cpp=.o))
A_FILES		:= $(wildcard $(LIB_DIR)/*.a)

.PHONY: all clean

all: $(OUT_DIR)/$(BIN_NAME) $(O_FILES)

$(BIN_DIR)/%.o: %.cpp
	@$(MKDIR) -p $(dir $@)
	$(CC) -c -o $@ $< -I $(INC_DIR) -I $(SRC_DIR) $(CFLAGS)

$(OUT_DIR)/$(BIN_NAME): $(O_FILES)
	@$(MKDIR) -p $(OUT_DIR)
	$(CC) -o $@ $(O_FILES) $(A_FILES) $(CFLAGS) $(LFLAGS)

clean:
	@$(RM) -r $(BIN_DIR)
	@$(RM) $(OUT_DIR)/$(BIN_NAME)
