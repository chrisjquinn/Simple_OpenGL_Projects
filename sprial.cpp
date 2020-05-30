#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//added - CQ
#include <cmath>
#include <iostream>

// redraw callback
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    // draw stuff -- into the backbuffer
    glColor3f(1.0f, 0.0f, 0.0f); //should be red

    //glColor3f(1.0f, 1.0f, 1.0f); // white
    glLineWidth(2.0f); // thick
    
    float first_x = 0.7f;
    float first_y = 0.7f;
    
    float steps = 0.7; //number of spirals you want to do.
    //want to create a spiral (square) that starts at 0.7,0.7 and then spirals inwards.
    glBegin(GL_LINE_STRIP);
        for (float i=0; i<steps; i += 0.1)
        {
            glVertex2f(first_x - i, first_y - i);
            glVertex2f(first_x - i, -first_y + i);
            glVertex2f(-first_x + i,-first_y + i);
            glVertex2f(-first_x + i , first_y - (i+0.1));
            

            // debug to standard out
            //std::cout << "" << x << "f, " << y << "f" <<std::endl;
        }
    glEnd();

    glutSwapBuffers(); // swap the backbuffer with the front
}


//
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    
    // set the GL display mode: double buffering and RGBA colour mode
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA); // flags bitwise OR'd together
    
    glutInitWindowSize(512, 512); // window size
    glutInitWindowPosition(50, 50); // where on screen

    glutCreateWindow("Double Buffer");

    glutDisplayFunc(display); // set display callback

    glClearColor(0.0f, 1.0f, 0.0f, 0.0f); // clear colour -- green!
    
    glutMainLoop(); // go into the main loop and wait for window events...

    return 0;
}
