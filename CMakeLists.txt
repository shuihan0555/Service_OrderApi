# Exchange DEPT.
# Savin Chen
cmake_minimum_required(VERSION 2.8)
project(OrderAPI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11  -DKEEP_ALIVE_TEST -DDEBUG  -DLOCALHOSTIP ")



include_directories(${PROJECT_SOURCE_DIR}/fixinclude)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/tnode2.1/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/)



set(SOURCE_FILE 
	src/Public.cpp
	src/BaseThread.cpp
	src/OrderConsumer.cpp
	src/CRealtimeResponseFromME.cpp
	src/OrderAckSender.cpp
	src/MemPoolMgr.cpp
	src/ConfigFileReader.cpp
	src/main.cpp
	)



link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/tnode2.1/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/)

add_executable(bin/OrderAPI ${SOURCE_FILE})


target_link_libraries(bin/OrderAPI quickfix tnode2 rabbitmq ev xlogger glog pthread)

