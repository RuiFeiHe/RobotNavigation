#pragma once
#include <iostream>
#include "math.h"

//#define k 5//定义障碍物数量
int ispath(float A[2], float B[2], float C[2], float ds1);//检测A是否挡在BC线上
int ispathall(float A[16][2], float B[2], float C[2], float ds1, int k);//检测所有点是否挡在BC线上





