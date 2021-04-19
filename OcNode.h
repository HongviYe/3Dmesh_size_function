#pragma once
#include <cstdlib>
#include <vector>
#include "vertex.h"
#include "MY_EPS.h"

template <class T>
class OcNode {

public:

	//����
	OcNode <T>(const vertex &newCenter, const vertex &newRange) {

		for (int i = 0; i < 8; i++)
		{
			child[i] = nullptr;
		}

		center = newCenter;
		range = newRange;
		leaf = true;
	}

	//����
	~OcNode() { for (int i = 0; i < 8; ++i) if (child[i]) { delete child[i]; } }

public:
	//�������
	vertex center;

	//�����ռ����������Χ
	vertex range;

	//��Ҷ�ڵ�ʹ��
	bool leaf;
	OcNode* child[8];

	//Ҷ�ڵ���صĿռ���Ͱ
	std::vector <std::pair <vertex, T> > bucket;

};
