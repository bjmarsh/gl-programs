#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "../ComplexClass/complex.cpp"

float width=700;
float height=700;
float xMin=-2;
float xMax=1;
float yMin=-1.5;
float yMax=1.5;
float cReal[9]={-0.74543,-0.835,-0.4,0.285,-0.8,0.45,-0.70176,0.4,-1.254159};
float cImag[9]={0.11301,-0.2321,0.6,0.01,0.156,0.1428,-0.3842,0.3,0.1618};
int patternNum=0;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void Iterate(Complex c, int maxIter)
{
   Complex z(0,0);
   float zrtemp;
   int count=0;
   do{
      zrtemp=z.real*z.real-z.imag*z.imag+c.real;
      z.imag=2*z.real*z.imag+c.imag;
      z.real=zrtemp;
      count++;
   }while((z.real*z.real+z.imag*z.imag<100)&&(count<maxIter));
   float red,blue,green;
   red=blue=green=0;
   if(count<maxIter){
      blue=count;
      blue=log(blue)/3;
      red=count;
      red/=24;
      red*=red*red;
      green=count;
      green/=24;
      green*=green*green;
   }
   glBegin(GL_POINTS);
     glColor3f(red,green,blue);
     glVertex2f((c.real-xMin)/(xMax-xMin)*2-1,(c.imag-yMin)/(yMax-yMin)*2-1);
   glEnd();
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   for(float x=xMin; x<=xMax; x+=(xMax-xMin)/width){
     for(float y=yMin; y<=yMax; y+=(yMax-yMin)/height){
       Iterate(Complex(x,y),400); 
     }
   }
   glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   width=w;
   height=h;
   glOrtho(-1.0,1.0,-1.0,1.0,-0.0,0.0);
}

void mouse(int button, int state, int x, int y){
     if((button==GLUT_LEFT_BUTTON)&&(state==GLUT_DOWN)){
       float dif=xMax-xMin;
       float temp=x/width;
       temp=temp*dif+xMin;
       xMin=temp-0.25*dif;
       xMax=temp+0.25*dif;
       dif=yMax-yMin;
       temp=height-y+30;
       temp/=width;
       temp=temp*dif+yMin;
       yMin=temp-0.25*dif;
       yMax=temp+0.25*dif;
       glutPostRedisplay();
     }
}

void keyboard(unsigned char key, int x, int y)
{
     switch(key){
        case 'r':
             xMin=-2;
             xMax=1;
             yMin=-1.5;
             yMax=1.5;
             glutPostRedisplay();
             break;
     }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (700, 700); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("Mandelbrot Set");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
