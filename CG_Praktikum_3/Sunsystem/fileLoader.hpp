#ifndef FILELOADER_HPP
#define FILELOADER_HPP

#include <string>
#include <vector>
#include <GL/gl.h>
#include <cmath>


namespace cg    {

class Mesh
{
public:

    std::vector<GLfloat> v;
    std::vector<GLfloat> vt;
    std::vector<GLfloat> vn;
    std::vector<GLuint> v_f;
    std::vector<GLuint> vt_f;
    std::vector<GLuint> vn_f;
};



class FileLoader
{
public:
    std::string loadShader(const std::string &path);
    Mesh loadObj(const std::string &path);
    void multi(const std::string &path);
    Mesh calcNormals();

    enum LineType   {
        GeometricVertices, TextureVertices, VertexNormals, ParameterSpaceVertices
    };



private:
    void readVector(LineType lineType, std::istream &is);
    void readFace(std::istream &is);


    std::string name;

    Mesh object;


};
// noch zu implementieren o, s, usemtl, mtllib


}

class VecFloat3
{
public:
    VecFloat3(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}


    const GLfloat operator* (const VecFloat3 &operand)
    {
        return x*operand.x + y*operand.y + z*operand.z;
    }

    const VecFloat3 operator* (const GLfloat operand)
    {
        return VecFloat3(x*operand, y*operand, z*operand);
    }

    const VecFloat3 crossProduct(const VecFloat3 &operand)
    {
        return VecFloat3(y*operand.z - z*operand.y,
                         z*operand.x - x*operand.z,
                         x*operand.y - y*operand.x);
    }

    const VecFloat3 operator-(const VecFloat3 &operand)
    {
        return VecFloat3(x-operand.x, y-operand.y, z-operand.z);
    }

    const VecFloat3 operator+(const VecFloat3 &operand)
    {
        return VecFloat3(x+operand.x, y+operand.y, z+operand.z);
    }

    const GLfloat length()
    {
        return sqrt(x*x + y*y + z*z);
    }

    void normalize()
    {
        GLfloat length = this->length();
        x = x/length;
        y = y/length;
        z = z/length;
    }


    GLfloat x;
    GLfloat y;
    GLfloat z;
};

#endif // FILELOADER_HPP
