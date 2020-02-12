#include "ispath.h"
using namespace std;
float max(float a, float b) {
	if (a > b)return a;
	return b;
}
float min(float a, float b) {
	if (a > b)return b;
	return a;
}

int ispath(float A[2], float B[2], float C[2], float ds1) {
	float area, s, a, b, c, d;
	c = sqrt((A[0] - B[0]) * (A[0] - B[0]) + (A[1] - B[1]) * (A[1] - B[1]));
	b = sqrt((A[0] - C[0]) * (A[0] - C[0]) + (A[1] - C[1]) * (A[1] - C[1]));
	a = sqrt((C[0] - B[0]) * (C[0] - B[0]) + (C[1] - B[1]) * (C[1] - B[1]));
	s = 0.5 * (a + b + c);
	area = sqrt(s * (s - a) * (s - b) * (s - c));
	d = 2 * area / a;


	if (d < ds1 && ((A[0] <= (max(B[0], C[0])) || A[0] >= min(B[0], C[0])) && (A[1] <= (max(B[1], C[1])) && A[1] >= min(B[1], C[1])))) {
		return 1;
	}
	return 0;
}
int ispathall(float A[16][2], float B[2], float C[2], float ds1, int k) {
	int i;
	for (i = 0; i < k; i++) {
		if (ispath(A[i], B, C, ds1)) return 1;
	}
	return 0;
};

