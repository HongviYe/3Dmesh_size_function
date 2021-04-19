#include  "BKMesh.h"
using namespace std;


int main() {
	BKMesh::GetSingletonPtr()->GetFromFile("f6_cf.pls");
	BKMesh::GetSingletonPtr()->InsertToOctree();
	clock_t startTime, endTime;
	 startTime = clock();//计时开始
	for (int j = 0; j < 20000; j++) {
		BKMesh::GetSingletonPtr() -> GetSize(0,0,0);
	}
	endTime = clock();//计时结束
	cout << "The run time is: " << (double)(endTime - startTime) / ((clock_t)1000) << "s" << endl;
	
	return 0;
}