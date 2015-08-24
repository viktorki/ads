#include <type_traits>
#include <cstdlib>

template<typename T>
struct Node
{
    T value;
    Node *succ[32];
    
    Node()
    {
        int i;
        for(i = 0; i < 32; i++)
            succ[i] = NULL;
    }
};

template<typename T>
class PersistentVector {
    static_assert(std::is_scalar<T>::value,
        "PersistentVector can only be used with primitive types. Instantiate with a pointer type for"
        "something more complicated.");
    
public:
    PersistentVector();
    
    // Returns the value of the element at position *index*.
    T operator[](int index) const;
    
    // Returns a new vector with the element at position *index* replaced by *value*.
    PersistentVector update(int index, T value) const;
    
    // Returns a new vector with *value* appended at the end.
    PersistentVector append(T value) const;
    
    // Returns a new vector that's the same as this one but without the last element.
    PersistentVector pop() const;
    
    int size() const;
    
private:
    Node<T> *root;
    
    int elements_count;
    
    int depth;
    
    T find(int, const Node<T> *, int) const;
    
    void update(int, const T &, Node<T> *, Node<T> *&, int) const;
    
    void append(const T &, Node<T> *, Node<T> *&, int) const;
    
    void pop(Node<T> *, Node<T> *&, int) const;
};

template<typename T>
PersistentVector<T>::PersistentVector()
{
    elements_count = 0;
    depth = 0;
}

template<typename T>
T PersistentVector<T>::operator[](int index) const
{
    return find(index, root, depth);
}

template<typename T>
T PersistentVector<T>::find(int index, const Node<T> *root, int level) const
{
    if(level == 0)
        return root->value;
    return find(index, root->succ[(index >> 5 * (level - 1)) & 31], level - 1);
}

template<typename T>
PersistentVector<T> PersistentVector<T>::update(int index, T value) const
{
    PersistentVector result;
    update(index, value, root, result.root, depth);
    result.elements_count = elements_count;
    result.depth = depth;
    return result;
}

template<typename T>
void PersistentVector<T>::update(int index, const T &value, Node<T> *old_root, Node<T> *&new_root, int level) const
{
    int pos, i;
    new_root = new Node<T>;
    if(level == 0)
        new_root->value = value;
    else
    {
        pos = (index >> 5 * (level - 1)) & 31;
        for(i = 0; i < 32; i++)
            if(i == pos)
                update(index, value, old_root->succ[i], new_root->succ[i], level - 1);
            else
                new_root->succ[i] = old_root->succ[i];
    }
}

template<typename T>
PersistentVector<T> PersistentVector<T>::append(T value) const
{
    int temp = elements_count;
    PersistentVector<T> result;
    while(temp != 0 && (temp & 31) == 0)
        temp >>= 5;
    if(temp == 1)
    {
        result.root = new Node<T>;
        result.root->succ[0] = root;
        append(value, NULL, result.root->succ[1],  depth);
        result.depth = depth + 1;
    }
    else
    {
        append(value, root, result.root, depth);
        result.depth = depth;
    }
    result.elements_count = elements_count + 1;
    return result;
}

template<typename T>
void PersistentVector<T>::append(const T &value, Node<T> *old_root, Node<T> *&new_root, int level) const
{
    int pos, i;
    new_root = new Node<T>;
    if(level == 0)
        new_root->value = value;
    else
    {
        pos = (elements_count >> 5 * (level - 1)) & 31;
        if(old_root != NULL)
            for(i = 0; i < 32; i++)
                if(i == pos)
                    append(value, old_root->succ[i], new_root->succ[i], level - 1);
                else
                    new_root->succ[i] = old_root->succ[i];
        else
            append(value, NULL, new_root->succ[pos], level - 1);
    }
}

template<typename T>
PersistentVector<T> PersistentVector<T>::pop() const
{
    int temp = elements_count - 1;
    PersistentVector<T> result;
    while(temp != 0 && (temp & 31) == 0)
        temp >>= 5;
    if(temp == 1)
    {
        result.root = root->succ[0];
        result.depth = depth - 1;
    }
    else
    {
        pop(root, result.root, depth);
        result.depth = depth;
    }
    result.elements_count = elements_count - 1;
    return result;
}

template<typename T>
void PersistentVector<T>::pop(Node<T> *old_root, Node<T> *&new_root, int level) const
{
    int pos, i;
    if(level != 0 && (elements_count - 1 & (1 << 5 * level) - 1) != 0)
    {
        new_root = new Node<T>;
        pos = (elements_count - 1 >> 5 * (level - 1)) & 31;
        for(i = 0; i < 32; i++)
            if(i == pos)
                pop(old_root->succ[i], new_root->succ[i], level - 1);
            else
                new_root->succ[i] = old_root->succ[i];
    }
}

template<typename T>
int PersistentVector<T>::size() const
{
    return elements_count;
}

