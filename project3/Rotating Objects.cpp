/*********
CTIS164 - Template Source Program
----------
STUDENT :serkan	-akbulut
SECTION :
HOMEWORK:shooting busses-
----------
PROBLEMS: If your program does not function correctly,
explain here which parts are not running.
hocam cpp actýgýmýzda çalýsmýyor sanýrým tam anlamadým kopyalayýp visual da yeni proje acýp ona yapýþtýrdýgýmýzda
çalýsýyor program.
*********/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  50 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer
#define SIZE 5
#define D2R 0.0174532

/* Global Variables for Template File */

bool timer = false;
bool vurmak = false;
int oyunDurum = 1;
int sure = 20;
int azaltici = 0;
int sayici = 0;
int burstbusses;
int totalbusses;
int totalpoint;
int lastpoint;
int roketx, rokety;
int  winWidth, winHeight; // current Window width and height

typedef struct {
	int x, y = 50;
	bool gorulme = true;

}buss_t;

buss_t busses[SIZE];

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
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
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

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
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

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	roketx = 0;
	rokety = 370;
	sure = 20;
	azaltici = 0;
	oyunDurum = 1;
	sayici = 0;
	timer = false;
	burstbusses = 0;
	totalbusses = 0;
	lastpoint = 0;
	totalpoint = 0;

	int i, b = -455;
	for (i = 0; i < SIZE; i++)
	{
		busses[i].x = b;
		b -= 130;
		busses[i].gorulme = true;
	}

	busses[0].y -= 62;
	busses[4].y -= 128;
	busses[1].y -= 194;
	busses[3].y -= 260;
	busses[2].y -= 326;
}

void drawGradient(int x1, int y1, int w, int h, float r, float g, float b) {
	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	glVertex2f(x1, y1);
	glVertex2f(x1 + w, y1);
	glColor3f(r + 0.4, g + 0.4, b + 0.4);
	glVertex2f(x1 + w, y1 - h);
	glVertex2f(x1, y1 - h);
	glEnd();
	glColor3f(0.1, 0.1, 0.1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x1, y1);
	glVertex2f(x1 + w, y1);
	glVertex2f(x1 + w, y1 - h);
	glVertex2f(x1, y1 - h);
	glEnd();
}

