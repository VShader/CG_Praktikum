#ifndef SUNSYSTEM_HPP
#define SUNSYSTEM_HPP

#include <string>
#include <GL/gl.h>
#include "tree.hpp"
#include <QMatrix4x4>

namespace cg {


class Planet
{
public:
    Planet(std::string name) : name(name) {}
    Planet(std::string name, GLuint vao, const QMatrix4x4 &scale, const QMatrix4x4 &rotation,
           const QMatrix4x4 &orbit, const QMatrix4x4 &orbit_rotation)
        : name(name), vao(vao), scale(scale), rotation(rotation),
          orbit(orbit), orbit_rotation(orbit_rotation) {}


    std::string name;
    GLuint vao;
    QMatrix4x4 scale;
    QMatrix4x4 rotation;
    QMatrix4x4 orbit;
    QMatrix4x4 orbit_rotation;
    QMatrix4x4 resultmatrix;
};




class Sunsystem : public Tree<Planet>
{
public:
    void run();

//    void addPlanet(Planet *planet);
//    void addSubSystem();
//    int getSubSystems();



private:
    Tree<Planet> *position;

    void run(Leaf<Planet> *currentPlanet, QMatrix4x4 &mat);
//    void rekDelete(TreeElement partTree);
};
}

#endif  // SUNSYSTEM_HPP
