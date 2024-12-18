#pragma once
#include<windows.h>
#include<fstream>
#include<string>
#include<iostream>

extern const int MAX_MSG_SIZE;
extern std::string projectDir;

std::string getProjectDir();
std::string getSenderExePath(std::string buildDir);
std::string getReceiverExePath(std::string buildDir);
