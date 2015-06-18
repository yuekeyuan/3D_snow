#ifndef __SNOW_H
#define __SNOW_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <gl/glut.h>
#include <gl/glext.h>
#include <gl/gltools.h>

#define  PI 3.141592653
#define SNOW_NUMBER 1000

//这样做有点高耦合性，改一下
//#define WINDOW_SIZE_X 800
//#define WINDOW_SIZE_Y 600
//#define VIEW_SIZE 600
static float window_size_x = 0.0f;
static float window_size_y = 0.0f;
static float view_size      = 0.0f;

static GLuint backgroundTieTu = NULL;

void restSnowState(struct  snowState &state);
void snow(float size, GLfloat width1, GLfloat width2, GLfloat width3);
bool LoadTexture(LPSTR szFileName, GLuint &texid);
void drawBackground();

//定义一个雪花的各种信息
typedef struct  snowState
{
	int id;
	//雪花的类型
	int type;
	//当前位置
	float x;
	float y;
	float z;
	//雪花的偏移方向
	float direction_x;
	float direction_y;
	float direction_z;
	//雪花的自旋方向
	float rotate_x;
	float rotate_y;
	float rotate_z;
	//雪花的单位旋转量
	float rotate_vector_x;
	float rotate_vector_y;
	float rotate_vector_z;
	//雪花的大小信息
	float size;
	float width1;
	float width2;
	float width3;
	//其他信息
	float color[4];
} snowState;

snowState snowstate[SNOW_NUMBER];

//初始化所有雪花的状态
void initSnowState(float windowSizeX, float windowSizeY, float viewSize){
		
	window_size_x = windowSizeX;
	window_size_y = windowSizeY;
	view_size     = viewSize * 2;

	srand( (unsigned int)time(0) );
	for (int i = 0; i < SNOW_NUMBER; i++){
		snowstate[i].id = i;
		snowstate[i].type = rand();
		
		snowstate[i].x = rand() % ((int)view_size);
		snowstate[i].y = rand() % ((int)view_size) + view_size;
		snowstate[i].z = rand() % ((int)100) - 200;

		
		//方向因素控制在 [0,2] 之间的数字
		//这个好烦，雪花到底该往哪儿飘？
		snowstate[i].direction_x = (rand() % 1000)/1000.0f -0.5f;  //x方向偏移量是 在 [-0.5, 0.5] 之间
		snowstate[i].direction_y = (rand() % 2000)/1000.0f + 1.0f; //保证它下飘    在 [1, 3] 之间
		snowstate[i].direction_z = (rand() % 1000)/1000.0f - 0.5f; //z方向偏移量是 在 [-0.5, 0.5] 之间
		
		snowstate[i].rotate_vector_x = (rand() % 3000)/1000.0f -1.5f;
		snowstate[i].rotate_vector_x = (rand() % 3000)/1000.0f -1.5f;
		snowstate[i].rotate_vector_x = (rand() % 3000)/1000.0f -1.5f;

		snowstate[i].width1 = rand();
		snowstate[i].width2 = rand();
		snowstate[i].width3 = rand();

		snowstate[i].color[0] = 1;
	}

	//初始化贴图文件
	LoadTexture("back2.bmp", backgroundTieTu);
}

//改变每一朵雪花的状态
void changeSnowState(){

	for(int i=0;i<SNOW_NUMBER;i++){
		
		//改变自旋状态信息
		snowstate[i].rotate_x += snowstate[i].rotate_vector_x;
		snowstate[i].rotate_y += snowstate[i].rotate_vector_y;
		snowstate[i].rotate_z += snowstate[i].rotate_vector_z;

		//改变位置的状态信息, 当雪花到达底部的时候重新生成位置信息
		snowstate[i].x += snowstate[i].direction_x;
		if(snowstate[i].x < -20  || snowstate[i].x >= view_size){
			restSnowState(snowstate[i]);
		}

		snowstate[i].y -= snowstate[i].direction_y;
		
		/*
		if(snowstate[i].y < -20){
			restSnowState(snowstate[i]);
		}
		*/

		snowstate[i].z -= snowstate[i].direction_z;
		/*
		if(snowstate[i].z < -100 || snowstate[i].z > 0){
			restSnowState(snowstate[i]);
		}
		*/
	
		//改变雪花本身的方向信息。

		//改变雪花的矢量信息
		//printf("snow state pos: (%f, %f, %f)\n", snowstate[i].x, snowstate[i].y, snowstate[i].z);
	}
	
}

//目前只设置 位置信息
void restSnowState(struct  snowState &state){
	state.x = rand() % ((int)view_size);
	state.y = rand() % ((int)view_size) + view_size;
	state.z = rand() % ((int)100) - 200/ 2.0f;

	//方向因素控制在 [0,2] 之间的数字
	//这个好烦，雪花到底该往哪儿飘？
	
	//state.direction_x = (rand() % 1000)/1000.0f -0.5f;  //x方向偏移量是 在 [-0.5, 0.5] 之间
	//state.direction_y = (rand() % 2000)/1000.0f + 1.0f; //保证它下飘    在 [1, 3] 之间
	//state.direction_z = (rand() % 1000)/1000.0f - 0.5f;

	//state.width1 = rand();
	//state.width2 = rand();
	//state.width3 = rand();

	//state.color[0] = 1;
}

//根据snow的信息，绘制所有的雪花
void drawSnows(){
	//在这个时候绘制背景图层
	for(int i=0;i<SNOW_NUMBER;i++){
		glTranslatef(snowstate[i].x, snowstate[i].y, snowstate[i].z);
		glRotatef(snowstate[i].rotate_x, 1,0,0);
		glRotatef(snowstate[i].rotate_y, 0,1,0);
		glRotatef(snowstate[i].rotate_z, 0,0,1);
		snow(10,5,3,2);
		glRotatef(-snowstate[i].rotate_x, 1,0,0);
		glRotatef(-snowstate[i].rotate_y, 0,1,0);
		glRotatef(-snowstate[i].rotate_z, 0,0,1);
		glTranslatef(-snowstate[i].x, -snowstate[i].y, -snowstate[i].z);
	}
}

//绘制我们的背景图形
void drawBackground(){
	glBindTexture(GL_TEXTURE_2D, backgroundTieTu);
	glBegin(GL_QUADS);
		glTranslatef(-100,-100,0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0,  -view_size/2.0f); // 纹理和四边形的左下
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0, view_size , -view_size/2.0f); // 纹理和四边形的右下
		glTexCoord2f(1.0f, 1.0f); glVertex3f(view_size, view_size, -view_size/2.0f); // 纹理和四边形的右上
		glTexCoord2f(1.0f, 0.0f); glVertex3f(view_size, 0 , -view_size/2.0f); // 纹理和四边形的左上	
		glTranslatef(100, 100, 0);
	glEnd();
}

//单个的雪花
void snow(float size, GLfloat width1, GLfloat width2, GLfloat width3){
	GLfloat x, y, angle;
	GLfloat xx, xy;
	glBegin(GL_LINES);{
		glColor3f(1.0,1.0,1.0);
		for(float angle=0; angle<=(2* PI + 0.01); angle+=(PI/6)){
			//////////////////////////////////////////////////////////////////////////
			glLineWidth(width1);
			glLineWidth(100);
			glVertex2f(0,0);
			x = size * sin(angle);
			y = size * cos(angle);
			glVertex2f(x, y);
			//////////////////////////////////////////////////////////////////////////
			glLineWidth(width2);
			glVertex2f(x/2, y/2);
			xx = x/2 + (size/2) * sin(angle + PI/6);
			xy = y/2 + (size/2) * cos(angle + PI/6);
			glVertex2f(xx, xy);

			glVertex2f(x/2, y/2);
			xx = x/2 + (size/2) * sin(angle - PI/6);
			xy = y/2 + (size/2) * cos(angle - PI/6);
			glVertex2f(xx, xy);
			//////////////////////////////////////////////////////////////////////////
			glLineWidth(width3);
			glVertex2f(2*x/3, 2*y/3);
			xx = 2*x/3 + (size/3) *sin(angle + PI/6);
			xy = 2*y/3 + (size/3) *cos(angle + PI/6);
			glVertex2f(xx, xy);

			glVertex2f(2*x/3, 2*y/3);
			xx = 2*x/3 + (size/3) *sin(angle - PI/6);
			xy = 2*y/3 + (size/3) *cos(angle - PI/6);
			glVertex2f(xx, xy);
		}
	}glEnd();
}

//使用贴图
bool LoadTexture(LPSTR szFileName, GLuint &texid)      // Creates Texture From A Bitmap File
{
	HBITMAP hBMP;              // Handle Of The Bitmap
	BITMAP BMP;              // Bitmap Structure
	glGenTextures(1, &texid);           // Create The Texture
	hBMP=(HBITMAP)LoadImage(GetModuleHandle(NULL), szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );
	if (!hBMP)               // Does The Bitmap Exist?
		return FALSE;             // If Not Return False

	GetObject(hBMP, sizeof(BMP), &BMP);         // Get The Object
	// hBMP:        Handle To Graphics Object
	// sizeof(BMP): Size Of Buffer For Object Information
	// &BMP:        Buffer For Object Information

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);        // Pixel Storage Mode (Word Alignment / 4 Bytes)

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texid);         // Bind To The Texture ID
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear Min Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear Mag Filter
	glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	DeleteObject(hBMP);             // Delete The Object

	return TRUE;              // Loading Was Successful
}

#endif 
