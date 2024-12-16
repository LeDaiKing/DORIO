cmake_minimum_required(VERSION 3.28)
project(CS202Group02)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

# SFML FetchContent
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# nlohmann/json FetchContent
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(run ${SOURCES})
target_link_libraries(run PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio nlohmann_json::nlohmann_json)
target_compile_features(run PRIVATE cxx_std_17)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_custom_target(
    regenerate_makefile
    COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Regenerating Makefile with CMake"
)
set_source_files_properties(${CMAKE_SOURCE_DIR}/CMakeLists.txt PROPERTIES GENERATED TRUE)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/CMakeFiles/Makefile.cmake
    COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR}
    DEPENDS ${SOURCES}
    COMMENT "Automatically updating Makefile if source files change"
)

add_dependencies(run regenerate_makefile)

if(WIN32)
    add_custom_command(
        TARGET run
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:run>
        VERBATIM)
endif()