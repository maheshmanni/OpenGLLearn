#pragma once
namespace GL_LEARN
{
    class OpenGLFrameBuffer
    {
    public:
        OpenGLFrameBuffer(unsigned int iWidth, unsigned int iHeight);
        ~OpenGLFrameBuffer();
        bool InValidate();
        void Bind();
        void UnBind();
        void Resize(unsigned int iWidth, unsigned int iHeight);
        unsigned int GetColorTexture();
    private:
        unsigned int m_iMSFBO;
        unsigned int m_iFBO;
        unsigned int m_iWidth;
        unsigned int m_iHeight;
        unsigned int m_iColorTexId;
        unsigned int m_iNumSamples;
    };
}

