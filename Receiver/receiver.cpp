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

    fstream file(binFilename,std::ios::binary | std::ios::out | std::ios::trunc);
    if (!file.is_open())
    {
        cout << "Error openning binary file." << std::endl;
        return -1;
    }
    QueueControl queue{ 0, 0 };
    file.write(reinterpret_cast<char*>(&queue), sizeof(QueueControl));

    Message emptyMessage = { "", false };
    for (int i = 0; i < recordNumber; ++i) {
        file.write(reinterpret_cast<char*>(&emptyMessage), sizeof(Message));
    }
    file.close();

    int processAmount;
    cout << "Enter the amount of Sender processes:\t";
    cin >> processAmount;
    
    HANDLE mutex = CreateMutexA(nullptr, FALSE, MUTEX_NAME);
    HANDLE readyEvent = CreateEventA(nullptr, TRUE, FALSE, READY_EVENT_NAME);
    HANDLE exitEvent = CreateEventA(nullptr, TRUE, FALSE, "ExitEvent");
    HANDLE emptyEvent = CreateEventA(nullptr, TRUE, FALSE, "EmptyEvent");
    
    if (!mutex || !readyEvent || !exitEvent ||!emptyEvent) {
        cout << "Failed to create synchronization objects.\n";
        return -1;
    }

    string senderArgs = getSenderExePath(projectDir) + " " + binFilename;
    for (int i = 0; i < processAmount; i++)
    {
        if (!startSenderProcess(senderArgs + " " + to_string(i)))
        {
            cout << "Sender process #" << i << " isn't created" << endl;
        }
    }

    for (int i = 0; i < processAmount; i++)
    {
        WaitForSingleObject(readyEvent, INFINITE);
        ResetEvent(readyEvent);
    }

    cout << "All Sender processes are ready"<<endl;

    while (true) {
        std::string command;
        std::cout << "Enter command (read/exit): ";
        std::cin >> command;

        if (command == "read") {
            WaitForSingleObject(mutex, INFINITE);

            file.open(binFilename, std::ios::binary | std::ios::in | std::ios::out);
            file.seekg(0);
            file.read(reinterpret_cast<char*>(&queue), sizeof(QueueControl));

            if (queue.readIndex == queue.writeIndex) {
                std::cout << "Queue is empty. Waiting...\n";
                file.close();
                ReleaseMutex(mutex);
                WaitForSingleObject(emptyEvent, INFINITE);
                continue;
            }

            file.seekg(sizeof(QueueControl) + queue.readIndex * sizeof(Message));
            Message msg;
            file.read(reinterpret_cast<char*>(&msg), sizeof(Message));

            if (msg.isFilled) {
                std::cout << "Received: " << msg.text << "\n";
                msg.isFilled = false;

                file.seekp(sizeof(QueueControl) + queue.readIndex * sizeof(Message));
                file.write(reinterpret_cast<char*>(&msg), sizeof(Message));

                queue.readIndex = (queue.readIndex + 1) % recordNumber;
                file.seekp(0);
                file.write(reinterpret_cast<char*>(&queue), sizeof(QueueControl));
            }

            file.close();
            ReleaseMutex(mutex);

        }
        else if (command == "exit") {
            SetEvent(exitEvent);
            break;
        }
    }

    CloseHandle(mutex);
    CloseHandle(readyEvent);
    CloseHandle(emptyEvent);
    CloseHandle(exitEvent);

	return 0;
}