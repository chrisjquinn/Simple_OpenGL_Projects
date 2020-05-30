#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <stddef.h>
#include <iostream>

#include <cmath>

// values controlled by fast keys
float g_angle = 0.0f;
float g_xoffset = 0.0f;
float g_yoffset = 0.0f;

// increments
const float g_angle_step = 10.0f; // degrees
const float g_offset_step = 10.0f; // world coord units

// so we can install and de-install an idle handler
bool g_idle_installed = false;

// handle for display list
unsigned int g_the_square = 0;

//random angle, recalled when needed
float rand_angle = float(drand48() * 360);

unsigned int make_square()
{
	static float vertex[4][2] =
		{
			{-1.0f, -1.0f},
			{1.0f, -1.0f},
			{1.0f, 1.0f},
			{-1.0f,1.0f}
		};

	// request a single display list handle
	unsigned int handle = glGenLists(1); 

	glNewList(handle, GL_COMPILE);

	glBegin(GL_LINE_LOOP); 
		for (size_t i=0;i<4;i++)
			glVertex2fv(vertex[i]);
	glEnd();
	glEndList();

	return handle;
}

void checkCollision(){
	//This will check if the square is going out of the edges of the window. Note it is glitchy
	//as it will get stuck in a rut picking random angles and moving around out of the window.

	
	//400 is used due to the square being 100x100
	if(g_xoffset <= -400.0f){
		//pick a new random angle between 0 and 90
		rand_angle = (float(drand48() * 90));
		g_xoffset = -399.9f;
		//rand_angle = (float(drand48() * 90) || float((drand48() * 90) + 270));
		std::cerr << "\t collision "<< std::endl;
	}
	else if (g_xoffset >= 400.0f) {
		//pick a new random angle between 90 and 180
		rand_angle = (float(drand48() * 90) +90);
		g_xoffset = 399.9f;
		//rand_angle = (float((drand48() * 90) + 90) || float((drand48() * 90) + 180));
		std::cerr << "\t collision "<< std::endl;
	}
	else if (g_yoffset <= -400.0f) {
		//pick a new random angle between 0 and 90
		rand_angle = (float(drand48() * 90));
		g_yoffset = -399.9f;
		//rand_angle = (float((drand48() * 90)) || float((drand48() * 90) + 90));
		std::cerr << "\t collision "<< std::endl;
	}
	else if (g_yoffset >= 400.0f) {
		//pick a new random angle between 180 and 270
		rand_angle = float((drand48() * 90)+180);
		g_yoffset = 399.9f;

		//rand_angle = (float((drand48() * 90) + 180) || float((drand48() * 90) + 270));
		std::cerr << "\t collision "<< std::endl;
	}
}

// our idle handler
void idle()
{
	std::cerr << "\t idle handler called..." << std::endl;
	std::cerr << "\t offset x: "<< g_xoffset <<", y: " << g_yoffset << std::endl;
	//do the bouncing incrememnt calculations here, window 1000 x 1000
	checkCollision();
	std::cerr << "\t random angle is " << rand_angle << std::endl;

	float direction_speed = 10.0f;
	if(rand_angle <= 90){
		g_xoffset += direction_speed*cos(rand_angle);
		g_yoffset += direction_speed*sin(rand_angle);
	}
	else if (rand_angle <= 180){
		g_xoffset -= direction_speed*cos(180 - rand_angle);
		g_yoffset += direction_speed*sin(180 - rand_angle);
	}
	else if (rand_angle <= 270){
		g_xoffset -= direction_speed*cos(270 - rand_angle);
		g_yoffset -= direction_speed*sin(270 - rand_angle);
	}
	else {
		g_xoffset += direction_speed*cos(360 - rand_angle);
		g_yoffset -= direction_speed*sin(360 - rand_angle);
	}
	

	glutPostRedisplay(); // uncomment if you change any drawing state
}

void visibility(int vis)
{
	if (vis==GLUT_VISIBLE)
	{
		std::cerr << "\t visible" << std::endl;
		if (g_idle_installed)
		{
			glutIdleFunc(idle);
		}
	}
	else
	{
		std::cerr << "\t NOT visible" << std::endl;
		glutIdleFunc(NULL);
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); 
	glColor3f(1.0f, 1.0f, 1.0f); 
	glLineWidth(2.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslatef(500.0f+g_xoffset, 500.0f+g_yoffset, 0.0f);
		glScalef(100.0f, 100.0f, 1.0f);
		glRotatef(g_angle, 0.0f, 0.0f, 1.0f); 
		
		// execute a pre-compiled display list
		glCallList(g_the_square);

	glPopMatrix(); // done with stack
	glutSwapBuffers(); 
}

// will get which key was pressed and x and y positions if required
void keyboard(unsigned char key, int, int)
{
	std::cerr << "\t you pressed the " << key << " key" << std::endl;

	switch (key)
	{
		case 'q': exit(1); // quit!

		// clockwise rotate
		case 'r': g_angle += g_angle_step; break;

		case 'a':
				if (!g_idle_installed)
				{
					glutIdleFunc(idle);
					g_idle_installed = true;
				}
				else
				{
					glutIdleFunc(NULL);
					g_idle_installed = false;
				}
	}

	glutPostRedisplay(); // force a redraw
}

// any special key pressed like arrow keys
void special(int key, int, int)
{
	// handle special keys
	switch (key)
	{
		case GLUT_KEY_LEFT: g_xoffset -= g_offset_step; break;
		case GLUT_KEY_RIGHT: g_xoffset += g_offset_step; break;
		case GLUT_KEY_UP: g_yoffset += g_offset_step; break;
		case GLUT_KEY_DOWN: g_yoffset -= g_offset_step; break;
	}

	glutPostRedisplay(); // force a redraw
}

void init()
{
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 1000);
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f); 

	// make square display list
	g_the_square = make_square();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA); 
	glutInitWindowSize(512, 512); 
	glutInitWindowPosition(50, 50); 
	glutCreateWindow("Idle Test"); 
	glutDisplayFunc(display); 

	// handlers for keyboard input
	glutKeyboardFunc(keyboard); 
	glutSpecialFunc(special); 

	// visibility
	glutVisibilityFunc(visibility); 

	init(); 
	glutMainLoop(); 

	return 0; 
}
