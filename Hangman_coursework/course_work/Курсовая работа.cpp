#define NOMINMAX
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <clocale>
#include <signal.h>

// Кросс-платформа
#ifdef _WIN32
#include <windows.h>
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#else
#include <locale>
#endif

using namespace std;

// Цветовые коды ANSI
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string WHITE = "\033[37m";
const string CYAN = "\033[36m";
const string BOLD = "\033[1m";

// Цветной вывод
void enableColors() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

// Очистка экрана
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Функция для настройки локали
void setRussianLocale() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
#else
    setlocale(LC_ALL, "ru_RU.UTF-8");
#endif
}

// Проверки русской буквы
bool isRussianLetter(char c) {
    c = tolower(c);
    return (c >= 'а' && c <= 'я') || c == 'ё';
}

// Загрузка слов из файла
vector<string> loadWordsFromFile(const string& filename) {
    vector<string> words;
    ifstream file(filename);

    if (file.is_open()) {
        string word;
        while (getline(file, word)) {
            if (!word.empty()) {
                words.push_back(word);
            }
        }
        file.close();
    }
    else {
        words = { "программирование", "алгоритм", "компьютер",
                 "виселица", "разработка", "интерфейс" };
    }

    return words;
}

// Рисование линии для табло
void drawLine(int n) {
    cout << CYAN;
    for (int i = 0; i < n; ++i)
        cout << " ---";
    cout << RESET << endl;
}

// Отображение табло
void ShowTable(const string& table) {
    drawLine(table.size());
    cout << CYAN << "|" << RESET;
    for (char c : table) {
        if (c == '_') {
            cout << " " << RED << c << RESET << " " << CYAN << "|" << RESET;
        }
        else {
            cout << " " << GREEN << c << RESET << " " << CYAN << "|" << RESET;
        }
    }
    cout << endl;
    drawLine(table.size());
}

// Отображение использованных букв
void ShowUsedLetters(const vector<char>& usedLetters) {
    if (!usedLetters.empty()) {
        cout << YELLOW << "Использованные буквы: " << RESET;
        for (char c : usedLetters) {
            cout << GREEN << c << RESET << " ";
        }
        cout << endl;
    }
}

