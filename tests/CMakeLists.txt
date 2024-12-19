 list(APPEND CMAKE_PREFIX_PATH "D:/IT/WIN32API/OS_Lab4/vcpkg_installed/x64-windows/share/Catch2")
 set(Catch2_DIR "D:/IT/WIN32API/OS_Lab4/vcpkg_installed/x64-windows/share/Catch2")

 find_package(Catch2 REQUIRED)

  add_executable(SenderTest sendertest.cpp)

  target_link_libraries(SenderTest PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

  target_include_directories(SenderTest PUBLIC ${CMAKE_SOURCE_DIR}/Sender ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}//Receiver)