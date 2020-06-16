#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
void errHandler(const char * why, const int exitCode = 1){
	cerr << why << endl;
	exit(exitCode);
}

int main(int argc, char **argv)
{
	// Подготовка структуры с адресом программы
	sockaddr_in * selfAddr = new (sockaddr_in);
	selfAddr -> sin_family = AF_INET;
	selfAddr -> sin_port = 0;
	selfAddr -> sin_addr.s_addr = 0;
	
	// Подготовка структуры с адресом сервера daytime
	sockaddr_in * remoteAddr = new (sockaddr_in);
	remoteAddr -> sin_family = AF_INET;
	remoteAddr -> sin_port = htons(13);
	remoteAddr -> sin_addr.s_addr = inet_addr("82.179.90.12");
	
	// Создание сокета
	int mySocket = socket(AF_INET, SOCK_STREAM, 0);
	if(mySocket == -1)
		errHandler("Ошибка создания сокета", 2);
	
	// Связь сокета с адресом программы
	int rc = bind(mySocket, (const sockaddr*)selfAddr, sizeof(sockaddr_in));
	if(rc == -1){
		close(mySocket);
		errHandler("Ошибка бинда", 3);
	}	
	// Подключение сокета к удалённому адресу
	rc = connect(mySocket, (const sockaddr*)remoteAddr, sizeof(sockaddr_in));
	if(rc == -1){
		close(mySocket);
		errHandler("Ошибка подключения", 4);
	}
	
	// Создание буфера
	char * buf = new char[256];
	strcpy(buf, "Дай время");
	int msgLen = strlen(buf);
	
	// Отправка буфера через сокет
	rc = send(mySocket, buf, msgLen, 0);
	if(rc == -1){
		close(mySocket);
		errHandler("Ошибка отправки", 5);
	}
	cout << "Отправили: " << buf << endl;
	
	// Получение ответа от сервера
	rc = recv(mySocket, (char *)buf, 256, 0);
	if(rc == -1){
		close(mySocket);
		errHandler("Ошибка получения ответа", 6);
	}
	cout << "Получили: " << buf << endl;
	
	delete [] buf;
	
	return 0;
}
