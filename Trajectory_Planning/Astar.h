#pragma once

#include <vector>
#include <list>

const int kCost1 = 10; //ֱ��һ������
const int kCost2 = 14; //б��һ������

struct Point1
{
	int x, y; //�����꣬����Ϊ�˷��㰴��C++�����������㣬x������ţ�y��������
	int F, G, H; //F=G+H
	Point1* parent; //parent�����꣬����û����ָ�룬�Ӷ��򻯴���
	Point1(int _x, int _y) :x(_x), y(_y), F(0), G(0), H(0), parent(NULL)  //������ʼ��
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
	bool isCanreach(const Point1* point, const Point1* target, bool isIgnoreCorner) const; //�ж�ĳ���Ƿ����������һ���ж�
	Point1* isInList(const std::list<Point1*>& list, const Point1* point) const; //�жϿ���/�ر��б����Ƿ����ĳ��
	Point1* getLeastFpoint(); //�ӿ����б��з���Fֵ��С�Ľڵ�
	//����FGHֵ
	int calcG(Point1* temp_start, Point1* point);
	int calcH(Point1* point, Point1* end);
	int calcF(Point1* point);
private:
	std::vector<std::vector<int>> maze;
	std::list<Point1*> openList;  //�����б�
	std::list<Point1*> closeList; //�ر��б�
};
