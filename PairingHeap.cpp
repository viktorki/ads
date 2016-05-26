#include <cstdlib>

template <class T> struct PairingHeapNode
{
	T key;
	PairingHeapNode *leftChild, *leftSibling, *rightSibling, *parent;
};

template <class T> class PairingHeap
{
public:
	PairingHeap();
	void merge(PairingHeap<T> &);
	PairingHeapNode<T> *insert(const T &);
	T getMin() const;
	T extractMin();
	void decreaseKey(PairingHeapNode<T> *, const T &);
	int getSize() const;

private:
	PairingHeapNode<T> *root;
	int size;
	PairingHeapNode<T> *merge(PairingHeapNode<T> *, PairingHeapNode<T> *);
	PairingHeapNode<T> *merge_pairs(PairingHeapNode<T> *);
};

template <class T> PairingHeap<T>::PairingHeap()
{
	root = NULL;
	size = 0;
}

template <class T> void PairingHeap<T>::merge(PairingHeap<T> &ph)
{
	root = merge(root, ph.root);
	size += ph.size;
	ph.root = NULL;
	size = 0;
}

template <class T> PairingHeapNode<T> *PairingHeap<T>::insert(const T &key)
{
	PairingHeapNode<T> *new_root = new PairingHeapNode<T>;
	new_root->key = key;
	new_root->leftChild = NULL;
	new_root->leftSibling = NULL;
	new_root->rightSibling = NULL;
	new_root->parent = NULL;
	root = merge(root, new_root);
	size++;
	return new_root;
}

template <class T> T PairingHeap<T>::getMin() const
{
	return root->key;
}

template <class T> T PairingHeap<T>::extractMin()
{
	T result = root->key;
	PairingHeapNode<T> *current = root->leftChild;
	delete root;
	root = merge_pairs(current);
	size--;
	return result;
}

template <class T> void PairingHeap<T>::decreaseKey(PairingHeapNode<T> *element, const T &new_key)
{
	if (element->key > new_key)
	{
		element->key = new_key;
		if (element != root)
		{
			if (element->parent == NULL)
				element->leftSibling->rightSibling = element->rightSibling;
			else
			{
				element->parent->leftChild = element->rightSibling;
				if (element->rightSibling != NULL)
					element->rightSibling->parent = element->parent;
			}
			element->leftSibling = NULL;
			element->rightSibling = NULL;
			element->parent = NULL;
			root = merge(root, element);
		}
	}
}

template <class T> int PairingHeap<T>::getSize() const
{
	return size;
}

template <class T> PairingHeapNode<T> *PairingHeap<T>::merge(PairingHeapNode<T> *root1, PairingHeapNode<T> *root2)
{
	if (root1 == NULL)
		return root2;
	if (root2 == NULL)
		return root1;
	if (root1->key <= root2->key)
	{
		if (root1->leftChild != NULL)
		{
			root2->rightSibling = root1->leftChild;
			root1->leftChild->leftSibling = root2;
			root1->leftChild->parent = NULL;
		}
		root1->leftChild = root2;
		root2->parent = root1;
		return root1;
	}
	if (root2->leftChild != NULL)
	{
		root1->rightSibling = root2->leftChild;
		root2->leftChild->leftSibling = root1;
		root2->leftChild->parent = NULL;
	}
	root2->leftChild = root1;
	root1->parent = root2;
	return root2;
}

template <class T> PairingHeapNode<T> *PairingHeap<T>::merge_pairs(PairingHeapNode<T> *root)
{
	PairingHeapNode<T> *next, *next_next;
	if (root != NULL)
	{
		if (root->rightSibling != NULL)
		{
			next = root->rightSibling;
			next_next = next->rightSibling;
			root->leftSibling = NULL;
			root->rightSibling = NULL;
			root->parent = NULL;
			next->leftSibling = NULL;
			next->rightSibling = NULL;
			next->parent = NULL;
			return merge(merge(root, next), merge_pairs(next_next));
		}
		root->leftSibling = NULL;
		root->parent = NULL;
	}
	return root;
}

int main()
{
  return 0;
}
