#pragma once
#include<windows.h>
#include<fstream>
#include<string>
#include<iostream>

extern const int MAX_MSG_SIZE;
extern std::string projectDir;
extern const char* MUTEX_NAME;
extern const char* READY_EVENT_NAME;

std::string getProjectDir();
std::string getSenderExePath(std::string buildDir);
std::string getReceiverExePath(std::string buildDir);

struct Message;
struct Message {
    char text[20];
    bool isFilled;
};


struct QueueControl {
    int readIndex;
    int writeIndex;
};