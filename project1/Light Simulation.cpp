/*********
CTIS164 - Template Source Program
----------
STUDENT :SERKAN AKBULUT
SECTION :
HOMEWORK:LIGHT SITIMULATION H-4
----------
PROBLEMS:hocam isikli toplarin gezegenlere carpinca yon degistirmelerini yapmak istedim ama distance olayini hesaplayamadim tam olarak. bazen carpiyorlar bazen carpmadan yon degistiriyorlar bunu ek olarak yaptim ama tam halledemedim.Bunu nasil cozebilirim daha sonra yardim almak isterim.
----------
ADDITIONAL FEATURES:
*********/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON       1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

typedef struct {
	float r, g, b;
} color_t;

typedef struct {
	vec_t   pos;
	color_t color;
	vec_t   vel;
	bool display;
} light_t;

typedef struct {
	vec_t pos;
	vec_t N;
} vertex_t;

typedef struct {
	color_t color;
	float angle;
	float speed;
	bool direction;
	float radius;
} gezegen_t;
gezegen_t gezegen[4];
#define   MAX 4
bool oyunbaslatma = true;

light_t light[MAX] =
{
	{ { 0, 0 },{ 1, 1, 0 },{ 3,  2 } ,{ true } },
	{ { 200, 100 },{ 1, 1, 1 },{ -2, -1 },{ true } },
	{ { -200, 100 },{ 1, 0, 1 },{ 3, -2 },{ true } },
	{ { 0,0 },{ 1,1,0 },{ 3,-2 },true }

};

color_t mulColor(float k, color_t c) {
	color_t tmp = { k * c.r, k * c.g, k * c.b };
	return tmp;
}

color_t addColor(color_t c1, color_t c2) {
	color_t tmp = { c1.r + c2.r, c1.g + c2.g, c1.b + c2.b };
	return tmp;
}

// To add distance into calculation
// when distance is 0  => its impact is 1.0
// when distance is 350 => impact is 0.0
// Linear impact of distance on light calculation.
double distanceImpact(double d) {

	if (d >= 250)
		return 0;
	return (-1.0 / 350.0) * d + 1.0;
}

color_t calculateColor(light_t source, vertex_t v) {
	vec_t L = subV(source.pos, v.pos);
	vec_t uL = unitV(L);
	float factor = dotP(uL, v.N) * distanceImpact(magV(L));
	return mulColor(factor, source.color);
}


//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

