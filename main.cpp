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

int main()
{
    string receiverArgs = getReceiverExePath(projectDir);
    //cout << receiverArgs;

    if (startReceiverProcess(receiverArgs))
    {
        cout << "Receiver process finished"<<endl;
    }

}
