cmake_minimum_required(VERSION 3.10)
project(CountingRhymeTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключаем Google Test через FetchContent (автоматическая загрузка)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

# Основная программа (ваше решение)
add_executable(solution main.cpp)

# Тесты
add_executable(run_tests test_solution.cpp)
target_link_libraries(run_tests PRIVATE solution gtest_main)

# Регистрация тестов в CTest
enable_testing()
add_test(NAME CountingRhymeTests COMMAND run_tests)