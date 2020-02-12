#include <iomanip>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <string.h>
#include <vector>

#include <ctime>
#include "Ws2tcpip.h"
#include "Winsock2.h"
#include "zss_cmd.pb.h"
#include "zss_debug.pb.h"
#include "vision_detection.pb.h"
#include "solve.h"


using std::cin;
using std::cout;
using std::cerr;
using std::endl;
clock_t start, end, startcmd, endcmd;
double dur;

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "ws2_32.lib")
int mm = 0;                                   //运动车编号 


int SOCKADDR_IN_SIZE = sizeof(SOCKADDR_IN);
const u_short DEFAULT_PORT = 23333;//接收图像
const u_short CMD_PORT = 50001;//发送运动指令
const size_t MSG_BUF_SIZE = 2 * 2048;
const size_t IP_BUF_SIZE = 2*256;
const int vel = 20;
const double pi = 3.1415926;
int end1x = 200, end1y = 200, end2x = -200, end2y = -200;
int main() {

	int robo_id;
	Vision_DetectionFrame VDF;
	char SendBuffer[1000];
	int SendBuffer_size = 1000;
	int RecvBuffer_size = 1000;
	sockaddr_in SendAddr;
	int SendPort = 20001;//发送debug信息

	WSADATA wsa_data;
	SOCKET sock_serv = INVALID_SOCKET;
	SOCKADDR_IN addr_serv, addr_clt;
	char ip_buf[IP_BUF_SIZE];
	char msg_buf[MSG_BUF_SIZE];

	int ret_val = 0;
	ret_val = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (ret_val != 0) {
		cerr << "WSAStartup() function failed with error: " << WSAGetLastError() << "\n";
		//	system("pause");
		return 1;
	}

	SecureZeroMemory(&addr_serv, SOCKADDR_IN_SIZE);
	addr_serv.sin_family = AF_INET;
	addr_serv.sin_port = htons(DEFAULT_PORT);
	addr_serv.sin_addr.S_un.S_addr = ADDR_ANY;
	//
	sock_serv = socket(addr_serv.sin_family, SOCK_DGRAM, IPPROTO_UDP);//SOCKET socket(int af, int type, int protocol);
	if (sock_serv == INVALID_SOCKET) {
		cerr << "socket() function failed with error: " << WSAGetLastError() << "\n";
		WSACleanup();
		//	system("pause");
		return 1;
	}

	ret_val = bind(sock_serv, (SOCKADDR*)& addr_serv, SOCKADDR_IN_SIZE);
	if (ret_val != 0) {
		cerr << "bind() function failed with error: " << WSAGetLastError() << "\n";
		//	system("pause");
		return 1;
	}
	cout << "A UDP server has started successfully..." << endl;
	//
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	/*WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata)) {
		std::cout << "WSAStartup failed" << std::endl;
		WSACleanup();
		return 0;
	}*/

	SOCKET client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (client == SOCKET_ERROR) {
		std::cout << "socket failed" << std::endl;
		closesocket(client);
		WSACleanup();
		return 0;
	}

	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;//define address family
	serveraddr.sin_port = htons(50001);//save port number;htons turn.
	inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr.S_un.S_addr);//turn 10 to 2


	std::cout << "connect..." << std::endl;
	//draw line


	SendAddr.sin_family = AF_INET;
	SendAddr.sin_port = htons(SendPort);
	inet_pton(AF_INET, "127.0.0.1", &SendAddr.sin_addr.S_un.S_addr);
	float cunchu = 0;
	while (true) {
		Debug_Msgs msgs;
		ret_val = recvfrom(sock_serv, msg_buf, MSG_BUF_SIZE, 0, (SOCKADDR*)& addr_clt, &SOCKADDR_IN_SIZE);
		cout << "接收消息: " << ret_val << endl;
		inet_ntop(addr_clt.sin_family, &addr_clt, ip_buf, IP_BUF_SIZE);
		Vision_DetectionFrame current;
		current.ParseFromArray(msg_buf, 2 * 2048);

		//障碍物信息
		float a[32][2] = { 0 };

		for (int i4 = 0; i4 < current.robots_blue_size(); i4++) {
			Vision_DetectionRobot robots_blue = current.robots_blue(i4);
			a[i4][0] = current.robots_blue(i4).x() / 10;
			a[i4][1] = current.robots_blue(i4).y() / 10;
		}
		for (int i4 = 0; i4 < current.robots_yellow_size(); i4++) {
			Vision_DetectionRobot robots_yellow = current.robots_yellow(i4);
			a[i4 + current.robots_blue_size()][0] = current.robots_yellow(i4).x() / 10;
			a[i4 + current.robots_blue_size()][1] = current.robots_yellow(i4).y() / 10;
		}
		//Sleep(1000);
		if (abs(cunchu - a[4][0])>7) {
			cunchu = a[4][0];
			int k = current.robots_yellow_size() + current.robots_blue_size();

			cout << k << current.robots_yellow_size() << current.robots_blue_size() << endl;
			cout << "cunchu" << cunchu << endl;
			cout << "a[4][0]" << a[4][0] << endl;

			data1 dat;
			dat = solve(-240, -150, 240, 150, a, k);
			cout << dat.ds << endl;

			cout << "while true" << endl;
			SecureZeroMemory(msg_buf, MSG_BUF_SIZE);
			memset(SendBuffer, 0, SendBuffer_size);
			for (int i = 0; i < (dat.ds - 1); i++) {
				Debug_Msg* msg = msgs.add_msgs();
				msg->set_type(Debug_Msg::LINE);
				msg->set_color(Debug_Msg::RED);

				Point* point1 = new Point;
				Point* point2 = new Point;
				point1->set_x(dat.X[i]); point1->set_y(dat.Y[i]);
				point2->set_x(dat.X[i + 1]); point2->set_y(dat.Y[i + 1]);
				Debug_Line* l = new Debug_Line;
				l->set_allocated_start(point1);
				l->set_allocated_end(point2);
				l->set_forward(true);
				l->set_back(false);

				msg->set_allocated_line(l);
				if (msgs.SerializePartialToArray(SendBuffer, SendBuffer_size)) {
					//		std::cout << "draw success" << std::endl;
				}
				else {
					std::cout << "draw failed" << std::endl;
				}
				if (sendto(client, SendBuffer, SendBuffer_size, 0, (SOCKADDR*)& SendAddr, sizeof(SendAddr)) == SOCKET_ERROR) {
					std::cout << "sendto failed" << std::endl;
				}
				else {
					std::cout << "sendto success" << std::endl;
				}
			}
			Sleep(1000);
		}
		
	}
	ret_val = shutdown(sock_serv, SD_BOTH);
	closesocket(client);
	closesocket(sock_serv);
	closesocket(client);
	WSACleanup();
	cout << "server shutdown..." << endl;
	system("pause");
	return 0;
}


