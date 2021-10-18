#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <thread>

#pragma warning(disable: 4996)
#define PORT 5000
using namespace std;
SOCKET ClientSocket;

void se() {
	char massage[256];
	while (true) {
		recv(ClientSocket, massage, sizeof(massage), NULL);
		cout << "---NEW MASSAGE " << massage << endl;
	}
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "russian");
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if(WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Ошибка подключения библиотеки" << endl;
		exit(1);
	}
	  
	SOCKADDR_IN address;
	int sizeofaddr = sizeof(address);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(PORT);
	address.sin_family = AF_INET;

	ClientSocket = socket(AF_INET, SOCK_STREAM, NULL);
	if(connect(ClientSocket, (SOCKADDR*)&address, sizeof(address)) != 0) {
		cout << "Сокет Клиента не подлючился :((" << endl;
		return 1;
	}
	cout << "УРА подключился" << endl;
	char massage[256];
	recv(ClientSocket, massage, sizeof(massage), NULL);
	cout << massage << endl;
	thread SendingMassage(se);
	
	char timeMassage[256];
	while (true) {
		cin.getline(timeMassage, sizeof(timeMassage));
		send(ClientSocket, timeMassage, sizeof(timeMassage), NULL);
		Sleep(10);

	}
	//close(Connection);
	SendingMassage.join();
	system("pause");
	return 0;
}