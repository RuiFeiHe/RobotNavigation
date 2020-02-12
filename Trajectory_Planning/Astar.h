#pragma once

#include <vector>
#include <list>

const int kCost1 = 10; //直移一格消耗
const int kCost2 = 14; //斜移一格消耗

struct Point1
{
	int x, y; //点坐标，这里为了方便按照C++的数组来计算，x代表横排，y代表竖列
	int F, G, H; //F=G+H
	Point1* parent; //parent的坐标，这里没有用指针，从而简化代码
	Point1(int _x, int _y) :x(_x), y(_y), F(0), G(0), H(0), parent(NULL)  //变量初始化
	{
	}
};

class Astar
{
public:
	void InitAstar(std::vector<std::vector<int>>& _maze);
	std::list<Point1*> GetPath(Point1& startPoint, Point1& endPoint, bool isIgnoreCorner);

private:
	Point1* findPath(Point1& startPoint, Point1& endPoint, bool isIgnoreCorner);
	std::vector<Point1*> getSurroundPoints(const Point1* point, bool isIgnoreCorner) const;
	bool isCanreach(const Point1* point, const Point1* target, bool isIgnoreCorner) const; //判断某点是否可以用于下一步判断
	Point1* isInList(const std::list<Point1*>& list, const Point1* point) const; //判断开启/关闭列表中是否包含某点
	Point1* getLeastFpoint(); //从开启列表中返回F值最小的节点
	//计算FGH值
	int calcG(Point1* temp_start, Point1* point);
	int calcH(Point1* point, Point1* end);
	int calcF(Point1* point);
private:
	std::vector<std::vector<int>> maze;
	std::list<Point1*> openList;  //开启列表
	std::list<Point1*> closeList; //关闭列表
};
