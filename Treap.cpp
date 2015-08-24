#include <cstdlib>

class TreapNode
{
public:
	TreapNode(int k)
	{
		key = k;
		priority = (float) rand() / RAND_MAX;
		left = NULL;
		right = NULL;
	}
	int key;	// key provided by user
	float priority;	// node's generated priority
	TreapNode* left;	// pointer for the left subtree
	TreapNode* right;	// pointer for the right subtree
};

class Treap
{
public:
	Treap();
	~Treap();

	/**
	* Inserts a node in the treap with the provided @key
	*
	* @param key: key to be inserted
	*/
	void insert(int key);

	/**
	* Removes the node from the treap with the provided @key
	*
	* @param key: key to be removed
	*/
	void remove(int key);

	/**
	* Checks whether a given @key is already in the treap
	*
	* @param key: key to be searched for
	* @return true, if the key is in the treap, and false, otherwise
	*/
	bool containsKey(int key) const;

private:
	Treap& operator=(const Treap&);
	Treap(const Treap&);
	void deleteSubtree(TreapNode *);
	void insert(TreapNode *&, TreapNode *&, int);
	void remove(TreapNode *&, int);
	bool containsKey(TreapNode *, int) const;
	void rotateLeft(TreapNode *&);
	void rotateRight(TreapNode *&);

	TreapNode *root;
};

Treap::Treap()
{
	root = NULL;
}

Treap::~Treap()
{
	deleteSubtree(root);
}

void Treap::insert(int key)
{
	insert(root, root, key);
}

void Treap::remove(int key)
{
	remove(root, key);
}

bool Treap::containsKey(int key) const
{
	return containsKey(root, key);
}

void Treap::deleteSubtree(TreapNode *node)
{
	if (node != NULL)
	{
		deleteSubtree(node->left);
		deleteSubtree(node->right);
		delete node;
	}
}

void Treap::insert(TreapNode *&root, TreapNode *&parent, int key)
{
	if (root == NULL)
		root = new TreapNode(key);
	else if (root->key > key)
		insert(root->left, root, key);
	else
		insert(root->right, root, key);

	if (root->priority < parent->priority)
		if (parent->key > key)
			rotateRight(parent);
		else
			rotateLeft(parent);
}

void Treap::remove(TreapNode *&root, int key)
{
	if (root != NULL)
		if (root->key > key)
			remove(root->left, key);
		else if (root->key < key)
			remove(root->right, key);
		else if (root->left != NULL || root->right != NULL)
			if (root->right == NULL)
			{
				rotateRight(root);
				remove(root->right, key);
			}
			else if (root->left == NULL)
			{
				rotateLeft(root);
				remove(root->left, key);
			}
			else if (root->left->priority < root->right->priority)
			{
				rotateRight(root);
				remove(root->right, key);
			}
			else
			{
				rotateLeft(root);
				remove(root->left, key);
			}
		else
		{
			root = NULL;
			delete root;
		}
}

bool Treap::containsKey(TreapNode *root, int key) const
{
	if (root == NULL)
		return false;
	if (root->key == key)
		return true;
	if (root->key > key)
		return containsKey(root->left, key);
	return containsKey(root->right, key);
}

void Treap::rotateLeft(TreapNode *&node)
{
	TreapNode *temp = node->right;

	node->right = temp->left;
	temp->left = node;
	node = temp;
}

void Treap::rotateRight(TreapNode *&node)
{
	TreapNode *temp = node->left;

	node->left = temp->right;
	temp->right = node;
	node = temp;
}

int main()
{
	return 0;
}
