#include "types.hpp"
#include <vector>

class Box {
    private:
        GLfloat m_lengthX;
        GLfloat m_lengthY;
        GLfloat m_lengthZ;
        std::vector<GLfloat> m_vertices;

    public:
        static const unsigned NumVertices = 36;

        Box(
            Point3d origin,
            GLfloat lengthX,
            GLfloat lengthY,
            GLfloat lengthZ,
            TextureDesc * tex
            );

        ~Box();

        std::vector<GLfloat> getVertices();
};
