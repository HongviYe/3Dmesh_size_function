#pragma once
#include <cstdlib>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
#include <stdlib.h>
#include "vertex.h"
#include "MY_EPS.h"
#include "OcNode.h"


//�˲����ռ䷽λ
						   //z y x
#define LOW_BACK_LEFT   0  //�º���,000
#define UP_BACK_LEFT    1  //�Ϻ���,001
#define LOW_FRONT_LEFT  2  //��ǰ��,010
#define UP_FRONT_LEFT   3  //��ǰ��,011

#define LOW_BACK_RIGHT  4  //�º���,100
#define UP_BACK_RIGHT   5  //�Ϻ���,101
#define LOW_FRONT_RIGHT 6  //��ǰ��,110
#define UP_FRONT_RIGHT  7  //��ǰ��,111

//ö��ֵ���ж��ڵ��Ƿ�����ڲ�ѯ������
enum enclosure_status
{
	NODE_NOT_IN_REGION,
	NODE_PARTIALLY_IN_REGION,
	NODE_CONTAINED_BY_REGION
};

using namespace std;

template <typename T>
class OcTree
{
public:
	//���������
	OcTree <T>(const vertex& center, const  vertex &range,
		unsigned bucketSize = 10, unsigned depth = 16);
	~OcTree();


	pair <vertex, T> findNearestPoint(const vertex& point);
	//�ⲿ�����ӿ�
	void insert(const vertex& v, T data);

	bool remove(const vertex& v, T data);
	vector <pair <vertex, T> > getObjectsInRegion(const vertex &minXYZ, const vertex &maxXYZ);
	
	
	OcNode<T>* getroot() { return this->root; }

private:
	//�ڲ������ӿ�
	OcNode<T>* childNode(const vertex& v, OcNode<T>* node);
	void insert(const vertex& v, T data, OcNode<T>* node, unsigned depth);
	vertex newCenter(int direction, OcNode <T>* node);
	int direction(const vertex& point, OcNode <T>* node);

	void reduce(stack <OcNode<T>*>& node);
	enclosure_status getEnclosureStatus(const vertex& center, const vertex& range, const vertex& minXYZ, const vertex& maxXYZ);
	void addAllPointsToResults(OcNode<T>* node, vector <pair <vertex, T> >& results);
	bool pointInRegion(const vertex& point, const vertex& minXYZ, const vertex& maxXYZ);
	
	//����
	OcNode<T>* root;
	unsigned maxDepth;//������
	unsigned maxBucketSize;//�ڵ�������صĵ���
};


template <typename T>
OcTree<T>::OcTree(const vertex& center, const vertex &range,
	unsigned bucketSize, unsigned depth)
{
	root = new OcNode<T>(center, range);
	maxDepth = depth;
	maxBucketSize = bucketSize;
}

template <typename T>
OcTree<T>::~OcTree()
{
	delete root;
	root = nullptr;
}

/*
	һ������
*/

//��˲����в���ڵ�
template <typename T>
void OcTree<T>::insert(const vertex& v, T data)
{
	insert(v, data, root, 0);
}


template <typename T>
void OcTree<T>::insert(const vertex& v, T data, OcNode<T>* node, unsigned depth)
{
	//�����Ҷ�ڵ�
	if (node->leaf)
	{
		//�ڵ��Ͱû��
		if (node->bucket.size() < maxBucketSize)
		{
			node->bucket.push_back({ v,data });
		}
		//�ڵ��Ͱ���ˣ����������
		//1�����С��������
		else if (depth < maxDepth)
		{
			node->leaf = false;

			//������һ��
			insert(v, data, childNode(v, node), depth + 1);

			//������ڵ��Ͱ�еĿռ��Ҳ������һ��
			for (int i = 0; i < node->bucket.size(); ++i) {
				insert(node->bucket[i].first, node->bucket[i].second,
					childNode(node->bucket[i].first, node), depth + 1);
			}
			node->bucket.clear();
			node->bucket.shrink_to_fit();
		}
		//2����ȴ��ڵ���������ʱ
		else
		{
			node->bucket.push_back({ v,data });
			node->leaf = true;
		}

	}

	//��Ҷ�ڵ�
	else
	{
		//������һ��
		insert(v, data, childNode(v, node), depth + 1);
	}
}

//�ж�Ӧ�ò�����һ���ӽڵ���
template <typename T>
OcNode<T>* OcTree<T>::childNode(const vertex& v, OcNode<T>* node)
{
	//��¼��λ
	unsigned dir = direction(v, node);

	//����ӽڵ���ڣ��Ǿͷ���
	if (node->child[dir] != nullptr) {
		return node->child[dir];
	}

	//�����ڣ����½�һ���ڵ�
	else {
		vertex r(node->range.x / 2.0, node->range.y / 2.0, node->range.z / 2.0);
		node->child[dir] = new OcNode<T>(newCenter(dir, node), r);
		return node->child[dir];
	}

}

