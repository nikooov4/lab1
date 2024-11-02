cmake_minimum_required(VERSION 3.10)

project()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} 
    main.cpp
    tests.cpp
    interface.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}
    /path_to_python_include  
    /path_to_numpy_include  
)

target_link_libraries(${PROJECT_NAME} 
    python3.10
)

