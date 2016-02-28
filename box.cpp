#include "box.hpp"
#include "quad.hpp"

Box::Box(
        Point3d origin,
        GLfloat lengthX,
        GLfloat lengthY,
        GLfloat lengthZ,
        TextureDesc * tex
        ):
    m_lengthX(lengthX),
    m_lengthY(lengthY),
    m_lengthZ(lengthZ),
    m_vertices()
{
    // Create six quads, one for each face of the box
    Quad ** quads = new Quad*[NUM_FACES];

    quads[BOTTOM] = new Quad(
            origin,
            origin+Point3d(0,0,lengthZ),
            origin+Point3d(lengthX,0,lengthZ),
            origin+Point3d(lengthX,0,0),
            tex, BOTTOM
            );

    quads[FRONT] = new Quad(
            origin,
            origin+Point3d(0,lengthY,0),
            origin+Point3d(lengthX,lengthY,0),
            origin+Point3d(lengthX,0,0),
            tex, FRONT
            );

    quads[LEFT] = new Quad(
            origin+Point3d(0,0,lengthZ),
            origin+Point3d(0,lengthY,lengthZ),
            origin+Point3d(0,lengthY,0),
            origin,
            tex, LEFT
            );

    quads[TOP] = new Quad(
            origin+Point3d(0,lengthY,0),
            origin+Point3d(0,lengthY,lengthZ),
            origin+Point3d(lengthX,lengthY,lengthZ),
            origin+Point3d(lengthX,lengthY,0),
            tex, TOP
            );

    quads[BACK] = new Quad(
            origin+Point3d(0,0,lengthZ),
            origin+Point3d(0,lengthY,lengthZ),
            origin+Point3d(lengthX,lengthY,lengthZ),
            origin+Point3d(lengthX,0,lengthZ),
            tex, BACK
            );

    quads[RIGHT] = new Quad(
            origin+Point3d(lengthX,0,0),
            origin+Point3d(lengthX,lengthY,0),
            origin+Point3d(lengthX,lengthY,lengthZ),
            origin+Point3d(lengthX,0,lengthZ),
            tex, RIGHT
            );

    // Add each face's vertices to the box's vertex array
    for(int face = 0; face < NUM_FACES; face++)
    {
        Quad * q = quads[face];
        std::vector<GLfloat> currentVect = q->getVertices();
        m_vertices.insert(
            m_vertices.end(),
            currentVect.begin(),
            currentVect.end()
            );
        delete q;
    }
    delete[] quads;
}

Box::~Box()
{}

// Return a vector containing a flattened vertex array of all vertices for all
// faces of the box
std::vector<GLfloat> Box::getVertices()
{
    return m_vertices;
}
