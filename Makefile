CFLAGS = -Wall -Werror -Wextra -std=cpp17 -Iheaders
CC = g++

BUILD_DIR = build
SRC_DIR = src
TEST_DIR = tests

.PHONY: app all clean client logger test_logger test_client 

all: app logger test_logger test_client

$(BUILD_DIR)/liblogger.so : $(SRC_DIR)/logger.cpp
	$(CC) -fPIC -shared $< -o $(BUILD_DIR)/liblogger.so 

logger: $(BUILD_DIR)/liblogger.so

app: $(SRC_DIR)/app.cpp $(SRC_DIR)/client.cpp $(BUILD_DIR)/liblogger.so
	$(CC) $^ -Lbuild -llogger -Wl,-rpath=build -o $(BUILD_DIR)/$@

test_logger: $(SRC_DIR)/logger.cpp $(TEST_DIR)/test_logger.cpp
	$(CC) $^ -o $(BUILD_DIR)/$@

test_client: $(SRC_DIR)/client.cpp $(BUILD_DIR)/liblogger.so $(TEST_DIR)/test_client.cpp
	$(CC) $^ -Lbuild -llogger -Wl,-rpath=build -o $(BUILD_DIR)/$@

clean:
	rm -r build/*
	rm *.txt

client_sntz: $(SRC_DIR)/client.cpp $(BUILD_DIR)/liblogger.so
	$(CC) $^ -Lbuild -llogger -Wl,-rpath=build -o $(BUILD_DIR)/$@ -fsanitize=thread