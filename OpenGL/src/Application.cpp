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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"



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
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
        -50.0f, -50.0f, 0.0f, 0.0f, //0      0,0     These are texture coordinates
        50.0f,  -50.0f, 1.0f, 0.0f, //1      1,0
        50.0f,  50.0f, 1.0f, 1.0f, //2       1, 1
        -50.0f, 50.0f, 0.0f, 1.0f //3        0, 1
    };

    /* create an index buffer which tells you which order
    to draw our triangles*/
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    

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

    /*Here we create our projection matrix*/
    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));


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
    /*Create the context for ImGui which is used for creating our GUI*/
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    glm::vec3 translationA(200, 200, 0);
    glm::vec3 translationB(400, 200, 0);

    /*Time to write shaders!!! */ 
    float r = 0.0f;
    float increment = 0.05f;


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        /*Create the IMGui Frame*/
        ImGui_ImplGlfwGL3_NewFrame();
  

        /*Bind Buffers on each call*/
        // shader.Bind();
        va.Bind();
        ib.Bind();

        {
            /*Mess with model projection matrices*/
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);

            glm::mat4 mvp = proj * view * model;
            shader.Bind();
            /*Issue a draw call for our buffer*/
            /*Uniforms are used per draw call, uniforms are set per draw*/
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);

        }


        {
            /*Mess with model projection matrices*/
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);

            glm::mat4 mvp = proj * view * model;
            /*Issue a draw call for our buffer*/
            /*Uniforms are used per draw call, uniforms are set per draw*/
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);

        }


        if (r > 1.0f) {
            increment = -0.05f;
        }
        else if (r < 0.0f) {
            increment = 0.05f;
        }

        r += increment;


        /*Define objects on our frame*/
        {
            ImGui::SliderFloat3("TranslationA", &translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f 
            ImGui::SliderFloat3("TranslationB", &translationB.x, 0.0f, 960.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        /*Render our ImGui frame*/
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    /*Shut down our ImGui frame*/
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
};