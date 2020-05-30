#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <stdlib.h>
#include <stddef.h>




void drawSquare(float x, float y, float size){
	float vertex[4][2] = {
		{x + (size)/2, y + (size)/2},
		{x + (size)/2, y - (size)/2},
		{x - (size)/2, y - (size)/2},
		{x - (size)/2, y + (size)/2}
	};
	glBegin(GL_LINE_LOOP);
		for(size_t i = 0; i < 4; i++){
			glVertex2fv(vertex[i]);
		}
	glEnd();
}

void fractalSqaure(float x, float y, float size, int depth) {
	drawSquare(x,y,size);
	if (depth > 1){
		//this does the upper, lower, side and side. The others also then do 4, which is
		//technically wrong. 
		fractalSqaure(x, (y + (size)/2 + (size)/4), size/2, depth -1);
		fractalSqaure(x + ((size)/2 + (size)/4), y , size/2, depth -1);
		fractalSqaure(x, y - (size)/2 - (size)/4, size/2, depth -1);
		fractalSqaure(x - (size)/2 - (size)/4, y , size/2, depth -1);
	}

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); 
	
	glColor3f(1.0f, 1.0f, 1.0f);

	// work on MODELVIEW matrix stack
	glMatrixMode(GL_MODELVIEW);


	glPushMatrix();
	glScalef(300.0f,300.f,0.0f);
	fractalSqaure(0,0,1,7);
	glPopMatrix();



	glutSwapBuffers(); 
}

void init()
{
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluOrtho2D(-500, 500, -500, 500);
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f); 
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA); 
	glutInitWindowSize(750, 750); 
	glutInitWindowPosition(50, 50); 
	glutCreateWindow("Rotate"); 
	glutDisplayFunc(display); 

	init(); 
	glutMainLoop(); 

	return 0; 
}
