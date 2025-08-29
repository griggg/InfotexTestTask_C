CFLAGS = -Wall -Werror -Wextra -std=cpp17
CC = g++

.PHONY: all clean client logger test_logger

BUILD_DIR = build
SRC_DIR = src
TEST_DIR = tests

all: client test_logger

$(BUILD_DIR)/liblogger.so : $(SRC_DIR)/logger.cpp
	$(CC) -fPIC -shared $< -o $(BUILD_DIR)/liblogger.so 
	
client: $(SRC_DIR)/client.cpp $(BUILD_DIR)/liblogger.so
	$(CC) $^ -Lbuild -llogger -Wl,-rpath=build -o $(BUILD_DIR)/$@

test_logger: $(SRC_DIR)/logger.cpp $(TEST_DIR)/test_logger.cpp
	$(CC) $^ -o $(BUILD_DIR)/$@

clean:
	rm -r build/*

client_sntz: $(SRC_DIR)/client.cpp $(BUILD_DIR)/liblogger.so
	$(CC) $^ -Lbuild -llogger -Wl,-rpath=build -o $(BUILD_DIR)/$@ -fsanitize=thread