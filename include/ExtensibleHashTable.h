#pragma once
#include <vector>

#include "./Bucket.h"

class ExtensibleHashTable {
	std::vector<Bucket *> directory;
	int globalDepth{};
	int MAX_BUCKET_SIZE{};

	// Helper functions
	void init(int, int);
	void deinit();
	ExtensibleHashTable &copy(const ExtensibleHashTable &);

	int hash(int n);
	int newBucketIndex(int);
	void grow(void);
	void split(int);

	public:
	ExtensibleHashTable();
	ExtensibleHashTable(int);
	ExtensibleHashTable(const ExtensibleHashTable &);
	ExtensibleHashTable &operator=(const ExtensibleHashTable &);
	~ExtensibleHashTable();
	void insert(int);
	bool remove(int);
	bool find(int);
	void print();
};
