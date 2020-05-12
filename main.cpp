#include <iostream>
#include <algorithm>
#include <set>

using namespace std;

template<class C>
class Container {
public:
    // Виртуальные методы, должны быть реализованы вашим контейнером
    virtual void insert(int value) = 0;

    virtual bool exists(int value) = 0;

    virtual void remove(int value) = 0;

    // И этот тоже, хотя к нему потом ещё вернёмся
    virtual void print() = 0;

    //virtual void Display() = 0;

    // Виртуальный деструктор (пока просто поверьте, что он нужен)
    virtual ~Container() {};
};


template<class A>
class Node {
private:
    A data;
    Node<A> *l, *r;
    int h;
public:
    template<class T> friend
    class AVLTree;

    Node() : data(0), l(NULL), r(NULL), h(0) {}

    Node(const A i = 0) : data(i), l(nullptr), r(nullptr), h(0) {}

    Node(const Node &x) : data(x.data), l(x.l), r(x.r), h(x.h) {}

    ~Node() {
        //if (l) delete l;
        //if (r) delete r;
    }
};


template<class T>
class AVLTree : public Container<T> {
public:
    AVLTree() : root(nullptr) {}

    void remove(T value) override {
        Doremi(root, value);
    }

    bool exists(T key) override {
        return _exists(root, key);
    }

    void insert(const T key) override {
        root = _insert(root, key);
    }

    void Display() {
        _display(root);
    }

    void Delete() {
        _delete(root);
    }

    void print() override {
        for (int i = 0; i < _height(root)+1; i++) {
            viewTreeLevel(i, root);
            cout << endl;
        }
    }

    Node<T> *Find(AVLTree t, int key) {

        return _find(t.root, key);
    }

    ~AVLTree()
    {
        delete root;
    }

private:
    Node<T> *root;

    /*void _remove(const Node <T> * pNode, T key){
        Node <T> * tmp = _find(pNode, key);
        if(!tmp -> right && !tmp -> left)
            delete pNode;
        if()
    }*/
    bool _exists(Node<T> *pNode, const T key) {
        return _find(pNode, key) != nullptr;
    }

    int _height(const Node<T> *pNode) {
        //t++;
        return pNode ? pNode->h : 0;
    }

    int _balance(const Node<T> *pNode) {
        return pNode ? (pNode->l ? pNode->l->h : 0) - (pNode->r ? pNode->r->h : 0) : 0;
    }

    Node<T> *_l_rotation(Node<T> *pNode) {
        Node<T> *tmpNode = pNode->r;
        pNode->r = tmpNode->l;
        tmpNode->l = pNode;
        pNode->h = max(_height(pNode->l), _height(pNode->r)) + 1;
        tmpNode->h = max(_height(tmpNode->l), _height(tmpNode->r)) + 1;
        return tmpNode;
    }

    Node<T> *_r_rotation(Node<T> *pNode) {
        Node<T> *tmpNode = pNode->l;
        pNode->l = tmpNode->r;
        tmpNode->r = pNode;
        pNode->h = max(_height(pNode->l), _height(pNode->r)) + 1;
        tmpNode->h = max(_height(tmpNode->l), _height(tmpNode->r)) + 1;
        return tmpNode;
    }

    Node<T> *_R_rotation(Node<T> *pNode) {
        Node<T> *tmpNode = pNode->l;
        pNode->l = _l_rotation(tmpNode);
        return _r_rotation(pNode);
    }

    Node<T> *_L_rotation(Node<T> *pNode) {
        Node<T> *tmpNode = pNode->r;
        pNode->r = _r_rotation(tmpNode);
        return _l_rotation(pNode);
    }

    Node<T> *_balancing(Node<T> *pNode) {
        int b_factor = _balance(pNode);
        if (b_factor > 1) {
            if (_balance(pNode->l) >= 0) pNode = _r_rotation(pNode);
            else pNode = _R_rotation(pNode);
        }
        else {
            if (b_factor < -1) {
                if (_balance(pNode->r) <= 0) pNode = _l_rotation(pNode);
                else pNode = _L_rotation(pNode);
            }
        }
        return pNode;
    }

    Node<T> *_insert(Node<T> *pNode, const T key) {
        if (!pNode) return new Node<T>(key);
        else if (key < pNode->data) {
            pNode->l = _insert(pNode->l, key);
            pNode = _balancing(pNode);
        }
        else if (key > pNode->data) {
            pNode->r = _insert(pNode->r, key);
            pNode = _balancing(pNode);
        }
        else {
            //Already existed
            return nullptr;
        }
        pNode->h = max(_height(pNode->l), _height(pNode->r)) + 1;
        return pNode;
    }

    void _display(Node<T> *pNode) {
        if (!pNode) return;
        _display(pNode->l);
        cout << pNode->data << ' ';
        _display(pNode->r);
    }

    /*	void _delete(Node <T> * pNode)
    {
        if (!pNode) return;
        if(pNode -> l != NULL)
        {
            _delete(pNode -> l);
        }
        if(pNode -> r != NULL)
        {
            _delete(pNode -> r);
        }
        delete pNode;
        //_remove(pNode, pNode -> data);
    }*/

