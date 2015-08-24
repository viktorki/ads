/**
* Interval Tree implementation in C++
* @author: Viktor Ivanov
* @keywords: Data Structures, Dynamic RMQ, Interaval update and query
* @modified:
*
* Implement a dynamic RMQ with interval query and update. The update should
* add a certain value to all cells in a given interval, while the query should
* return the maximum value inside an interval.
*/

#ifndef __INTERVAL_TREE_H__
#define __INTERVAL_TREE_H__

#include <algorithm>
#include <vector>
using namespace std;

template <typename T> class IntervalTree {
public:

	/**
	* Creates a new interval tree with initial values given in values.
	* Each update and query should allow indices [0, values.size() - 1].
	*/
	IntervalTree(const vector<T>& values) {
		int tree_size, i;
		interval_size = values.size();
		tree_size = interval_size << 1;
		tree = new T[tree_size];
		lazy = new T[tree_size];
		for (i = 1; i < tree_size; i++)
			lazy[i] = 0;
		init(values, 1, 0, interval_size - 1);
	}

	~IntervalTree() {
		delete[] tree;
		delete[] lazy;
	}

	/**
	* Adds the value add to each element in the interval [idx1, idx2].
	*/
	void update(int idx1, int idx2, T add) {
		update(idx1, idx2, add, 1, 0, interval_size - 1);
	}

	/**
	* Returns the maximum value in the interval [idx1, idx2].
	*/
	T query(int idx1, int idx2) {
		return query(idx1, idx2, 1, 0, interval_size - 1);
	}

private:

	void init(const vector<T> &values, int pos, int begin, int end) {
		int mid;
		if (begin == end)
			tree[pos] = values[begin];
		else {
			mid = (begin + end) >> 1;
			init(values, pos << 1, begin, mid);
			init(values, (pos << 1) + 1, mid + 1, end);
			tree[pos] = max(tree[pos << 1], tree[(pos << 1) + 1]);
		}
	}

	void update(int idx1, int idx2, const T &add, int pos, int begin, int end) {
		int mid;
		if (lazy[pos] != 0) {
			tree[pos] += lazy[pos];
			if (begin != end) {
				lazy[pos << 1] += lazy[pos];
				lazy[(pos << 1) + 1] += lazy[pos];
			}
			lazy[pos] = 0;
		}
		if (begin >= idx1 && end <= idx2) {
			tree[pos] += add;
			if (begin != end) {
				lazy[pos << 1] += add;
				lazy[(pos << 1) + 1] += add;
			}
		} else if (begin <= idx2 && end >= idx1) {
			mid = (begin + end) >> 1;
			update(idx1, idx2, add, pos << 1, begin, mid);
			update(idx1, idx2, add, (pos << 1) + 1, mid + 1, end);
			tree[pos] = max(tree[pos << 1], tree[(pos << 1) + 1]);
		}
	}

	T query(int idx1, int idx2, int pos, int begin, int end) const {
		int mid;
		if (begin > idx2 || end < idx1)
			return -2100000000;
		if (lazy[pos] != 0) {
			tree[pos] += lazy[pos];
			if (begin != end) {
				lazy[pos << 1] += lazy[pos];
				lazy[(pos << 1) + 1] += lazy[pos];
			}
			lazy[pos] = 0;
		}
		if (begin >= idx1 && end <= idx2)
			return tree[pos];
		mid = (begin + end) >> 1;
		return max(query(idx1, idx2, pos << 1, begin, mid), query(idx1, idx2, (pos << 1) + 1, mid + 1, end));
	}

	T *tree, *lazy;

	int interval_size;
};

#endif // #ifndef __INTERVAL_TREE_H__


#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>

const int MAX_IDX = 65536;

const int NUM_QUERIES = 65536;
const int QUERY = 0;
const int UPDATE = 1;

struct Query {
	int x1, x2;
	int val, type;
};

int n;
int init[MAX_IDX]; // Initial values
Query a[NUM_QUERIES]; // Save queries
int ans1[NUM_QUERIES], ans2[NUM_QUERIES]; // Save answers

void solveTrees() {
	IntervalTree <int> tree(vector<int>(init, init + MAX_IDX));
	for (int i = 0; i < n; i++) {
		if (a[i].type == QUERY)
			ans1[i] = tree.query(a[i].x1, a[i].x2);
		else {
			ans1[i] = -1;
			tree.update(a[i].x1, a[i].x2, a[i].val);
		}
	}
}

int dummy[MAX_IDX];
void dummyUpdate(int x1, int x2, int val) {
	for (int x = x1; x <= x2; x++)
		dummy[x] += val;
}

int dummyQuery(int x1, int x2) {
	int ret = -2100000000;
	for (int x = x1; x <= x2; x++)
		ret = max(ret, dummy[x]);
	return ret;
}

void solveDummy() {
	for (int i = 0; i < MAX_IDX; i++)
		dummy[i] = init[i];
	for (int i = 0; i < n; i++) {
		if (a[i].type == QUERY)
			ans2[i] = dummyQuery(a[i].x1, a[i].x2);
		else {
			ans2[i] = -1;
			dummyUpdate(a[i].x1, a[i].x2, a[i].val);
		}
	}
}


int rand30() {
	return (((rand() & (((1 << 10) - 1) << 3)) >> 3) << 0) |
		(((rand() & (((1 << 10) - 1) << 3)) >> 3) << 10) |
		(((rand() & (((1 << 10) - 1) << 3)) >> 3) << 20);
}

int main(void) {
	// Generate test;
	srand(42);

	for (int i = 0; i < MAX_IDX; i++)
		init[i] = rand30() % 2000001 - 1000000;

	n = NUM_QUERIES; // Set number of queries
	for (int i = 0; i < n; i++) {
		a[i].x1 = rand30() % MAX_IDX; a[i].x2 = rand30() % MAX_IDX;
		if (a[i].x1 > a[i].x2)
			std::swap(a[i].x1, a[i].x2);

		a[i].val = rand30() % 20001 - 10000;
		a[i].type = rand30() % 2 ? QUERY : UPDATE;
	}

	unsigned startTime;
	// Solve with interval trees
	startTime = clock();
	solveTrees();
	fprintf(stdout, "Interval trees execution time: %.3lfs\n",
		(double)(clock() - startTime) / (double)CLOCKS_PER_SEC);

	// Check by dummy
	startTime = clock();
	solveDummy();
	fprintf(stdout, "Dummy solution execution time: %.3lfs\n",
		(double)(clock() - startTime) / (double)CLOCKS_PER_SEC);

	int correct = 1;
	for (int i = 0; i < n; i++) {
		if (a[i].type == QUERY) {
			if (ans1[i] != ans2[i]) {
				correct = 0;
				break;
			}
		}
	}
	fprintf(stdout, "Solution is %s.\n", correct ? "correct" : "incorect");
	return 0;
}
