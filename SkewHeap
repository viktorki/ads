/*
* Data type that represents a template of min-heap implementation of Skew heap data stucture
*/
template<typename T>
class SkewHeap
{
public:
	SkewHeap()
	{
		root = NULL;
		size = 0;
	}

	~SkewHeap()
	{
		deleteSubtree(root);
	}

	/*
	* Method that adds a node with data provided by @value in the current skew heap.
	*
	* @param value: value to be added in the heap
	*/
	void add(const T& value)
	{
		Node *newNode = new Node(value);
		root = merge(root, newNode);
		size++;
	}

	/*
	* This method removes and returns the smallest element in the current skew heap.
	*
	* @returns the removed element or throws exception if there are no any elements in the heap
	*/
	T removeMin()
	{
		T minValue = root->value;
		Node* temp = root;
		root = merge(root->left, root->right);
		delete temp;
		size--;
		return minValue;
	}

	/*
	* Tests whether there are any elements in the current heap.
	*
	* @returns true, if there are no any elements and false, otherwise
	*/
	bool empty() const
	{
		return size == 0;
	}

	/*
	* Method that merges the current skew heap with the given by @other.
	* This method destructs the @other skew heap while merging it.
	*
	* @param other: reference to the skew heap data structure that will be merged with the current one
	*/
	void merge(SkewHeap<T>& other)
	{
		root = merge(root, other.root);
		size += other.size;
		other.root = NULL;
		other.size = 0;
	}

private:
	/*
	* Data type that represents a node in the Skew heap
	*/
	class Node
	{
	public:
		Node(const T& value)
		{
			this->value = value;
			this->left = NULL;
			this->right = NULL;
		}

		~Node()
		{
		}

		T value; // data value
		Node* left; // left subtree
		Node* right; // right subtree
	};

	void deleteSubtree(Node *root)
	{
		if (root != NULL)
		{
			deleteSubtree(root->left);
			deleteSubtree(root->right);
			delete root;
		}
	}

	/*
	* Method that merges two skew heap data structures pointed to their roots by @root1 and @root2
	*
	* @param root1: pointer to the root of the first skew heap
	* @param root2: pointer to the root of the second skew heap
	*
	* @returns a pointer to the root of the merged data structure
	*/
	Node* merge(Node* root1, Node* root2)
	{
		if (root1 == NULL)
			return root2;
		if (root2 == NULL)
			return root1;
		if (root1->value <= root2->value)
		{
			Node* temp = root1->left;
			root1->left = merge(root1->right, root2);
			root1->right = temp;
			return root1;
		}
		Node* temp = root2->left;
		root2->left = merge(root1, root2->right);
		root2->right = temp;
		return root2;
	}

	/*
	* Pointer to the root for the current skew heap
	*/
	Node* root;

	int size;
};

int main()
{
	return 0;
}