    void _delete(Node<T> *pNode) {
        if (pNode != NULL) {
            if (pNode->l != NULL) {
                _delete(pNode->l);
            }
            if (pNode->r != NULL) {
                _delete(pNode->r);
            }
            delete pNode;
        }
    }

    Node<T> *_find_min(Node<T> *pNode) {
        return pNode->l ? _find_min(pNode->l) : pNode;
    }

    Node<T> *_find_max(const Node<T> *pNode) {

        return pNode->r ? _find_max(pNode->r) : pNode;
    }

    Node<T> *_find(Node<T> *pNode, const T key) {
        if (pNode == nullptr) return nullptr;
        else if (pNode->data == key) return pNode;
        else return pNode->data > key ? _find(pNode->l, key) : _find(pNode->r, key);
    }
    Node<T>* _removemin(Node<T>* p) // удаление узла с минимальным ключом из дерева p
    {
        if (p->l == nullptr)
            return p->r;
        p->l = _removemin(p->l);
        return _balancing(p);
    }
    void _remove(Node<T> *pNode, const T key) {
        Node<T> *tmpNode = _find(pNode, key);
        if (tmpNode == nullptr) {
            delete tmpNode;
            return;
        }
        //Left and right subtree
        if (tmpNode->l && tmpNode->r) {
            Node<T>* minTmpNode = _find_min(tmpNode->r);
            minTmpNode->r = _removemin(tmpNode->r);
            minTmpNode->l = tmpNode->l;
            /*minTmpNode->l = pNode->l;
            minTmpNode->r = pNode->r;
            pNode = minTmpNode;*/
            //tmpNode->data = minTmpNode->data;
            pNode = minTmpNode;
            delete tmpNode, minTmpNode;

            return;
        }

        //Only left subtree
        if (tmpNode->l && !(tmpNode->r)) {
            //Node* leftTmpNode = pNode->l;
            pNode = pNode->l;
            delete tmpNode;
            return;
        }
        //Only right subtree
        if (tmpNode->r && !(tmpNode->l)) {
            pNode = pNode->r;
            delete tmpNode;
            return;
        }
        //No subtrees
        if (!(tmpNode->l && tmpNode->r)) {
            pNode = nullptr;
            delete tmpNode;
            return;
        }
        /*if (tmpNode->l && tmpNode->r) {
            //Left and right subtree
            Node<T> *maxTmpNode = _find_min(tmpNode);
            tmpNode->data = maxTmpNode->data;

            delete maxTmpNode;
        }
        else {
            if (tmpNode->l) {
                //only left subtree
                Node<T> *leftTmpNode = tmpNode->l;
                tmpNode->data = leftTmpNode->data;
                tmpNode->l = leftTmpNode->l;
                tmpNode->r = leftTmpNode->r;
                delete leftTmpNode;

            }
            else {
                if (tmpNode->r) {
                    //only right subtree
                    Node<T> *rightTmpNode = tmpNode->r;
                    tmpNode->data = rightTmpNode->data;
                    tmpNode->l = rightTmpNode->l;
                    tmpNode->r = rightTmpNode->r;
                    delete rightTmpNode;
                }
                else {
                    tmpNode->l = nullptr;
                    tmpNode->r = nullptr;
                    //no subtree
                    delete tmpNode;
                }


            }
        }*/
    }
    Node<T> *Doremi(Node<T> *x, int k) //~_remove
    {
        if (!x) return nullptr;
        if (k < x->data) x->l = Doremi(x->l, k);
        else if (k > x->data) x->r = Doremi(x->r, k);
        else
        {
            Node<T> *y = x->l;
            Node<T> *z = x->r;
            delete x;
            if (!z) return y;
            Node<T>* min = SearchMin(z);
            min->r = DeleteMin(z);
            min->l = y;
            return _balancing(min);
        }
        return _balancing(x);
    }
    Node<T> *SearchMin(Node<T> *x)
    {
        if (x->l) return SearchMin(x->l);
        else return x;
    }
    Node<T> *DeleteMin(Node<T> *x)
    {
        if (x->l == nullptr) return x->r;
        x->l = DeleteMin(x->l);
        return _balancing(x);
    }
    void viewTreeLevel(int i, const Node<T> *pNode) {
        if (i == 0) cout << pNode->data << ' ';
        else {
            if (pNode->l) viewTreeLevel(i - 1, pNode->l);
            if (pNode->r) viewTreeLevel(i - 1, pNode->r);
        }
    }
};


/*
int main()
{
	AVLTree<int> t;
	t.Insert(42);
	for(int i = 0; i < 10; ++i)
		t.Insert(i);
	t.ViewLevels();
	cout<<t.Find(t, 42)<<endl;

	//t.ViewLevels();
	t.Delete();
	//t.ViewLevels();
}
 */
int main() {
    //Container<int> *c = reinterpret_cast<Container<int> *>(new AVLTree<int>());
    Container<int>* c = new AVLTree<int>();
    for (int i = 1; i < 10; i++) {
        c->insert(i * i);
    }


    cout << "Container after creation:" << endl;
    c->print();

    if (c->exists(25))
        cout << "Search for value 25: found" << endl;
    c->print();
    if (!c->exists(111))
        cout << "Search for value 111: not found" << endl;
    c->print();
    //c->Display();

    c->remove(36);
    cout << "Container after deletion of the element:" << endl;
    c->print();

    delete c;
    return 0;
}