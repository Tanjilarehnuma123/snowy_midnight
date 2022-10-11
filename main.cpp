#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#define ESC 27
#define SPACE 32

int width = 640, height = 480;

int depth = 0, circleRadius = 2, cnt = 1;


float vdata[4][3] = {
	{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {-1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}
};

void normalize(float v[3]){

	float d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

	for (int i = 0; i < 3; i++) {
		v[i] /= d;
	}
}

void drawtriangle(float *v1, float *v2, float *v3){

	glBegin(GL_TRIANGLES);
	glVertex3fv(v1);
	glVertex3fv(v2);
	glVertex3fv(v3);
	glEnd();
}

void subdivideCircle(int radius, float *v1, float *v2, int depth){

	float v11[3], v22[3], v00[3] = { 0, 0, 0 }, v12[3];
	int i;

	if (depth == 0){
		glColor3f(v1[0] * v1[0], v1[1] * v1[1], v1[2] * v1[2]);

		for (i = 0; i < 3; i++){
			v11[i] = v1[i] * radius;
			v22[i] = v2[i] * radius;
		}
		drawtriangle(v11, v22, v00);

		return;
	}

	// V12 is in the direction of (v1 + v2)
	v12[0] = v1[0] + v2[0];
	v12[1] = v1[1] + v2[1];
	v12[2] = v1[2] + v2[2];

	normalize(v12);

	subdivideCircle(radius, v1, v12, depth - 1);
	subdivideCircle(radius, v12, v2, depth - 1);
}

void drawCircle(int circleRadius){	// draw a circle with center at the origin in xy plane

	// The 4 triangles
	subdivideCircle(circleRadius, vdata[0], vdata[1], depth);
	subdivideCircle(circleRadius, vdata[1], vdata[2], depth);
	subdivideCircle(circleRadius, vdata[2], vdata[3], depth);
	subdivideCircle(circleRadius, vdata[3], vdata[0], depth);
}

void display(void){

	if (circleRadius > width / 2 || circleRadius == 1){
		cnt = -cnt;
		depth++;
		depth = depth % 5;
	}

	circleRadius += cnt;

	glClear(GL_COLOR_BUFFER_BIT);
	drawCircle(circleRadius);

	glutSwapBuffers();
}

static void Reshape(int w, int h){

	glClearColor(0.019, 0.125, 0.258, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	width = w; height = h;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width / 2, width / 2, -height / 2, height / 2, -1.0, 1.0);
}

static void Key(unsigned char key, int x, int y){

	switch (key){
	case ESC:
		exit(0);
	case SPACE:
		glutIdleFunc(NULL);
		display();
		break;
	}
}

int main(int argc, char **argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Press ESC to exit or Keep pressing SPACE to control the movement manually");

	glutKeyboardFunc(Key);
	glutReshapeFunc(Reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);

	glutMainLoop();
}