// Ввод буквы с проверкой
char input(vector<char>& usedLetters) {
    char c;
    bool validInput = false;

    while (!validInput) {
        cout << CYAN << "Введите букву: " << RESET;
        cin >> c;

        if (cin.eof()) {
            cin.clear();
            cout << RED << "Пожалуйста, введите букву русского алфавита.\n" << RESET;
            continue;
        }

        if (!isRussianLetter(c)) {
            cout << RED << "Ошибка! Пожалуйста, введите букву русского алфавита.\n" << RESET;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        c = tolower(c);

        if (find(usedLetters.begin(), usedLetters.end(), c) != usedLetters.end()) {
            cout << BLUE << "Буква уже была использована! Попробуйте другую.\n" << RESET;
            continue;
        }

        validInput = true;
    }

    usedLetters.push_back(c);
    return c;
}

// Открытие букв на табло
bool openLetters(const string& word, string& table, char c) {
    bool found = false;
    for (int i = 0; i < word.size(); ++i) {
        if (word[i] == c) {
            table[i] = c;
            found = true;
        }
    }
    return found;
}

//Рисунок Виселицы
void draw(int err) {
    cout << BOLD << YELLOW;
    switch (err) {
    case 0:
        cout << "  ______\n";
        cout << "  |    |\n";
        cout << "  |\n";
        cout << "  |\n";
        cout << "  |\n";
        cout << "  |\n";
        cout << "__|________\n";
        break;
    case 1:
        cout << "  ______\n";
        cout << "  |    |\n";
        cout << "  |    " << RED << "O" << YELLOW << "\n";
        cout << "  |\n";
        cout << "  |\n";
        cout << "  |\n";
        cout << "__|________\n";
        break;
    case 2:
        cout << "  ______\n";
        cout << "  |    |\n";
        cout << "  |    " << RED << "O" << YELLOW << "\n";
        cout << "  |    " << RED << "|" << YELLOW << "\n";
        cout << "  |\n";
        cout << "  |\n";
        cout << "__|________\n";
        break;
    case 3:
        cout << "  ______\n";
        cout << "  |    |\n";
        cout << "  |    " << RED << "O" << YELLOW << "\n";
        cout << "  |   " << RED << "/|" << YELLOW << "\n";
        cout << "  |\n";
        cout << "  |\n";
        cout << "__|________\n";
        break;
    case 4:
        cout << "  ______\n";
        cout << "  |    |\n";
        cout << "  |    " << RED << "O" << YELLOW << "\n";
        cout << "  |   " << RED << "/|\\" << YELLOW << "\n";
        cout << "  |\n";
        cout << "  |\n";
        cout << "__|________\n";
        break;
    case 5:
        cout << "  ______\n";
        cout << "  |    |\n";
        cout << "  |    " << RED << "O" << YELLOW << "\n";
        cout << "  |   " << RED << "/|\\" << YELLOW << "\n";
        cout << "  |   " << RED << "/" << YELLOW << "\n";
        cout << "  |\n";
        cout << "__|________\n";
        break;
    case 6:
        cout << "  ______\n";
        cout << "  |    |\n";
        cout << "  |    " << BLUE << "O" << YELLOW << "\n";
        cout << "  |   " << BLUE << "/|\\" << YELLOW << "\n";
        cout << "  |   " << BLUE << "/ \\" << YELLOW << "\n";
        cout << "  |\n";
        cout << "__|________\n";
        break;
    }
    cout << RESET;
}

// Инструкция
void ShowInstructions() {
    clearScreen();
    cout << BOLD << CYAN << "=== ИНСТРУКЦИЯ ПО ИГРЕ 'ВИСЕЛИЦА' ===\n" << RESET;
    cout << GREEN << "1. Цель игры: " << RESET << "угадать загаданное слово, вводя буквы по одной.\n";
    cout << GREEN << "2. У вас есть 6 попыток, " << RED << "после чего игра завершается.\n" << RESET;
    cout << GREEN << "3. Если буква есть в слове: " << RESET << "она откроется на табло " << GREEN << "(зеленым цветом).\n" << RESET;
    cout << GREEN << "4. Если буквы нет в слове: " << RESET << "к виселице добавится часть тела " << RED << "(красным цветом).\n" << RESET;
    cout << GREEN << "5. Использованные буквы: " << RESET << "отображаются " << YELLOW << "желтым цветом.\n" << RESET;
    cout << GREEN << "6. Для выхода: " << RESET << "используйте пункт меню 3 или " << RED << "Ctrl+C\n" << RESET;
    cout << BOLD << CYAN << "=====================================\n\n" << RESET;
    cout << "Нажмите Enter для возврата в меню...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Главное меню
void ShowMainMenu() {
    clearScreen();
    cout << BOLD << CYAN << "=== ДОБРО ПОЖАЛОВАТЬ В ИГРУ 'ВИСЕЛИЦА' ===\n" << RESET;
    cout << WHITE << "1. " << WHITE << "Начать игру\n";
    cout << CYAN << "2. " << CYAN << "Показать инструкцию\n";
    cout << RED << "3. " << RED << "Выйти из игры\n";
    cout << BOLD << CYAN << "=======================================\n" << RESET;
    cout << CYAN << "Выберите действие (1-3): " << RESET;
}

// Обработчик Ctrl+C
void handleCtrlC(int signal) {
    cout << "\n" << RED << "Игра завершена по запросу пользователя (Ctrl+C).\n" << RESET;
    exit(0);
}

int main() {
    enableColors();
    setRussianLocale();
    signal(SIGINT, handleCtrlC);

    srand(time(0));
    vector<string> words = loadWordsFromFile("words.txt");
    if (words.empty()) {
        cout << RED << "Не удалось загрузить слова из файла." << RESET << endl;
        return 1;
    }

    bool exitProgram = false;
    while (!exitProgram) {
        ShowMainMenu();

        int menuChoice;
        while (!(cin >> menuChoice) || (menuChoice < 1 || menuChoice > 3)) {
            if (cin.eof()) {
                cin.clear();
                ShowMainMenu();
                continue;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Ошибка! Введите число от 1 до 3: " << RESET;
        }

        switch (menuChoice) {
        case 1: {
            bool playAgain = true;
            while (playAgain) {
                string word = words[rand() % words.size()];
                string table(word.size(), '_');
                vector<char> usedLetters;
                int err = 0;

                while (table != word && err < 6) {
                    clearScreen();
                    cout << BOLD << BLUE << "=== ИГРАЕМ! ===\n" << RESET;
                    cout << YELLOW << "Осталось попыток: " << (6 - err) << "\n\n" << RESET;
                    draw(err);
                    cout << endl;
                    ShowTable(table);
                    cout << endl;
                    ShowUsedLetters(usedLetters);
                    cout << endl;
                    char c = input(usedLetters);
                    if (!openLetters(word, table, c))
                        ++err;
                }

                clearScreen();
                draw(err);
                cout << endl;
                ShowTable(table);
                cout << endl;

                if (table == word) {
                    cout << GREEN << "Поздравляем! Вы угадали слово! " << BOLD << word << RESET << endl;
                }
                else {
                    cout << RED << "Вы проиграли! Загаданное слово: " << BOLD << word << RESET << endl;
                }

                cout << YELLOW << "\nПопробуете еще раз? (1 - Да, 0 - Нет): " << RESET;
                int choice;
                while (!(cin >> choice) || (choice != 0 && choice != 1)) {
                    if (cin.eof()) {
                        cin.clear();
                        cout << YELLOW << "Пожалуйста, введите 1 или 0: " << RESET;
                        continue;
                    }
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << YELLOW << "Пожалуйста, введите 1 или 0: " << RESET;
                }
                playAgain = (choice == 1);
            }
            break;
        }
        case 2:
            ShowInstructions();
            break;
        case 3:
            exitProgram = true;
            break;
        }
    }

    cout << BOLD << GREEN << "\nСпасибо за игру! До свидания!\n" << RESET;
    return 0;
}