//�������vertex����OcNode���Ŀ������򣬷��ش���λ������
template <typename T>
int OcTree<T>::direction(const vertex& point, OcNode <T>* node)
{
	unsigned X = 0, Y = 0, Z = 0;
	X |= ((MoreEqu(point.x, node->center.x)) << 2);
	Y |= ((MoreEqu(point.y, node->center.y)) << 1);
	Z |= ((MoreEqu(point.z, node->center.z)) << 0);
	return (X | Y | Z);
}

//�����µ�����
template <typename T>
vertex OcTree<T>::newCenter(int direction, OcNode <T>* node)
{
	vertex v(node->center.x, node->center.y, node->center.z);

	switch (direction) {
	case UP_FRONT_RIGHT://111
		v.x += node->range.x / 4.0;
		v.y += node->range.y / 4.0;
		v.z += node->range.z / 4.0;
		break;
	case LOW_FRONT_RIGHT://110
		v.x += node->range.x / 4.0;
		v.y += node->range.y / 4.0;
		v.z -= node->range.z / 4.0;
		break;
	case UP_BACK_RIGHT://101
		v.x += node->range.x / 4.0;
		v.y -= node->range.y / 4.0;
		v.z += node->range.z / 4.0;
		break;
	case LOW_BACK_RIGHT://100
		v.x += node->range.x / 4.0;
		v.y -= node->range.y / 4.0;
		v.z -= node->range.z / 4.0;
		break;
	case UP_FRONT_LEFT://011
		v.x -= node->range.x / 4.0;
		v.y += node->range.y / 4.0;
		v.z += node->range.z / 4.0;
		break;
	case LOW_FRONT_LEFT://010
		v.x -= node->range.x / 4.0;
		v.y += node->range.y / 4.0;
		v.z -= node->range.z / 4.0;
		break;
	case UP_BACK_LEFT://001
		v.x -= node->range.x / 4.0;
		v.y -= node->range.y / 4.0;
		v.z += node->range.z / 4.0;
		break;
	case LOW_BACK_LEFT://000
		v.x -= node->range.x / 4.0;
		v.y -= node->range.y / 4.0;
		v.z -= node->range.z / 4.0;
		break;
	}
	return v;
}


/*
	����ɾ��
*/

template <typename T>
bool OcTree<T>::remove(const vertex& v, T data)
{
	stack<OcNode<T>*>nodes;
	nodes.push(root);
	OcNode<T>*top = nodes.top();
	unsigned dir;

	while (!top->leaf)
	{
		dir = direction(v, top);
		if (top->child[dir])
		{
			nodes.push(top->child[dir]);
			top = nodes.top();
		}
		else
		{
			return false;
		}
	}

	for (int i = 0; i < top->bucket.size(); ++i) {
		// vertex found, delete from bucket
		if (top->bucket[i].second == data) {
			top->bucket.erase(top->bucket.begin() + i);
			reduce(nodes);
			return true;
		}
		else {
			return false;
		}
	}

}

template <typename T>
void OcTree<T>::reduce(stack <OcNode<T>*>& nodes)
{
	bool canReduce = true;
	nodes.pop();
	while (canReduce && !nodes.empty()) {
		canReduce = true;
		OcNode<T>* top = nodes.top();
		int numKeys = 0;
		for (int i = 0; i < 8; ++i) {

			//������ڵ�ĺ��ӽڵ����з�Ҷ�ڵ㣬�ǾͲ���ɾ��������ڵ�
			if (top->child[i] && !top->child[i]->leaf) {
				canReduce = false;
				return;
			}
			else if (top->child[i] && top->child[i]->leaf) {
				numKeys += top->child[i]->bucket.size();
			}
		}
		//numKeys���Ǹ��ڵ��Ҷ�ӽ����
		//���numKeys <= maxBucketSize��canReduce����true
		//���numKeys > maxBucketSize��canReduce����false
		canReduce &= (numKeys <= maxBucketSize);

		//ɾ������ƥ���ڵ�ľ������
		if (canReduce) {
			for (int i = 0; i < 8; ++i) {
				if (top->child[i]) {
					for (int j = 0; j < top->child[i]->bucket.size(); ++j) {
						top->bucket.emplace_back(top->child[i]->bucket[j]);
					}
					delete top->child[i];
					top->child[i] = nullptr;
				}
			}
			top->leaf = true;
		}
		nodes.pop();
	}
	return;
}

/*
	������ѯ
*/

template <typename T>
void OcTree<T>::addAllPointsToResults(OcNode<T>* node, vector <pair <vertex, T> >& results)
{
	if (node->leaf) {
		results.insert(results.end(), node->bucket.begin(), node->bucket.end());
	}
	else {
		for (int i = 0; i < 8; ++i) {
			if (node->child[i]) {
				addAllPointsToResults(node->child[i], results);
			}
		}
	}
}


