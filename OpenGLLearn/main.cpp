#include <stdlib.h>
#include <iostream>
#include <glad/glad.h>

#include "GLFW/glfw3.h"
#include <string>
#include "OpenGLShaderCompiler.h"
#include "OpenGLFrameBuffer.h"

#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


static std::string VertexShader = R"(
#version 330 core
layout(location = 0) in vec3 pos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view* model * vec4(pos, 1.0);
}
)";

static std::string FragmentShader = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
)";

static std::string QuadVS = R"(
#version 330 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 texCoord;
out vec2 fragTexCoord;
void main()
{
    fragTexCoord = texCoord;
    gl_Position = vec4(pos, 0.0, 1.0);
}
)";

static std::string QuadFS = R"(
#version 330 core
in vec2 fragTexCoord;
uniform sampler2D bindTex;
out vec4 FragColor;
void main()
{
    FragColor = texture(bindTex, fragTexCoord);
}

)";

GL_LEARN::OpenGLFrameBuffer* m_pFrameBuffer = nullptr;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
unsigned int iWidth = 1024;
unsigned int iHeight = 1024;
void InitGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void framebuffer_size_callback(GLFWwindow* pWindow, int w, int h)
{
    iWidth = w;
    iHeight = h;
    glViewport(0, 0, w, h);
    if (m_pFrameBuffer)
        m_pFrameBuffer->Resize(w, h);
}
float lastX = (float)iWidth / 2.0;
float lastY = (float)iHeight / 2.0;
bool firstMouse = true;
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

GLFWwindow* CreateGLFWWindow()
{
    GLFWwindow* window = glfwCreateWindow(1024, 1024, "OpenGLLearn", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    return window;
}

int main()
{
    InitGLFW();
    GLFWwindow* window = CreateGLFWWindow();
    if (window == nullptr)
    {
        std::cerr << "failed to create window!";
        glfwTerminate();
    }

    // glad: load all OpenGL function pointers
 // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    GL_LEARN::OpenGLShaderCompiler shaderCompiler;
    std::string errMsg;
    shaderCompiler.CompilerShader(VertexShader.c_str(), FragmentShader.c_str(), errMsg);
    
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
       // positions   // texCoords
       -1.0f,  1.0f,  0.0f, 1.0f,
       -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

       -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };

    GLuint quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GL_LEARN::OpenGLShaderCompiler quadShader;
    quadShader.CompilerShader(QuadVS.c_str(), QuadFS.c_str(), errMsg);
    if (!m_pFrameBuffer)
        m_pFrameBuffer = new GL_LEARN::OpenGLFrameBuffer(1024, 1024);
    while (!glfwWindowShouldClose(window))
    {
        m_pFrameBuffer->Bind();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderCompiler.Bind();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)iWidth / (float)iHeight, 0.1f, 100.0f);

        shaderCompiler.SetUniformMat("model", &model[0][0]);
        shaderCompiler.SetUniformMat("view", &view[0][0]);
        shaderCompiler.SetUniformMat("projection", &projection[0][0]);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        shaderCompiler.UnBind();
        glBindVertexArray(0);

        m_pFrameBuffer->UnBind();

        glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        quadShader.Bind();
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, m_pFrameBuffer->GetColorTexture());
        glDrawArrays(GL_TRIANGLES, 0, 6);
        quadShader.UnBind();
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return EXIT_SUCCESS;
}