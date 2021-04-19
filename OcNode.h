#pragma once
#include <cstdlib>
#include <vector>
#include "vertex.h"
#include "MY_EPS.h"

template <class T>
class OcNode {

public:

	//构造
	OcNode <T>(const vertex &newCenter, const vertex &newRange) {

		for (int i = 0; i < 8; i++)
		{
			child[i] = nullptr;
		}

		center = newCenter;
		range = newRange;
		leaf = true;
	}

	//析构
	~OcNode() { for (int i = 0; i < 8; ++i) if (child[i]) { delete child[i]; } }

public:
	//点的中心
	vertex center;

	//点代表空间域的三个范围
	vertex range;

	//非叶节点使用
	bool leaf;
	OcNode* child[8];

	//叶节点挂载的空间点的桶
	std::vector <std::pair <vertex, T> > bucket;

};
