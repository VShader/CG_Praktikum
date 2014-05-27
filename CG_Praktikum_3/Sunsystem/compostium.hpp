#ifndef COMPOSTIUM_HPP
#define COMPOSTIUM_HPP

#include <vector>
#include <QDebug>

namespace cg {

template<class T>
class TreeElement
{
public:
    virtual ~TreeElement() {}
};


template<class T>
class Leaf : public TreeElement<T>
{
public:
    Leaf(T *data) : data(data) {}
    ~Leaf()
    {
        qDebug() << "Destructor of Leaf";
    }


private:
    T *data;
};



template<class T>
class Tree : public TreeElement<T>
{
public:
    ~Tree()
    {
        qDebug() << "Destructor of Tree";
        for(auto n : childs) {
            delete n;
        }
    }


    TreeElement* append(TreeElement<T> *tree)
    {
        childs.push_back(tree);
    }

private:
    std::vector<TreeElement<T>*> childs;
};
}
#endif  // COMPOSTIUM_HPP
