CFLAGS = -Wall -Werror -Wextra -std=cpp17
CC = g++

.PHONY: all clean client logger test_logger

BUILD_DIR = build
SRC_DIR = src
TEST_DIR = tests

all: client logger test_logger

logger: $(SRC_DIR)/logger.cpp

client: $(SRC_DIR)/client.cpp $(SRC_DIR)/logger.cpp
	$(CC) $^ -o $(BUILD_DIR)/$@

test_logger: $(SRC_DIR)/logger.cpp $(TEST_DIR)/test_logger.cpp
	$(CC) $^ -o $(BUILD_DIR)/$@

clean:
	rm -r build/*