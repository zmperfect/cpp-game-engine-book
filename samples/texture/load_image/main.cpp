#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/transform2.hpp>
#include<glm/gtx/euler_angles.hpp>

#include <stdlib.h>
#include <stdio.h>

#include "vertex_data.h"
#include "shader_source.h"
#include "texture2d.h"

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

GLFWwindow* window;
GLuint vertex_shader, fragment_shader, program;
GLint mvp_location, vpos_location, vcol_location;

//初始化OpenGL
void init_opengl()
{
    glfwSetErrorCallback(error_callback); // 设置错误信息回调

    // 初始化GLFW，如果失败直接退出
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // 设置OpenGL版本为2.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(960, 640, "Simple example", nullptr, nullptr);// 创建窗口，宽高960*640，标题为Simple example，无父窗口，无共享上下文
    if (!window) // 如果创建失败，直接退出
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window); // 设置当前上下文
    gladLoadGL(glfwGetProcAddress); // 加载OpenGL函数指针
    glfwSwapInterval(1); // 设置垂直同步
}

//编译、链接Shader
void compile_shader()
{
    //创建顶点Shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    //指定Shader源码
    glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
    //编译Shader
    glCompileShader(vertex_shader);

    //创建片段Shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    //指定Shader源码
    glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
    //编译Shader
    glCompileShader(fragment_shader);

    //创建GPU程序
    program = glCreateProgram();
    //附加Shader
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    //Link
    glLinkProgram(program);
}

//创建Texture
void CreateTexture(std::string image_file_path)
{
    Texture2D* texture2d=Texture2D::LoadFromFile(image_file_path);
}

int main()
{
    init_opengl();

    CreateTexture("../data/images/red.png");

    compile_shader();

    mvp_location = glGetUniformLocation(program, "u_mvp");
    vpos_location = glGetAttribLocation(program, "a_pos");
    vcol_location = glGetAttribLocation(program, "a_color");

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glm::mat4 model,view, projection, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        float ratio = width / static_cast<float>(height);

        glViewport(0, 0, width, height); // 设置视口
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // 清空颜色缓冲
        glClearColor(49.f/255,77.f/255,121.f/255,1.f); // 设置清空颜色

        glm::mat4 trans = glm::translate(glm::vec3(0,0,0)); //不移动顶点坐标;

        static float rotate_eulerAngle=0.f; //旋转角度
        rotate_eulerAngle+=1;   //每帧增加1度
        glm::mat4 rotation = glm::eulerAngleYXZ(glm::radians(rotate_eulerAngle), glm::radians(rotate_eulerAngle), glm::radians(rotate_eulerAngle)); //使用欧拉角旋转;

        glm::mat4 scale = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f)); //缩放;
        model = trans*scale*rotation; //模型矩阵,先缩放，再旋转，最后平移

        view = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0,0), glm::vec3(0, 1, 0)); //视图矩阵

        projection=glm::perspective(glm::radians(60.f),ratio,1.f,1000.f); //透视投影矩阵

        mvp=projection*view*model; //模型视图投影矩阵

        //指定GPU程序(就是指定顶点着色器、片段着色器)
        glUseProgram(program);
        {
            glEnable(GL_DEPTH_TEST);//开启深度测试
            glEnable(GL_CULL_FACE);//开启背面剔除

            glEnableVertexAttribArray(vpos_location);//启用顶点坐标属性
            //上传顶点坐标数据
            glVertexAttribPointer(vpos_location, 3, GL_FLOAT, false, sizeof(glm::vec3), kPositions);
            glEnableVertexAttribArray(vcol_location);//启用顶点颜色属性
            //上传顶点颜色数据
            glVertexAttribPointer(vcol_location, 3, GL_FLOAT, false, sizeof(glm::vec4), kColors);
            //上传mvp矩阵
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);

            //void glDrawArrays(GLenum mode,GLint first,GLsizei count);
            glDrawArrays(GL_TRIANGLES, 0, 6*6);//表示从第0个顶点开始画，总共画6个面，每个面6个顶点。
        }
        

        glfwSwapBuffers(window);//交换缓冲
        glfwPollEvents();//轮询事件
    }

    glfwDestroyWindow(window);//销毁窗口

    glfwTerminate();//终止GLFW
    exit(EXIT_SUCCESS);//退出程序
}