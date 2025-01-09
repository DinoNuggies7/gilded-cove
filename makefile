CCACHE = ccache
CFLAGS = -Wall -Ofast -I./raylib/src/
LDFLAGS = -Ofast -lm -lcjson -L./raylib/src/ -lraylib
OUT = gilded-cove

SRC_DIR = src
BUILD_DIR = build

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

all: clean $(OBJ_FILES)
	$(CCACHE) $(CC) $(BUILD_DIR)/*.o $(LDFLAGS) -o $(OUT) $(EXFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CCACHE) $(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o $(OUT) $(OUT).exe
	mkdir -p $(BUILD_DIR)

.PHONY: clean
