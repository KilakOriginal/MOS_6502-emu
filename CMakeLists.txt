cmake_minimum_required(VERSION 3.28.3)

project(MOS_6503-emu)
file(GLOB SOURCE_FILES src/*.c)
file(GLOB HEADER_FILES hdr/*.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
