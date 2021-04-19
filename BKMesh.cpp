#pragma once
#include "BKMesh.h"

double BKMesh::GetSize(double x, double y, double z)
{
	double min_size = std::numeric_limits<double>::max();
	vertex source(x, y, z);
	vertex size(target_size,target_size,target_size);
	while (true) {
		auto obj = BKMesh::GetSingletonPtr()->GetTree()->getObjectsInRegion(source - size, source + size);
		double farest = size.x;
		for (auto j : obj) {
			if (min_size > (j.first - source).magnitude()*expand_ratio + j.second) {
				min_size = (j.first - source).magnitude()*expand_ratio + j.second;
				//cout << (j.first - source).magnitude()*expand_ratio << " "<< j.second << " " << endl;;
			}
			//cout << j.second << " ";
		}
		//cout << farest << " " << obj.size() << " " << min_size << endl;
		if (min_size < farest*expand_ratio)
			break;

		size = size * 2;
	}
	return min_size;
}

void BKMesh::InsertToOctree()
{
	if (init_size.empty())
		CaculateDefaultSize();
	auto world = GetWorld();

	source_tree = new OcTree<double>((world.first + world.second) / 2, (world.second - world.first));
	for (int i = 0; i < point.size(); i++) {
		source_tree->insert(point[i], init_size[i]);
	}
}

pair<vertex, vertex> BKMesh::GetWorld()
{
	vertex world_min(point[0]), world_max(point[0]);
	for (auto coord : point) {
		for (int i = 0; i < 3; i++) {
			world_min[i] = min(world_min[i], coord[i]);
			world_max[i] = max(world_max[i], coord[i]);
		}
	}
	return pair<vertex, vertex>(world_min, world_max);
}

void BKMesh::GetFromMemory(int npt, int nelm, double * pt, int * elm)
{
	point.resize(npt);
	for (int i = 0; i < npt; i++) {
		point[i][0] = pt[3 * i];
		point[i][1] = pt[3 * i+1];
		point[i][2] = pt[3 * i+2];
	}
	connection.resize(nelm);
	for (int i = 0; i < nelm; i++) {
		connection[i][0] = elm[3 * i]+1;
		connection[i][1] = elm[3 * i + 1]+1;
		connection[i][2] = elm[3 * i + 2]+1;
	}
	InsertToOctree();
	return;
}

void BKMesh::GetFromMemory(int npt, int nelm, double * pt, int * elm, double * size)
{
	GetFromMemory(npt,nelm,pt,elm);
	init_size.resize(point.size());
	for (int i = 0; i < npt; i++) {
		init_size[i] = size[i];
	}
	return;
}

void BKMesh::CaculateDefaultSize()
{

	init_size = vector<double>(point.size(), 0);
	init_conn = vector<int>(point.size(), 0);
	for (auto i : connection) {
		for (int k = 0; k < 3; k++) {
			int j = (k + 1) % 3;
			init_size[i[k] - 1] += (point[i[k] - 1] - point[i[j] - 1]).magnitude();
			init_conn[i[k] - 1]++;
		}
	}
	target_size = 0;
	for (int i = 0; i < point.size(); i++) {
		if(init_conn[i])
		init_size[i] /= init_conn[i];
		target_size += init_size[i];
	}
	target_size /= point.size();

}