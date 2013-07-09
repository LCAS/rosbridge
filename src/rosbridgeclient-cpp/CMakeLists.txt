

cmake_minimum_required( VERSION 2.6 FATAL_ERROR )

find_package( Boost 1.34 COMPONENTS thread )

link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( 
	${Boost_INCLUDE_DIRS} 
	${CMAKE_SOURCE_DIR}/jsoncpp/include/
)

add_executable (
	rosbridgeclient_cpp
	jsoncpp/src/lib_json/json_reader.cpp
	jsoncpp/src/lib_json/json_value.cpp
	jsoncpp/src/lib_json/json_writer.cpp
	RosbridgeClient.cpp
	RosbridgeClient.h
	main.cpp
    )

add_library(
	rosbridgeclient
	jsoncpp/src/lib_json/json_reader.cpp
	jsoncpp/src/lib_json/json_value.cpp
	jsoncpp/src/lib_json/json_writer.cpp
	RosbridgeClient.cpp
	RosbridgeClient.h
    )

target_link_libraries (
    rosbridgeclient_cpp
    ${Boost_LIBRARIES}
#    ${CMAKE_SOURCE_DIR}/jsoncpp-src-0.5.0/libs/linux-gcc-4.4.3/libjson_linux-gcc-4.4.3_libmt.a

    )
