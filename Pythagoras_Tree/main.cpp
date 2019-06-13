#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>

#define PI 3.14159295358979
#define SR2 1.414213562373

float width,height;
int code;
int numLevels=5;
float elev=PI/4;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   srand(time(NULL));
}

void drawSquares(float x, float y, float s, float angle, int level)
{
     float f=(float)level/(float)numLevels;
     glColor3f(3*f/5,1-2*f/3,0);
     if(level>0){
       glBegin(GL_POLYGON);
         glVertex2f(x,y);
         glVertex2f(x+s*cos(angle),y+s*sin(angle));
         glVertex2f(x+s*SR2*cos(angle+PI/4),y+s*SR2*sin(angle+PI/4));
         glVertex2f(x+s*cos(angle+PI/2),y+s*sin(angle+PI/2));
       glEnd();
       float p=tan(elev)/2+1;
       drawSquares(x+s*cos(angle+PI/2),y+s*sin(angle+PI/2),s/2/cos(elev),angle+elev,level-1);
       drawSquares(x+s*sqrt(0.25+p*p)*cos(angle+atan(2*p)),y+s*sqrt(0.25+p*p)*sin(angle+atan(2*p)),s/2/cos(elev),angle-elev,level-1);
     }
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f(1,1,1);
   
   drawSquares(width/2-50,0,100,0,numLevels);
   
   glutSwapBuffers ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
   width=w;
   height=h;
}

void keyboard(unsigned char key, int x, int y)
{
     switch(key){
       case '+':
       case '=':
            numLevels++;
            break;
       case '-':
       case '_':
            if(numLevels>1)numLevels--;
            break;
       case '[':
       case '{':
            if(elev-PI/20>=-0.01)elev-=PI/20;
            break;
       case ']':
       case '}':
            if(elev+PI/20<=PI/4+0.01)elev+=PI/20;
            break;
     }         
     glutPostRedisplay();              
}

void motion(int x, int y)
{
     elev=(height-(float)y)/height*PI/4;
     if(elev>PI/4)elev=PI/4;
     if(elev<0)elev=0;
     glutPostRedisplay();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (700, 500); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("Koch Snowflake");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMotionFunc(motion);
   glutMainLoop();
   return 0;
}
