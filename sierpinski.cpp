#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <stdlib.h>
#include <stddef.h>
#include <math.h>

void draw_triangle(float x, float y, float size)
{
	// in model cooridnates centred at (0,0)
	//size denotes a length of an edge of the equilteral triangle
	float height = (float) size * (sqrt(3) / 2);
	float vertex[3][2] =
		{
			{x , y  + (height/2)},
			{x + (size/2), y - (height/2)},
			{x - (size/2), y - (height/2)}
		};

	glBegin(GL_TRIANGLES); 
		for (size_t i=0;i<3;i++)
			glVertex2fv(vertex[i]);
	glEnd();

    
}

void draw_upsidedown_triangle(float x, float y, float size)
{
	//Same as the previous method, but the triangle is upside down
	float height = (float) size * (sqrt(3) / 2);
	float vertex[3][2] =
		{
			{x +(size/2), y  + (height/2)},
			{x , y - (height/2)},
			{x - (size/2), y + (height/2)}
		};

	glBegin(GL_TRIANGLES); 
		for (size_t i=0;i<3;i++)
			glVertex2fv(vertex[i]);
	glEnd();

}


void fractalTriangle(float x, float y, float size, int depth){
	glColor3f(1.0f,0.0f,0.0f);
	draw_triangle(x,y,size);
	float height = (float) size * (sqrt(3) /2);
	glColor3f(1.0f,1.0f,1.0f);
	draw_upsidedown_triangle(x,y - height/4,size/2);
	if(depth > 1){
		fractalTriangle(x, y + height/4, size/2, depth-1);
		fractalTriangle(x + size/4, y - height/4, size/2, depth-1);
		fractalTriangle(x - size/4, y - height/4, size/2, depth-1);
	}
}



void display()
{
	glClear(GL_COLOR_BUFFER_BIT); 
	
	glColor3f(1.0f, 0.0f, 0.0f);

	fractalTriangle(0,0,2,7);

	glutSwapBuffers(); 
}

void init()
{
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluOrtho2D(-1, 1, -(sqrt(3)/2), (sqrt(3)/2));
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); 
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA); 
	glutInitWindowSize(800 * (2*sqrt(3) / 3), 800); 
	glutInitWindowPosition(50, 50); 
	glutCreateWindow("Sierpinski"); 
	glutDisplayFunc(display); 

	init(); 
	glutMainLoop(); 

	return 0; 
}
