cmake_minimum_required(VERSION 4.0)
project(juniper)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB TEST_SRC tests/*.cpp )

find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Catch2 3 REQUIRED)

include_directories(${ZLIB_INCLUDE_DIR})

add_executable(juniper)
target_sources(juniper PRIVATE ${SOURCE_FILES})
target_link_libraries(juniper PRIVATE ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES})


add_executable(tests)
target_sources(tests PRIVATE ${TEST_SRC} )
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES} )
