CC		:= g++
CFLAGS	:= -std=c++11 -Wall `sdl2-config --cflags --static-libs`
LFLAGS	:= -lSDL2 -lSDL2_image -lcurl

MKDIR	= mkdir
RM		= rm

BIN_NAME := frontend

INC_DIR := include
BIN_DIR := bin
SRC_DIR := src
LIB_DIR := lib
OUT_DIR := output

PLATFORM := $(shell uname -s)
RPI := $(shell grep -o BCM2709 /proc/cpuinfo)

CPP_FILES	:= $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)

ifeq ($(RPI), BCM2709)
	CPP_FILES := $(filter-out src/Context.cpp, $(CPP_FILES))
	CFLAGS += -DRASPBERRY_PI
	LFLAGS += -lEGL -lGLESv2
else
	CPP_FILES := $(filter-out src/Context_Pi.cpp,$(CPP_FILES))
	CFLAGS += -DGLEW_STATIC
	LFLAGS += -lGLEW -lGL
endif

O_FILES		:= $(addprefix $(BIN_DIR)/, $(CPP_FILES:.cpp=.o))
A_FILES		:= $(wildcard $(LIB_DIR)/*.a)

.PHONY: all clean

all: $(OUT_DIR)/$(BIN_NAME) $(O_FILES)
	@$(MKDIR) -p $(OUT_DIR)/dat
	@$(MKDIR) -p $(OUT_DIR)/emulator
	@$(MKDIR) -p $(OUT_DIR)/game
	@$(MKDIR) -p $(OUT_DIR)/logo

$(BIN_DIR)/%.o: %.cpp
	@$(MKDIR) -p $(dir $@)
	$(CC) -c -o $@ $< -I $(INC_DIR) -I $(SRC_DIR) $(CFLAGS)

$(OUT_DIR)/$(BIN_NAME): $(O_FILES)
	@$(MKDIR) -p $(OUT_DIR)
	$(CC) -o $@ $(O_FILES) $(A_FILES) $(CFLAGS) $(LFLAGS)

clean:
	@$(RM) -r $(BIN_DIR)
	@$(RM) $(OUT_DIR)/$(BIN_NAME)
