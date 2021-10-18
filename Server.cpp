#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <thread>

#pragma warning(disable: 4996)
#define PORT 5000
using namespace std; 
SOCKET HowMuchClientsMassiv[100];
int countOfClients = 0;

void cl(int i) {
	char massage[256];
	while (true) {
		recv(HowMuchClientsMassiv[i], massage, sizeof(massage), NULL);
		for (int j = 0; j < countOfClients; j++) {
			if (i == j) {
				continue;
			}
			send(HowMuchClientsMassiv[i], massage, sizeof(massage), NULL);
		}
	}
}
int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "russian");

	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Ошибка подключения библиотеки" << std::endl;
		exit(1);
	}
	 
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;

	const char option = 1;
	SOCKET SocketListen = socket(AF_INET, SOCK_STREAM, NULL);

	setsockopt(SocketListen, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if (SocketListen == -1) {
		cerr << " Сокет не создался" << endl;
		return -1;
	}
	else {
		cerr << "Сокет Listen создался" << endl;
	}

	bind(SocketListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(SocketListen, SOMAXCONN);
	
	int ss = 0;
	ss = PORT;
	cout << "Сервер слушает порт # " << ss << endl;

	SOCKET ServerSocket;
	for (int i = 0; i < 100; i++) {
		ServerSocket = accept(SocketListen, (SOCKADDR*)&addr, &sizeofaddr);

		if (ServerSocket == 0) {
			cout << "Сокет Server не создался ;(" << endl;
		}
		else {
			cout << "Клиент подключился УРА" << endl;
			char msg[256] = "*Сообщение отправлено*";
			send(ServerSocket, msg, sizeof(msg), NULL);
			HowMuchClientsMassiv[i] = ServerSocket;
			countOfClients++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)cl, (LPVOID)i, NULL, NULL);
			//SendingMassage.join();
		}

	}
	
	system("pause");
	return 0;
}