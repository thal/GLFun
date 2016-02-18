# OpenGL Fun

A little visual demo that I am creating with the aim of learning OpenGL
techniques

## Installation

### Linux
Depends on the following libraries: GLFW, GLEW, glm, and SOIL. Obtain them through your
package manager, or install them yourself.

Compile with `g++ main.cpp -o main -lglfw -lGLEW -lGL -lSOIL`

### Windows (VC++)
The VC++ project is configured to statically link the required libraries. For this to work, the library files must be downloaded and placed in the appropriate locations.
- glfw3.h, glew.h, SOIL.h, and the glm headers must be added to the include path.
- glfw3.lib, glew32.lib, and SOIL.lib must be added to the library path
