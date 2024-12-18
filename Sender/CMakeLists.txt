project (Sender)

add_executable(Sender sender.cpp ${CMAKE_SOURCE_DIR}/globals.cpp)

target_include_directories(Sender PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})