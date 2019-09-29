cmake_minimum_required(VERSION 3.1)

project(luabinding)

set(CMAKE_C_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Lua "5.1" EXACT REQUIRED)

file(GLOB SOURCE_FILES "*.c")
file(GLOB SOURCE_HEADERS "*.h")
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${SOURCE_HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${LUA_LIBRARIES})

message("LUA: ${LUA_INCLUDE_DIR}")
