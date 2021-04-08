#include "OpenGLShaderCompiler.h"
#include "glad/glad.h"

namespace GL_LEARN
{

    OpenGLShaderCompiler::OpenGLShaderCompiler()
    {
        m_iProgramId = 0;
    }

    OpenGLShaderCompiler::~OpenGLShaderCompiler()
    {
        glDeleteProgram(m_iProgramId);
    }

    bool OpenGLShaderCompiler::CompilerShader(const char* vsSource, const char* fsScource, std::string& errMsg)
    {
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vsSource, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            errMsg = "ERROR::SHADER::VERTEX::COMPILATION_FAILED " + std::string(infoLog);
            return false;
        }
        // fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fsScource, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            errMsg = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED " + std::string(infoLog);
            return false;
        }
        // link shaders
        m_iProgramId = glCreateProgram();
        glAttachShader(m_iProgramId, vertexShader);
        glAttachShader(m_iProgramId, fragmentShader);
        glLinkProgram(m_iProgramId);
        // check for linking errors
        glGetProgramiv(m_iProgramId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_iProgramId, 512, NULL, infoLog);
            errMsg = "ERROR::SHADER::PROGRAM::LINKING_FAILED " + std::string(infoLog);
            return false;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    unsigned int OpenGLShaderCompiler::GetProgramId()
    {
        return m_iProgramId;
    }

    void OpenGLShaderCompiler::Bind()
    {
        glUseProgram(m_iProgramId);
    }

    void OpenGLShaderCompiler::UnBind()
    {
        glUseProgram(0);
    }

    void OpenGLShaderCompiler::SetUniformMat(const char* key, float* aMatrix)
    {
        std::map<std::string, unsigned int>::const_iterator iter = mShaderIdMap.find(key);
        unsigned int iLocation = 0;
        if (iter == mShaderIdMap.end())
        {
            iLocation = glGetUniformLocation(m_iProgramId, key);
            mShaderIdMap[key] = iLocation;
        }
        else
            iLocation = iter->second;
        glUniformMatrix4fv(iLocation, 1, GL_FALSE, aMatrix);
    }

}