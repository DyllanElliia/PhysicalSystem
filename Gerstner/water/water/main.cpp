// water.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <GL/glut.h>
#include <soil.h>

#include "water.h"
//#include "waterMore.h"
//#include "waterMoreLog.h"
//#include "waterMoreGerstner.h"

#define RESOLUTION 151

static float	rotate_x = 30;
static float	rotate_y = 15;
static float	translate_z = 4;

static float surface[6 * RESOLUTION * (RESOLUTION + 1)];
static float normal[6 * RESOLUTION * (RESOLUTION + 1)];

bool wire_frame = false;
bool normals = false;

void renderScene(void)
{
	const float t = glutGet(GLUT_ELAPSED_TIME) / 1000.;
	const float delta = 2. / RESOLUTION;
	const unsigned int length = 2 * (RESOLUTION + 1);
	const float xn = (RESOLUTION + 1) * delta + 1;
	unsigned int i;
	unsigned int j;
	float x;
	float y;
	unsigned int indice;
	unsigned int preindice;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0, 0, -translate_z);
	glRotatef(rotate_y, 1, 0, 0);
	glRotatef(rotate_x, 0, 1, 0);

	/* Vertices */
	for (j = 0; j < RESOLUTION; j++)
	{
		y = (j + 1) * delta - 1;
		for (i = 0; i <= RESOLUTION; i++)
		{
			float get_x, get_z;
			indice = 6 * (i + j * (RESOLUTION + 1));

			x = i * delta - 1;

			surface[indice + 4] = waveHeight(glm::vec2(x,y), t, get_x, get_z);
			surface[indice + 3] = get_x;
			surface[indice + 5] = get_z;
			if (j != 0)
			{
				/* Values were computed during the previous loop */
				preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
				surface[indice] = surface[preindice + 3];
				surface[indice + 1] = surface[preindice + 4];
				surface[indice + 2] = surface[preindice + 5];
			}
			else
			{
				surface[indice + 1] = waveHeight(glm::vec2(x, -1), t, get_x, get_z);
				surface[indice] = get_x;
				surface[indice + 2] = get_z;
			}
			/* Normals */
			glm::vec3 n = waveNormal(glm::vec2(surface[indice], surface[indice + 2]), t);
			normal[indice] = n.x;
			normal[indice + 1] = n.y;
			normal[indice + 2] = n.z;

			n = waveNormal(glm::vec2(surface[indice + 3], surface[indice + 5]), t);
			normal[indice + 3] = n.x;
			normal[indice + 4] = n.y;
			normal[indice + 5] = n.z;

		}
	}

	glDisable(GL_TEXTURE_2D);
	/* The ground */
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor4f(1.0f, 0.9f, 0.7f, 1.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glEnd();

	glTranslatef(0, 0.2, 0);

	/* Render wireframe? */
	if (wire_frame != 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	/* The water */
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normal);
	glVertexPointer(3, GL_FLOAT, 0, surface);
	for (i = 0; i < RESOLUTION; i++)
		glDrawArrays(GL_TRIANGLE_STRIP, i * length, length);

	/* Draw normals? */
	if (normals != 0)
	{
		glDisable(GL_TEXTURE_2D);
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		for (j = 0; j < RESOLUTION; j++)
			for (i = 0; i <= RESOLUTION; i++)
			{
				indice = 6 * (i + j * (RESOLUTION + 1));
				glVertex3fv(&(surface[indice]));
				glVertex3f(surface[indice] + normal[indice] / 50,
					surface[indice + 1] + normal[indice + 1] / 50,
					surface[indice + 2] + normal[indice + 2] / 50);
			}
		glEnd();
	}

	/* End */
	glutSwapBuffers();
	glutPostRedisplay();
}

GLfloat LightAmbient[] = { 0.9,0.9,0.9,1 };
GLfloat LightDiffuse[] = { 0.9,0.9,0.9,1 };
GLfloat LightPosition[] = { 1,1,-0.5,0 };

void InitGL_old()
{
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

void changeSize(int w, int h)
{
	if (h == 0)
		h = 1;
	float ratio = w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(20, ratio, 0.1, 15);

	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
	case 27:
		exit(EXIT_SUCCESS);
		break;
	case 'l':
		wire_frame = !wire_frame;
		break;
	case 'n':
		normals = !normals;
		break;
	default:
		break;
	}
}

int left_click = GLUT_UP;
int right_click = GLUT_UP;
int xold = 0, yold = 0;

void Mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		left_click = state;
		break;
	case GLUT_RIGHT_BUTTON:
		right_click = state;
		break;
	default:
		break;
	}
	xold = x;
	yold = y;
}

void mouseMotion(int x, int y)
{
	if (GLUT_DOWN == left_click)
	{
		rotate_y += (y - yold) / 5.0;
		rotate_x += (x - xold) / 5.0;
		if (rotate_y > 90)
			rotate_y = 90;
		if (rotate_y < -90)
			rotate_y = -90;
		glutPostRedisplay();
	}
	if (GLUT_DOWN == right_click)
	{
		translate_z += (yold - y) / 50.f;
		if (translate_z > 10)
			translate_z = 10;
		if (translate_z < 0.5)
			translate_z = 0.5;
		glutPostRedisplay();
	}
	xold = x;
	yold = y;
}

UINT caustic_texture;

int LoadGLTextures()
{
	caustic_texture = SOIL_load_OGL_texture
	(
		"../texture/reflection.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	if (!caustic_texture)
		return false;

	glBindTexture(GL_TEXTURE_2D, caustic_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
}

int InitGL()
{
	if (!LoadGLTextures())
		return false;


	glClearColor(0, 0, 0, 0);
	glClearDepth(1.f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return true;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Water");

	InitGL();

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(Keyboard);

	glutMouseFunc(Mouse);
	glutMotionFunc(mouseMotion);

	glutMainLoop();

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
