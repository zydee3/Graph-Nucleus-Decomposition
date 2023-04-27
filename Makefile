# march=native: host processor architecture specific optimizations.



# Compilers
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g -O3 -march=native -gdwarf-4

# Main Directories
BASE_DIR = src
OBJS_DIR = bin
TEST_DIR = test

# Sub Directories
ALGS_DIR = $(BASE_DIR)/algorithms
COLS_DIR = $(BASE_DIR)/collections
UTIL_DIR = $(BASE_DIR)/utilities

# Source Files
MAIN_SRCS = $(wildcard $(BASE_DIR)/*.c)
ALGS_SRCS = $(wildcard $(ALGS_DIR)/*.c)
COLS_SRCS = $(wildcard $(COLS_DIR)/*.c)
UTIL_SRCS = $(wildcard $(UTIL_DIR)/*.c)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)

# Object Files
MAIN_OBJS = $(MAIN_SRCS:$(BASE_DIR)/%.c=$(OBJS_DIR)/%.o)
ALGS_OBJS = $(ALGS_SRCS:$(ALGS_DIR)/%.c=$(OBJS_DIR)/%.o)
COLS_OBJS = $(COLS_SRCS:$(COLS_DIR)/%.c=$(OBJS_DIR)/%.o)
UTIL_OBJS = $(UTIL_SRCS:$(UTIL_DIR)/%.c=$(OBJS_DIR)/%.o)
TEST_OBJS = $(TEST_SRCS:$(TEST_DIR)/%.c=$(OBJS_DIR)/%.o)

# Executables
MAIN_EXEC = $(OBJS_DIR)/graphs
TEST_EXEC = $(OBJS_DIR)/tests

all: $(MAIN_EXEC) $(TEST_EXEC)

$(MAIN_EXEC): $(MAIN_OBJS) $(ALGS_OBJS) $(COLS_OBJS) $(UTIL_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(TEST_EXEC): $(TEST_OBJS) $(ALGS_OBJS) $(COLS_OBJS) $(UTIL_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJS_DIR)/%.o: $(BASE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/%.o: $(ALGS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/%.o: $(COLS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/%.o: $(UTIL_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -f $(OBJS_DIR)/*.o $(MAIN_EXEC) $(TEST_EXEC)