project(Receiver)

add_executable (Receiver receiver.cpp ${CMAKE_SOURCE_DIR}/globals.cpp)

target_include_directories(Receiver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})