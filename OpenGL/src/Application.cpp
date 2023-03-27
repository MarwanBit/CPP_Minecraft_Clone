#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*Standard Libraries*/
#include <iostream>
#include <fstream>
#include <string>
#include<sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /*GLFW Version Control/ Compatibility, switch between CORE AND COMPAT*/
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    /*Test to make sure GLFW is working correctly*/
    if (glewInit() != GLEW_OK) {
        std::cout << "Error" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, //0
        0.5f,   -0.5f, 1.0f, 0.0f, //1
        0.5f,  0.5f, 1.0f, 1.0f, //2
        -0.5f, 0.5f, 0.0f, 1.0f //3
    };

    /* create an index buffer which tells you which order
    to draw our triangles*/
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    

    /*Creates and Binds Vertex Array*/
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    VertexArray va;
    /*Now we are going to create a buffer for our triangle*/
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0));

    /* create the index buffer object, bind it and send to gpu*/
    IndexBuffer ib(indices, 6);

    /*Create a shader object which we will use to handle shaders and uniforms*/
    Shader shader{ "res\\shaders\\Basic.shader" };
    shader.Bind();

    /*use a uniform to send variables into shaders*/
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);


    Texture texture("res/textures/Grass_Block.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);


    /*Unbind buffers for the point of demonstration*/
    ib.UnBind();
    shader.UnBind();
    vb.UnBind();
    va.UnBind();

    Renderer renderer;

    /*Time to write shaders!!! */ 
    float r = 0.0f;
    float increment = 0.05f;


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        /*Bind Buffers on each call*/
        shader.Bind();
        va.Bind();
        ib.Bind();
   
        /*Issue a draw call for our buffer*/
        /*Uniforms are used per draw call, uniforms are set per draw*/
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        renderer.Draw(va, ib, shader);

        if (r > 1.0f) {
            increment = -0.05f;
        }
        else if (r < 0.0f) {
            increment = 0.05f;
        }

        r += increment;

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    glfwTerminate();
    return 0;
};