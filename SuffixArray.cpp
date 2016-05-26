#include <cstring>
#include <vector>
using namespace std;

const int ALPHABET_SIZE = 256;

class SuffixArray
{
public:
    SuffixArray(const char *);
    ~SuffixArray();
    vector<int> search(const char *) const;
private:
    int length, *suffix_array, *lcp_array, *lcp_lr;
    char *s;
    void construct_suffix_array(int[], int[], int, int);
    void radix_sort(int[], int[], int[], int, int);
    void construct_lcp_array();
    void construct_lcp_lr(int, int, int);
    int left_search(const char *, int, int, int, int, int) const;
    int right_search(const char *, int, int, int, int, int) const;
};

SuffixArray::SuffixArray(const char *word)
{
    int *si, i;
    length = strlen(word);
    s = new char[length + 1];
    si = new int[length + 3];
    strcpy(s, word);
    suffix_array = new int[length];
    for(i = 0; i < length; i++)
        si[i] = s[i];
    si[length] = 0;
    si[length + 1] = 0;
    si[length + 2] = 0;
    construct_suffix_array(si, suffix_array, length, ALPHABET_SIZE);
    construct_lcp_array();
    lcp_lr = new int[length << 2];
    construct_lcp_lr(1, 0, length - 2);
    delete[] si;
}

SuffixArray::~SuffixArray()
{
    delete[] s;
    delete[] suffix_array;
    delete[] lcp_array;
    delete[] lcp_lr;
}

void SuffixArray::construct_suffix_array(int s[], int sa[], int n, int alphabet_size)
{
    int n0 = (n + 2) / 3, n2 = n / 3, n02, *s12, *sa12, n1 = (n + 1) / 3, *s0, *sa0, t, i, j = 0, p = 0, c0 = -1, c1 = -1, c2 = -1, new_alphabet_size = 0, k;
    n02 = n0 + n2;
    s12 = new int[n02 + 3];
    s12[n02] = 0;
    s12[n02 + 1] = 0;
    s12[n02 + 2] = 0; 
    sa12 = new int[n02 + 3];
    sa12[n02] = 0;
    sa12[n02 + 1] = 0;
    sa12[n02 + 2] = 0;
    s0 = new int[n0];
    sa0 = new int[n0];
    t = n0 - n1;
    for(i = 0; i < n + t; i++)
        if(i % 3 != 0)
            s12[j++] = i;
    radix_sort(s12, sa12, s + 2, n02, alphabet_size);
    radix_sort(sa12, s12, s + 1, n02, alphabet_size);  
    radix_sort(s12, sa12, s, n02, alphabet_size);
    for(i = 0; i < n02; i++)
    {
        if(s[sa12[i]] != c0 || s[sa12[i] + 1] != c1 || s[sa12[i] + 2] != c2)
        { 
            c0 = s[sa12[i]];
            c1 = s[sa12[i] + 1];
            c2 = s[sa12[i] + 2];
            new_alphabet_size++;
        }
        if(sa12[i] % 3 == 1)
            s12[sa12[i] / 3] = new_alphabet_size;
        else
            s12[sa12[i] / 3 + n0] = new_alphabet_size;
    }
    if(new_alphabet_size < n02)
    {
        construct_suffix_array(s12, sa12, n02, new_alphabet_size);
        for(i = 0; i < n02; i++)
            s12[sa12[i]] = i + 1;
    }
    else
        for(i = 0; i < n02; i++)
            sa12[s12[i] - 1] = i; 
    j = 0;
    for(i = 0; i < n02; i++)
        if(sa12[i] < n0)
            s0[j++] = 3 * sa12[i];
    radix_sort(s0, sa0, s, n0, alphabet_size);
    for(k = 0; k < n; k++)
    {
        if(sa12[t] < n0)
            i = sa12[t] * 3 + 1;
        else
            i = (sa12[t] - n0) * 3 + 2;
        j = sa0[p];
        if(sa12[t] < n0 && (s[i] < s[j] || s[i] == s[j] && s12[sa12[t] + n0] <= s12[j / 3]) || sa12[t] >= n0 && (s[i] < s[j] || (s[i] == s[j] && (s[i + 1] < s[j + 1] || s[i + 1] == s[j + 1] && s12[sa12[t] - n0 + 1] <= s12[j / 3 + n0]))))
        {
            sa[k] = i;
            t++;
            if(t == n02)
            {
                k++;
                while(p < n0)
                    sa[k++] = sa0[p++];
            }
        }
        else
        { 
            sa[k] = j;
            p++; 
            if(p == n0)
            {
                k++;
                while(t < n02)
                {
                    if(sa12[t] < n0)
                        sa[k] = sa12[t] * 3 + 1;
                    else
                        sa[k] = (sa12[t] - n0) * 3 + 2;
                    k++;
                    t++;
                }
            }
        }
    }
    delete [] s12;
    delete [] sa12;
    delete [] sa0;
    delete [] s0;
}

