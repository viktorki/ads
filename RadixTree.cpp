#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

struct Edge;

struct Node
{
	int data;
	map<char, Edge *> edges;

	Node()
	{
		data = -1;
	}
};

struct Edge
{
	string label;
	Node *target;
};

class RadixTree
{
public:
	RadixTree();
	~RadixTree();
	void insert(const char *, int);
	int find(const char *) const;
	vector<int> getAllWithPrefix(const char *) const;
	bool remove(const char*);

private:
	Node *root;

	void del(Node *);
	int find(const char *, Node *) const;
	void insert(const char *, int, Node *&);
	vector<int> getAllWithPrefix(const char *, Node *) const;
	void dfs(Node *, vector<int> &) const;
	bool remove(const char*, Node *&);
};

RadixTree::RadixTree()
{
	root = new Node;
}

RadixTree::~RadixTree()
{
	del(root);
}

void RadixTree::del(Node *root)
{
	map<char, Edge *>::iterator it;
	for (it = root->edges.begin(); it != root->edges.end(); it++)
		del(it->second->target);
	delete root;
}

void RadixTree::insert(const char *word, int data)
{
	insert(word, data, root);
}

void RadixTree::insert(const char *word, int data, Node *&root)
{
	int word_length = strlen(word), label_length, pos;
	string label;
	map<char, Edge *>::iterator it;
	Edge *e;

	if (word_length == 0)
		root->data = data;
	else
	{
		it = root->edges.find(word[0]);
		if (it == root->edges.end())
		{
			e = new Edge;
			e->label = word;
			e->target = new Node;
			e->target->data = data;
			root->edges[word[0]] = e;
		}
		else
		{
			label = it->second->label;
			label_length = label.size();
			pos = 0;
			while (pos < word_length && pos < label_length && word[pos] == label[pos])
				pos++;
			if (pos == label_length)
				insert(word + label_length, data, it->second->target);
			else
			{
				e = new Edge;
				e->label = label.substr(pos);
				e->target = it->second->target;
				Node *node = new Node;
				node->edges[e->label[0]] = e;
				it->second->label = label.substr(0, pos);
				it->second->target = node;
				insert(word + pos, data, node);
			}
		}
	}
}

int RadixTree::find(const char* word) const
{
	return find(word, root);
}

int RadixTree::find(const char *word, Node *root) const
{
	int word_length = strlen(word), label_length, i;
	map<char, Edge *>::iterator it;
	string label;

	if (word_length == 0)
		return root->data;

	it = root->edges.find(word[0]);
	if (it == root->edges.end())
		return -1;

	label = it->second->label;
	label_length = label.size();
	if (label_length > word_length)
		return -1;

	for (i = 0; i < label_length; i++)
		if (word[i] != label[i])
			return -1;

	return find(word + label_length, it->second->target);
}

vector<int> RadixTree::getAllWithPrefix(const char* prefix) const
{
	return getAllWithPrefix(prefix, root);
}

vector<int> RadixTree::getAllWithPrefix(const char* prefix, Node *root) const
{
	int prefix_length = strlen(prefix), label_length, i;
	map<char, Edge *>::iterator it;
	string label;
	vector<int> result;

	if (prefix_length == 0)
	{
		dfs(root, result);
		return result;
	}

	it = root->edges.find(prefix[0]);
	if (it == root->edges.end())
		return result;

	label = it->second->label;
	label_length = label.size();
	if (label_length > prefix_length)
	{
		for (i = 0; i < prefix_length; i++)
			if (prefix[i] != label[i])
				return result;

		dfs(it->second->target, result);
		return result;
	}

	for (i = 0; i < label_length; i++)
		if (prefix[i] != label[i])
			return result;

	return getAllWithPrefix(prefix + label_length, it->second->target);
}

void RadixTree::dfs(Node *root, vector<int> &result) const
{
	map<char, Edge *>::iterator it;

	if (root->data != -1)
		result.push_back(root->data);

	for (it = root->edges.begin(); it != root->edges.end(); it++)
		dfs(it->second->target, result);
}

bool RadixTree::remove(const char *word)
{
	return remove(word, root);
}

bool RadixTree::remove(const char *word, Node *&root)
{
	int word_length = strlen(word), label_length, i;
	map<char, Edge *>::iterator it;
	string label;

	it = root->edges.find(word[0]);
	if (it == root->edges.end())
		return false;

	label = it->second->label;
	label_length = label.size();
	if (label_length > word_length)
		return false;

	for (i = 0; i < label_length; i++)
		if (word[i] != label[i])
			return false;

		if (word_length == label_length)
		{
			if (it->second->target->edges.empty())
			{
				delete it->second->target;
				delete it->second;
				root->edges.erase(it);
			}
			else if (it->second->target->edges.size() == 1)
			{
				it->second->label += it->second->target->edges.begin()->second->label;
				Node *temp = it->second->target;
				it->second->target = it->second->target->edges.begin()->second->target;
				delete temp->edges.begin()->second;
				delete temp;
			}
			else
				it->second->target->data = -1;

			return true;
		}

	return remove(word + label_length, it->second->target);
}

bool compare(pair<string, int> p1, pair<string, int> p2)
{
	return p1.second > p2.second;
}

int main(int argc, char* argv[])
{
	ifstream dictionary(argv[1]), text(argv[2]);
	int data, result_length, i, q, words_length;
	char word[1024];
	vector<int> result;
	vector<pair<string, int>> words;
	RadixTree rpt;

	while (1)
	{
		dictionary >> word >> data;
		if (dictionary.eof())
			break;
		rpt.insert(word, data);
	}

	while (1)
	{
		text >> word;
		if (text.eof())
			break;
		result = rpt.getAllWithPrefix(word);
		result_length = result.size();
		q = 0;
		for (i = 0; i < result_length; i++)
			q += result[i];
		words.push_back(pair<string, int>(word, q));
	}

	sort(words.begin(), words.end(), compare);

	words_length = words.size();
	for (i = 0; i < words_length; i++)
		cout << words[i].first << " " << words[i].second << endl;

	return 0;
}
