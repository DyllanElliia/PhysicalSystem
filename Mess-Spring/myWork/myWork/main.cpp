#include <gl/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_master/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_master/stb_image_write.h>

//#include <soil.h>

#include "Elastic.h"
#include "ground.h"

float windowHeight, windowWidth;

// 创建全局变量；
Elastic model;
#define GRAVITY glm::vec3(0, -0.15, 0)

bool dynamicFuzzy = false;	// 运动模糊开关；
float setLevel = 0.7;		// 运动模糊程度；

void dynamicFuzzy_(float level)
{
	// 以减色模式作为临时混色方案；
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, 0);

	glColor4f(0, 0, 0, level);
	glBegin(GL_QUADS);
	glVertex3f(-windowWidth,  -	windowHeight, 0);
	glVertex3f(	windowWidth,  -	windowHeight, 0);
	glVertex3f(	windowWidth,	windowHeight, 0);
	glVertex3f(-windowWidth,	windowHeight, 0);
	glEnd();

	// 恢复混色方案为加色方案；
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}



void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(EXIT_SUCCESS);
	case 'f':
		dynamicFuzzy = !dynamicFuzzy;
		break;
	}

}

void processSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		setLevel += 0.05;
		if (setLevel > 1)
			setLevel = 1;
		break;
	case GLUT_KEY_DOWN:
		setLevel -= 0.05;
		if (setLevel < 0)
			setLevel = 0;
		break;
	}
}

void Init_()
{
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);

	glClearDepth(2);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

}

// Setup the rendering state
void SetupRC(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//当窗口改变大小时由GLUT函数库调用
void ChangeSize(int w, int h)
{
	float aspectRatio;

	//防止被0所除
	if (h == 0) h = 1;

	//把视口设置为窗口的大小
	glViewport(0, 0, w, h);

	//重置坐标系统
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//建立剪裁区域（左、右、底、顶、近、远）
	aspectRatio = (float)w / (float)h;

	if (w <= h){
		windowWidth = 100;
		windowHeight = 100 / aspectRatio;
		glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
	}
	else{
		windowWidth = 100.0 * aspectRatio;
		windowHeight = 100;
		glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void RenderScene(void)
{
	if (dynamicFuzzy)	// 动态模糊；
	{
		glClear(GL_DEPTH_BUFFER_BIT);						// Clear depth buffer
		dynamicFuzzy_(setLevel);
	}
	else
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear screen and depth buffer

	glMatrixMode(GL_MODELVIEW);

	//Fire.draw(texture[0]);
	model.drawSpring();

	glutSwapBuffers();
}


void TimerFunction(int value)
{
	//Update();
	model.frame();
	glutPostRedisplay();
	glutTimerFunc(10,TimerFunction,1);
}



int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,640);
	glutCreateWindow("Elastic system");

	// Setup the rendering state
	//light_();
	//Init_();
	// SetupRC();

	// register callbacks
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(10, TimerFunction, 1);
	//注册按键；
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	
	// 创建弹性物体
	//model.set1D_rope(30, glm::vec3(-1, 20, 0), glm::vec3(50, 50, 0), 3, 0.3);
	model.set2D_ring(40, glm::vec3(0, 20, 0), glm::vec3(50, 50, 0), 3, 0.031);
	//model.set3D_TriangularPyramid(glm::vec3(0, 20, 0), glm::vec3(50, 50, 0), 1, 0.001);
	model.setOutForce(GRAVITY);

	// 添加碰撞物体；
	model.addObject(new ground(glm::vec3(0, 18.5, 0), glm::vec3(0, 1, 0)));
	//model.addObject(new ground(glm::vec3(0, 18, 0), glm::vec3(1, 1, 0)));
	//model.addObject(new ground(glm::vec3(0, 18, 0), glm::vec3(-1, 1, 0)));

	// enter GLUT event processing loop
	glutMainLoop();

	return 0;
}