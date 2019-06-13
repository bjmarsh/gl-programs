#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "../GLText/GLText.cpp"

#define PI 3.1415926535898

float rotX=0,rotY=0;
int prevX=-1,prevY;
float w,h;
float range=1;
float detail=300;
int equNum=1;
string equation="r(t,p)=cos(t)+sin(4p)";
float delta=1, mult=1, theta1=0, theta2=0, alpha=0;
int counter=0;
bool isPaused=false;
float separation;

void init(void) 
{
   glClearColor (0, 0, 0, 1);
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glShadeModel (GL_SMOOTH);
   srand(time(NULL));
   equNum=rand()%7+1;
   separation=rand()%8+3;
   //glEnable(GL_LINE_SMOOTH);
}

float getR(float t1, float t2)
{
      switch(equNum){
        case 1:
             return (cos(t1)+sin(4*t2));
             break;
        case 2:
             return (cos(t1)+cos(t2));
             break;
        case 3:
             return (2*sin(t1+t2));
             break;
        case 4:
             return (t1/2.8*sin(3*t1));
        case 5:
             return (sin(t1)+cos(t1+t2));
             break;
        case 6:
             return t1*t2/7/PI;
             break;
        case 7:
             return atan(t1*t2);
             break;
      }
}

     

void display(void)
{    
     for(float theta1=0; theta1<=2*PI; theta1+=PI/150){
     mult+=0.00001*delta;
     counter++;
     if(counter==150000){
       counter=0;
       delta*=-1;
    }
    for(float i=0; i<=20; i++){
         float r=getR(theta1,theta2-separation*i*PI/detail)*mult;
         theta1+=PI/150;
         float r2=getR(theta1,theta2-separation*i*PI/detail)*mult;
         glColor4f(theta1/2/PI,fabs(r)/3,theta2/2/PI,(i==0)?alpha*0.7:alpha*(1-i/20)*0.3);
         glBegin(GL_LINES);
           glVertex3f(r2*cos(theta2-separation*i*PI/detail)*cos(theta1),r2*sin(theta2-separation*i*PI/detail),r2*cos(theta2-separation*i*PI/detail)*sin(theta1));
           theta1-=PI/150;
           glVertex3f(r*cos(theta2-separation*i*PI/detail)*cos(theta1),r*sin(theta2-separation*i*PI/detail),r*cos(theta2-separation*i*PI/detail)*sin(theta1));
         glEnd();
    }
    }
    usleep(1000);
     glutSwapBuffers(); 
     theta2+=PI/detail;
     glClear (GL_COLOR_BUFFER_BIT); 
  
}

void reshape (int width, int height)
{
   const float ar = (float) width / (float) height;
    w=width;
    h=height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}


void mouse(int button, int state, int x, int y)
{
     prevX=x;
     prevY=y;
      
     glutPostRedisplay();
}

void motion(int x, int y)
{
     if(x>prevX)rotY+=abs(x-prevX);
     if(x<prevX)rotY-=abs(x-prevX);
     if(y>prevY)rotX+=abs(y-prevY);
     if(y<prevY)rotX-=abs(y-prevY);
     prevX=x;
     prevY=y;
     glutPostRedisplay();
}

void passive(int x, int y)
{
     if(prevX==-1){
       prevX=x;
       prevY=y;
     }else if(abs(x-prevX)>2 || abs(y-prevY)>2)
       exit(0);
}

void idle()
{
     if(!isPaused){
       glLoadIdentity();
       glTranslatef(0,0,-7);
       glRotatef(theta2/4/PI*720,1,1,1);
       if(theta2<PI)
         alpha=theta2/PI;
       if(theta2>3*PI)
         alpha=(4*PI-theta2)/PI;
       if(theta2>=4*PI){
         theta2=0;
         separation=rand()%8+3;
         int temp=equNum;
         do{
           equNum=rand()%7+1;
         }while(equNum==temp);
       }
       glutPostRedisplay();
     }
}

void keyboard(unsigned char key, int x, int y)
{
     switch(key){
       case ',':
       case '<':
            if(range>0.5)
              range-=0.5;
              break;
       case '.':
       case '>':
            range+=0.5;
            break;
       case '-':
       case '_':
            if(detail>5)
              detail-=5;
              break;
       case '=':
       case '+':
            detail+=5;
            break;
       case '[':
       case '{':
            if(equNum>1)
              equNum--;
            getR(0,0);
            break;
       case ']':
       case '}':
            if(equNum<7)
              equNum++;
            getR(0,0);
            break;
       case 'p':
       case 'P':
            isPaused=!isPaused;
            break;
     }
     glutPostRedisplay();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (700, 700); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("Spherical Polar Graphs");
   init ();
   glutFullScreen();
   glutSetCursor(GLUT_CURSOR_NONE);
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(passive);
   glutKeyboardFunc(keyboard);
   glutIdleFunc(idle);
   glutMainLoop();
   return 0;
}
