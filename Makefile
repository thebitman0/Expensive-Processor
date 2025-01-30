CC := gcc
CFLAGS := -s -O3
TARGET := exproc
SRC_DIR := src/
OBJ_DIR := obj/
LIBS := -lb
SRCS := $(wildcard $(SRC_DIR)*.c)
OBJS := $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

compile: $(OBJS)

link: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(TARGET)

all: compile link

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
