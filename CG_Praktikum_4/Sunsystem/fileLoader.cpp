#include "fileLoader.hpp"
#include <fstream>
#include <istream>
#include <sstream>
#include <limits>
#include <algorithm>
#include "config.hpp"
#ifdef OpenMP
    #include <omp.h>
#endif // OpenMP

using namespace cg;

std::string FileLoader::loadShader(const std::string& path)
{
    std::string result;
    std::string temp;
    std::ifstream ifs(path);
    if(!ifs) return "Error";

    while(std::getline(ifs, temp)) result += "\n" + temp;
    if(ifs.eof()) return result;
}



Mesh FileLoader::loadObj(const std::string& path)
{
    Mesh object;
    std::ifstream ifs(path);
    std::string type;
    std::string line;

//    while(getline(ifs, in)) result.push_back(in);

    while(ifs >> type)
    {
        getline(ifs, line);
        if(type == "v")
        {
            geometricVericesQueue.push(std::move(line));
        } else if(type == "vt")
        {
            textureVericesQueue.push(std::move(line));
        } else if(type == "vn")
        {
            vertexNormalsQueue.push(std::move(line));
        } else if(type == "f")
        {
            faceQueue.push(std::move(line));
        } else
        {
            // if comment
            if(type[0] == '#');
        }
    }
    ifs.close();

    ReadThread gv(*this, GeometricVertices, object, geometricVericesQueue);
    ReadThread tv(*this, TextureVertices, object, textureVericesQueue);
    ReadThread n(*this, VertexNormals, object, vertexNormalsQueue);
    ReadThread f(*this, Face, object, faceQueue);

    gv.start();
    tv.start();
    n.start();
    f.start();
    gv.wait();
    tv.wait();
    n.wait();
    f.wait();

    return object;
}



void FileLoader::calcNormals(Mesh& object)
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
        std::vector<NormalContainer> normal(object.v.size()/3);

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
//            tempVec.normalize();

            // if normal is in index add new normal to old
            normal[index1].push_back(tempVec);
            normal[index2].push_back(tempVec);
            normal[index3].push_back(tempVec);

        }
        for(size_t i=0; i<normal.size(); ++i)
        {
            tempNormal[i] = new VecFloat3(normal[i].getNormal());
        }
   }

    //normalize
    object.vn.resize(tempNormal.size()*3);
#pragma omp parallel for
    for(size_t i=0; i<tempNormal.size(); ++i)
    {
        VecFloat3* n = tempNormal[i];
        n->normalize();
        object.vn[i*3+0] = n->x;
        object.vn[i*3+1] = n->y;
        object.vn[i*3+2] = n->z;
        delete n;
    }
}



void FileLoader::readVector(std::istream& is, std::vector<GLfloat>& vec)
{
    GLfloat in;
    while (is >> in) vec.push_back(in);
    is.clear();
}



void FileLoader::readFace(std::istream& is, Mesh& object)
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



void FileLoader::NormalContainer::push_back(VecFloat3 normal)
{
    bool includes = false;
    for(VecFloat3& x : vec) includes = x==normal;
    if(!includes)   vec.push_back(std::move(normal));
}



VecFloat3 FileLoader::NormalContainer::getNormal()
{
    VecFloat3 result;
    for(VecFloat3& x : vec) result += x;
    return std::move(result);
}



void FileLoader::ReadThread::run()
{
    while(!queue.empty())
    {
        std::stringstream ss(std::move(queue.front()));
        doOp(ss);
        queue.pop();
    }
}
