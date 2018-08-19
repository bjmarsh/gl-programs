#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include "../ComplexClass/complex.cpp"

#ifndef PI
#define PI 3.1415926535898
#endif
#define N 300
#define STATES 4

float t=0;
float expect, uncert;

float freq[STATES] = {7.95,10.6,15.01,21.19};
float coeffs[STATES] = {1.0/3.0, 2.0/3.0, 2.0/3.0, 0};
float mag[N]={0};
float phase[N]={0};

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear (GL_COLOR_BUFFER_BIT);
   glShadeModel (GL_FLAT);
   srand (time(NULL));
}

void computeVals()
{
     expect=uncert=0;
     for(int i=0; i<N; i++){
       float x = (float)i/(N-1);
       Complex psi = Complex(0,0);
       for(int n=0; n<STATES; n++){
         psi += coeffs[n]*sin((n+1)*PI*x)*cexp(Complex(0,-1)*freq[n]*t);
       }
       mag[i]=(psi*psi.cconj()).real;
       phase[i]=psi.cangle();
       expect+=x*mag[i]*2.0*1.0/N;
     }
     for(int i=0; i<N; i++){
       float x = (float)i/(N-1);
       uncert+=pow(x-expect,2)*mag[i]*2.0*1.0/N;
     }
     //uncert=sqrt(uncert);
}

void drawCircle(float x, float y, float r)
{
     glBegin(GL_POLYGON);
       for(float a=0; a<2*PI; a+=PI/20)
         glVertex2f(x+r*cos(a),y+r*sin(a));
     glEnd();
}

void display(void)
{
   
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1,1,1);
   
   glBegin(GL_LINES);
     glVertex2f(0,1);
     glVertex2f(0,0);
     
     glVertex2f(0,0);
     glVertex2f(1,0);
   glEnd();
  
   computeVals();
   glColor3f(1,1,1);
   float scale=1./3;
   
   for(int i=1; i<N; i++){
       float x1 = (float)(i-1)/(N-1);
       float x2 = (float)i/(N-1);
       float c[3];
       SetPhaseColor(phase[i], 1, c);
       glColor3f(c[0], c[1], c[2]);
       glBegin(GL_QUADS);
         glVertex2f(x1,0);
         glVertex2f(x1,mag[i-1]*scale);
         glVertex2f(x2,mag[i]*scale);
         glVertex2f(x2,0);
       glEnd();
   }
  
   glColor3f(1,1,1);
   glBegin(GL_LINES);
     glVertex2f(expect-uncert,-0.1);
     glVertex2f(expect+uncert,-0.1);
   glEnd();
   
   drawCircle(expect,-0.1,0.01);
   //drawCircle(.5-64/(81*PI*PI)*cos((freq[1]-freq[0])*t)-128/(75*PI*PI)*cos((freq[2]-freq[1])*t),-0.15,0.01);
  
   glutSwapBuffers();

}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   float ar = (float)w/h;
   gluOrtho2D (0.5-0.7*ar, 0.5+0.7*ar, -0.2, 1.2);
}

void idle()
{
     usleep(20000);
     t+=0.01;
     glutPostRedisplay();
}

void key(unsigned char key, int x2, int y2)
{
    switch (key) 
    {
        case 27 :
            exit(0);
            break;
    }

    glutPostRedisplay();
}

void keyboardSpecial (int key, int x, int y)
{
 
}

void motion(int x, int y)
{
    
}

void mouse(int button, int state, int x2, int y2)
{
     
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (100, 50);
   glutCreateWindow ("1D Wavefunction");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);
   glutKeyboardFunc(key);
   glutSpecialFunc (keyboardSpecial);
   glutPassiveMotionFunc(motion);
   glutMouseFunc(mouse);
   glutMainLoop();
   return 0;
}
