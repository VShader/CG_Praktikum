#ifndef FILELOADER_HPP
#define FILELOADER_HPP

#include <string>
#include <vector>
#include <queue>
#include <GL/gl.h>
#include <cmath>
#include <QThread>
#include <QMutex>
#include <istream>


namespace cg    {

class VecFloat3
{
public:
    VecFloat3() {}
    VecFloat3(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}
    VecFloat3(const VecFloat3& ref) = default;
    VecFloat3(VecFloat3&& ref) = default;

    bool operator==(const VecFloat3& ref) const {return x==ref.x && y==ref.y && z==ref.z;}

    VecFloat3& operator=(VecFloat3&& ref) = default;

    GLfloat operator* (const VecFloat3& operand)
    {
        return x*operand.x + y*operand.y + z*operand.z;
    }

    VecFloat3 operator* (const GLfloat operand)
    {
        return std::move(VecFloat3(x*operand, y*operand, z*operand));
    }

    VecFloat3 operator/ (const GLfloat operand)
    {
        return std::move(VecFloat3(x/operand, y/operand, z/operand));
    }

    VecFloat3 crossProduct(const VecFloat3& operand)
    {
        return std::move(VecFloat3(y*operand.z - z*operand.y,
                         z*operand.x - x*operand.z,
                         x*operand.y - y*operand.x));
    }

    VecFloat3 operator-(const VecFloat3& operand)
    {
        return VecFloat3(x-operand.x, y-operand.y, z-operand.z);
    }

    VecFloat3 operator+(const VecFloat3& operand)
    {
        return VecFloat3(x+operand.x, y+operand.y, z+operand.z);
    }

    void operator+=(const VecFloat3& operand)
    {
        x = x+operand.x;
        y = y+operand.y;
        z = z+operand.z;
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
    std::string loadShader(const std::string& path);
    Mesh loadObj(const std::string& path);
    void calcNormals(Mesh& object);

    enum LineType   {
        GeometricVertices, TextureVertices, VertexNormals, ParameterSpaceVertices,
        Face
    };


private:
    void read(LineType lineType, std::istream& is, Mesh& object);
    void readVector(std::istream& is, std::vector<GLfloat>& vec);
    void readFace(std::istream& is, Mesh& object);


    std::string name;
    std::queue<std::string> geometricVericesQueue;
    std::queue<std::string> textureVericesQueue;
    std::queue<std::string> vertexNormalsQueue;
    std::queue<std::string> faceQueue;
    QMutex mutex;


    class NormalContainer
    {
    public:
        void push_back(const VecFloat3 &normal);
        VecFloat3 getNormal();


    private:
        std::vector<VecFloat3> vec;
    };


    class ReadThread : public QThread
    {
    public:
        ReadThread(FileLoader& parent, LineType lineType, Mesh& mesh, std::queue<std::string>& queue)
            : queue(queue)
        {
            switch(lineType)
            {
            case GeometricVertices:
                doOp = [&](std::istream& is) {parent.readVector(is, mesh.v);};
                break;

            case TextureVertices:
                doOp = [&](std::istream& is) {parent.readVector(is, mesh.vt);};
                break;

            case VertexNormals:
                doOp = [&](std::istream& is) {parent.readVector(is, mesh.vn);};
                break;

            case Face:
                doOp = [&](std::istream& is) {parent.readFace(is, mesh);};
            }
        }


        void run();

    private:
        std::queue<std::string>& queue;
        std::function<void(std::istream& is)> doOp;
    };

};
// noch zu implementieren o, s, usemtl, mtllib




}

#endif // FILELOADER_HPP
