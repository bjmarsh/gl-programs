#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "../ComplexClass/complex.cpp"

double width=700;
double height=700;
double xMin=-1.5;
double xMax=1.5;
double yMin=-1;
double yMax=1;
double cReal[9]={-0.74543,-0.835,-0.4,0.285,-0.8,0.45,-0.70176,0.4,-1.254159};
double cImag[9]={0.11301,-0.2321,0.6,0.01,0.156,0.1428,-0.3842,0.3,0.1618};
int patternNum=1;
int colorDivs[3];

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   srand (time(NULL));
   for(int i=0; i<=2; i++)
      colorDivs[i]=rand()%8+2;
}

void Iterate(Complex z, int maxIter)
{
   double xx=z.real;
   double yy=z.imag;
   Complex c(cReal[patternNum],cImag[patternNum]);
   double zrtemp;
   int count=0;
   do{
      //zrtemp=zr*zr-zi*zi+c.real;
      //zi=2*zr*zi+c.imag;
      //zr=zrtemp;
      //z=(z^2)+c;
      zrtemp=z.real*z.real-z.imag*z.imag+c.real;
      z.imag=z.real*z.imag*2+c.imag;
      z.real=zrtemp;
      count++;
   }while((z.cabs()<4)&&(count<maxIter));
   double red,blue,green;
   red=pow(log(count)/colorDivs[0],3);
   blue=pow(log(count)/colorDivs[1],3);
   green=pow(log(count)/colorDivs[2],3);
   glBegin(GL_POINTS);
     glColor3f(red,green,blue);
     glVertex2f((xx-xMin)/(xMax-xMin)*2-1,(yy-yMin)/(yMax-yMin)*2-1);
   glEnd();
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   
   double xc=(xMin+xMax)/2;
   xMin=xc-(yMax-yMin)/2*width/height;
   xMax=xc+(yMax-yMin)/2*width/height;
   
   for(double x=xMin; x<=xMax; x+=(xMax-xMin)/width){
     for(double y=yMin; y<=yMax; y+=(yMax-yMin)/height){
       Iterate(Complex(x,y),2000); 
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
       double dif=xMax-xMin;
       double temp=x/width;
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
             xMin=-1.5;
             xMax=1.5;
             yMin=-1.5;
             yMax=1.5;
             glutPostRedisplay();
             break;
        case 27:
             exit(0);
             break;
     }
}

void special(int key, int x, int y)
{
     switch(key){
        case GLUT_KEY_LEFT:
             patternNum--;
             if(patternNum<0)patternNum=8;
             for(int i=0; i<=2; i++)
               colorDivs[i]=rand()%8+2;
             glutPostRedisplay();
             break;
        case GLUT_KEY_RIGHT:
             patternNum++;
             if(patternNum>8)patternNum=0;
             for(int i=0; i<=2; i++)
               colorDivs[i]=rand()%6+3;
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
   glutCreateWindow ("Julia Sets");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutMainLoop();
   return 0;
}