//�ж�һ�����Ƿ���������
template <typename T>
pair <vertex, T> OcTree<T>::findNearestPoint(const vertex& point)
{
	pair <vertex, T> ans;
	OcNode<T>* pnode= root;
	while (!pnode->leaf) {
		for (int i = 0; i < 8; i++) {
			if (pointInRegion(point, child[i].center-child[i].range/2, child[i].center + child[i].range / 2)) {
				pnode = leaf;
			}
		}
		
	}

	return ans;
}


//�ж�һ�����Ƿ���������
template <typename T>
bool OcTree<T>::pointInRegion(const vertex& point, const vertex& minXYZ, const vertex& maxXYZ)
{
	if (
		(MoreEqu(point.x, minXYZ.x)) && (Less(point.x, maxXYZ.x)) &&
		(MoreEqu(point.y, minXYZ.y)) && (Less(point.y, maxXYZ.y)) &&
		(MoreEqu(point.z, minXYZ.z)) && (Less(point.z, maxXYZ.z))
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//�ж���centerΪ���ģ�rangeΪ��Χ�������Ƿ��ڴ���ѯ������
template <typename T>
enclosure_status OcTree<T>::getEnclosureStatus(const vertex& center, const vertex& range,
	const vertex& minXYZ, const vertex& maxXYZ)
{

	int enclosedPts = 0;

	double xLength = range.x / 2.0;
	double yLength = range.y / 2.0;
	double zLength = range.z / 2.0;


	double NodeCubeMin[3] = { center.x - xLength, center.y - yLength, center.z - zLength };
	double NodeCubeMax[3] = { center.x + xLength, center.y + yLength, center.z + zLength };


	//һ���޽�
	if (
		Less(NodeCubeMax[0], minXYZ.x) ||
		Less(NodeCubeMax[1], minXYZ.y) ||
		Less(NodeCubeMax[2], minXYZ.z) ||
		MoreEqu(NodeCubeMin[0], maxXYZ.x) ||
		MoreEqu(NodeCubeMin[1], maxXYZ.y) ||
		MoreEqu(NodeCubeMin[2], maxXYZ.z)
		)
	{
		return NODE_NOT_IN_REGION;
	}
	//��ȫ����
	else if
		(
			Less(NodeCubeMax[0], maxXYZ.x) &&
			Less(NodeCubeMax[1], maxXYZ.y) &&
			Less(NodeCubeMax[2], maxXYZ.z) &&
			MoreEqu(NodeCubeMin[0], minXYZ.x) &&
			MoreEqu(NodeCubeMin[1], minXYZ.y) &&
			MoreEqu(NodeCubeMin[2], minXYZ.z)
			)
	{
		return NODE_CONTAINED_BY_REGION;
	}
	//�����н�
	else
	{
		return NODE_PARTIALLY_IN_REGION;
	}
}

template <typename T>
vector <pair <vertex, T> > OcTree<T>::getObjectsInRegion(const vertex &minXYZ, const vertex &maxXYZ)
{
	vector <pair <vertex, T> > results;
	queue <OcNode<T>*> nodes;
	nodes.push(root);

	while (!nodes.empty()) {
		OcNode<T>* top = nodes.front();
		if (top->leaf) {
			enclosure_status status = getEnclosureStatus(top->center, top->range, minXYZ, maxXYZ);
			switch (status) {
				// this node is completely contained within the search region
			case NODE_CONTAINED_BY_REGION:
				// add all elements to results
				results.insert(results.end(), top->bucket.begin(), top->bucket.end());
				break;

				// this node is partially contained by the region
			case  NODE_PARTIALLY_IN_REGION:
				// search through this leaf node's bucket
				for (int i = 0; i < top->bucket.size(); ++i) {
					// check if this point is in the region
					if (pointInRegion(top->bucket[i].first, minXYZ, maxXYZ)) {
						results.emplace_back(top->bucket[i]);
					}
				}
				break;

				// this node definitely has no points in the region
			case NODE_NOT_IN_REGION:
				// do nothing
				break;
			}
		}
		else {



			for (int i = 0; i < 8; ++i) {
				if (top->child[i]) {
					
					// check if this nodes children could have points in the region
					enclosure_status status = getEnclosureStatus(top->child[i]->center, top->child[i]->range, minXYZ, maxXYZ);
					switch (status) {
						// this node is completely contained by region, add all points within
					case NODE_CONTAINED_BY_REGION:
						addAllPointsToResults(top->child[i], results);
						break;

						// this node might contain points in the region
					case NODE_PARTIALLY_IN_REGION:
						nodes.push(top->child[i]);
						break;

						// no points in region, discontinue searching this branch
					case NODE_NOT_IN_REGION:
						break;
					}
				}
			}
		}
		nodes.pop();
	}
	return results;
}


