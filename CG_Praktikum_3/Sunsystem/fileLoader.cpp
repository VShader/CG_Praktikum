#include "fileLoader.hpp"
#include <fstream>
#include <istream>
#include <sstream>
#include <limits>
#include <iostream>
#include "config.hpp"
#ifdef OpenMP
    #include <omp.h>
#endif // OpenMP

using namespace cg;

std::string FileLoader::loadShader(const std::string &path)
{
    std::string result;
    std::string temp;
    std::ifstream ifs(path);
    if(!ifs) return "Error";

    while(std::getline(ifs, temp)) result += "\n" + temp;
    if(ifs.eof()) return result;
}



Mesh FileLoader::loadObj(const std::string &path)
{
    std::ifstream ifs(path);
    std::string in;

    while(ifs >> in)
    {
        if(in == "v")
        {
            readVector(GeometricVertices, ifs);
        } else if(in == "vt")
        {
            readVector(TextureVertices, ifs);
        } else if(in == "vn")
        {
            readVector(VertexNormals, ifs);
        } else if(in == "f")
        {
            readFace(ifs);
        } else
        {
            // if comment
            if(in[0] == '#');
        }
    }
    ifs.close();
    return object;

}



void FileLoader::multi(const std::string &path)
{
//    std::ifstream ifs(path);
//    std::string in;
//    std::vector<std::string> result;

//    // fill string buffer
//    while(getline(ifs, in)) result.push_back(in);

//    // set size of each vector to buffer size;
//    v.resize(result.size());
//    vt.resize(result.size());
//    vn.resize(result.size());

//    std::istringstream ss(result);
//    int threadNumber = 0;
////#pragma omp parallel private(threadNumber)
//    {
////        threadNumber =  omp_get_thread_num();
//    while(ss >> in)
//    {
//        if(in == "v")
//        {
//            readVector(GeometricVertices, ss);
//        } else if(in == "vt")
//        {
//            readVector(TextureVertices, ss);
//        } else if(in == "vn")
//        {
//            readVector(VertexNormals, ss);
//        } else if(in == "f")
//        {
//            readFace(ss);
//        } else
//        {
//            // if comment
//            if(in[0] == '#');
//        }
//    }
//    }
//    ifs.close();

//    // delete unused space
//    v.shrink_to_fit();
//    vt.shrink_to_fit();
    //    vn.shrink_to_fit();
}



Mesh FileLoader::calcNormals()
{
    object.vn.clear();
    object.vn_f.clear();

    std::vector<VecFloat3*> tempNormal;

    if(object.v_f.size() == 0)
    {
        tempNormal.resize(object.v.size()/9, NULL);

        //foreach triangle
#pragma omp parallel for
        for(unsigned int i=0; i<object.v.size()/9; ++i)
        {
            VecFloat3 p1(object.v[i*9+0], object.v[i*9+1], object.v[i*9+2]);
            VecFloat3 p2(object.v[i*9+3], object.v[i+1*9+4], object.v[i+1*9+5]);
            VecFloat3 p3(object.v[i*9+6], object.v[i*9+7], object.v[i*9+8]);

            VecFloat3 u = p2-p1;
            VecFloat3 v = p3-p1;

            tempNormal[i] = new VecFloat3(u.crossProduct(v));
        }
    }
    // if index based vertices
    else
    {
        tempNormal.resize(object.v.size()/3, NULL);
        object.vn_f = object.v_f;
        VecFloat3 tempVec;

        //foreach triangle
        for(unsigned int i=0; i<object.v_f.size()/3; ++i)
        {
            // p.x is v[v_f -1]
            GLuint index1 = object.v_f[3*i+0];
            GLuint index2 = object.v_f[3*i+1];
            GLuint index3 = object.v_f[3*i+2];
            VecFloat3 p1(object.v[index1*3+0], object.v[index1*3+1], object.v[index1*3+2]);
            VecFloat3 p2(object.v[index2*3+0], object.v[index2*3+1], object.v[index2*3+2]);
            VecFloat3 p3(object.v[index3*3+0], object.v[index3*3+1], object.v[index3*3+2]);

            VecFloat3 u = p2-p1;
            VecFloat3 v = p3-p1;
            tempVec = u.crossProduct(v);

            // if normal is in index add new normal to old
            if(tempNormal[index1])  *tempNormal[index1] += tempVec;
            else    tempNormal[index1] = new VecFloat3(tempVec);
            if(tempNormal[index2])  *tempNormal[index2] += tempVec;
            else    tempNormal[index2] = new VecFloat3(tempVec);
            if(tempNormal[index3])  *tempNormal[index3] += tempVec;
            else    tempNormal[index3] = new VecFloat3(tempVec);
        }
   }

    //normalize
    for(VecFloat3 *n : tempNormal)
    {
        n->normalize();
        object.vn.push_back(n->x);
        object.vn.push_back(n->y);
        object.vn.push_back(n->z);
        delete n;
    }
    return object;
}



void FileLoader::readVector(LineType lineType, std::istream &is)
{
    std::vector<GLfloat> *pv;

    switch(lineType)
    {
    case GeometricVertices:
        pv = &object.v;
        break;

    case TextureVertices:
        pv = &object.vt;
        break;

    case VertexNormals:
        pv = &object.vn;
        break;
    }

    GLfloat in;
    while (is >> in) pv->push_back(in);
    is.clear();
}



void FileLoader::readFace(std::istream &is)
{
    GLuint in;

    while(is.good())
    {
        // read index of geometry vertex
        while(is >> in) object.v_f.push_back(in-1);
        if(is.fail())
        {
            is.clear();
            char sign;
            is >> sign;
            if(sign == '/')
            {
                // read index of texture vertex
                if(is >> in) object.vt_f.push_back(in-1);
                if(is.fail())
                {
                    is.clear();
                    char sign;
                    is >> sign;
                    if(sign == '/')
                    {
                        // read index of normal vertex
                        if(is >> in) object.vn_f.push_back(in-1);
                    }
                }
            }
        }
    }
    is.clear();
}
