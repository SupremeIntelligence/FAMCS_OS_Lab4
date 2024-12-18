#include"globals.h"

using namespace std;

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

int main(int argc, char* argv[])
{
    cout << "Receiver process started." << endl;
    string binFilename;
    int recordNumber;
    cout << "Enter the name of the binary file:\t";
    cin >> binFilename;
    cout << "Enter the number of the records in the binary file:\t";
    cin >> recordNumber;

    ofstream binOutput(binFilename, std::ios::binary);
    if (!binOutput.is_open())
    {
        std::cerr << "Error openning binary file." << std::endl;
        return -1;
    }
    string senderArgs = getSenderExePath(projectDir);
    senderArgs += " " + binFilename + " " + to_string(recordNumber);

	return 0;
}