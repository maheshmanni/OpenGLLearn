#pragma once
#include <string>
#include <map>
namespace GL_LEARN
{
    class OpenGLShaderCompiler
    {
    public:
        OpenGLShaderCompiler();
        ~OpenGLShaderCompiler();
        bool CompilerShader(const char* vsSource, const char* fsScource, std::string& errMsg);

        unsigned int GetProgramId();
        void Bind();
        void UnBind();

        void SetUniformMat(const char* key, float* aMatrix);
    private:
        unsigned int m_iProgramId;
        std::map<std::string, unsigned int> mShaderIdMap;
    };

}

