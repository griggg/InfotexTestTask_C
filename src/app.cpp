#include "../headers/client.h"
#include "../headers/print.h"


void print_menu() {
    print("------------");
	print("Меню логгера");
	print(
		"1) log <сообщение> <важность=INFO/WARNING/ERROR> (по умолч. INFO) - "
		"Записать сообщение");
	print("2) cdp <INFO/WARNING/ERROR> - Сменить уровень приоритета у логгера");
	print("3) exit - завершить работу");
    print("4) help - показать команды");
	print("------------");
}

int main() {
	std::cout << "Инициализация " << "Logger" << std::endl;
	std::cout << "Напишите <имя файла логгера> <приоритет=" 
        << GREEN("INFO") << "/" 
        << YELLOW("WARNING") << "/" 
        << RED("ERROR") << "> (по умолч. INFO)"
        << std::endl;

    int successInit = 0;

    Client client;
    std::string line;

    while (!successInit) {
        try {
            std::getline(std::cin, line);
            client.loggerInit(line);
            successInit = 1;
        } catch (std::runtime_error &e) {
            print(e.what());
            print("Попробуйте другой путь");
        }
    }
	

	print_menu();

	while (1) {
		std::getline(std::cin, line);
		std::vector<std::string> args = split(line, ' ');
		if (args.size() == 0) {
			// print("не получено аргументов");
			continue;
		}
		if (args[0] == "log") {
			client.log(args);
		} else if (args[0] == "cdp") {
            try {
			    client.changePriorityLogLevel(args);
                std::cout << GREEN("Успешно. Уровень приоритета установлен") << std::endl;
            } catch (std::invalid_argument &e) {
                std::cerr << e.what() << std::endl;
            }
		} else if (args[0] == "exit") {
			break;
		} else if (args[0] == "help") {
            print_menu();
        }else {
			print("Неизвестная команда: " + args[0]);
		}
	}
}