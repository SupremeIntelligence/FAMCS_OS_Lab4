#include"globals.h"

const int MAX_MSG_SIZE = 20;
const char* MUTEX_NAME = "FileAccessMutex";
const char* READY_EVENT_NAME = "SenderReadyEvent";
std::string projectDir = getProjectDir();

std::string getProjectDir()
{
    char projectDirectory[MAX_PATH];
    if (GetCurrentDirectoryA(MAX_PATH, projectDirectory))
    {
        std::string buildDir = projectDirectory; //getting substring of the form .../project/out/build/x64-debug
        /* std::cout << buildDir << "\n";
         size_t pos = buildDir.find("\\build\\Debug");

         buildDir.erase(pos);
         std::cout << buildDir << "\n";
         buildDir += "\\build\\";
         std::cout << buildDir << "\n";*/
        return buildDir;
    }
    else
    {
        std::cerr << "\nFailed to get current project directory\n";
        return " ";
    }
}

std::string getSenderExePath(std::string buildDir)
{
    buildDir += "\\Sender\\Sender.exe";
    return buildDir;
}

std::string getReceiverExePath(std::string buildDir)
{
    buildDir += "\\Receiver\\Receiver.exe";
    return buildDir;
}

