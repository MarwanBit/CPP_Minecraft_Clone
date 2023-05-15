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
#include "Face.h"

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

    /*
    float positions[] = {
        50.0f, 50.0f, 0.0f, 0.0f, //0      0,0     These are texture coordinates
        150.0f,  50.0f, 1.0f, 0.0f, //1      1,0
        150.0f,  150.0f, 1.0f, 1.0f, //2       1, 1
        50.0f, 150.0f, 0.0f, 1.0f //3        0, 1
    };
    */

    glm::vec3 pos0{ 50.0f, 50.0f, 150.0f };
    glm::vec3 pos1{ 150.0f, 50.0f, 150.0f };
    glm::vec3 pos2{ 150.0f, 150.0f, 150.0f };
    glm::vec3 pos3{ 50.0f, 150.0f, 150.0f };
    //Now we create a face object
    Face grassFace{ pos0, pos1, pos2, pos3, "res/textures/Grass_Block.png", "res\\shaders\\Basic.shader" };

    float* positions = grassFace.getPositions();

    /* create an index buffer which tells you which order
    to draw our triangles*/
   
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
    layout.Push<float>(4);
    va.AddBuffer(vb, layout);

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, 0));

    /* create the index buffer object, bind it and send to gpu*/
    // Change to 3 to generate entire square (having problems with generating triangles)
    IndexBuffer ib(grassFace.getIndices(), 6);

    /*Here we create our projection matrix*/
    glm::mat4 proj_exp = glm::perspective(35.0f, 1.0f, 0.5f, 400.0f);
    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 1.0f));


    /*Create a shader object which we will use to handle shaders and uniforms*/
    Shader shader = grassFace.getShader();
    shader.Bind();

    /*use a uniform to send variables into shaders*/
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);


    Texture texture = grassFace.getTexture();
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

    float angle = 0.0f;
    float increment = 1.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        /*Create the IMGui Frame*/
        ImGui_ImplGlfwGL3_NewFrame();
  

        /*Bind Buffers on each call*/
        shader.Bind();
        va.Bind();
        ib.Bind();

        {
            /*Mess with model projection matrices*/
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            //model = glm::rotate<float>(model, 0.0f, glm::vec3{0.0f,1.0f,0.0f});

            glm::mat4 mvp = proj_exp * view * model;

           
            shader.Bind();
            /*Issue a draw call for our buffer*/
            /*Uniforms are used per draw call, uniforms are set per draw*/
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);

        }

       
        /*Define objects on our frame*/
        {
            ImGui::SliderFloat3("TranslationA", &(translationA.x), 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f 
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }


        /*Render our ImGui frame*/
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());


        //increment the rotation value
        angle += increment;
        
    }

    /*Shut down our ImGui frame*/
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
};