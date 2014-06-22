#include "sunsystem.hpp"

using namespace cg;

//Sunsystem::~Sunsystem()
//{
////    foreach (TreeElement, root.childs) {
////        rekDelete(childs);
////    }
//}



//void Sunsystem::addPlanet(Planet *planet)
//{
////    position->append(new Leaf(planet));
//}

//void Sunsystem::addSubSystem()
//{
////    position->append(new Tree());
////    position = tree;
//}



////void Sunsystem::rekDelete(TreeElement partTree)
////{
////    if(partTree);
////    foreach (TreeElement, partTree) {
////        rekDelete(TreeElement partTree);
////    }
////}


void Sunsystem::run()
{
    root->data->resultmatrix = root->data->orbit_rotation * root->data->orbit;
    for(Leaf<Planet> *n : root->childs)
    {
        // orbit rotation*translation
        // run(rtMatrix)
        run(n, root->data->resultmatrix);
    }
    // rotation;
    // save finalMatrix
    root->data->resultmatrix *= root->data->rotation *= root->data->scale;
}

void Sunsystem::run(Leaf<Planet> *currentPlanet, QMatrix4x4 &mat)
{
    currentPlanet->data->resultmatrix = mat * currentPlanet->data->orbit_rotation
            * currentPlanet->data->orbit;
    for(Leaf<Planet> *n : currentPlanet->childs)
    {
        run(n, currentPlanet->data->resultmatrix);
    }
    // rotation;
    // save finalMatrix
    currentPlanet->data->resultmatrix *= currentPlanet->data->rotation * currentPlanet->data->scale;
}
