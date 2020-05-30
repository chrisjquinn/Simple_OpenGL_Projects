#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <stdlib.h>
#include <stddef.h>

#include <unistd.h>


const int number_of_iterations = 500000; //500,000
int current_iteration = 0;



void display()
{

	glClear(GL_COLOR_BUFFER_BIT); //clear the screen

	double r;
	double x;
	double y;
	float coord[2];

	coord[0] = drand48();
	coord[1] = drand48();

	for(int i = 0; i < number_of_iterations; i++){
		r = drand48();

		if(r < 0.85){
			x =   0.85 * coord[0] + 0.04 * coord[1] + 0.0;
			y = - 0.04 * coord[0] + 0.85 * coord[1] + 1.6;
		}
		else if (r < 0.92){
			x =   0.20 * coord[0] - 0.26 * coord[1] + 0.0;
			y =   0.23 * coord[0] + 0.22 * coord[1] + 1.6;
		}
		else if(r < 0.99){
			x = - 0.15 * coord[0] + 0.28 * coord[1] + 0.0;
			y =   0.26 * coord[1] + 0.24 * coord[1] + 0.44;
		}
		else{
			x =   0.0;
			y =                     0.16 * coord[1];
		}


		coord[0] = x;
		coord[1] = y;

		glBegin(GL_POINTS);
			glVertex2fv(coord);
		glEnd();
 
	}

	//Clear all buffers
	glFlush();
	//Swap the buffers
	// glutSwapBuffers(); 
}

void init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); //White
	glColor3f(0.133f, 0.545f, 0.133f); //Forest Green

	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluOrtho2D(-4, 4, -1, 11);

	glMatrixMode(GL_MODELVIEW);

	return;

}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA); 
	glutInitWindowSize(1000, 1000); 
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("BarnsleyFern"); 
	glutDisplayFunc(display); 

	init(); 
	glutMainLoop(); 

	return 0; 
}
