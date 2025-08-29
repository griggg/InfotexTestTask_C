#include "../headers/client.h"

#define RED(text)    ("\033[31m" text "\033[0m")
#define GREEN(text)  ("\033[32m" text "\033[0m")
#define BLUE(text)   ("\033[34m" text "\033[0m")
#define YELLOW(text) ("\033[33m" text "\033[0m")

static void print(std::string m) {
    std::cout << m << std::endl;
}

int main() {
	std::cout << "Инициализация " << RED("Logger") << std::endl;
	std::cout << "Напишите <имя файла логгера> <приоритет=" 
        << GREEN("INFO") << "/" 
        << YELLOW("WARNING") << "/" 
        << RED("ERROR") << "> (по умолч. INFO)"
        << std::endl;

	std::string line;
	std::getline(std::cin, line);
	Client client(line);

	print("------------");
	print("Меню логгера");
	print(
		"1) log <сообщение> <важность=INFO/WARNING/ERROR> (по умолч. INFO) - "
		"Записать сообщение");
	print("2) cdp <INFO/WARNING/ERROR> - Сменить уровень приоритета у логгера");
	print("3) exit - завершить работу");
	print("------------");

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
			client.changePriorityLogLevel(args);
		} else if (args[0] == "exit") {
			if (args.size() == 1) {
				break;
			}
		} else {
			print("Неизвестная команда: " + args[0]);
		}
	}
}