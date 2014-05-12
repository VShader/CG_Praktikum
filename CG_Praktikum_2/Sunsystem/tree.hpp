#ifndef TREE_HPP
#define TREE_HPP

#include <string>
#include <vector>
#include <QDebug>

namespace cg
{

template<class T>
class Leaf
{
public:
    Leaf(T *data) : data(data) {}
    ~Leaf()
    {
        qDebug() << "Delete Leaf" << childs.size();
        for(auto n : childs)
        {
            delete n;
        }
    }


    Leaf* addChild(T *data)
    {
        Leaf *tmp = new Leaf<T>(data);
        childs.push_back(tmp);
        return tmp;
    }

    Leaf<T>* getChild(unsigned int index)
    {
        return childs[index];
    }


    T *data;
    std::vector<Leaf<T>*> childs;
};




template<class T>
class Tree
{
public:
    Tree() : root(NULL), position(NULL) {}
    Tree(T *data) : root(new Leaf<T*>(data), position(root)) {}

    ~Tree()
    {
        delete root;
    }



    Leaf<T>* addChild(T *data)
    {
        Leaf<T> *tmp = new Leaf<T>(data);
        if(!root)
        {
            root = tmp;
            position = root;
            return tmp;
        }
        else
        {
            position->childs.push_back(tmp);
            return tmp;
        }
    }

    Leaf<T>* getChild(unsigned int index)
    {
        if(!root) return NULL;
        return root->childs[index];
    }

protected:
    Leaf<T> *root;
    Leaf<T> *position;
};

}

#endif // TREE_HPP
