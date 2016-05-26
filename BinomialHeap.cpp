#include <cstdlib>

template<class T>
struct Node
{
	Node *right;
	Node *parent;
	Node *leftmostChild;
	T key;
};

template<class T>
class BinomialHeap
{
private:
	// The root of the heap. Should be kept NULL all the time the heap is empty
	Node<T> *root;
	// The number of elements currently stored in the heap.
	int size;
	// the key of the minimal element currently stored in the heap
	Node<T> *min_element;
	// Mergest two binomial trees with equal degree
	Node<T> *mergeTree(Node<T> *leftTree, Node<T> *rightTree);

protected:
	/**
	* Auxiliary function to be used when uniting two heaps.
	*
	* This function should merge the two given root lists in the root list of a
	* single binomial heap - the result of the merging.
	*
	* This method is virtual for ease of testing. Still you need to implement it.
	*
	* |NOTE| This method could have been static as long as it will not access any
	* |NOTE| of the fields of the class. Still it is not static, because in some
	* |NOTE| of the tests it will be overridden.
	*
	* @param leftRoot - the first node of the root list of the first heap
	* @param leftSize - the number of the elements in the left heap
	* @param rightRoot - the first node of the root list of the second heap
	* @param rightSize - the number of the elements in the right heap
	* @return the first node of the root list of the binomial heap result of
	*		the merging of the two given heaps
	*/
	virtual Node<T>* consolidate(Node<T>* leftRoot, int leftSize,
		Node<T>* rightRoot, int rightSize);
public:
	/**
	* Construct empty binomial heap
	*/
	BinomialHeap();

	/**
	* Construct heap with the given elements.
	* Expected runneing time O(elementsSize)
	* @param elements - array containing the elements
	* @param elementsSize - the number of elements in the array
	*/
	BinomialHeap(T elements[], int elementsSize);

	/**
	* @return - the key of the minimal element currently stored in the heap
	*/
	T getMin() const;

	/**
	* @return - the root of the heap. Will be used for testing purposes.
	*/
	Node<T>* getRoot() const;

	/**
	* @return - the number of the elements currently stored in the heap.
	*/
	int getSize() const;

	/**
	* Extracts (removes) the element with minimal key from the heap
	* @return - the key of the minimal element currently stored in the heap
	*/
	T extractMin();

	/**
	* Unites the current heap with the one given.
	* It is expected that the structure of otherHeap will be destroyed in this method.
	*
	* @param otherHeap - the heap with which we will unite. It will be destroyed in this method
	*/
	void unite(BinomialHeap<T>& otherHeap);

	/**
	* Inserts element with the new given key in the heap.
	* @param newKey - the key of the element to be inserted
	* @return a pointer to the newly created element in the heap
	*/
	Node<T>* push(const T& newKey);

	/**
	* Decreases the key of the given element to the given amount.
	* It is guaranteed the ptr is real pointer to element of the heap.
	* WARNING!!!
	*       No changes should be made if the newly given key is larger than
	*       the current value of the key.
	* WARNING!!!
	*
	* @param ptr - a pointer to the element which key is to be modified
	* @param newKey - the new value of the key to be used for element pointed by ptr
	*/
	void decreaseKey(Node<T>* ptr, const T& newKey);
};

template <class T> Node<T> *BinomialHeap<T>::mergeTree(Node<T> *leftRoot, Node<T> *rightRoot)
{
	if (leftRoot->key < rightRoot->key)
	{
		rightRoot->parent = leftRoot;
		rightRoot->right = leftRoot->leftmostChild;
		leftRoot->leftmostChild = rightRoot;
		return leftRoot;
	}
	leftRoot->parent = rightRoot;
	leftRoot->right = rightRoot->leftmostChild;
	rightRoot->leftmostChild = leftRoot;
	return rightRoot;
}