void SuffixArray::radix_sort(int a[], int b[], int r[], int n, int alphabet_size)
{
    int i, *c = new int[alphabet_size + 1], t, sum = 0;
    for(i = 0; i <= alphabet_size; i++)
        c[i] = 0;
    for(i = 0; i < n; i++)
        c[r[a[i]]]++;
    for(i = 0; i <= alphabet_size; i++)
    {
        t = c[i];
        c[i] = sum;
        sum += t;
    }
    for(i = 0; i < n; i++)
        b[c[r[a[i]]]++] = a[i];
    delete[] c;
}

void SuffixArray::construct_lcp_array()
{
    int i, *rank = new int[length], k = 0, j;
    lcp_array = new int[length];
    for(i = 0; i < length; i++)
        rank[suffix_array[i]] = i;
    for(i = 0; i < length; i++)
    {
        if(k != 0)
            k--;
        if(rank[i] == length - 1)
            k = 0;
        else
        {
            j = suffix_array[rank[i] + 1];
            while(i + k < length && j + k < length && s[i + k] == s[j + k])
                k++;
            lcp_array[rank[i]] = k;
        }
    }
    delete[] rank;
}

void SuffixArray::construct_lcp_lr(int pos, int left, int right)
{
    int mid;
    if(left == right)
        lcp_lr[pos] = lcp_array[left];
    else
    {
        mid = left + right >> 1;
        construct_lcp_lr(pos << 1, left, mid);
        construct_lcp_lr((pos << 1) + 1, mid + 1, right);
        lcp_lr[pos] = min(lcp_lr[pos << 1], lcp_lr[(pos << 1) + 1]);
    }
}

vector<int> SuffixArray::search(const char* pattern) const
{
    int k = 0, pattern_length = strlen(pattern), mid = length >> 1, match_pos, temp;
    vector<int> result;
    while(k < pattern_length && pattern[k] == s[suffix_array[mid] + k])
        k++;
    if(k == pattern_length)
        match_pos = mid;
    else if(pattern[k] < s[suffix_array[mid] + k])
        match_pos = left_search(pattern, pattern_length, 0, mid, k, 2);
    else
        match_pos = right_search(pattern, pattern_length, mid, length - 1, k, 3);
    if(match_pos != -1)
    {
        result.push_back(suffix_array[match_pos]);
        temp = match_pos;
        while(temp < length - 1 && lcp_array[temp] >= pattern_length)
            result.push_back(suffix_array[++temp]);
        while(match_pos >= 1 && lcp_array[match_pos - 1] >= pattern_length)
            result.push_back(suffix_array[--match_pos]);
    }
    return result;
}

int SuffixArray::left_search(const char* pattern, int pattern_length, int left, int right, int k, int lcp_lr_pos) const
{
    int mid = (left + right - 1 >> 1) + 1;
    if(mid == right)
    {
        if(k <= lcp_lr[lcp_lr_pos])
        {
            while(k < pattern_length && pattern[k] == s[suffix_array[left] + k])
                k++;
            if(k == pattern_length)
                return left;
            return -1;
        }
        return -1;
    }
    lcp_lr_pos <<= 1;
    if(k < lcp_lr[lcp_lr_pos + 1])
        return left_search(pattern, pattern_length, left, mid, k, lcp_lr_pos);
    if(k > lcp_lr[lcp_lr_pos + 1])
        return left_search(pattern, pattern_length, mid, right, k, lcp_lr_pos + 1);
    while(k < pattern_length && pattern[k] == s[suffix_array[mid] + k])
        k++;
    if(k == pattern_length)
        return mid;
    if(left == right)
        return -1;
    if(pattern[k] < s[suffix_array[mid] + k])
        return left_search(pattern, pattern_length, left, mid, k, lcp_lr_pos);
    return right_search(pattern, pattern_length, mid, right, k, lcp_lr_pos + 1);
}

int SuffixArray::right_search(const char* pattern, int pattern_length, int left, int right, int k, int lcp_lr_pos) const
{
    int mid = (left + right - 1 >> 1) + 1;
    if(mid == right)
    {
        if(k <= lcp_lr[lcp_lr_pos])
        {
            while(k < pattern_length && pattern[k] == s[suffix_array[right] + k])
                k++;
            if(k == pattern_length)
                return right;
            return -1;
        }
        return -1;
    }
    lcp_lr_pos <<= 1;
    if(k < lcp_lr[lcp_lr_pos])
        return right_search(pattern, pattern_length, mid, right, k, lcp_lr_pos + 1);
    if(k > lcp_lr[lcp_lr_pos])
        return right_search(pattern, pattern_length, left, mid, k, lcp_lr_pos);
    while(k < pattern_length && pattern[k] == s[suffix_array[mid] + k])
        k++;
    if(k == pattern_length)
        return mid;
    if(left == right)
        return -1;
    if(pattern[k] < s[suffix_array[mid] + k])
        return left_search(pattern, pattern_length, left, mid, k, lcp_lr_pos);
    return right_search(pattern, pattern_length, mid, right, k, lcp_lr_pos + 1);
}

int main()
{
    return 0;
}
