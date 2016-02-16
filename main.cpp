#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL/SOIL.h>

#include <iostream>
#include <fstream>
#include <stdlib.h>

#define BUFFER_OFFSET(i) ((void*)(i))

using namespace glm;

// 6 faces * 2 triangles per face * 3 vertices per triangle
const unsigned numVertices = 36;
// x, y, z coordinates, s, t texture coordinates

const unsigned numVertexComponents = 5;
// Texture resolution: 738px x 100px x 406px
const GLfloat dimX = 7.375;
const GLfloat dimY = 1.0;
const GLfloat dimZ = 4.0625;

GLfloat objX = 738.f;
GLfloat objY = 100.f;
GLfloat objZ = 406.f;

unsigned texW = 2288;
unsigned texH = 912;

const GLfloat verts[numVertices][numVertexComponents] = {
    // Bottom
    { 0.0, 0.0, 0.0, objZ/texW, (objZ+objY)/texH },   // 0
    { 0.0, 0.0, dimZ,  objZ/texW, (2*objZ + objY)/texH },  // 1
    { dimX, 0.0, 0.0, (objZ+objX)/texW, (objZ+objY)/texH },   // 3
    { 0.0, 0.0, dimZ,  objZ/texW, (2*objZ + objY)/texH },  // 1
    { dimX, 0.0, dimZ, (objZ+objX)/texW, (2*objZ + objY)/texH },  // 2
    { dimX, 0.0, 0.0, (objZ+objX)/texW, (objZ+objY)/texH },   // 3
    // Front
    { 0.0, 0.0, 0.0,  objZ/texW, objZ/texH },   // 0
    { 0.0, dimY, 0.0,  objZ/texW, (objZ+objY)/texH },  // 4
    { dimX, 0.0, 0.0, (objZ+objX)/texW, objZ/texH },   // 3
    { 0.0, dimY, 0.0,  objZ/texW, (objZ+objY)/texH },  // 4
    { dimX, dimY, 0.0, (objZ+objX)/texW, (objZ+objY)/texH },  // 7
    { dimX, 0.0, 0.0, (objZ+objX)/texW, objZ/texH },   // 3
    // Left
    { 0.0, 0.0, dimZ,  0.0, (objZ+objY)/texH },  // 1
    { 0.0, dimY, dimZ,  objZ/texW, (objZ+objY)/texH }, // 5
    { 0.0, 0.0, 0.0,  0.0, objZ/texH },   // 0
    { 0.0, dimY, dimZ,  objZ/texW, (objZ+objY)/texH }, // 5
    { 0.0, dimY, 0.0,  objZ/texW, objZ/texH },  // 4
    { 0.0, 0.0, 0.0,  0.0, objZ/texH },   // 0
    // Back
    { dimX, dimY, dimZ, 1.0, (objZ+objY)/texH }, // 6
    { 0.0, 0.0, dimZ,  (2*objZ + objX)/texW, objZ/texH },  // 1
    { dimX, 0.0, dimZ, 1.0, objZ/texH },  // 2
    { 0.0, dimY, dimZ,  (2*objZ + objX)/texW, (objZ+objY)/texH }, // 5
    { 0.0, 0.0, dimZ,  (2*objZ + objX)/texW, objZ/texH },  // 1
    { dimX, dimY, dimZ, 1.0, (objZ+objY)/texH }, // 6
    // Right
    { dimX, 0.0, 0.0, (objZ+objX)/texW, objZ/texH },   // 3
    { dimX, dimY, 0.0, (2*objZ + objX)/texW, objZ/texH },  // 7
    { dimX, 0.0, dimZ, (objZ+objX)/texW, (objZ+objY)/texH },  // 2
    { dimX, dimY, dimZ, (2*objZ + objX)/texW, (objZ+objY)/texH }, // 6
    { dimX, 0.0, dimZ, (objZ+objX)/texW, (objZ+objY)/texH },  // 2
    { dimX, dimY, 0.0, (2*objZ + objX)/texW, objZ/texH },  // 7
    // Top
    { 0.0, dimY, dimZ,  objZ/texW, objZ/texH }, // 5
    { dimX, dimY, dimZ, (objZ+objX)/texW, objZ/texH }, // 6
    { dimX, dimY, 0.0, (objZ+objX)/texW, 0.0 },  // 7
    { 0.0, dimY, 0.0,  objZ/texW, 0.0 },  // 4
    { 0.0, dimY, dimZ,  objZ/texW, objZ/texH }, // 5
    { dimX, dimY, 0.0, (objZ+objX)/texW, 0.0 }   // 7
};