void vprint2(int x, int y, float size, char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

//
// To display onto window using OpenGL commands
//
void gezegencizimi(gezegen_t gezegen, float radius) {

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.3, 0.3, 0.3);
	float x = radius * cos(gezegen.angle * D2R), y = radius * sin(gezegen.angle * D2R);
	glVertex2f(x, y);

	
	
		for (int j = 0; j <= 100; j++) {
			gezegen.angle = 2 * PI*j / 100;

			vec_t v = { x + gezegen.radius*cos(gezegen.angle), y + gezegen.radius*sin(gezegen.angle) };
			vertex_t P = { { v.x, v.y } };

			P.N = unitV(subV({ v.x, v.y }, { x, y }));

			color_t res = { 0, 0, 0 };
			for (int k = 0; k <= 3; k++) {
				if (light[k].display)
					res = addColor(res, calculateColor(light[k], P));
			}

			glColor3f(res.r, res.g, res.b);
			glVertex2f(v.x, v.y);
		}
		glEnd();
	
}
void sifirlama() {
	int i;
	for (i = 0; i < MAX-1; i++)
	{
		light[i].pos.x = (rand() % 400)*((rand() % 2 == 0) ? -1 : 1);
		light[i].pos.y = (rand() % 400)*((rand() % 2 == 0) ? -1 : 1);

		light[i].vel.x = (rand() % 10)*((rand() % 2 == 0) ? -1 : 1);
		light[i].vel.y = (rand() % 10)*((rand() % 2 == 0) ? -1 : 1);


		gezegen[i].radius = rand() % 20 + 10;
		gezegen[i].speed = (rand() % 5 + 2) * ((rand() % 2 == 0) ? -1 : 1);

		gezegen[i].angle = rand() % 361;


	}
}
void display() {

	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// light source 
	
		for (int i = 0; i < MAX; i++) {
			if (light[i].display)
			{
				glColor3f(light[i].color.r, light[i].color.g, light[i].color.b);
				if (i == MAX - 1)
					circle(light[i].pos.x, light[i].pos.y, 60);
				else
					circle(light[i].pos.x, light[i].pos.y, 10);
			}
		}
		for (int x = -400; x <= 400; x++) {
			vertex_t P = { { x, -100 },{ 0, 1 } };
			color_t res = { 0, 0, 0 };
		}
		float radius = 150;
		for (int i = 0; i < 3; i++) {
			gezegencizimi(gezegen[i], radius);
			radius += 50;
		
	}
	glColor3f(1, 1, 1);
	vprint(-375, 365, GLUT_BITMAP_9_BY_15, "HOMEWORK -4-");
	vprint(-375, 340, GLUT_BITMAP_9_BY_15, "by SERKAN AKBULUT");
	vprint(-240, -380, GLUT_BITMAP_HELVETICA_10, "F1(SUN):%s", (light[3].display) ? "ON" : "OFF");
	vprint(-70, -380, GLUT_BITMAP_HELVETICA_10, "F3(White):%s", (light[1].display) ? "ON" : "OFF");
	vprint(-160, -380, GLUT_BITMAP_HELVETICA_10, "F2(Pink):%s", (light[2].display) ? "ON" : "OFF");
	vprint(20, -380, GLUT_BITMAP_HELVETICA_10, "F4(yellow):%s", (light[0].display) ? "ON" : "OFF");
	vprint(110, -380, GLUT_BITMAP_HELVETICA_10, "F5(Anim):%s", (oyunbaslatma) ? "ON" : "OFF");
	vprint(200, -380, GLUT_BITMAP_HELVETICA_10, "F6:Restart");


	

	glutSwapBuffers();

}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}
//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		light[3].display = !light[3].display;
		break;
	case GLUT_KEY_F2:
		light[2].display = !light[2].display;
		break;
	case GLUT_KEY_F3:
		light[1].display = !light[1].display;
		break;
	case GLUT_KEY_F4:
		light[0].display = !light[0].display;
		break;
	case GLUT_KEY_F5:
		oyunbaslatma = !oyunbaslatma;
		break;
	case GLUT_KEY_F6 :
		sifirlama();
		break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	if (oyunbaslatma)
	{	
		for (int i = 0; i < 3; i++)
		{
			gezegen[i].angle += 1;
		}
		/* HOCAM EK OLARAK YAPMAK ISTEDIGIM BOLUM BURASI BURAYI CALISTIRINCA BAHSETTIGIM HATA MEYDANA GELIYOR EK OLARAK YAPMAYA CALISTIM AMA SANIRIM TAM OLMADI BU COMMENTI KALDIRINCA HATAYI GOREBILIRSINIZ HOCAM
		float radius;
		for (int i = 0; i < 3; i++)
		{
			radius = 150;
			float distance = sqrt((light[i].pos.x - radius * cos(gezegen[0].angle * D2R)*cos(gezegen[0].angle * D2R)) * (light[i].pos.x - radius * cos(gezegen[0].angle * D2R)*cos(gezegen[0].angle * D2R)) + (light[i].pos.y - radius * sin(gezegen[0].angle * D2R)*sin(gezegen[0].angle *D2R)) * (light[i].pos.y - radius * sin(gezegen[0].angle * D2R)));
			if (distance <= gezegen[0].radius + 10)
			{
				light[i].vel.x *= -1;
				light[i].vel.y *= -1;
			}
		}
		for (int i = 0; i < 3; i++)
		{
			radius = 200;
			float distance = sqrt((light[i].pos.x - radius * cos(gezegen[1].angle * D2R)*cos(gezegen[1].angle * D2R)) * (light[i].pos.x - radius * cos(gezegen[1].angle * D2R)*cos(gezegen[1].angle * D2R)) + (light[i].pos.y - radius * sin(gezegen[1].angle * D2R)*sin(gezegen[1].angle *D2R)) * (light[i].pos.y - radius * sin(gezegen[1].angle * D2R)));
			if (distance <= gezegen[1].radius + 10)
			{
				light[i].vel.x *= -1;
				light[i].vel.y *= -1;
			}
		}

		for (int i = 0; i < 3; i++)
		{
			radius = 250;
			float distance = sqrt((light[i].pos.x - radius * cos(gezegen[2].angle * D2R)*cos(gezegen[2].angle * D2R)) * (light[i].pos.x - radius * cos(gezegen[2].angle * D2R)*cos(gezegen[2].angle * D2R)) + (light[i].pos.y - radius * sin(gezegen[2].angle * D2R)*sin(gezegen[2].angle *D2R)) * (light[i].pos.y - radius * sin(gezegen[2].angle * D2R)));
			if (distance <= gezegen[2].radius + 10)
			{
				light[i].vel.x *= -1;
				light[i].vel.y *= -1;
			}
		}
		*/
		

		for (int i = 0; i < MAX - 1; i++) {
			light[i].pos = addV(light[i].pos, light[i].vel);

			// Reflection from Walls.
			if (light[i].pos.x > 390 || light[i].pos.x < -390) {
				light[i].vel.x *= -1;
			}

			if (light[i].pos.y > 390 || light[i].pos.y < -390) {
				light[i].vel.y *= -1;
			}
		}
	}
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0; i < 3; i++) {
		gezegen[i].color = { 128 , 128 , 128 };
		gezegen[i].direction = rand() & 1;
		gezegen[i].angle = rand() % 361;
		gezegen[i].speed = rand() % 2 + 5;
		gezegen[i].radius = rand() % 6 + 28;
	}

}

void main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Light Simulation");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}