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
    
    // This is to draw a variant on the squares, this is such that there are
    // squares within each other, but also slighly changed by angle.
    
    // draw stuff -- into the backbuffer
    
    const float pi = atan(1)*4; // define pi

    const size_t steps = 4; // how many steps once round the circle
    const float theta_step = 2*pi/float(steps); // angle step in radians

    float radius = 1;
    for(float j = 0.0; j < radius ; j += 0.1){
    glBegin(GL_QUADS);
        if (j == 0.0){
            radius = 1;
        }
        else {
            radius -= 0.1;
        }
        float theta = 0.0f;
        glColor3f(1.0f,1.0f,1.0f);
        for (size_t i=0;i<steps;i++, theta+=theta_step)
        {
            float x = radius * cos(theta);
            float y = radius * sin(theta);
            glVertex2f(x, y);
            //std::cout << "" << x << "f, " << y << "f" <<std::endl;
            
        }
        radius -= 0.1;
        glColor3f(0.0f,0.0f,0.0f);
        for (size_t i=0;i<steps;i++, theta+=theta_step + 0.05)
        {
            float x = radius * cos(theta);
            float y = radius * sin(theta);
            glVertex2f(x, y);
            std::cout << "" << x << "f, " << y << "f" <<std::endl;
        }
    glEnd();
    }
    
    
    
    
    
    // Below is for squares within themselves.
    
    
    
//    glBegin(GL_QUADS);
//        //for the red squares
//        for (float i=0; i<1.0; i += 0.2)
//        {
//            glColor3f(0.0f, 0.0f, 0.0f); //should be black
//            glVertex2f(first_x - i, first_y - i);
//            glVertex2f(first_x - i, -first_y + i);
//            glVertex2f(-first_x + i,-first_y + i);
//            glVertex2f(-first_x + i , first_y - i);
//
//            glColor3f(1.0f,1.0f,1.0f); //should be white
//            glVertex2f(first_x - (i+0.1), first_y - (i+0.1));
//            glVertex2f(first_x - (i+0.1), -first_y + (i+0.1));
//            glVertex2f(-first_x + (i+0.1),-first_y + (i+0.1));
//            glVertex2f(-first_x + (i+0.1) , first_y - (i+0.1));
//
//
//
//            // debug to standard out
//            //std::cout << "" << x << "f, " << y << "f" <<std::endl;
//        }
//    glEnd();

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
