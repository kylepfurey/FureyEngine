// .h
// OpenGL Shader Class Script
// by Kyle Furey

#pragma once
#include "../Standard/Standard.h"

namespace FureyEngine {
    /**
     * Represents an automatically compiled shader program.<br/>
     * used to render images and geometry to a window in OpenGL.
     */
    class Shader final {
        // SHADER PATHS

        /** The path to the vertex shader. */
        const std::string VertexPath;

        /** The path to the fragment shader. */
        const std::string FragmentPath;

        // SHADER IDS

        /** The ID of the vertex shader. */
        GLuint VertexID;

        /** The ID of the vertex shader. */
        GLuint FragmentID;

        // SHADER PROGRAM

        /** The ID of this shader program. */
        GLuint MyProgramID;

        // TOTAL SHADERS

        /** The total number of shader programs open. */
        static int TotalShaders;

    public:
        // CONSTRUCTORS

        /** Creates a new shader program comprised of a compiled vertex and fragment shader. */
        explicit Shader(const std::string &VertexShaderPath,
                        const std::string &FragmentShaderPath);

        /** Delete copy constructor. */
        Shader(const Shader &Copied) = delete;

        /** Delete move constructor. */
        Shader(Shader &&Moved) = delete;

        // ASSIGNMENT OPERATORS

        /** Delete copy constructor. */
        Shader &operator=(const Shader &Copied) = delete;

        /** Delete move constructor. */
        Shader &operator=(Shader &&Moved) = delete;

        // DESTRUCTOR

        /** Properly destroys this shader program. */
        ~Shader();

        // GETTERS

        /** Returns the path to the now compiled vertex shader. */
        [[nodiscard]] std::string VertexShaderPath() const;

        /** Returns the path to the now compiled fragment shader. */
        [[nodiscard]] std::string FragmentShaderPath() const;

        /** Returns the ID of the now compiled vertex shader. */
        [[nodiscard]] GLuint VertexShaderID() const;

        /** Returns the ID of the now compiled fragment shader. */
        [[nodiscard]] GLuint FragmentShaderID() const;

        /** Returns the ID of this shader program. */
        [[nodiscard]] GLuint ProgramID() const;

        /** Returns the total number of shader programs open. */
        [[nodiscard]] static int Total();

        // COMPILATION

        /** Compiles the shader at the given path through OpenGL. */
        [[nodiscard]] static GLuint Compile(const std::string &Path, const GLenum &ShaderType);

        /**
         * Creates a shader program with the given compiled shaders through OpenGL.<br/>
         * NOTE: This automatically deletes the shaders associated with the given IDs.
         */
        [[nodiscard]] static GLuint CreateProgram(const std::initializer_list<GLuint> &ShaderIDs);
    };
}
