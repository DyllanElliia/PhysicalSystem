#include <iostream>
#include <string>
#include <gl/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_master/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_master/stb_image_write.h>

#include "Frame.h"

//#include <soil.h>

struct Info
{
    int argc;
    char** argv;
};

void createWindow(Info i, std::string name, glm::vec2 windowPosition, glm::vec2 windowSize, unsigned int mode)
{
    glutInit(&(i.argc), (i.argv));
    glutInitDisplayMode(mode);
    glutInitWindowPosition(windowPosition.x, windowPosition.y);
    glutInitWindowSize(windowSize.x, windowSize.y);
    glutCreateWindow(name.c_str());
}

inline void clearColor()
{
    glClearColor(0.6f, 0.4f, 0.7f, 1.f);
}

void changeSizeOrtho(int w, int h)
{
    //防止高度为0；
    if (h == 0)
        h = 1;

    //设置窗口大小；
    glViewport(0, 0, w, h);

    //重置坐标系统；
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //建立裁剪区域（左、右、底、顶、近、远）；
    GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;

    if (w > h)
        glOrtho(-1.0 * aspectRatio, 1.0 * aspectRatio, -1.0, 1.0, 1.0, -1.0);
    else
        glOrtho(-1.0, 1.0, -1.0 / aspectRatio, 1.0 / aspectRatio, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void changeSizePerspective(int w, int h)
{
    //防止高度为0；
    if (h == 0)
        h = 1;

    float ratio = 1.0 * w / h;

    //单位化投影矩阵；
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //设置窗口大小；
    glViewport(0, 0, w, h);

    //设置正确的投影矩阵；
    gluPerspective(45, ratio, 1, 100);

    //设置模型视图矩阵；
    glMatrixMode(GL_MODELVIEW);

}

GLfloat speed = 2;
glm::vec3 color(1.f, 0.f, 0.f);
GLfloat angle = 0;
GLfloat xrot, yrot, xspeed, yspeed, z = -5;

bool light = true;

GLuint filter = 0;
unsigned int texture[2];
Frame frame;

void processNormalKeys(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(EXIT_SUCCESS);
    case '1':
        speed = 1;
        break;
    case '2':
        speed = 2;
        break;
    case '3':
        speed = 3;
        break;
    case '4':
        speed = 4;
        break;
    case 'a':
        color = glm::vec3(1, 0, 0);
        break;
    case 's':
        color = glm::vec3(0, 1, 0);
        break;
    case 'd':
        color = glm::vec3(0, 0, 1);
        break;
    case 'l':
        light = !light;
        light ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
        break;
    case 't':
        filter = (++filter) % 2;
        std::cout << "filter change: " << filter << std::endl;
        break;
    }

}





void processSpecialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_F1:
        color = glm::vec3(1, 0, 0);
        break;
    case GLUT_KEY_F2:
        color = glm::vec3(0, 1, 0);
        break;
    case GLUT_KEY_F3:
        color = glm::vec3(0, 0, 1);
        break;


    }
}


void renderScene()
{
    //清空颜色缓冲；
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0, 0, z);

    

    glRotatef(xrot, 1, 0, 0);
    glRotatef(yrot, 0, 1, 0);
    //图形绘制；
    glColor4f(1, 0, 0, 1);
    glutSolidCube(1);

    //让Opengl处理提供给它的绘图指令；
    //glFlush();//单缓冲；
    glutSwapBuffers();//双缓冲，前后缓冲交换；
    xrot += xspeed;
    yrot += yspeed;
}

