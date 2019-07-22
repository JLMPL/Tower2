cmake_minimum_required(VERSION 3.5.0)
project(ASS)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(PROJECT_LINK_LIBS -lPhysX3Extensions -lfreetype -lGL -lGLEW -lSDL2 -llua5.3 -lassimp -lPhysX3_x64 -lPhysX3Common_x64 -lPhysX3CharacterKinematic_x64 -lPhysX3Cooking_x64)

link_directories(/usr/local/lib)
link_directories(./)

# -fno-exceptions
add_definitions(-g -std=c++17 -Wfatal-errors -Wall)

include_directories(/usr/include/freetype2)
include_directories(src)
include_directories(src/ThirdParty)
include_directories(src/ThirdParty/PhysX)
include_directories(src/ThirdParty/ImGui)

file(GLOB src0 "src/*.cpp")
file(GLOB src1 "src/*/*.cpp")
file(GLOB src2 "src/*/*/*.cpp")
file(GLOB src3 "src/*/*/*/*.cpp")

add_executable(out
    ${src0}
    ${src1}
    ${src2}
    ${src3}
)
target_link_libraries(out ${PROJECT_LINK_LIBS})
