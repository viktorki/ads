#include <cstdlib>

template <class T> struct Node
{
	T key;
	int degree;
	Node *left_child, *left_sibling, *right_sibling, *parent;
	bool marked;
};

template <class T> class FibonacciHeap
{
public:
	FibonacciHeap();
	Node<T> *insert(const T &);
	T getMin() const;
	T extractMin();
	void merge(FibonacciHeap<T> &);
	void decreaseKey(Node <T> *, const T &);
	int getSize() const;

private:
	Node<T> *min_element;
	int size;
	void consolidate();
	void swap_roots(Node<T> *, Node<T> *);
	void cut(Node<T> *, Node<T> *);
	void cascading_cut(Node<T> *);
};

template <class T> FibonacciHeap<T>::FibonacciHeap()
{
	min_element = NULL;
	size = 0;
}

template <class T> Node<T> *FibonacciHeap<T>::insert(const T &key)
{
	Node<T> *new_node = new Node<T>;
	new_node->key = key;
	new_node->degree = 0;
	new_node->left_child = NULL;
	new_node->parent = NULL;
	new_node->marked = false;
	if (min_element == NULL)
	{
		min_element = new_node;
		min_element->left_sibling = min_element;
		min_element->right_sibling = min_element;
	}
	else
	{
		new_node->left_sibling = min_element->left_sibling;
		new_node->right_sibling = min_element;
		min_element->left_sibling = new_node;
		new_node->left_sibling->right_sibling = new_node;
		if (min_element->key > key)
			min_element = new_node;
	}
	size++;
	return new_node;
}

template <class T> T FibonacciHeap<T>::getMin() const
{
	return min_element->key;
}

template <class T> T FibonacciHeap<T>::extractMin()
{
	T result = min_element->key;
	Node<T> *current, *next;
	if (size == 1)
		min_element = NULL;
	else
	{
		current = min_element->left_child;
		if (current != NULL)
		do
		{
			next = current->right_sibling;
			current->left_sibling = min_element->left_sibling;
			current->right_sibling = min_element;
			min_element->left_sibling = current;
			current->left_sibling->right_sibling = current;
			current->parent = NULL;
			current = next;
		} while (current != min_element->left_child);
		current = min_element;
		min_element->left_sibling->right_sibling = min_element->right_sibling;
		min_element->right_sibling->left_sibling = min_element->left_sibling;
		min_element = min_element->right_sibling;
		delete current;
		consolidate();
	}
	size--;
	return result;
}

template <class T> void FibonacciHeap<T>::merge(FibonacciHeap<T> &fh)
{
	Node<T> *temp, *fh_temp;
	if (min_element == NULL)
		min_element = fh.min_element;
	else if (fh.min_element != NULL)
	{
		temp = min_element->right_sibling;
		fh_temp = fh.min_element->right_sibling;
		min_element->right_sibling = fh_temp;
		fh.min_element->right_sibling = temp;
		temp->left_sibling = fh.min_element;
		fh_temp->left_sibling = min_element;
		if (min_element->key > fh.min_element->key)
			min_element = fh.min_element;
	}
	size += fh.size;
	fh.min_element = NULL;
	fh.size = 0;
}

template <class T> void FibonacciHeap<T>::consolidate()
{
	int temp = size, max_degree = 0, i;
	Node<T> **a, *x = min_element, *last = min_element->left_sibling;
	while (temp != 0)
	{
		temp >>= 1;
		max_degree++;
	}
	a = new Node<T> *[max_degree + 1];
	for (i = 0; i <= max_degree; i++)
		a[i] = NULL;
	while (1)
	{
		while (a[x->degree] != NULL)
		{
			Node<T> *y = a[x->degree];
			if (x->key > y->key)
				swap_roots(x, y);
			y->left_sibling->right_sibling = y->right_sibling;
			y->right_sibling->left_sibling = y->left_sibling;
			y->parent = x;
			if (x->left_child == NULL)
			{
				y->left_sibling = y;
				y->right_sibling = y;
			}
			else
			{
				y->left_sibling = x->left_child->left_sibling;
				y->right_sibling = x->left_child;
				y->right_sibling->parent = NULL;
				y->right_sibling->left_sibling = y;
				y->left_sibling->right_sibling = y;
			}
			x->left_child = y;
			y->marked = false;
			a[x->degree++] = NULL;
		}
		a[x->degree] = x;
		if (x == last)
			break;
		x = x->right_sibling;
	}
	min_element = NULL;
	for (i = 0; i < max_degree; i++)
	if (a[i] != NULL)
	if (min_element == NULL)
	{
		min_element = a[i];
		min_element->left_sibling = min_element;
		min_element->right_sibling = min_element;
		min_element->parent = NULL;
	}
	else
	{
		a[i]->left_sibling = min_element->left_sibling;
		a[i]->right_sibling = min_element;
		min_element->left_sibling = a[i];
		a[i]->left_sibling->right_sibling = a[i];
		if (min_element->key > a[i]->key)
			min_element = a[i];
	}
	delete[] a;
}

template <class T> void FibonacciHeap<T>::swap_roots(Node<T> *x, Node<T> *y)
{
	T temp_key = x->key;
	Node<T> *temp_child = x->left_child;
	x->key = y->key;
	y->key = temp_key;
	x->left_child = y->left_child;
	y->left_child = temp_child;
	if (x->left_child != NULL)
		x->left_child->parent = x;
	if (y->left_child != NULL)
		y->left_child->parent = y;
}

template <class T> void FibonacciHeap<T>::decreaseKey(Node<T> *element, const T &new_key)
{
	if (element->key > new_key)
	{
		element->key = new_key;
		Node<T> *y = element->parent;
		if (y != NULL && element->key < y->key)
		{
			cut(element, y);
			cascading_cut(y);
		}
		if (min_element->key > element->key)
			min_element = element;
	}
}

template <class T> void FibonacciHeap<T>::cut(Node<T> *x, Node<T> *y)
{
	if (x->right_sibling == x)
		y->left_child = NULL;
	else
	{
		if (y->left_child == x)
		{
			y->left_child = y->left_child->right_sibling;
			y->left_child->parent = y;
		}
		x->left_sibling->right_sibling = x->right_sibling;
		x->right_sibling->left_sibling = x->left_sibling;

	}
	y->degree--;
	x->left_sibling = min_element->left_sibling;
	x->right_sibling = min_element;
	min_element->left_sibling = x;
	x->left_sibling->right_sibling = x;
	x->parent = NULL;
	x->marked = false;
}

template <class T> void FibonacciHeap<T>::cascading_cut(Node<T> *y)
{
	Node<T> *z = y->parent;
	if (z != NULL)
		if (y->marked)
		{
			cut(y, z);
			cascading_cut(z);
		}
		else
			y->marked = true;
}

template <class T> int FibonacciHeap<T>::getSize() const
{
	return size;
}

int main()
{
  return 0;
}
