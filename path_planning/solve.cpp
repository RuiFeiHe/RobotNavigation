#include "solve.h"
using namespace std;

data1 solve(float start_x, float start_y, float end_x, float end_y, float a[16][2], int k) {
	data1 dat;
	int x, y, i, j;
	vector<vector<int>> maze;
	maze.resize(m);//行数为m
	for (i = 0; i < m; i++)//每行列数为n
		maze[i].resize(n);
	//之后可采用遍历的方式为a数组赋值
	if (start_x > 270)start_x = 270;
	if (start_x < -270)start_x = -270;
	if (start_y > 170)start_y = 170;
	if (start_y < -170)start_y = -170;
	if (end_x > 270)end_x = 270;
	if (end_x < -270)end_x = -270;
	if (end_y > 170)end_y = 170;
	if (end_y < -270)end_y = -270;


	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
			maze[i][j] = 0;
	}
	for (i = 0; i < k; i++) {
		x = int(-a[i][1] * 0.1 + 23);
		y = int(a[i][0] * 0.1 + 30.5);
		for (int i3 = -2; i3 < 3; i3++) {
			for (int j3 = -2; j3 < 3; j3++)
				if (x + i3 > 0 && x + i3 < 45 && y + j3 > 0 && y + j3 < 60)
					maze[x + i3][y + j3] = 1;
		}
	}
	/*while (1) {
		if (maze[int(-0.1*start_y + 23)][int(0.1*start_x + 30.5)] == 1)
		{
			start_y = start_y + 10;
		}
		else break;
	}*/
	int i1;
	float sx = start_x, sy = start_y;

	while (1) {

		if (maze[int(-0.1 * start_y + 23)][int(0.1 * start_x + 30.5)] == 1) {

			i1 = 0;
			for (i = 1; i < k; i++) {
				if (((sx - a[i][0]) * (sx - a[i][0]) + (sy - a[i][1]) * (sy - a[i][1])) < ((sx - a[i1][0]) * (sx - a[i1][0]) + (sy - a[i1][1]) * (sy - a[i1][1]))) {
					i1 = i;
				}
			}
			float a1 = start_x - a[i1][0], a2 = start_y - a[i1][1];
			if (start_x - a[i1][0] < 0 && a1 < a2 && a1 < -a2) { start_x = start_x - 10; }
			else if (a1 > 0 && -a1 < a2 && -a1 < -a2) { start_x = start_x + 10; }
			else if (a2 < 0 && a2 < a1 && a2 < -a1) { start_y = start_y - 10; }
			else { start_y = start_y - 10; }
		}
		else break;
	}
	while (1) {
		if (maze[int(-0.1 * end_y + 23)][int(0.1 * end_x + 30.5)] == 1)
		{
			end_x = end_x + 10;
		}
		else break;
	}
	//根据障碍物坐标设定栅格值
	Astar astar;
	astar.InitAstar(maze);

	//设置起始和结束点
	Point1 start(int(-0.1 * start_y + 23), int(0.1 * start_x + 30.5));
	Point1 end(-0.1 * end_y + 22.5, 0.1 * end_x + 30);
	//A*算法找寻路径
	list<Point1*> path = astar.GetPath(start, end, false);
	//打印

	i = 0;
	for (auto& p : path)
	{
		maze[p->x][p->y] = 4;
		dat.X[i] = float(p->y);
		dat.Y[i] = float(p->x);
		i++;

	}


	for (int i = 0; i < path.size(); i++)
	{

		dat.X[i] = (dat.X[i] - 30) * 10;
		dat.Y[i] = (22.5 - dat.Y[i]) * 10;

	}
	dat.ds = path.size();

	data1 dat1;
	float start2a[2];
	float starti[2];
	float starti1[2];

	int start2 = 0;

	dat1.X[0] = dat.X[0];
	dat1.Y[0] = dat.Y[0];
	j = 1;
	for (i = start2 + 1; i < path.size() - 1; i++) {
		start2a[0] = dat.X[start2];
		start2a[1] = dat.Y[start2];
		starti[0] = dat.X[i];
		starti[1] = dat.Y[i];

		starti1[0] = dat.X[i + 1];
		starti1[1] = dat.Y[i + 1];
		//		cout << "sovle: " << i << endl;
		if (ispathall(a, start2a, starti, 25, k) == 0 && ispathall(a, start2a, starti1, 25, k) == 1) {
			dat1.X[j] = dat.X[i];
			dat1.Y[j] = dat.Y[i];
			j++;
			start2 = i;


		}


		dat1.X[j] = dat.X[path.size() - 1];
		dat1.Y[j] = dat.Y[path.size() - 1];
		dat1.ds = j + 1;
	}

	return dat1;
}
