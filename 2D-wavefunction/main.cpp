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
#define N 100
#define STATES 4

float t=0;

float coeffs[STATES][STATES] = {{1,0,0,0},
                                {0,0,0,1},
                                {0,1,0,0},
                                {0,0,0,0},
                               };
float phaseOffset[STATES][STATES] = {{0,0,0,0},
                                     {0,0,0,0},
                                     {0,0,0,0},
                                     {0,0,0,0},
                                    };                               
                               
float mag[N][N]={0};
float phase[N][N]={0};

void normalize()
{
     float sqsum=0;
     for(int i=0; i<STATES; i++)
       for(int j=0; j<STATES; j++)
         sqsum+=pow(coeffs[i][j],2);
         
     sqsum=sqrt(sqsum)/2;
     for(int i=0; i<STATES; i++)
       for(int j=0; j<STATES; j++)
         coeffs[i][j]/=sqsum;
}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glShadeModel (GL_SMOOTH);
   glClear (GL_COLOR_BUFFER_BIT);
   srand (time(NULL));
   normalize();
}

void computeVals()
{
     for(int i=0; i<N; i++){
       for(int j=0; j<N; j++){
           float x = (float)i/(N-1);
           float y = (float)j/(N-1);
           Complex psi = Complex(0,0);
           for(int nx=1; nx<=STATES; nx++){
             for(int ny=1; ny<=STATES; ny++){
               float freq = 4*sqrt(nx*nx+ny*ny);
               psi += coeffs[nx-1][ny-1]*sin(nx*PI*x)*sin(ny*PI*y)*cexp(Complex(0,-1)*(freq*t+phaseOffset[nx-1][ny-1]));
             }
           }
           mag[i][j] =(psi*psi.cconj()).real;
           phase[i][j] =psi.cangle();
       }
     }
}


void display(void)
{
   
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1,1,1);
   
   computeVals();
   float scale=1./3;
   float c[3];

   for(int i=1; i<N-1; i++){
     for(int j=1; j<N-1; j++){
       float x1 = (float)(i-1)/(N-2);
       float x2 = (float)i/(N-2);
       float y1 = (float)(j-1)/(N-2);
       float y2 = (float)j/(N-2);
       glBegin(GL_QUADS);
         SetPhaseColor(phase[i-1][j-1], mag[i-1][j-1]+0.01, c);
         glColor3f(c[0], c[1], c[2]);
         glVertex2f(x1,y1);
         
         SetPhaseColor(phase[i-1][j], mag[i-1][j]+0.01, c);
         glColor3f(c[0], c[1], c[2]);
         glVertex2f(x1,y2);
         
         SetPhaseColor(phase[i][j], mag[i][j]+0.01, c);
         glColor3f(c[0], c[1], c[2]);
         glVertex2f(x2,y2);
         
         SetPhaseColor(phase[i][j-1], mag[i][j-1]+0.01, c);
         glColor3f(c[0], c[1], c[2]);
         glVertex2f(x2,y1);
       glEnd();
     }
   }
  
   glutSwapBuffers();

}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   float ar = (float)w/h;
   gluOrtho2D (0,1,0,1);
}

void idle()
{
     usleep(3000);
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

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("2D Wavefunctions");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);
   glutMainLoop();
   return 0;
}
