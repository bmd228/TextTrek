# CMakeList.txt: проект CMake для tesseract; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.12)
add_subdirectory ("fastText")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
project(server)
find_package(asio-grpc CONFIG REQUIRED)
find_package(oatpp CONFIG REQUIRED)
find_package(oatpp-swagger CONFIG REQUIRED)
find_package(redis++ CONFIG REQUIRED)
find_package(unofficial-poppler CONFIG REQUIRED)
find_package(Boost REQUIRED )
find_package(spdlog CONFIG REQUIRED)
find_package(Tesseract CONFIG REQUIRED)
set(ASIO_GRPC_USE_BOOST_CONTAINER ON)
asio_grpc_protobuf_generate(
    GENERATE_GRPC 
    OUT_VAR "ASIO_GRPC_EXAMPLE_PROTO_SOURCES"
    OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated"
  #  IMPORT_DIRS "${CMAKE_SOURCE_DIR}/proto/"
    PROTOS "${CMAKE_SOURCE_DIR}/proto/cascade.proto")
# Добавьте источник в исполняемый файл этого проекта.
file(GLOB_RECURSE CPP_FILES http/*.cpp)
file(GLOB_RECURSE HPP_FILES http/*.hpp)
file(GLOB_RECURSE WORKER_CPP_FILES workers/*.cpp)
file(GLOB_RECURSE WORKER_HPP_FILES workers/*.h)
file(GLOB_RECURSE  MOODYCAMEL_FILES moodycamel/*.h)
add_executable (server  ${CPP_FILES} ${HPP_FILES} ${MOODYCAMEL_FILES} ${WORKER_CPP_FILES} ${WORKER_HPP_FILES} "main.cpp" "main.h" "server_grpc.cpp"  ${ASIO_GRPC_EXAMPLE_PROTO_SOURCES})

target_include_directories(server SYSTEM
                               PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>")

target_link_libraries(server PRIVATE asio-grpc::asio-grpc oatpp::oatpp oatpp::oatpp-swagger redis++::redis++ unofficial::poppler::poppler-cpp unofficial::poppler::poppler-private PNG::PNG  Boost::boost spdlog::spdlog spdlog::spdlog_header_only libtesseract)

add_definitions(-DOATPP_SWAGGER_RES_PATH="${OATPP_BASE_DIR}bin/oatpp-swagger/res" -DOATPP_DISABLE_ENV_OBJECT_COUNTERS)
target_compile_features(server PUBLIC cxx_std_17)
target_compile_options(server PRIVATE /GR- /await /wd4238)