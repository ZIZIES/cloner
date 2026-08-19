CC = gcc
CFLAGS = -Wall -Wextra -g
HEADERS = src/cloner.h
BUILD_DIR = build
SRCS = src/main.c src/posix.c src/windows.c src/util.c

$(BUILD_DIR)/cloner: $(SRCS) $(HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

run: $(BUILD_DIR)/cloner
	./$(BUILD_DIR)/cloner

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean run