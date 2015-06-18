#include <stdio.h>
#include <math.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include "Snow.h"

#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 600
#define VIEW_SIZE 600

static float fElect1 = 0.0;
static float pos = 0.0;

static float rotate_angle_x = 0.0f;
static float rotate_angle_y = 0.0f;
static float rotate_angle_z = 0.0f;

void display1(){
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	//把原点移动到左下
	glTranslatef(-VIEW_SIZE, -VIEW_SIZE, 0);
	//旋转一下
	glRotatef(rotate_angle_x, 1.0f, 0.0f, 0.0f);
	glRotatef(rotate_angle_y, 0.0f, 1.0f, 0.0f);
	glRotatef(rotate_angle_z, 0.0f, 0.0f, 1.0f);

	drawBackground();
	drawSnows();

	glutSwapBuffers();
}

void SetupRc(){
	glEnable(GL_TEXTURE_2D);   // 启用二维纹理
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor(196/255.0, 214/255.0f, 234/255.0f, 1.0f);
	initSnowState(WINDOW_SIZE_X, WINDOW_SIZE_Y, VIEW_SIZE);
}

void changeSize(int w, int h){
	if(h == 0)
		h = 1;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(-VIEW_SIZE, VIEW_SIZE, -VIEW_SIZE, VIEW_SIZE, -VIEW_SIZE, VIEW_SIZE);
	gluPerspective(70.0f, w* 1.0f / h, 1.0f, VIEW_SIZE );
	//gluOrtho3D(-VIEW_SIZE, VIEW_SIZE, -VIEW_SIZE, VIEW_SIZE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void timerFun(int unused){
	changeSnowState();
	glutTimerFunc(10,timerFun, 1);
	glutPostRedisplay();
}


// Respond to arrow keys by moving the camera frame of reference
void keyboardFun(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'x':
		rotate_angle_x = rotate_angle_x + 1;
		break;
	case 'y':
		rotate_angle_y = rotate_angle_y + 1;
		break;
	case 'z':
		rotate_angle_z = rotate_angle_z + 1;
		break;
	default:
		break;
	}
	//printf("\n the pos x: %f y: %f z: %f", rotate_angle_x, rotate_angle_y, rotate_angle_z);
	// Refresh the Window
	glutPostRedisplay();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("ͼԪ");
	glutDisplayFunc(display1);
	glutReshapeWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	glutReshapeFunc(changeSize);
	glutTimerFunc(10,timerFun,1);
	glutKeyboardFunc(keyboardFun);
	SetupRc();

	glutMainLoop();
	return 0;
}