static void error_callback(int error, const char* description)
{
    std::cerr<<description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void loadShader(GLuint &shaderObject, GLenum type, const char *filename)
{
    shaderObject = glCreateShader(type);

    // Load shader source from file
    std::ifstream file(filename);
    std::string str = "";
    char c;
    while(file.get(c))
    {
        str+=c;
    }
    file.close();

    // Compile shader
    const char * shaderSrc = str.c_str();
    glShaderSourceARB(shaderObject, 1, &shaderSrc, NULL);
    glCompileShader(shaderObject);

    GLint blen = 0;
    GLsizei slen = 0;

    glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH , &blen);
    if (blen > 1)
    {
        GLchar* compiler_log = new GLchar[blen];
        glGetInfoLogARB(shaderObject, blen, &slen, compiler_log);
        std::cerr<<"compiler_log for "<<filename<<": "<<compiler_log;
        delete[] compiler_log;
    }
}

int main(void)
{
    if(!glfwInit())
        exit(EXIT_FAILURE);


    GLFWwindow * window;
    glfwSetErrorCallback(error_callback);

    window = glfwCreateWindow(640, 480, "A Box", NULL, NULL);
    if(!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if(err)
    {
        std::cerr<<glewGetErrorString(err);
        exit(EXIT_FAILURE);
    }

    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);

    // Set up shaders
    GLuint vertexShader, fragmentShader;
    loadShader(vertexShader, GL_VERTEX_SHADER, "vertex.vert");
    loadShader(fragmentShader, GL_FRAGMENT_SHADER, "fragment.frag");

    GLuint ProgramObject = glCreateProgram();
    glAttachShader(ProgramObject, vertexShader);
    glAttachShader(ProgramObject, fragmentShader);
    glLinkProgram(ProgramObject);

    GLint linked;
    glGetProgramiv(ProgramObject, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        std::cerr<<"SHADERS NOT LINKED >:(";
        exit(EXIT_FAILURE);
    }

    glUseProgram(ProgramObject);

    // Set up vertex buffer

    GLuint vBuffer;
    GLuint elementBuffer;

    glGenBuffers(1, &vBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    GLint vPosition = glGetAttribLocation(ProgramObject, "vPosition");

    // Set up texture
    GLuint tex;
    // Load image to texture with SOIL
    tex = SOIL_load_OGL_texture
        (
         "texture/scaled/texture.png",
         SOIL_LOAD_AUTO,
         SOIL_CREATE_NEW_ID
         ,0
        );

    GLint texAttrib = glGetAttribLocation(ProgramObject, "in_tex_coord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), BUFFER_OFFSET(3*sizeof(GLfloat)));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);

    // Projection matrix
    mat4 proj = perspective(radians(45.0f), 640.f/480.f, 0.1f, 100.f);
    GLint projMatLoc = glGetUniformLocation(ProgramObject, "projectionMatrix");
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(proj));

    GLint mvMatLoc = glGetUniformLocation(ProgramObject, "modelViewMatrix");

    // Enable blending
    glClearColor(221.f/255.f, 54.f/255.f, 255.f/255.f, 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window))
    {
        glViewport(0, 0, 640, 480);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update ModelView matrix with rotation
        float rot = glfwGetTime() * 50.f;
        mat4 tran = translate(mat4(1.0f), vec3(0.0f, 0.0f, -10.0f));
        mat4 rotx = rotate(tran, radians(rot)*1.1f, vec3(-1.0f, 0.0f, 0.0f));
        mat4 view = rotate(rotx, radians(rot), vec3(0.0f, 1.0f, 0.0f));
        mat4 model = scale(mat4(1.0f), vec3(0.5f));
        mat4 modelView = view * model;
        glUniformMatrix4fv(mvMatLoc, 1, GL_FALSE, value_ptr(modelView));

        glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
        glDrawArrays(GL_TRIANGLES, 0, numVertices);

        glFlush();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
