cmake_minimum_required(VERSION 3.12)
project(TopTrackerDemo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCES_PATH ${CMAKE_SOURCE_DIR}/source)

set(HEADERS
	${SOURCES_PATH}/TopTracker.h
	${SOURCES_PATH}/TopTrackerTest.h
)

set (SOURCES
	${SOURCES_PATH}/main.cpp
	${SOURCES_PATH}/TopTracker.cpp
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

# Включаем директорию с заголовочными файлами
# target_include_directories(toptracker_test PRIVATE ${PROJECT_SOURCE_DIR})