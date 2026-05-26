#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Класс инкапсулирует логику игры. 
// Внешний код не имеет прямого доступа к внутренним данным.
class CountingGame {
private:
    std::vector<std::string> children_;
    int rhyme_word_count_;

    // Вспомогательные методы парсинга. Статические, т.к. не требуют доступа к состоянию объекта.
    static int countWords(const std::string& line) {
        std::istringstream iss(line);
        std::string temp;
        int count = 0;
        while (iss >> temp) ++count;
        return count;
    }

    static std::vector<std::string> parseNames(const std::string& line) {
        std::vector<std::string> names;
        std::istringstream iss(line);
        std::string name;
        while (iss >> name) {
            names.push_back(name);
        }
        return names;
    }

public:
    // Конструктор инициализирует объект. Гарантирует, что экземпляр всегда валиден.
    CountingGame(const std::string& rhyme_line, const std::string& names_line)
        : children_(parseNames(names_line)), 
          rhyme_word_count_(countWords(rhyme_line)) 
    {
        // По условию задачи дети и слова гарантированно есть, но добавим защиту
        if (children_.empty() || rhyme_word_count_ <= 0) {
            throw std::invalid_argument("Invalid input data");
        }
    }

    // Публичный интерфейс. Метод const, т.к. не изменяет состояние объекта.
    std::string getLeader() const {
        // Индекс ведущего: (количество слов - 1) % количество детей
        // -1 используется потому, что счёт начинается с 1, а индексация с 0
        size_t leader_index = (rhyme_word_count_ - 1) % children_.size();
        return children_[leader_index];
    }
};

// Разделение ответственности: ввод/вывод вынесены за пределы бизнес-логики
class FileHandler {
public:
    static bool readGameInput(const std::string& filename, 
                              std::string& out_rhyme, 
                              std::string& out_names) {
        std::ifstream fin(filename);
        if (!fin.is_open()) return false;
        
        std::getline(fin, out_rhyme);
        std::getline(fin, out_names);
        return true;
    }

    static bool writeResult(const std::string& filename, const std::string& result) {
        std::ofstream fout(filename);
        if (!fout.is_open()) return false;
        
        fout << result << '\n';
        return true;
    }
};

int main() {
    std::string rhyme_line, names_line;
    
    // 1. Чтение данных (отвечает FileHandler)
    if (!FileHandler::readGameInput("INPUT.TXT", rhyme_line, names_line)) {
        std::cerr << "Ошибка: не удалось открыть INPUT.TXT\n";
        return 1;
    }

    try {
        // 2. Создание объекта игры (инкапсуляция состояния и логики)
        CountingGame game(rhyme_line, names_line);
        
        // 3. Получение результата через публичный интерфейс
        std::string leader = game.getLeader();
        
        // 4. Запись результата (отвечает FileHandler)
        FileHandler::writeResult("OUTPUT.TXT", leader);
    } 
    catch (const std::exception& e) {
        std::cerr << "Ошибка логики: " << e.what() << '\n';
        return 1;
    }

    return 0;
}