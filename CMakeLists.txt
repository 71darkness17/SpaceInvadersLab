cmake_minimum_required(VERSION 3.28)
project(spaceTest)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавляем директории для заголовочных файлов
include_directories(inc)

# Добавляем библиотеку
add_library(Objects src/objects.cpp)
add_library(Realization src/game.cpp)


# Найти SFML (предпочтительно с помощью пакета CMake)
#find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)



# Проверка компилятора и добавление флагов для GCC
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using GCC compiler")
    add_compile_options(-Wall -Wextra -O2)
endif()

# Добавляем исполняемый файл
add_executable(Game src/main.cpp)

# Линкуем библиотеку с приложением
target_link_libraries(Game Realization Objects)


# Линковка с библиотеками SFML
target_link_libraries(Game sfml-graphics sfml-window sfml-system)
