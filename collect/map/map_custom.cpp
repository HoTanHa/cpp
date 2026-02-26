#include <bits/stdc++.h>
using namespace std;
#ifndef __mymap__cpp__
#define __mymap__cpp__
template <class A, class B>
struct node
{
    pair<A, B> elem;
    node *L, *R;
    node(pair<A, B> x, node *t = 0, node *p = 0)
    {
        elem = x;
        L = t;
        R = p;
    }
};
template <class A, class B>
struct slist
{
    node<A, B> *data;
    slist *next;
    slist(node<A, B> *d)
    {
        data = d;
        next = 0;
    }
};
template <class A, class B>
class ite
{
private:
    slist<A, B> *c = 0, *F = 0, *L = 0;
    void inorder(node<A, B> *H)
    {
        if (H)
        {
            inorder(H->L);
            L = (!F ? F : L->next) = new slist<A, B>(H);
            inorder(H->R);
        }
    }

public:
    ite() { c = F = L = 0; }
    ite<A, B>(node<A, B> *H)
    {
        inorder(H);
        c = F;
    }
    void operator++() { c = c->next; }
    pair<A, B> operator*() { return c->data->elem; }
    bool operator!=(ite<A, B> IT) { return c != IT.c; }
};

template <class A, class B>
struct Map
{
private:
    node<A, B> *root;
    int n;
    void add(node<A, B> *&H, pair<A, B> x)
    {
        if (H == 0)
            H = new node<A, B>(x);
        else
            add(H->elem > x ? H->L : H->R, x);
    }
    int Max(node<A, B> *H) { return !H->R ? H->elem : Max(H->R); }
    int Min(node<A, B> *H) { return !H->L ? H->elem : Min(H->L); }
    void remove(node<A, B> *&H, pair<A, B> x)
    {
        if (!H)
            return;
        if (x == H->elem)
        {
            if (!H->L)
                H = H->R;
            else
            {
                H->elem = Max(H->L);
                remove(H->L, H->elem);
            }
        }
        else
            remove(x < H->elem ? H->L : H->R, x);
    }
    node<A, B> *find(node<A, B> *H, A a)
    {
        if (!H)
            return 0;
        if (H->elem.first == a)
            return H;
        return find(H->elem.first < a ? H->L : H->R, a);
    }
    B &found(node<A, B> *H, A a)
    {
        while (H->elem.first != a)
        {
            if (H->elem.first < a)
                H = H->R;
            else
                H = H->L;
        }
        return H->elem.second;
    }

public:
    typedef ite<A, B> iterator;
    iterator begin()
    {
        return iterator(root);
    }
    iterator end() { return 0; }
    Map()
    {
        root = 0;
        n = 0;
    }
    int size() { return n; }
    bool empty() { return n == 0; }
    B &operator[](A a)
    {
        node<A, B> *p = find(root, a);
        if (!p)
        {
            n++;
            B z;
            add(root, make_pair(a, z));
        }
        return found(root, a);
    }
};
#endif
int main()
{
    Map<string, int> M;
    M["tom"] = 7;
    M["ca"] = 9;
    M["thit"] = 5;
    for (Map<string, int>::iterator it = M.begin(); it != M.end(); ++it)
        cout << (*it).first << " " << (*it).second << "\n";
}