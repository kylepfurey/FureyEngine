// .cpp
// OpenGL Shader Class Script
// by Kyle Furey

#include "Shader.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Creates a new shader program comprised of a compiled vertex and fragment shader.
    Shader::Shader(const std::string &VertexShaderPath,
                   const std::string &FragmentShaderPath)
        : VertexPath(VertexShaderPath), FragmentPath(FragmentShaderPath) {
        ++TotalShaders;
        VertexID = Compile(VertexShaderPath, GL_VERTEX_SHADER);
        FragmentID = Compile(FragmentShaderPath, GL_FRAGMENT_SHADER);
        MyProgramID = CreateProgram({VertexID, FragmentID});
    }

    // DESTRUCTOR

    // Properly destroys this shader program.
    Shader::~Shader() {
        glDeleteProgram(MyProgramID);
        --TotalShaders;
    }

    // GETTERS

    // Returns the path to the now compiled vertex shader.
    std::string Shader::VertexShaderPath() const {
        return VertexPath;
    }

    // Returns the path to the now compiled fragment shader.
    std::string Shader::FragmentShaderPath() const {
        return FragmentPath;
    }

    // Returns the ID of the now compiled vertex shader.
    GLuint Shader::VertexShaderID() const {
        return VertexID;
    }

    // Returns the ID of the now compiled fragment shader.
    GLuint Shader::FragmentShaderID() const {
        return FragmentID;
    }

    // Returns the ID of this shader program.
    GLuint Shader::ProgramID() const {
        return MyProgramID;
    }

    // Returns the total number of shader programs open.
    int Shader::Total() {
        return TotalShaders;
    }

    // COMPILATION

    /** Compiles the shader at the given path through OpenGL. */
    GLuint Shader::Compile(const std::string &Path, const GLenum &ShaderType) {
        // Read the file at the given path
        std::ifstream File(Path);
        if (!File.is_open()) {
            throw std::runtime_error("ERROR: OpenGL failed to read the shader at " + Path + ".");
        }
        std::string Text;
        std::string CurrentLine;
        while (std::getline(File, CurrentLine)) {
            Text += CurrentLine + '\n';
        }
        File.close();

        // Create a new shader, compile it, and return its ID
        const GLchar *Source = Text.c_str();
        const GLuint ID = glCreateShader(ShaderType);
        glShaderSource(ID, 1, &Source, nullptr);
        glCompileShader(ID);
        GLint Result;
        glGetShaderiv(ID, GL_COMPILE_STATUS, &Result);
        if (Result == GL_FALSE) {
            char InfoLog[512];
            glGetShaderInfoLog(ID, sizeof(InfoLog), nullptr, InfoLog);
            glDeleteShader(ID);
            throw std::runtime_error("ERROR: OpenGL failed to compile the shader!\nOpenGL Error: " +
                                     std::string(InfoLog));
        }
        return ID;
    }

    // Creates a shader program with the given compiled shaders through OpenGL.
    // Note: This automatically deletes the shaders associated with the given IDs.
    GLuint Shader::CreateProgram(const std::initializer_list<GLuint> &ShaderIDs) {
        // Create a new shader program
        const GLuint Program = glCreateProgram();
        if (Program == 0) {
            throw std::runtime_error("ERROR: OpenGL failed to create a shader program!");
        }
        for (const GLuint &ShaderID: ShaderIDs) {
            glAttachShader(Program, ShaderID);
        }

        // Link the new shaders to the program and return the program's ID
        glLinkProgram(Program);
        GLint Result;
        glGetProgramiv(Program, GL_LINK_STATUS, &Result);
        if (Result == GL_FALSE) {
            char InfoLog[512];
            glGetProgramInfoLog(Program, sizeof(InfoLog), nullptr, InfoLog);
            glDeleteProgram(Program);
            throw std::runtime_error("ERROR: OpenGL failed to link the shaders to a program!\nOpenGL Error: " +
                                     std::string(InfoLog));
        }

        // Delete the compiled shaders
        for (const GLuint &ShaderID: ShaderIDs) {
            glDeleteShader(ShaderID);
        }
        return Program;
    }

    // STATIC VARIABLE INITIALIZATION

    // The current number of shader programs open.
    int Shader::TotalShaders = 0;
}
