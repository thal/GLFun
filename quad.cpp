#include "quad.hpp"

// Default constructor
Quad::Quad()
    :m_v1(), m_v2(), m_v3(), m_v4(), m_Texture()
{}

Quad::Quad(
        Point3d v1,
        Point3d v2,
        Point3d v3,
        Point3d v4,
        TextureDesc * tex,
        QuadFace face
        ):
    m_v1(v1), m_v2(v2), m_v3(v3), m_v4(v4), m_Texture(tex)
{
    // Set texture coordinates based on which face this is
    m_texCoords = new Point2d[4];
    switch(face)
    {
        case TOP:
           m_texCoords[0] = Point2d(tex->objLengthZ, tex->objLengthZ);
           m_texCoords[1] = Point2d(tex->objLengthZ, 0.0);
           m_texCoords[2] = Point2d((tex->objLengthX+tex->objLengthZ), 0.0);
           m_texCoords[3] = Point2d((tex->objLengthX+tex->objLengthZ), tex->objLengthZ);
            break;
        case BOTTOM:
           m_texCoords[0] = Point2d(tex->objLengthZ, tex->imgHeight);
           m_texCoords[1] = Point2d(tex->objLengthZ, (tex->objLengthZ+tex->objLengthY));
           m_texCoords[2] = Point2d((tex->objLengthX+tex->objLengthZ), (tex->objLengthZ+tex->objLengthY));
           m_texCoords[3] = Point2d((tex->objLengthX+tex->objLengthZ), tex->imgHeight );
           break;
        case LEFT:
           m_texCoords[0] = Point2d(0.0, tex->objLengthZ+tex->objLengthY);
           m_texCoords[1] = Point2d(0.0, tex->objLengthZ);
           m_texCoords[2] = Point2d(tex->objLengthZ, tex->objLengthZ);
           m_texCoords[3] = Point2d(tex->objLengthZ, tex->objLengthZ+tex->objLengthY);
           break;
        case RIGHT:
           m_texCoords[0] = Point2d(tex->objLengthZ+tex->objLengthZ, tex->objLengthZ+tex->objLengthY);
           m_texCoords[1] = Point2d(tex->objLengthZ+tex->objLengthZ, tex->objLengthZ);
           m_texCoords[2] = Point2d(2*tex->objLengthZ+tex->objLengthZ, tex->objLengthZ);
           m_texCoords[3] = Point2d(2*tex->objLengthZ+tex->objLengthZ, tex->objLengthZ+tex->objLengthY);
           break;
        case FRONT:
           m_texCoords[0] = Point2d(tex->objLengthZ, tex->objLengthZ+tex->objLengthY);
           m_texCoords[1] = Point2d(tex->objLengthZ, tex->objLengthZ);
           m_texCoords[2] = Point2d(tex->objLengthZ+tex->objLengthX, tex->objLengthZ);
           m_texCoords[3] = Point2d(tex->objLengthZ+tex->objLengthX, tex->objLengthZ+tex->objLengthY);
           break;
        case BACK:
           m_texCoords[0] = Point2d(2*tex->objLengthZ+tex->objLengthX, tex->objLengthZ+tex->objLengthY);
           m_texCoords[1] = Point2d(2*tex->objLengthZ+tex->objLengthX, tex->objLengthZ);
           m_texCoords[2] = Point2d(tex->imgWidth, tex->objLengthZ);
           m_texCoords[3] = Point2d(tex->imgWidth, tex->objLengthZ+tex->objLengthY);
           break;
    }

    // Scale texture coordinates by texture image size
    for(int i = 0; i < 4; i++)
    {
        m_texCoords[i].x /= tex->imgWidth;
        m_texCoords[i].y /= tex->imgHeight;
    }
}

Quad::~Quad()
{
    delete[] m_texCoords;
}

// Return a vector containing a flattened vertex array
std::vector<GLfloat> Quad::getVertices()
{
    unsigned verticesSize = NumComponents * NumVertices;

    GLfloat arr[verticesSize] = {
        m_v1.x, m_v1.y, m_v1.z, m_texCoords[0].x, m_texCoords[0].y,
        m_v2.x, m_v2.y, m_v2.z, m_texCoords[1].x, m_texCoords[1].y,
        m_v3.x, m_v3.y, m_v3.z, m_texCoords[2].x, m_texCoords[2].y,
        m_v1.x, m_v1.y, m_v1.z, m_texCoords[0].x, m_texCoords[0].y,
        m_v3.x, m_v3.y, m_v3.z, m_texCoords[2].x, m_texCoords[2].y,
        m_v4.x, m_v4.y, m_v4.z, m_texCoords[3].x, m_texCoords[3].y
    };
    std::vector<GLfloat> vertices(arr, arr + sizeof(arr)/sizeof(arr[0]));
    return vertices;
}
