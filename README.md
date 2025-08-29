# InfotexTestTask_C

## Архитектура
```
build
headers/
    Client.h     # класс обработчика клиентских команд
    ILogger.h    # интерфейс логгера
    Logger.h     # логгер
    LogLevel.h   # уровни логирования и вспом. ф-ии
    MockLogger.h # мок логгера для тестов
    Print.h      # функции для вывода текста
    Utils.h      # вспомогательные функции
src/
    app.cpp      #  содержит main, консольное приложение
    client.cpp
    logger.cpp
tests/
    test_client.cpp
    test_logger.cpp
```

## Запуск
1) Скопировать репозиторий (пример https)
```
git clone https://github.com/griggg/InfotexTestTask_C.git
```
2) В корне репозитория выполнить make

Появится папка build. 

3) Чтобы запустить консольное приложение, тестирующее библиотеку ввести
```
./build/app
```

## Взаимодействие с приложением

### Инициализация логгера
<img width="1212" height="424" alt="image" src="https://github.com/user-attachments/assets/991a09df-ac69-4e58-aae5-0a74997a943a" />

Нужно указать файл в который будут записываться сообщения и приоритет сообщений (опционально, по умолчанию приоритет=INFO). Сообщения важностью ниже чем приоритет будут игнорироваться. Относительный путь к файлу указывается от корневой директории. 

### Пример работы

<img width="1591" height="922" alt="image" src="https://github.com/user-attachments/assets/9a05861a-6f1c-402a-adec-d8c122da4eb3" />




