#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include "types.hpp"

typedef enum QuadFace{
    TOP = 0,
    BOTTOM,
    LEFT,
    RIGHT,
    FRONT,
    BACK,
    NUM_FACES
} QuadFace;

class Quad {
    private:
        Point3d m_v1;
        Point3d m_v2;
        Point3d m_v3;
        Point3d m_v4;
        TextureDesc * m_Texture;

        Point2d * m_texCoords;

    public:
        // Two triangles per quad means six vertices per quad
        static const unsigned NumVertices = 6;
        // Each vertex has three position coordinates and two texture coordinates
        static const unsigned NumComponents = 5;

        Quad();

        Quad(
            Point3d v1,
            Point3d v2,
            Point3d v3,
            Point3d v4,
            TextureDesc * tex,
            QuadFace face
            );

        ~Quad();

        std::vector<GLfloat> getVertices();
};
