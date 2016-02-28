#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL/SOIL.h>

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "box.hpp"
#include "quad.hpp"

#define BUFFER_OFFSET(i) ((void*)(i))

using namespace glm;

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

    // Set up texture
    // Texture resolution: 738px x 100px x 406px
    GLfloat dimX = 7.375;
    GLfloat dimY = 1.0;
    GLfloat dimZ = 4.0625;
    // Size of object in terms of pixels in the texture image
    GLfloat objX = 738.f;
    GLfloat objY = 100.f;
    GLfloat objZ = 406.f;
    // Size of the texture image in pixels
    unsigned texW = 2288;
    unsigned texH = 912;

    TextureDesc texDesc = {texW, texH, objX, objY, objZ};

    GLuint tex;
    // Load image to texture with SOIL
    tex = SOIL_load_OGL_texture (
         "texture/scaled/texture.png",
         SOIL_LOAD_AUTO,
         SOIL_CREATE_NEW_ID
         ,0
        );

    // Set up vertex buffer
    GLuint vBuffer;

    unsigned numVertices = Box::NumVertices * 3;
    unsigned numFloats = numVertices * Quad::NumComponents;
    unsigned verticesPerBox = Box::NumVertices * Quad::NumComponents;

    // Create box objects and buffer vertices
    Box box(Point3d(), dimX, dimY, dimZ, &texDesc);
    std::vector<GLfloat> vertsVect = box.getVertices();

    GLfloat * vertsArr = new GLfloat[numFloats];
    std::copy(vertsVect.begin(), vertsVect.end(), vertsArr);

    Box box2(Point3d(5,5,5), dimX, dimY, dimZ, &texDesc);
    vertsVect = box2.getVertices();
    std::copy(vertsVect.begin(), vertsVect.end(), vertsArr+verticesPerBox);

    Box box3(Point3d(10,10,10), dimX, dimY, dimZ, &texDesc);
    vertsVect = box3.getVertices();
    std::copy(vertsVect.begin(), vertsVect.end(), vertsArr+2*verticesPerBox);

    glGenBuffers(1, &vBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
    glBufferData(GL_ARRAY_BUFFER, numFloats*sizeof(GLfloat), vertsArr, GL_STATIC_DRAW);

    delete[] vertsArr;

    // Set up texture coord vertex attribute
    GLint vTexAttrib = glGetAttribLocation(ProgramObject, "in_tex_coord");
    glEnableVertexAttribArray(vTexAttrib);
    glVertexAttribPointer(vTexAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), BUFFER_OFFSET(3*sizeof(GLfloat)));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set up position coord vertex attribute
    GLint vPosAttrib = glGetAttribLocation(ProgramObject, "vPosition");
    glEnableVertexAttribArray(vPosAttrib);
    glVertexAttribPointer(vPosAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), BUFFER_OFFSET(0));

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
