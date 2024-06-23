#include "../include/ExtensibleHashTable.h"

#include <algorithm>
#include <bitset>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <set>
#include <stdexcept>
#include <vector>

#include "../include/Bucket.h"

using namespace std;

ExtensibleHashTable::ExtensibleHashTable() {
	init(1, 4);
}

ExtensibleHashTable::ExtensibleHashTable(int maxBucketSize) {
	init(1, maxBucketSize);
}

// Copy constructor
ExtensibleHashTable::ExtensibleHashTable(const ExtensibleHashTable &other) {
	copy(other);
}

// Overloaded assignment operator
ExtensibleHashTable &ExtensibleHashTable::operator=(const ExtensibleHashTable &other) {
	if (this == &other) {
		return *this;
	}
	return copy(other);
}

ExtensibleHashTable::~ExtensibleHashTable() {
	deinit();
}

void ExtensibleHashTable::init(int depth, int bucket_size) {
	this->globalDepth = depth;
	this->MAX_BUCKET_SIZE = bucket_size;

	for (int i = 0; i < 1 << depth; i++) {
		directory.push_back(new Bucket(depth));
	}
}

ExtensibleHashTable &ExtensibleHashTable::copy(const ExtensibleHashTable &other) {
	deinit();

	init(other.globalDepth, other.MAX_BUCKET_SIZE);

	for (const auto &bucket : other.directory) {
		for (const auto &key : bucket->values) {
			insert(key);
		}
	}

	return *this;
}

void ExtensibleHashTable::deinit() {
	std::set<Bucket *> deleted;
	for (auto bucket : directory) {
		if (deleted.find(bucket) == deleted.end()) {
			deleted.insert(bucket);
			delete bucket;
		}
	}

	directory.clear();
}

int ExtensibleHashTable::hash(int key) {
	return key & ((1 << globalDepth) - 1);
}

void ExtensibleHashTable::split(int bucket_no) {
	// if the bucket needs to be split that means we will create a new bucket
	// the new bucket might fall into the same directory size or we might have to grow it
	// localDepth & globalDepth are used to track if need to grow the directory or not

	auto localDepth = ++(directory.at(bucket_no)->localDepth);

	// if the local depth is greater than the global depth, we need to grow the directory
	if (localDepth > globalDepth)
		grow();

	// intialize the new bucket
	directory.at(newBucketIndex(bucket_no)) = new Bucket(localDepth);

	// clear the old bucket to make space for the re-insertion based on a new hash
	// but we need to save the values temporarily to re-insert them
	auto temp = directory.at(bucket_no)->values;
	directory.at(bucket_no)->values.clear();

	// insert the copied values into the old and new bucket

	for (auto key : temp) {
		insert(key);
	}
}

void ExtensibleHashTable::grow(void) {
	for (int i = 0; i < 1 << globalDepth; i++) {
		directory.push_back(directory[i]);
	}
	globalDepth++;
}

int ExtensibleHashTable::newBucketIndex(int bucket) {
	return bucket ^ (1 << (globalDepth - 1));
}

bool ExtensibleHashTable::find(int key) {
	if (directory[hash(key)]->find(key)) {
		return true;
	}
	return false;
}

void ExtensibleHashTable::insert(int key) {
	if (key < 0) {
		return;
	}
	try {
		auto &bucket = directory.at(hash(key))->values;

		if (bucket.size() < MAX_BUCKET_SIZE) {
			bucket.push_back(key);
		} else {
			split(hash(key));
			if (directory.at(hash(key))->values.size() >= MAX_BUCKET_SIZE) {
				throw std::runtime_error("Runtime Error: Bucket is full after split");
			}
			insert(key);
		}

	} catch (const std::exception &ex) {
		std::cout << "Exception Caught :: " << ex.what() << std::endl;
		exit(1);
	}
}

void ExtensibleHashTable::print() {
	std::set<Bucket *> printed;

	for (long unsigned int i = 0; i < directory.size(); ++i) {
		std::cout << i << ": " << std::setw(8) << std::setfill('0') << std::hex << directory[i] << std::dec << " --> ";

		if (printed.find(directory[i]) != printed.end()) {
			cout << endl;
			continue;
		}
		printed.insert(directory[i]);

		long unsigned int j = 0;
		std::cout << "[";
		for (; j < directory[i]->values.size(); ++j) {
			std::cout << directory[i]->values[j];
			if (j != MAX_BUCKET_SIZE - 1) {
				std::cout << ",";
			}
		}

		for (; j < MAX_BUCKET_SIZE; ++j) {
			std::cout << "-";
			if (j != MAX_BUCKET_SIZE - 1) {
				std::cout << ",";
			}
		}
		std::cout << "] (" << directory[i]->localDepth << ")\n";
	}
}

bool ExtensibleHashTable::remove(int key) {
	auto bucket = directory[hash(key)];
	auto position = bucket->findPos(key);

	if (position == std::end(bucket->values)) {
		return false;
	}

	bucket->values.erase(position);
	return true;
}