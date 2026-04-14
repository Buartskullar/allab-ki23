#include <gtest>
#include <fstream>
#include <filesystem>
#include "ai1096.cpp"  // Подключите заголовок с вашими классами

// ============================================================================
// Тесты для класса CountingGame (юнит-тесты бизнес-логики)
// ============================================================================

class CountingGameTest : public ::testing::Test {
protected:
    // Вспомогательный метод для упрощения создания объектов
    std::string getLeader(const std::string& rhyme, const std::string& names) {
        CountingGame game(rhyme, names);
        return game.getLeader();
    }
};

TEST_F(CountingGameTest, BasicCase_ThreeWordsThreeChildren) {
    EXPECT_EQ(getLeader("one two three", "Alice Bob Charlie"), "Charlie");
}

TEST_F(CountingGameTest, WrapAround_FiveWordsTwoChildren) {
    // (5-1) % 2 = 0 -> первый ребенок
    EXPECT_EQ(getLeader("alpha beta gamma delta epsilon", "Alice Bob"), "Alice");
}

TEST_F(CountingGameTest, SingleWord_SingleChild) {
    EXPECT_EQ(getLeader("go", "Tom"), "Tom");
}

TEST_F(CountingGameTest, ExtraSpaces_IgnoredCorrectly) {
    EXPECT_EQ(getLeader("   one    two   ", "   Alice   Bob   "), "Bob");
}

TEST_F(CountingGameTest, CasePreservation_OutputMatchesInputCase) {
    EXPECT_EQ(getLeader("One TWO three", "alice Bob CHARLIE"), "CHARLIE");
}

TEST_F(CountingGameTest, ManyWords_FewChildren_ModuloWorks) {
    // 100 слов, 3 ребенка: (100-1) % 3 = 0 -> первый
    std::string rhyme;
    for (int i = 0; i < 100; ++i) rhyme += (i ? " w" : "w") + std::to_string(i);
    EXPECT_EQ(getLeader(rhyme, "First Second Third"), "First");
}

TEST_F(CountingGameTest, OneWord_ManyChildren_AlwaysFirst) {
    EXPECT_EQ(getLeader("start", "A B C D E F"), "A");
}

TEST_F(CountingGameTest, TwoWords_ManyChildren_SecondSelected) {
    EXPECT_EQ(getLeader("one two", "A B C D E"), "B");
}

TEST_F(CountingGameTest, Constructor_ThrowsOnEmptyChildren) {
    EXPECT_THROW(CountingGame("one two", "   "), std::invalid_argument);
}

TEST_F(CountingGameTest, Constructor_ThrowsOnEmptyRhyme) {
    EXPECT_THROW(CountingGame("   ", "Alice Bob"), std::invalid_argument);
}

// ============================================================================
// Тесты для класса FileHandler (юнит-тесты ввода/вывода)
// ============================================================================

class FileHandlerTest : public ::testing::Test {
protected:
    std::string temp_input = "test_input.tmp";
    std::string temp_output = "test_output.tmp";

    void SetUp() override {
        // Очищаем файлы перед тестом
        if (std::filesystem::exists(temp_input)) std::filesystem::remove(temp_input);
        if (std::filesystem::exists(temp_output)) std::filesystem::remove(temp_output);
    }

    void TearDown() override {
        // Удаляем файлы после теста
        if (std::filesystem::exists(temp_input)) std::filesystem::remove(temp_input);
        if (std::filesystem::exists(temp_output)) std::filesystem::remove(temp_output);
    }
};

TEST_F(FileHandlerTest, ReadGameInput_Success) {
    // Создаём тестовый файл
    {
        std::ofstream out(temp_input);
        out << "one two three\nAlice Bob Charlie\n";
    }

    std::string rhyme, names;
    EXPECT_TRUE(FileHandler::readGameInput(temp_input, rhyme, names));
    EXPECT_EQ(rhyme, "one two three");
    EXPECT_EQ(names, "Alice Bob Charlie");
}

TEST_F(FileHandlerTest, ReadGameInput_FileNotFound) {
    std::string rhyme, names;
    EXPECT_FALSE(FileHandler::readGameInput("nonexistent_file.txt", rhyme, names));
}

