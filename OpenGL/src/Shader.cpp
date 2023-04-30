#include "Shader.h"

/*Standard Libraries*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"


/*Constructors and Destructors*/

Shader::Shader(const std::string& filepath): 
	m_FilePath(filepath), 
	m_RendererID(0) 
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}


Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}





/*Member Functions*/

void Shader::Bind() const {
    GLCall(glUseProgram(m_RendererID));
}


void Shader::UnBind() const {
    GLCall(glUseProgram(0));
}


void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    int location = GetUniformLocation(name);
    GLCall(glUniform4f(location, v0, v1, v2, v3));
}


void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    int location = GetUniformLocation(name);
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
}


void Shader::SetUniform1f(const std::string& name, float value) {
    int location = GetUniformLocation(name);
    GLCall(glUniform1f(location, value));
}


void Shader::SetUniform1i(const std::string& name, int value) {
    int location = GetUniformLocation(name);
    GLCall(glUniform1i(location, value));
}


int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
    }
    m_UniformLocationCache[name] = location;
 
    return location;
}


unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile" <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << "!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }


    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    /*Attach the vs, and fs to the program and link it to compile on the GPU*/
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                // set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                // set mode to fragment
                type = ShaderType::FRAGMENT;
            }

        }
        else {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str() };
}