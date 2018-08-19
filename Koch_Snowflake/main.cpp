#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>

#define PI 3.1415926535898

float width,height;
int levels=0;
float ANGLE=60*PI/180;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   srand(time(NULL));
}

void GenerateSnowflake(float turtleX, float turtleY, float turtleAngle, float lineLength, int level)
{
     if(level>0){
         char key[]="FLFRFLF";
         for(int i=0; i<7; i++){
            switch(key[i]){
               case 'F':
                    if(i==0 || i==6){
                      GenerateSnowflake(turtleX,turtleY,turtleAngle,(lineLength-2*lineLength/3*cos(ANGLE))/2,level-1);
                      turtleX+=(lineLength-2*lineLength/3*cos(ANGLE))/2*cos(turtleAngle);
                      turtleY+=(lineLength-2*lineLength/3*cos(ANGLE))/2*sin(turtleAngle);
                    }else{
                      GenerateSnowflake(turtleX,turtleY,turtleAngle,lineLength/3,level-1);
                      turtleX+=lineLength/3*cos(turtleAngle);
                      turtleY+=lineLength/3*sin(turtleAngle);  
                    }  
                    break;
               case 'L':
                    turtleAngle+=ANGLE;
                    break;
               case 'R':
                    turtleAngle-=2*ANGLE;
                    break;
            }
         }    
     }else{
         glBegin(GL_LINES);
           glVertex2f(turtleX,turtleY);
           glVertex2f(turtleX+lineLength*cos(turtleAngle),turtleY+lineLength*sin(turtleAngle));
         glEnd();
         //glutSwapBuffers();
     }
}
void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);

   GenerateSnowflake(width/2,height/2-width/6*sqrt(3),2*PI/3,width/2,levels);
   GenerateSnowflake(width/4,height/2+width/12*sqrt(3),0,width/2,levels);
   GenerateSnowflake(3*width/4,height/2+width/12*sqrt(3),4*PI/3,width/2,levels);

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
        case '-':
        case '_':
             if(levels>0)levels--;
             break;
        case '+':
        case '=':
             levels++;
             break;
        case '[':
             ANGLE-=10*PI/180;
             if(ANGLE<0)
               ANGLE=2*PI-ANGLE;
             break;
        case ']':
             ANGLE+=10*PI/180;
             if(ANGLE>2*PI)
               ANGLE-=2*PI;
             break;
     }
     glutPostRedisplay();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("Koch Snowflake");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
