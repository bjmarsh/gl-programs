#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <cmath>

#define PI 3.1415926535898
#define POINTS 20
#define COLORS 3

void GeneratePattern();
void Rotate(float angle);
void Shrink(float factor);
void DefineColors(int numColors);
float GetMaxDistance();

int points,colors;
float patternArray[16][40];
float colorArray[30];
int currentColor=0;
int currentColor2=1;
int colorCounter=0;
int counterMax=100;
int numRotations;
float angle,dilationFactor;
int isPaused=0;
int curX=-1,curY;
bool isScreenSaver=false;

void Restart()
{
     colors=rand()%9+2;
     angle=rand()%21-10;
     angle/=2;
     dilationFactor=rand()%10;
     dilationFactor/=1000;
     dilationFactor=0.997-dilationFactor;
     points=rand()%37+4;
     counterMax=rand()%176+25;
     currentColor=0;
     currentColor2=1;
     numRotations=rand()%4+1;
     GeneratePattern();
     DefineColors(colors);
     glClear(GL_COLOR_BUFFER_BIT);
}

float GetMaxDistance()
{
     float max=0;
     for(int i=0; i<=points-1; i++){
        if(sqrt(patternArray[0][i]*patternArray[0][i]+patternArray[1][i]*patternArray[1][i])>max)
          max=sqrt(patternArray[0][i]*patternArray[0][i]+patternArray[1][i]*patternArray[1][i]);
     }
     return max;
} 

void GeneratePattern()
{
     for(int i=0; i<=15; i++)
       patternArray[i][0]=0;
     
     int a=0,b=0;
     for(int i=1; i<=points-1; i++){
        a+=rand()%5-2;
        b+=rand()%5-2;
        if(a==0)a=1;
        if(b==0)b=1;
        patternArray[0][i]=a;
        patternArray[1][i]=b;
        patternArray[2][i]=-a;
        patternArray[3][i]=-b;
        patternArray[4][i]=-a;
        patternArray[5][i]=b;
        patternArray[6][i]=a;
        patternArray[7][i]=-b;
        patternArray[8][i]=b;
        patternArray[9][i]=a;
        patternArray[10][i]=-b;
        patternArray[11][i]=-a;
        patternArray[12][i]=-b;
        patternArray[13][i]=a;
        patternArray[14][i]=b;
        patternArray[15][i]=-a;
     }
     
     float max=20/GetMaxDistance();
     Shrink((float)max);
}

void DefineColors( int numColors )
{
     for(int i=0; i<=3*numColors-1; i++){
        colorArray[i]=rand()%101;
        colorArray[i]/=100;
     }
}

void Rotate(float angle)
{
     for(int i=0; i<=points-1; i++){
       for(int c=0; c<=14; c+=2){
         float x=patternArray[c][i];
         float y=patternArray[c+1][i];
         float dist=sqrt(x*x+y*y);
         x/=dist;
         y/=dist;
         float n=asin(y);
         if(x<0)n=n+2*(PI/2-n);
         n+=angle*PI/180;
         y=sin(n);
         x=sin(PI/2-n);
         x*=dist;
         y*=dist;
         patternArray[c][i]=x;
         patternArray[c+1][i]=y;
       }
     }
}

void Shrink(float factor)
{
     for(int i=0; i<=points-1; i++){
       for(int c=0; c<=14; c+=2){
         patternArray[c][i]*=factor;
         patternArray[c+1][i]*=factor;
       }
     }
}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear (GL_COLOR_BUFFER_BIT);
   glShadeModel (GL_FLAT);
   srand (time(NULL));
   Restart();
}

void display(void)
{
   //glClear (GL_COLOR_BUFFER_BIT);
   float red,green,blue;
   float multiplier=colorCounter;
   multiplier/=counterMax;
   red=(colorArray[3*currentColor]-colorArray[3*currentColor2])*multiplier;
   red=colorArray[3*currentColor]-red;
   green=(colorArray[3*currentColor+1]-colorArray[3*currentColor2+1])*multiplier;
   green=colorArray[3*currentColor+1]-green;
   blue=(colorArray[3*currentColor+2]-colorArray[3*currentColor2+2])*multiplier;
   blue=colorArray[3*currentColor+2]-blue;
   colorCounter++;
   if(colorCounter>counterMax){
      colorCounter=0;
      currentColor++;
      currentColor2++;
   }
   if(currentColor==colors)currentColor=0;
   if(currentColor2==colors)currentColor2=0;
   glColor3f (red,green,blue);
   
   for(int c=0;c<=numRotations-1;c++){
   glBegin(GL_LINES);
      for(int i=1; i<=points-2; i++){
         glVertex2f(patternArray[0][i],patternArray[1][i]);
         glVertex2f(patternArray[0][i+1],patternArray[1][i+1]);
         glVertex2f(patternArray[2][i],patternArray[3][i]);
         glVertex2f(patternArray[2][i+1],patternArray[3][i+1]);
         glVertex2f(patternArray[4][i],patternArray[5][i]);
         glVertex2f(patternArray[4][i+1],patternArray[5][i+1]);
         glVertex2f(patternArray[6][i],patternArray[7][i]);
         glVertex2f(patternArray[6][i+1],patternArray[7][i+1]);
         glVertex2f(patternArray[8][i],patternArray[9][i]);
         glVertex2f(patternArray[8][i+1],patternArray[9][i+1]);
         glVertex2f(patternArray[10][i],patternArray[11][i]);
         glVertex2f(patternArray[10][i+1],patternArray[11][i+1]);
         glVertex2f(patternArray[12][i],patternArray[13][i]);
         glVertex2f(patternArray[12][i+1],patternArray[13][i+1]);
         glVertex2f(patternArray[14][i],patternArray[15][i]);
         glVertex2f(patternArray[14][i+1],patternArray[15][i+1]);
         }
   glEnd();
   Rotate(90/numRotations);
   }
   Rotate(-90);
   
   glFlush ();
   usleep(5000);
   Rotate(angle);
   Shrink(dilationFactor);
   if(GetMaxDistance()<0.3)Restart();
   if(!isPaused)glutPostRedisplay();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glClear(GL_COLOR_BUFFER_BIT);
   gluOrtho2D (-14.64, 14.64, -10, 10);
}

void keyboard(unsigned char key, int x, int y)
{
     switch(key){
        case 'r':
             Restart();
             break;
        case 'p':
             if(!isPaused){
               isPaused=1;
             }else{
               isPaused=0;
               glutPostRedisplay();
             }
             break;
        case 27:
             exit(0);
             break;
     }
}

void passivemotion(int x, int y)
{
     if(curX==-1){
        curX=x;
        curY=y;
     }else{
        if(((abs(x-curX)>=3)||(abs(y-curY)>=3))&&isScreenSaver)
          exit(0);
     }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (1025,770); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("Spinning Fractals");
   glutFullScreen();
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutPassiveMotionFunc(passivemotion);
   glutMainLoop();
   return 0;
}