void renderSceneT()
{
    std::cout << "0" << std::endl;
    //清空颜色缓冲；
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    frame.draw(texture[0]);
    
    glLoadIdentity();
    glTranslatef(0, 0, z);

    glRotatef(xrot, 1, 0, 0);
    glRotatef(yrot, 0, 1, 0);

    glBindTexture(GL_TEXTURE_2D, texture[filter]);
    //图形绘制；
    glBegin(GL_QUADS);

    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, 1);
    glTexCoord2f(1, 0); glVertex3f( 1, -1, 1);
    glTexCoord2f(1, 1); glVertex3f( 1,  1, 1);
    glTexCoord2f(0, 1); glVertex3f(-1,  1, 1);

    glNormal3f(0, 0, -1);
    glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
    glTexCoord2f(1, 1); glVertex3f(-1,  1, -1);
    glTexCoord2f(0, 1); glVertex3f( 1,  1, -1);
    glTexCoord2f(0, 0); glVertex3f( 1, -1, -1);

    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 1); glVertex3f(-1, 1, -1);
    glTexCoord2f(0, 0); glVertex3f(-1, 1,  1);
    glTexCoord2f(1, 0); glVertex3f( 1, 1,  1);
    glTexCoord2f(1, 1); glVertex3f( 1, 1, -1);

    glNormal3f(0, -1, 0);
    glTexCoord2f(1, 1); glVertex3f(-1, -1, -1);
    glTexCoord2f(0, 1); glVertex3f( 1, -1, -1);
    glTexCoord2f(0, 0); glVertex3f( 1, -1,  1);
    glTexCoord2f(1, 0); glVertex3f(-1, -1,  1);

    glNormal3f(1, 0, 0);
    glTexCoord2f(1, 0); glVertex3f(1, -1, -1);
    glTexCoord2f(1, 1); glVertex3f(1,  1, -1);
    glTexCoord2f(0, 1); glVertex3f(1,  1,  1);
    glTexCoord2f(0, 0); glVertex3f(1, -1,  1);

    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, -1);
    glTexCoord2f(1, 0); glVertex3f(-1, -1,  1);
    glTexCoord2f(1, 1); glVertex3f(-1,  1,  1);
    glTexCoord2f(0, 1); glVertex3f(-1,  1, -1);
    glEnd();

    //让Opengl处理提供给它的绘图指令；
    //glFlush();//单缓冲；
    glutSwapBuffers();//双缓冲，前后缓冲交换；
    //xrot += xspeed;
    //yrot += yspeed;
}



void timerFunc(int value)
{
    //std::cout << "0" << std::endl;
    frame.simulate();
    //std::cout << "0" << std::endl;
    //重新设置计时器；
    glutPostRedisplay();
    glutTimerFunc(23, timerFunc, 1);
}

void light_()
{
    glEnable(GL_LIGHTING);

    GLfloat lightAmbient[] = { 1,1,1,1 };
    GLfloat lightDiffuse[] = { 1,1,1,1 };
    GLfloat lightPosition[] = { 0,0,2,1 };

    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    glEnable(GL_LIGHT1);
}

int loadTexture()
{
    unsigned char* tex[2];
    int info[2][3];
    for (int i = 0; i < 2; i++)
    {
        tex[i] = stbi_load("../texture/frame.png", &(info[i][0]), &(info[i][1]), &(info[i][2]), 0);
        std::cout << "n: " << info[i][2] << std::endl;
    }

    glGenTextures(2, &texture[0]);
    if (texture[0] == 0)
        return false;

    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info[0][0], info[0][1], 0, GL_RGBA, GL_UNSIGNED_BYTE, tex[0]); // 设置纹理对象内容
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // 设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // 设置纹理过滤方式

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    if (texture[1] == 0)
        return false;
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info[1][0], info[1][1], 0, GL_RGBA, GL_UNSIGNED_BYTE, tex[1]); // 设置纹理对象内容
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // 设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // 设置纹理过滤方式

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return true;
}

void InitGL(GLvoid)
{
    if (!loadTexture())
        return;
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0, 0, 0, 0.5);
    glClearDepth(1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // GLfloat gray[] = {0.9f, 0.0f, 0.0f, 1.0f};
    // glMaterialfv(GL_FRONT, GL_DIFFUSE,gray);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    frame.set(5000, glm::vec2(100, 100), glm::vec3(0, 0, 0), 1.0, 0.2);
    frame.setLife(2);
}


int main(int argc, char** argv)
{
    //初始化GLUT，并创建窗口；
    //使用双缓冲，目的是绘制流畅动画；
    //使用深度检测，可以正确显示多个图形；
    createWindow(Info{ argc, argv }, std::string("2018091605001_1.1"),
        glm::vec2(100, 100), glm::vec2(640, 640),
        GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    //设置渲染函数和回调函数；
    glutDisplayFunc(renderSceneT);   //将事件添加到循环队列中；
    glutIdleFunc(renderSceneT);
    glutReshapeFunc(changeSizePerspective); //添加改变窗口大小的事件；

    //设置计时器；
    glutTimerFunc(23, timerFunc, 1);

    //注册按键；
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);

    //clearColor();//清空颜色；
    InitGL();
    light_();

    //处理GLUT事件循环队列；
    glutMainLoop();

    return 0;
}
