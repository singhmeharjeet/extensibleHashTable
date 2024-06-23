#include "../include/Bucket.h"

#include <algorithm>
#include <bitset>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <set>
#include <stdexcept>
#include <vector>

using std::cout;
using std::endl;

Bucket::Bucket(int depth) {
	localDepth = depth;
}
std::vector<int>::iterator Bucket::findPos(int key) {
	for (auto it = values.begin(); it != values.end(); ++it) {
		if (*it == key) {
			return it;
		}
	}
	return values.end();
}
bool Bucket::find(int key) {
	return findPos(key) != values.end();
}