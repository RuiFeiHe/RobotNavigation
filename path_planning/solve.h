#pragma once
#include <iostream>
#include "Astar.h"
#include "ispath.h"

#define m 45 //���峤��
#define n 60 //���
struct data1 {
	float X[1000] = { 0 };
	float Y[1000] = { 0 };
	int ds;
};
data1 solve(float start_x, float start_y, float end_x, float end_y, float a[16][2], int k);//������ֹ�����ϰ�������õ�·����
