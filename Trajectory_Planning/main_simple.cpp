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
clock_t start, end, startcmd, endcmd,t0,t1;
double dur;

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "ws2_32.lib")
int mm = 0;                                   //运动车编号 


int SOCKADDR_IN_SIZE = sizeof(SOCKADDR_IN);
const u_short DEFAULT_PORT = 23333;//接收图像
const u_short CMD_PORT = 50001;//发送运动指令
const size_t MSG_BUF_SIZE = 2 * 2048;
const size_t IP_BUF_SIZE = 256;
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
	int j = 1;
	//障碍物

	startcmd = clock();
	float endx[2] = { 240,-240 }, endy[2] = { 150,-150 };
	while (true) {
		Debug_Msgs msgs;
		float startx, starty;

		//SecureZeroMemory(msg_buf, MSG_BUF_SIZE);

		//********************* xunxian
		ret_val = recvfrom(sock_serv, msg_buf, MSG_BUF_SIZE, 0, (SOCKADDR*)& addr_clt, &SOCKADDR_IN_SIZE);
		inet_ntop(addr_clt.sin_family, &addr_clt, ip_buf, IP_BUF_SIZE);
		Vision_DetectionFrame current;
		current.ParseFromArray(msg_buf, 2 * 2048);

		//障碍物信息
		float a[32][2] = { 0 };
		//cout << current.robots_blue_size() << endl;
		//cout << current.robots_yellow_size() << endl;
		//cout << current.balls.() << endl;
		for (int i4 = 0; i4 < current.robots_blue_size(); i4++) {
			Vision_DetectionRobot robots_blue = current.robots_blue(i4);
			if (current.robots_blue(i4).robot_id() != mm) {
				a[i4][0] = current.robots_blue(i4).x() / 10;
				a[i4][1] = current.robots_blue(i4).y() / 10;
				//cout<< current.robots_blue(i4).x()<<endl;
			}
			else {
				robo_id = i4;
				a[i4][0] = -295;
				a[i4][1] = -210;
				startx = current.robots_blue(robo_id).x() / 10;
				starty = current.robots_blue(robo_id).y() / 10;
			}
		}
		for (int i4 = 0; i4 < current.robots_yellow_size(); i4++) {
			Vision_DetectionRobot robots_yellow = current.robots_yellow(i4);
			a[i4 + current.robots_blue_size()][0] = current.robots_yellow(i4).x() / 10;
			a[i4 + current.robots_blue_size()][1] = current.robots_yellow(i4).y() / 10;

		}

		int k = current.robots_yellow_size() + current.robots_blue_size();


		data1 dat;
		j = 1 - j;
		dat = solve(startx, starty, endx[j], endy[j], a, k);
		float current_angle, required_angle;
		while (1) {
			char msg_buf[MSG_BUF_SIZE];
			int ret = recvfrom(sock_serv, msg_buf, MSG_BUF_SIZE, 0, (SOCKADDR*)& addr_clt, &SOCKADDR_IN_SIZE);
			//cout << ret << endl;
			inet_ntop(addr_clt.sin_family, &addr_clt, ip_buf, IP_BUF_SIZE);
			Vision_DetectionFrame current;
			current.ParseFromArray(msg_buf, 2 * 2048);
			current_angle = current.robots_blue(0).orientation();
			//Sleep(20);
			//cout << current_angle << endl;
			float temp_mubiao_angle = atan((dat.Y[1] - dat.Y[0]) / (dat.X[1] - dat.Y[0]));
			if (dat.X[1] - dat.X[0] < 0 && dat.Y[1] - dat.Y[0]>0)
				required_angle = temp_mubiao_angle + pi;
			else if (dat.X[1] - dat.Y[0] < 0 && dat.Y[1] - dat.Y[0] < 0)
				required_angle = temp_mubiao_angle - pi;
			else required_angle = temp_mubiao_angle;
			//cout << required_angle << endl;
			if (abs(required_angle - current_angle) < 0.1)
				break;
			Robots_Command cmd;
			Robot_Command* cm = cmd.add_command();
			cm->set_robot_id(mm);
			//cout<<"vx: "<<vx<<endl;
			cm->set_velocity_x(0);
			cm->set_velocity_y(0);
			cm->set_velocity_r(1);
			//cmd.set_delay(10);
			cm->set_kick(0);
			cm->set_power(100);
			cm->set_dribbler_spin(0);
			int size = cmd.ByteSize();
			char* buffer = (char*)malloc(size);
			memset(buffer, 0, size);
			cmd.SerializePartialToArray(buffer, size);
			int isend = sendto(client, buffer, size, 0, (SOCKADDR*)& serveraddr, sizeof(serveraddr));
			free(buffer);
			//cout << abs(required_angle - current_angle) << endl;
		}


		cout << "while true" << endl;

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
		//  control
		int i = 0;

		float my_angle, lianxian_angle, mubiao_angle, omega, alpha;
		float x_current, y_current, distance, vx,tempvx=0.,tempw1=0.,tempw2=0.;
		float current_vxacc, current_wacc;
		i = 0;
		while (1) {     //运动控制
			cout << "New destination: (" << dat.X[i + 1] << ", " << dat.Y[i + 1] << ")" << endl;
			while (1) {
				//等待接收消息
				ret_val = recvfrom(sock_serv, msg_buf, MSG_BUF_SIZE, 0, (SOCKADDR*)& addr_clt, &SOCKADDR_IN_SIZE);
				inet_ntop(addr_clt.sin_family, &addr_clt, ip_buf, IP_BUF_SIZE);
				Vision_DetectionFrame current;
				current.ParseFromArray(msg_buf, 2 * 2048);
				//读取所有机器人位置信息
				for (int i4 = 0; i4 < current.robots_blue_size(); i4++) {
					Vision_DetectionRobot robots_blue = current.robots_blue(i4);
					if (current.robots_blue(i4).robot_id() != mm) {
						a[i4][0] = current.robots_blue(i4).x() / 10;
						a[i4][1] = current.robots_blue(i4).y() / 10;
					}
					else {
						robo_id = i4;
						a[i4][0] = -295;
						a[i4][1] = -210;
						x_current = current.robots_blue(robo_id).x() / 10;
						y_current = current.robots_blue(robo_id).y() / 10;
						current_vxacc = current.robots_blue(i4).accelerate_x() / 1000;
						if (current_vxacc > 1) cout << "加速度过大" << endl;
					}
				}
				for (int i4 = 0; i4 < current.robots_yellow_size(); i4++) {
					Vision_DetectionRobot robots_yellow = current.robots_yellow(i4);
					a[i4 + current.robots_blue_size()][0] = current.robots_yellow(i4).x() / 10;
					a[i4 + current.robots_blue_size()][1] = current.robots_yellow(i4).y() / 10;

				}

				float temp_angle = atan((dat.Y[i + 1] - y_current) / (dat.X[i + 1] - x_current));

				if (dat.X[i + 1] - x_current < 0 && dat.Y[i + 1] - y_current>0)
					lianxian_angle = temp_angle + pi;
				else if (dat.X[i + 1] - x_current < 0 && dat.Y[i + 1] - y_current < 0)
					lianxian_angle = temp_angle - pi;
				else lianxian_angle = temp_angle;
				//lianxian_angle = lianxian_angle < 0 ? lianxian_angle + 2 * pi : lianxian_angle;

				Robots_Command cmd;
				Robot_Command* cm = cmd.add_command();
				my_angle = current.robots_blue(robo_id).orientation();//获取自身角度
				//my_angle = my_angle < 0 ? my_angle + 2*pi : my_angle;///-pi-p转换成0-2*pi
				float k_omega_lianxian = -5;//角速度第一个控制率
				float k_omega_mubiao = 0;//角速度第二个控制率
				//omega = k_omega_lianxian * (lianxian_angle - my_angle);
				if (i + 2 <= dat.ds - 1) {
					//cout<<"加入第二个控制率"<<endl;
					float temp_mubiao_angle = atan((dat.Y[i + 2] - dat.Y[i + 1]) / (dat.X[i + 2] - dat.Y[i + 1]));
					if (dat.X[i + 1] - x_current < 0 && dat.Y[i + 1] - y_current>0)
						mubiao_angle = temp_mubiao_angle + pi;
					else if (dat.X[i + 1] - x_current < 0 && dat.Y[i + 1] - y_current < 0)
						mubiao_angle = temp_mubiao_angle - pi;
					else mubiao_angle = temp_mubiao_angle;
					//	mubiao_angle = mubiao_angle < 0 ? mubiao_angle + 2 * pi : mubiao_angle;
				}
				else mubiao_angle = lianxian_angle;
				//cout << mubiao_angle - lianxian_angle << endl;
				alpha = lianxian_angle - my_angle;
				if (alpha > pi)
					alpha -= 2 * pi;
				else if (alpha < -pi)
					alpha += 2 * pi;
				omega = k_omega_lianxian * alpha + k_omega_mubiao * (mubiao_angle - lianxian_angle);

				//cout << "r:" << my_angle << "    mubiao:" << mubiao_angle << "    w:" << omega << endl;

				float k_vx = 0.8;//线速度控制率 0.8
				distance = sqrt((x_current - dat.X[i + 1]) * (x_current - dat.X[i + 1]) + (y_current - dat.Y[i + 1]) * (y_current - dat.Y[i + 1]));
				vx = k_vx * distance;


				cm->set_robot_id(mm);
				//cout<<"vx: "<<vx<<endl;
				if (vx > 200) vx = 200;
				if (i + 1 != dat.ds - 1)
					if (vx <= 50) vx = 50;
				//if (vx <= 100) vx = 100;
				if (abs(alpha) > pi / 6) vx = 30;

				if (vx - tempvx > 100 / 60)   // 限制线加速度
					vx = tempvx + 100 / 60;
				else if (tempvx - vx > 100 / 60)
					vx = tempvx - 100 / 60;
				tempvx = vx;

				if (double(omega - tempw1) > 0.5 / 60)   // 限制角加速度
					omega = tempw1 + 0.5 / 60;
				else if (double(tempw1 - omega) > 0.5 / 60)
					omega = tempw1 - 0.5 / 60;
				tempw1 = omega;

				cm->set_velocity_x(vx);
				cm->set_velocity_y(0);
				if (omega > 1) omega = 1;
				if (omega < -1) omega = -1;
				cm->set_velocity_r(omega);
				//cmd.set_delay(10);
				cm->set_kick(0);
				cm->set_power(100);
				cm->set_dribbler_spin(0);
				int size = cmd.ByteSize();
				char* buffer = (char*)malloc(size);
				memset(buffer, 0, size);
				cmd.SerializePartialToArray(buffer, size);
				int isend = sendto(client, buffer, size, 0, (SOCKADDR*)& serveraddr, sizeof(serveraddr));
				free(buffer);
				//Sleep(10);//随便一设，为了接收与发送率一致
				//cout << "distance: " << distance << endl;
				if (distance < 10)
					break;
			}
			if (i + 1 >= dat.ds - 1)
				break;
			i++;
		}
		Robots_Command cmd;
		Robot_Command* cm = cmd.add_command();
		cm->set_robot_id(mm);
		cout<<"vx: "<<vx<<endl;
		if (double(omega - tempw2) > 0.5 / 60)   // 限制角加速度
			omega = tempw2 + 0.5 / 60;
		else if (double(tempw2 - omega) > 0.5 / 60)
			omega = tempw2 - 0.5 / 60;
		tempw2 = omega;

		cm->set_velocity_x(0);
		cm->set_velocity_y(0);
		cm->set_velocity_r(omega);
		//cmd.set_delay(10);
		cm->set_kick(0);
		cm->set_power(100);
		cm->set_dribbler_spin(0);
		int size = cmd.ByteSize();
		char* buffer = (char*)malloc(size);
		memset(buffer, 0, size);
		cmd.SerializePartialToArray(buffer, size);
		int isend = sendto(client, buffer, size, 0, (SOCKADDR*)& serveraddr, sizeof(serveraddr));
		free(buffer);
		if (ret_val == SOCKET_ERROR) {
			cerr << "shutdown() function failed with error: " << WSAGetLastError() << "\n";
			closesocket(sock_serv);
			WSACleanup();
			//	system("pause");
			return 1;
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


