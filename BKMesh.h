#pragma once

#ifndef  _BKMESH_H_
#define _BKMESH_H_
#include "singleton.h"
#include "SizeOcTree.h"
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <exception>
#include <sstream>
#include<time.h>

static void readPls(string filename, vector<vertex>& points, vector<std::array<int, 3>>& connection) {
	ifstream fin(filename);
	if (!fin)
		throw std::exception("no such file");
	int number_of_point, number_of_element;
	string line;
	getline(fin, line);
	stringstream ss(line);
	ss >> number_of_element >> number_of_point;
	points.resize(number_of_point);
	connection.resize(number_of_element);
	for (int i = 0; i < number_of_point; i++) {
		fin >> line >> points[i][0] >> points[i][1] >> points[i][2];
	}
	for (int i = 0; i < number_of_element; i++) {
		fin >> line >> connection[i][0] >> connection[i][1] >> connection[i][2] >> line;
	}
}




class BKMesh :public Singleton<BKMesh> {
public:
	BKMesh() {
		init_size = vector<double>(point.size(), 0);
		init_conn = vector<int>(point.size(), 0);
		expand_ratio = 1.2;
	}
	void GetFromFile(string filename) {
		readPls(filename, point, connection);
		InsertToOctree();
	}
	void SetExpandRatio(double ratio) { expand_ratio = ratio; }
	double GetSize(double x, double y, double z);
	void InsertToOctree();
	OcTree<double>* GetTree() { return source_tree; }
	pair<vertex, vertex> GetWorld();
	void GetFromMemory(int npt, int nelm, double* pt, int* elm);
	void GetFromMemory(int npt, int nelm, double* pt, int* elm,double* size);
	double GetTargetSize() { return target_size; }
	void CaculateDefaultSize();
private:

	OcTree<double>* source_tree;
	vector<vertex> point;
	vector<std::array<int, 3>> connection;
	double target_size;
	double expand_ratio;
	vector<double> init_size;
	vector<int> init_conn;
};


#endif // ! _BKMESH_H_

