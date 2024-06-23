#pragma once
#include <vector>

class Bucket {
	public:
	int localDepth;
	std::vector<int> values;
	Bucket(int depth);
	std::vector<int>::iterator findPos(int key);
	bool find(int key);
};