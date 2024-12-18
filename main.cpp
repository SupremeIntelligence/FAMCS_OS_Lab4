// OS_Lab4.cpp: определяет точку входа для приложения.
//

#include"globals.h"

using namespace std;

bool startReceiverProcess(const string& cmdArgs)
{
    STARTUPINFO receiverSi;
    PROCESS_INFORMATION receiverPi;
    ZeroMemory(&receiverSi, sizeof(STARTUPINFO));
    receiverSi.cb = sizeof(STARTUPINFO);

    //cout << cmdArgs << endl;

    LPSTR lpszReceiverCommandLine = const_cast<LPSTR>(cmdArgs.c_str());

    if (!CreateProcess(
        nullptr,
        lpszReceiverCommandLine,
        nullptr,
        nullptr,
        false, 
        0,
        nullptr,
        nullptr,
        &receiverSi,
        &receiverPi))
    {
        cerr << "Receiver process isn't created." << endl;
        return false;
    }

    WaitForSingleObject(receiverPi.hProcess, INFINITE);

    CloseHandle(receiverPi.hProcess);
    CloseHandle(receiverPi.hThread);
    return true;
}

bool startSenderProcess(const string& cmdArgs)
{
    STARTUPINFO senderSi;
    PROCESS_INFORMATION senderPi;
    ZeroMemory(&senderSi, sizeof(STARTUPINFO));
    senderSi.cb = sizeof(STARTUPINFO);

    LPSTR lpszSenderCommandLine = const_cast<LPSTR>(cmdArgs.c_str());

    if (!CreateProcessA(
        nullptr,
        lpszSenderCommandLine,
        nullptr,
        nullptr,
        false, 0,
        nullptr,
        nullptr,
        &senderSi,
        &senderPi))
    {
        cerr << "Sender process isn't created." << endl;
        return false;
    }

    CloseHandle(senderPi.hProcess);
    CloseHandle(senderPi.hThread);
    return true;
}

int main()
{
    string receiverArgs = getReceiverExePath(projectDir);
    //cout << receiverArgs;

    if (startReceiverProcess(receiverArgs))
    {
        cout << "Receiver process finished";
    }

}