//
// To display onto window using OpenGL commands
//
void display() {
	// label
	glClearColor(0.8, 0.8, 0.8, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);




	drawGradient(-400, 400, 800, 800, 0.4, 0.4, 1);
	glColor3ub(128, 128, 128);
	glRectf(-400, 150, 400, -400);
	glColor3ub(127, 0, 255);
	glRectf(-15, 400, 30, 377.5);

	//roket ateþ
	glColor3ub(255, 128, 0);
	circle(7.5 + roketx, rokety, 8);

	//roket
	glColor3ub(64, 64, 64);
	glRectf(roketx, rokety - 50, 15 + roketx, rokety);
	//roket ucu
	glBegin(GL_TRIANGLES);
	glColor3ub(255, 51, 51);
	glVertex2f(0 + roketx, rokety - 50);
	glVertex2f(15 + roketx, rokety - 50);
	glVertex2f(7.5 + roketx, rokety - 60);
	glEnd();
	//yan kanatlar roket
	glBegin(GL_TRIANGLES);
	glColor3ub(96, 96, 96);
	glVertex2f(0 + roketx, rokety);
	glVertex2f(-10 + roketx, rokety);
	glVertex2f(0 + roketx, rokety - 20);
	glEnd();
	//yan kanat roket
	glBegin(GL_TRIANGLES);
	glColor3ub(96, 96, 96);
	glVertex2f(15 + roketx, rokety);
	glVertex2f(15 + roketx, rokety - 20);
	glVertex2f(25 + roketx, rokety);
	glEnd();



	//vprint(-200, 360, GLUT_BITMAP_9_BY_15, "  WELCOME TO MY BUSS RACE PRESS F1 TO START AGAIN ");

	/*glColor3ub(51, 255, 51);
	glRectf(250, 250, 350, 290);
	glColor3ub(153, 0, 0);
	vprint(270, 320, GLUT_BITMAP_9_BY_15, "WINNER");
	vprint(295, 265, GLUT_BITMAP_9_BY_15, "%d", win + 1);*/

	int i;


	for (i = 0; i < SIZE; i++)
	{
		if (busses[i].gorulme == true)
		{
			//WHEELS
			glColor3ub(204, 255, 204);
			circle(-25 + busses[i].x, 5 + busses[i].y, -8);
			circle(25 + busses[i].x, 5 + busses[i].y, -8);
			//body
			glColor3ub(204, 204, 255);
			glRectf(-50 + busses[i].x, 9 + busses[i].y, 50 + busses[i].x, 59 + busses[i].y);
			//window
			glColor3ub(0, 122, 122);
			glRectf(-40 + busses[i].x, 30 + busses[i].y, -30 + busses[i].x, 40 + busses[i].y);
			glRectf(-20 + busses[i].x, 30 + busses[i].y, -10 + busses[i].x, 40 + busses[i].y);
			glRectf(0 + busses[i].x, 30 + busses[i].y, 10 + busses[i].x, 40 + busses[i].y);
			glRectf(20 + busses[i].x, 30 + busses[i].y, 30 + busses[i].x, 40 + busses[i].y);

			//egzoz
			glColor3f(0, 0, 0);
			glRectf(-60 + busses[i].x, 14 + busses[i].y, -50 + busses[i].x, 18 + busses[i].y);
		}
	}
	glColor3ub(0, 0, 0);
	glRectf(250, 400, 400, -400);
	glColor3ub(255, 255, 102);
	vprint(270, 320, GLUT_BITMAP_9_BY_15, "TOTAL BUSSES");
	glColor3f(1, 1, 1);
	vprint(300, 305, GLUT_BITMAP_9_BY_15, "%02d", totalbusses);
	glColor3ub(255, 255, 102);
	vprint(270, 160, GLUT_BITMAP_9_BY_15, "BURST BUSSES");
	glColor3f(1, 1, 1);
	vprint(300, 145, GLUT_BITMAP_9_BY_15, "%02d", burstbusses);
	glColor3ub(255, 255, 102);
	vprint(270, 0, GLUT_BITMAP_9_BY_15, "LAST POINT");
	glColor3f(1, 1, 1);
	vprint(300, -15, GLUT_BITMAP_9_BY_15, "%02d", lastpoint);
	glColor3ub(255, 255, 102);
	vprint(270, -160, GLUT_BITMAP_9_BY_15, "REMANING TIME");
	glColor3f(1, 1, 1);
	vprint(300, -175, GLUT_BITMAP_9_BY_15, "%02d:%02d", sure, azaltici);
	glColor3ub(255, 255, 102);
	vprint(270, -320, GLUT_BITMAP_9_BY_15, "TOTAL POINTS");
	glColor3f(1, 1, 1);
	vprint(300, -335, GLUT_BITMAP_9_BY_15, "%02d", totalpoint);

	glColor3ub(255, 255, 0);
	glRectf(-400, -350, 400, -400);
	glColor3ub(255, 0, 0);
	vprint(-300, -375, GLUT_BITMAP_9_BY_15, "->TO RÝGHT,<- TO LEFT,<SPACEBAR> TO FIRE, PRESS <F1> TO PAUSE/RESTART");
	glutSwapBuffers();

}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (oyunDurum == 1 && timer)
	{
		if (key == 32 && vurmak == false)
		{
			vurmak = !vurmak;
		}
	}


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
	// Write your codes here.
	if (oyunDurum == 1 && key == GLUT_KEY_F1)
		timer = !timer;

	if (timer&&vurmak)
	{
		if (key == GLUT_KEY_RIGHT)
			roketx += 5;
		else if (key == GLUT_KEY_LEFT)
			roketx -= 5;
	}

	if (oyunDurum == 0 && key == GLUT_KEY_F1)
	{
		oyunDurum = 1;
		Init();
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

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
	int i;
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	if (oyunDurum == 1 && timer)
	{
		if (sure > 0 || azaltici != 0)
		{
			if (sayici % 20 == 0)
			{
				sure--;
				azaltici = 100;
			}
			sayici++;
			azaltici = azaltici - 5;

			for (i = 0; i < SIZE; i++)
				if (busses[i].x + 50 == -395)
					totalbusses++;
			for (i = 0; i < SIZE; i++)
			{
				if (busses[i].x + 50 < 315)
					busses[i].x += 10;

				else
				{
					busses[i].x = -455;
					busses[i].gorulme = true;
				}
			}
			if (vurmak)
			{
				if (rokety - 60 > -380)
				{
					rokety -= 10;
					for (i = 0; i < SIZE; i++)
					{
						if (busses[i].gorulme == true)
						{
							if (roketx - 10 >= busses[i].x - 45 && roketx + 25 <= busses[i].x + 45 && rokety - 60 <= busses[i].y + 59)
							{
								burstbusses++;
								if (roketx - 10 >= busses[i].x - 45 && roketx + 25 < busses[i].x)
									lastpoint += 3;
								else if (roketx - 10 >= busses[i].x  && roketx + 25 <= busses[i].x + 45)
									lastpoint += 5;
								totalpoint += lastpoint;
								busses[i].gorulme = false;
							}

						}
					}
				}
				else
				{
					rokety = 370;
					roketx = 0;
					vurmak = !vurmak;
				}
			}
		}
		else
		{
			timer = !timer;
			oyunDurum = 0;
		}

	}



	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif



void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("HW-2:\"SHOOTING THE BUSSES\" BY SERKAN AKBULUT");
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