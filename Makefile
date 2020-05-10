SRC_DIR := src
DRC_DIR := $(SRC_DIR)/sandbox
OBJ_DIR := build
DBJ_DIR := $(OBJ_DIR)
BIN_DIR := bin
DIN_DIR := $(BIN_DIR)

SRC := $(wildcard $(SRC_DIR)/*.c)
DRC := $(wildcard $(DRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DBJ := $(DRC:$(DRC_DIR)/%.c=$(DBJ_DIR)/%.o)
BIN := main
DIN := sandbox.so

BININC_DIR := include
DININC_DIR := $(BININC_DIR)
BINLIB_DIR := lib
DINLIB_DIR := $(BINLIB_DIR)

BINCC_FLAG := -I${BININC_DIR} -Wall
DINCC_FLAG := -I${DININC_DIR} -Wall -fPIC

BINLD_FLAG := -L${BINLIB_DIR}
DINLD_FLAG := -L${DINLIB_DIR} -shared -ldl

all: $(BIN_DIR)/$(BIN) $(DIN_DIR)/$(DIN)

$(BIN_DIR)/$(BIN): $(OBJ) | $(BIN_DIR)
	$(CC) $(BINLD_FLAG) $^ -o $@

$(DIN_DIR)/$(DIN): $(DBJ) | $(DIN_DIR)
	$(CC) $(DINLD_FLAG) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(BINCC_FLAG) -c $< -o $@

$(DBJ_DIR)/%.o: $(DRC_DIR)/%.c | $(DBJ_DIR)
	$(CC) $(DINCC_FLAG) -c $< -o $@

$(OBJ_DIR): $(BININC_DIR)
	mkdir $@

$(BIN_DIR): $(BINLIB_DIR)
	mkdir $@

$(BININC_DIR):
	mkdir $@

$(BINLIB_DIR):
	mkdir $@

run:
	$(BIN_DIR)/$(BIN) $(ARG)

clean:
	$(RM) -r $(OBJ_DIR)
	$(RM) -r $(DBJ_DIR)
	$(RM) -r $(BIN_DIR)
	$(RM) -r $(DIN_DIR)

.PHONY: all run clean
