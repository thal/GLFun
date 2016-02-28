#ifndef _TYPES_H_
#define _TYPES_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct Point3d{
    GLfloat x;
    GLfloat y;
    GLfloat z;

    Point3d(GLfloat x = 0.0, GLfloat y = 0.0, GLfloat z = 0.0)
        :x(x), y(y), z(z)
    {}

    Point3d operator+(const Point3d &rhs)
    {
        Point3d p(x+rhs.x, y+rhs.y, z+rhs.z);
        return p;
    }
} Point3d;

typedef struct Point2d{
    GLfloat x;
    GLfloat y;

    Point2d(GLfloat x = 0.0, GLfloat y = 0.0)
        :x(x), y(y)
    {}

    Point2d operator+(const Point2d &rhs)
    {
        Point2d p(x+rhs.x, y+rhs.y);
        return p;
    }
} Point2d;

typedef struct TextureDesc{
    unsigned imgWidth;
    unsigned imgHeight;
    GLfloat objLengthX;
    GLfloat objLengthY;
    GLfloat objLengthZ;
} TextureDesc;

#endif //_TYPES_H_