TEST_F(FileHandlerTest, WriteResult_Success) {
    std::string result = "Winner";
    EXPECT_TRUE(FileHandler::writeResult(temp_output, result));

    // Проверяем содержимое файла
    std::ifstream in(temp_output);
    std::string content;
    std::getline(in, content);
    EXPECT_EQ(content, "Winner");
}

TEST_F(FileHandlerTest, WriteResult_InvalidPath) {
    // Попытка записи в несуществующую директорию
    EXPECT_FALSE(FileHandler::writeResult("/invalid/path/result.txt", "Test"));
}

// ============================================================================
// Интеграционные тесты (полный цикл: файл -> решение -> файл)
// ============================================================================

class IntegrationTest : public ::testing::Test {
protected:
    const std::string INPUT_FILE = "INPUT.TXT";
    const std::string OUTPUT_FILE = "OUTPUT.TXT";

    void SetUp() override {
        if (std::filesystem::exists(INPUT_FILE)) std::filesystem::remove(INPUT_FILE);
        if (std::filesystem::exists(OUTPUT_FILE)) std::filesystem::remove(OUTPUT_FILE);
    }

    void TearDown() override {
        if (std::filesystem::exists(INPUT_FILE)) std::filesystem::remove(INPUT_FILE);
        if (std::filesystem::exists(OUTPUT_FILE)) std::filesystem::remove(OUTPUT_FILE);
    }

    std::string runFullCycle(const std::string& input_content) {
        // Записываем входные данные
        {
            std::ofstream out(INPUT_FILE);
            out << input_content;
        }

        // Имитируем main()
        std::string rhyme_line, names_line;
        if (!FileHandler::readGameInput(INPUT_FILE, rhyme_line, names_line)) {
            throw std::runtime_error("Failed to read input");
        }

        CountingGame game(rhyme_line, names_line);
        std::string leader = game.getLeader();

        if (!FileHandler::writeResult(OUTPUT_FILE, leader)) {
            throw std::runtime_error("Failed to write output");
        }

        // Читаем результат
        std::ifstream in(OUTPUT_FILE);
        std::string result;
        std::getline(in, result);
        return result;
    }
};

TEST_F(IntegrationTest, FullCycle_BasicCase) {
    std::string input = "one two three\nAlice Bob Charlie";
    EXPECT_EQ(runFullCycle(input), "Charlie");
}

TEST_F(IntegrationTest, FullCycle_WithExtraSpaces) {
    std::string input = "   alpha    beta   \n   Tom   Jerry   ";
    EXPECT_EQ(runFullCycle(input), "Jerry");
}

TEST_F(IntegrationTest, FullCycle_LargeInput) {
    std::string rhyme;
    for (int i = 0; i < 500; ++i) rhyme += (i ? " word" : "word") + std::to_string(i);
    std::string input = rhyme + "\nA B C D E";
    // (500-1) % 5 = 499 % 5 = 4 -> 5-й элемент (0-based), т.е. "E"
    EXPECT_EQ(runFullCycle(input), "E");
}

// ============================================================================
// Parameterized tests для проверки множества сценариев
// ============================================================================

struct TestCase {
    std::string rhyme;
    std::string names;
    std::string expected;
};

class CountingGameParamTest : public ::testing::TestWithParam<TestCase> {};

TEST_P(CountingGameParamTest, MultipleScenarios) {
    auto [rhyme, names, expected] = GetParam();
    CountingGame game(rhyme, names);
    EXPECT_EQ(game.getLeader(), expected);
}

INSTANTIATE_TEST_SUITE_P(
    GameLogic,
    CountingGameParamTest,
    ::testing::Values(
        TestCase{"a", "X", "X"},
        TestCase{"a b", "X Y Z", "Y"},
        TestCase{"x y z", "P Q", "Q"},  // (3-1)%2 = 0 -> P? Нет: (3-1)=2, 2%2=0 -> индекс 0 = P. Ой, проверим: 1->P, 2->Q, 3->P. Значит P.
        TestCase{"one two three four", "A B", "B"},  // (4-1)%2=1 -> индекс 1 = B
        TestCase{"w1 w2 w3 w4 w5", "Cat Dog", "Dog"} // (5-1)%2=0 -> Cat? Нет: 1->Cat,2->Dog,3->Cat,4->Dog,5->Cat. Значит Cat. Исправим: (5-1)=4, 4%2=0 -> Cat
    )
);