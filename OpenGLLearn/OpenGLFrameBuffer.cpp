#include "OpenGLFrameBuffer.h"
#include "glad/glad.h"
namespace GL_LEARN
{

    OpenGLFrameBuffer::OpenGLFrameBuffer(unsigned int iWidth, unsigned int iHeight):
        m_iWidth(iWidth),
        m_iHeight(iHeight)
    {
        m_iFBO = 0;
        m_iMSFBO = 0;
        m_iNumSamples = 16;
        InValidate();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {

    }

    bool OpenGLFrameBuffer::InValidate()
    {
        if (m_iFBO != 0)
        {
            glDeleteFramebuffers(1, &m_iFBO);
            glDeleteFramebuffers(1, &m_iMSFBO);
        }
        //create multisample frame buffer
        glGenFramebuffers(1, &m_iMSFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_iMSFBO);

        GLuint iMSTexture;
        glGenTextures(1, &iMSTexture);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, iMSTexture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_iNumSamples, GL_RGB, m_iWidth, m_iHeight, GL_TRUE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, iMSTexture, 0);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

        unsigned int iMSRBO;
        glGenRenderbuffers(1, &iMSRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, iMSRBO);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_iNumSamples, GL_DEPTH24_STENCIL8, m_iWidth, m_iHeight);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, iMSRBO);


        //normal framebuffer
        glGenFramebuffers(1, &m_iFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_iFBO);
        glGenTextures(1, &m_iColorTexId);
        glBindTexture(GL_TEXTURE_2D, m_iColorTexId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_iColorTexId, 0);

        // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_iWidth, m_iHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
        // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            return false;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return true;
    }

    void OpenGLFrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_iMSFBO);
    }

    void OpenGLFrameBuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::Resize(unsigned int iWidth, unsigned int iHeight)
    {
        m_iWidth = iWidth;
        m_iHeight = iHeight;
        InValidate();
    }

    unsigned int OpenGLFrameBuffer::GetColorTexture()
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_iMSFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_iFBO);
        glBlitFramebuffer(0, 0, m_iWidth, m_iHeight, 0, 0, m_iWidth, m_iHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return m_iColorTexId;
    }

}