# cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
# project(run)

# # Generate compile_commands.json
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Dependencies
# set(RAYLIB_VERSION 5.0)
# find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
# if (NOT raylib_FOUND) # If there's none, fetch and build raylib
#   include(FetchContent)
#   FetchContent_Declare(
#     raylib
#     DOWNLOAD_EXTRACT_TIMESTAMP OFF
#     URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
#   )
#   FetchContent_GetProperties(raylib)
#   if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
#     set(FETCHCONTENT_QUIET NO)
#     FetchContent_MakeAvailable(raylib)
#     set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
#   endif()
# endif()

# # Our Project

# include_directories(include)

# file(GLOB_RECURSE SOURCES "src/*.cpp")

# message("Source files: ${SOURCES}")

# add_executable(${PROJECT_NAME} ${SOURCES})
# #set(raylib_VERBOSE 1)
# target_link_libraries(${PROJECT_NAME} raylib)

# add_custom_target(
#     regenerate_makefile
#     COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR}
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#     COMMENT "Regenerating Makefile with CMake"
# )
# set_source_files_properties(${CMAKE_SOURCE_DIR}/CMakeLists.txt PROPERTIES GENERATED TRUE)

# add_custom_command(
#     OUTPUT ${CMAKE_BINARY_DIR}/CMakeFiles/Makefile.cmake
#     COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR}
#     DEPENDS ${SOURCES}
#     COMMENT "Automatically updating Makefile if source files change"
# )

# add_dependencies(${PROJECT_NAME} regenerate_makefile)

# # Web Configurations
# if (${PLATFORM} STREQUAL "Web")
#     set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
#     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
# endif()

# # Checks if OSX and links appropriate frameworks (Only required on MacOS)
# if (APPLE)
#     target_link_libraries(${PROJECT_NAME} "-framework IOKit")
#     target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
#     target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
# endif()
cmake_minimum_required(VERSION 3.28)
project(CS202Group02)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(run ${SOURCES})
target_link_libraries(run PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
target_compile_features(run PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET run
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
        VERBATIM)
endif()