template <class T> Node<T> *BinomialHeap<T>::consolidate(Node<T> *leftRoot, int leftSize, Node<T> *rightRoot, int rightSize)
{
	Node<T> *leftSecond, *rightSecond, *temp;
	if (leftSize == 0)
		return rightRoot;
	if (rightSize == 0)
		return leftRoot;
	while ((leftSize & 1) == 0 && (rightSize & 1) == 0)
	{
		leftSize >>= 1;
		rightSize >>= 1;
	}
	if ((leftSize & 1) == 1)
	{
		if ((rightSize & 1) == 1)
		{
			leftSecond = leftRoot->right;
			rightSecond = rightRoot->right;
			temp = mergeTree(leftRoot, rightRoot);
			if ((leftSize & 2) + (leftSize & 2) == 1)
			{
				if ((leftSize & 2) == 1)
				{
					temp->right = rightSecond;
					return consolidate(leftSecond, leftSize >> 1, temp, (rightSize + 1) >> 1);
				}
				temp->right = leftSecond;
				return consolidate(temp, (leftSize + 1) >> 1, rightSecond, rightSize >> 1);
			}
			temp->right = consolidate(leftSecond, leftSize >> 1, rightSecond, rightSize >> 1);
			return temp;
		}
		leftRoot->right = consolidate(leftRoot->right, leftSize >> 1, rightRoot, rightSize >> 1);
		return leftRoot;
	}
	rightRoot->right = consolidate(leftRoot, leftSize >> 1, rightRoot->right, rightSize >> 1);
	return rightRoot;
}

template<class T> BinomialHeap<T>::BinomialHeap()
{
	root = NULL;
	size = 0;
	min_element = NULL;
}

template<class T> BinomialHeap<T>::BinomialHeap(T elements[], int elementsSize)
{
	int i;
	size = 0;
	min_element = NULL;
	for (i = 0; i < elementsSize; i++)
		push(elements[i]);
}

template <class T> T BinomialHeap<T>::getMin() const
{
	return min_element->key;
}

template <class T> Node<T> *BinomialHeap<T>::getRoot() const
{
	return root;
}

template <class T> int BinomialHeap<T>::getSize() const
{
	return size;
}

template <class T> T BinomialHeap<T>::extractMin()
{
	Node<T> *current = min_element->leftmostChild, *temp;
	BinomialHeap<T> new_binomial_heap;
	new_binomial_heap.min_element = current;
	T result = min_element->key;
	while (current != NULL)
	{
		temp = new_binomial_heap.root;
		new_binomial_heap.root = current;
		new_binomial_heap.root->right = temp;
		new_binomial_heap.root->parent = NULL;
		new_binomial_heap.size = (new_binomial_heap.size << 1) + 1;
		if (new_binomial_heap.min_element->key > current->key)
			new_binomial_heap.min_element = current;
		current = current->right;
	}
	size -= new_binomial_heap.size + 1;
	if (root == min_element)
		root = root->right;
	else
	{
		temp = root;
		while (temp->right != min_element)
			temp = temp->right;
		temp->right = temp->right->right;
	}
	delete min_element;
	if (size == 0)
		min_element = NULL;
	else
	{
		min_element = root;
		temp = root->right;
		while (temp != NULL)
		{
			if (min_element->key > temp->key)
				min_element = temp;
			temp = temp->right;
		}
	}
	unite(new_binomial_heap);
	return result;
}

template <class T> void BinomialHeap<T>::unite(BinomialHeap<T>& otherHeap)
{
	root = consolidate(root, size, otherHeap.root, otherHeap.size);
	size += otherHeap.size;
	if (min_element == NULL || otherHeap.min_element != NULL && min_element->key > otherHeap.min_element->key)
		min_element = otherHeap.min_element;
	otherHeap.root = NULL;
	otherHeap.size = 0;
	otherHeap.min_element = NULL;
}

template <class T> Node<T> *BinomialHeap<T>::push(const T &newKey)
{
	Node <T> *new_node = new Node<T>;
	BinomialHeap<T> new_binomial_heap;
	new_node->right = NULL;
	new_node->parent = NULL;
	new_node->leftmostChild = NULL;
	new_node->key = newKey;
	new_binomial_heap.root = new_node;
	new_binomial_heap.size = 1;
	new_binomial_heap.min_element = new_node;
	unite(new_binomial_heap);
	return new_node;
}

template <class T> void BinomialHeap<T>::decreaseKey(Node<T> *ptr, const T &newKey)
{
	T temp;
	if (ptr->key > newKey)
	{
		ptr->key = newKey;
		while (ptr->parent != NULL && ptr->key < ptr->parent->key)
		{
			temp = ptr->key;
			ptr->key = ptr->parent->key;
			ptr->parent->key = temp;
			ptr = ptr->parent;
		}
		if (ptr->parent == NULL && min_element->key > ptr->key)
			min_element = ptr;
	}
}

int main()
{
  return 0;
}
