#pragma once
#include <iostream>
#include "Astar.h"
#include "ispath.h"

#define m 45 //定义长度
#define n 60 //宽度
struct data1 {
	float X[1000] = { 0 };
	float Y[1000] = { 0 };
	int ds;
};
data1 solve(float start_x, float start_y, float end_x, float end_y, float a[16][2], int k);//输入起止点与障碍物坐标得到路径点
