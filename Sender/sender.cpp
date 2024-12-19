#include"globals.h"

using namespace std;

int main(int argc, char* argv[])
{
	cout << "Sender process #" << argv[2] << " started" << endl;

	HANDLE mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, MUTEX_NAME);
	HANDLE readyEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, READY_EVENT_NAME);
    HANDLE exitEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, "ExitEvent");
    HANDLE emptyEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, "EmptyEvent");

	if (!mutex || !readyEvent || !exitEvent || !emptyEvent) {
		cerr << "Failed to open synchronization objects.\n";
		return -1;
	}

    std::fstream file(argv[1], std::ios::binary | std::ios::in | std::ios::out);

    SetEvent(readyEvent);

    while (WaitForSingleObject(exitEvent, 0) != WAIT_OBJECT_0) {
        std::string command;
        std::cout << "Enter command (send/exit): ";
        std::cin >> command;

        if (command == "send") {
            std::string message;
            std::cout << "Enter message (<20 chars): ";
            std::cin.ignore();
            std::getline(std::cin, message);

            if (message.size() >= MAX_MSG_SIZE) {
                std::cerr << "Message too long!\n";
                continue;
            }

            WaitForSingleObject(mutex, INFINITE);

            QueueControl queue;
            file.seekg(0);
            file.read(reinterpret_cast<char*>(&queue), sizeof(QueueControl));

            file.seekg(sizeof(QueueControl) + queue.writeIndex * sizeof(Message));
            Message msg;
            file.read(reinterpret_cast<char*>(&msg), sizeof(Message));

            if (!msg.isFilled) {
                std::strcpy(msg.text, message.c_str());
                SetEvent(emptyEvent);
                msg.isFilled = true;

                file.seekp(sizeof(QueueControl) + queue.writeIndex * sizeof(Message));
                file.write(reinterpret_cast<char*>(&msg), sizeof(Message));

                queue.writeIndex = (queue.writeIndex + 1) % MAX_MSG_SIZE;
                file.seekp(0);
                file.write(reinterpret_cast<char*>(&queue), sizeof(QueueControl));
            }
            else {
                std::cout << "Queue is full. Waiting...\n";
            }

            ReleaseMutex(mutex);

        }
        else if (command == "exit") {
            break;
        }
    }

    file.close();
    CloseHandle(mutex);
    CloseHandle(readyEvent);
    CloseHandle(emptyEvent);
    CloseHandle(exitEvent);

	return 0;
}