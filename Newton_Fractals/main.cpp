#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include "../ComplexClass/complex.cpp"
//#include "../GLText/GLText.cpp"

#define PI 3.14159265358979
#define MAXROOTS 9

double width,height;
double numRoots=5;
Complex roots[MAXROOTS];
double xMin=-2;
double xMax=2;
double yMin=-2;
double yMax=2;
double MAX_ITERS=30;
int colorMode=0;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   for(double i=0; i<numRoots; i++){
     roots[(int)i]=Complex(cos(i*2*PI/numRoots),sin(i*2*PI/numRoots));
     //roots[0]=Complex(-1.769292,0);
//     roots[1]=Complex(0.884646,0.589743);
//     roots[2]=Complex(0.884646,-0.589743);
   }
}

Complex f(Complex z)
{
        Complex result(1,0);
        for(int i=0; i<numRoots; i++)
          result*=(z-roots[i]);
        return result;
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   Complex smallVal(0.0001,0.0001);
   
   double xc=(xMin+xMax)/2;
   xMin=xc-(yMax-yMin)/2*width/height;
   xMax=xc+(yMax-yMin)/2*width/height;
   int n=0;
   
   for(double x=xMin; x<=xMax; x+=(xMax-xMin)/(double)width){
     for(double y=yMin; y<=yMax; y+=(yMax-yMin)/(double)height){
       double c=0;
       Complex z(x,y);
       int rootFound=0;
       while(rootFound==0 && c<MAX_ITERS){
         for(int i=0; i<numRoots; i++){
           if((z-roots[i]).cabs()<0.001){rootFound=i+1;break;}
         }
         Complex yval=f(z);
         z=z-(smallVal*(yval-smallVal))/(f(z+smallVal)-yval);
         c++;
       }
       double r=1,g=1,b=1;
       if(colorMode==0 || colorMode==1){
           if(rootFound==1){r=0.18;g=0.97;b=0.35;}
           if(rootFound==2){r=0.97;g=0.18;b=0.35;}
           if(rootFound==3){r=0.06;g=0.55;b=0.89;}
           if(rootFound==4){r=0.74;g=0.74;b=0.02;}
           if(rootFound==5){r=0.55;g=0.06;b=0.89;}
           if(rootFound==6){r=1;g=0.5;b=0;}
           if(rootFound==7){r=0.4;g=0;b=0.4;}
           if(rootFound==8){r=0;g=1;b=1;}
           if(rootFound==9){r=0.5;g=0.5;b=0.5;}
           double k=1-c/MAX_ITERS;
           if(!colorMode) glColor3d(r*k,g*k,b*k);
           else glColor3d(r*c/MAX_ITERS,g*c/MAX_ITERS,b*c/MAX_ITERS);
       }
       if(colorMode==2){
           r=(fmod(c,MAX_ITERS/5)/MAX_ITERS*5);
           g=(fmod(c,MAX_ITERS/8)/MAX_ITERS*8);
           b=(fmod(c,MAX_ITERS/2)/MAX_ITERS*2);
           glColor3d(r,g,b);
       }
       glBegin(GL_POINTS);
         glVertex2d((x-xMin)/(xMax-xMin)*2-1,(y-yMin)/(yMax-yMin)*2-1);
       glEnd();
     }
     n+=1;
     if(n%10==0)glFlush();
   }
   glFlush();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluOrtho2D (-1,1,-1,1);
   width=w;
   height=h;
}

void mouse(int button, int state, int x, int y)
{
     if(state==GLUT_UP){
       double xc=(double)x/(double)width*(xMax-xMin)+xMin;
       double yc=(double)(height-y)/(double)height*(yMax-yMin)+yMin;
       double xRange=xMax-xMin;
       double yRange=yMax-yMin;
       xMax=xc+xRange/4;
       xMin=xc-xRange/4;
       yMax=yc+yRange/4;
       yMin=yc-yRange/4;
       glutPostRedisplay();
     }
}

void keyboard(unsigned char key, int x, int y)
{
     switch(key){
       case 'R':
       case 'r':
            xMin=-2;
            xMax=2;
            yMin=-2;
            yMax=2;
            MAX_ITERS=30;
            break;
       case 'c':
       case 'C':
            colorMode=(colorMode+1)%3;
            break;
       case '=':
       case '+':
            if(numRoots<MAXROOTS)numRoots++;
            init();
            break;
       case '-':
       case '_':
            if(numRoots>3)numRoots--;
            init();
            break;
       case 27:
            exit(0);
            break;
       case '[':
       case '{':
            if(MAX_ITERS>10)MAX_ITERS-=10;
            break;
       case ']':
       case '}':
            MAX_ITERS+=10;
            break;
     }
     glutPostRedisplay();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("Newton Fractals");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
