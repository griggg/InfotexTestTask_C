CFLAGS = -Wall -Werror -Wextra -std=c++17 -Iheaders
CC = g++

BUILD_DIR = build
SRC_DIR = src
TEST_DIR = tests

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

TEST_CLIENT_SRC := $(TEST_DIR)/test_client.cpp
TEST_LOGGER_SRC := $(TEST_DIR)/test_logger.cpp

APP_TARGET = $(BUILD_DIR)/app
LOGGER_LIB = $(BUILD_DIR)/liblogger.so
TEST_CLIENT = $(BUILD_DIR)/test_client
TEST_LOGGER = $(BUILD_DIR)/test_logger

.PHONY: all clean logger app test_logger test_client test_valgrind client_sntz

all: $(BUILD_DIR) logger $(APP_TARGET) test_logger test_client

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

$(LOGGER_LIB): $(BUILD_DIR)/logger.o
	$(CC) -shared $^ -o $@

logger: $(LOGGER_LIB)

$(APP_TARGET): $(BUILD_DIR)/app.o $(BUILD_DIR)/client.o $(LOGGER_LIB)
	$(CC) $(CFLAGS) $^ -L$(BUILD_DIR) -llogger -Wl,-rpath=$(BUILD_DIR) -o $@

app: $(APP_TARGET)

$(TEST_LOGGER): $(TEST_LOGGER_SRC) $(BUILD_DIR)/logger.o
	$(CC) $(CFLAGS) $^ -o $@

test_logger: $(TEST_LOGGER)

$(TEST_CLIENT): $(TEST_CLIENT_SRC) $(BUILD_DIR)/client.o $(LOGGER_LIB)
	$(CC) $(CFLAGS) $^ -L$(BUILD_DIR) -llogger -Wl,-rpath=$(BUILD_DIR) -o $@

test_client: $(TEST_CLIENT)

client_sntz: $(SRC_DIR)/client.cpp $(LOGGER_LIB)
	$(CC) $(CFLAGS) $^ -L$(BUILD_DIR) -llogger -Wl,-rpath=$(BUILD_DIR) -fsanitize=thread -o $(BUILD_DIR)/client_sntz

clean:
	rm -rf $(BUILD_DIR) *.txt

test_valgrind: test_logger test_client
	@echo "Запуск тестов logger под Valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 $(TEST_LOGGER)
	@echo "Запуск тестов client под Valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 $(TEST_CLIENT)
	@echo "Valgrind: все тесты прошли успешно без ошибок памяти."
