#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include "../DrawNumbers/numbers.cpp"

#define PI 3.1415926535898

float x,y;
float deltaX=6,deltaY=6;
int xMax=600,yMax=500;
float paddle1Height=275;
float paddle2Height=275;
bool isPlaying=1;
int p1Score=0;
int p2Score=0;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear (GL_COLOR_BUFFER_BIT);
   glShadeModel (GL_FLAT);
   srand (time(NULL));
   x=rand()%220+15;
   y=rand()%220+15;
}

void drawBall(float x, float y, int radius, int detail){
     glBegin(GL_POLYGON);
     glColor3f(1,1,1);
       for(float angle=0; angle<=2*PI+1; angle+=2*PI/detail)
         glVertex2f(radius*sin(angle)+x,radius*sin(PI/2-angle)+y);
     glEnd();
}

void drawPaddle( float height1, float height2 )
{
     glBegin(GL_POLYGON);
       glColor3f(1,0,0);
       glVertex2f(5,height1);
       glVertex2f(5,height1-50);
       glVertex2f(10,height1-50);
       glVertex2f(10,height1);
     glEnd();
     glBegin(GL_POLYGON);
       glColor3f(0,0,1);
       glVertex2f(xMax-5,height2);
       glVertex2f(xMax-5,height2-50);
       glVertex2f(xMax-10,height2-50);
       glVertex2f(xMax-10,height2);
     glEnd();
}

void display(void)
{
   
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (0.0, 1.0, 0.0);
  
     x+=deltaX;
     y+=deltaY;
     drawBall(x,y,7,40);
     if(((y+deltaY*((xMax-10-x)/deltaX)<paddle2Height-25)&&(deltaX>0))){
        paddle2Height-=5.5;
     }else if(deltaX>0){
        paddle2Height+=5.5;
     }
     if(paddle2Height>yMax)paddle2Height=yMax;
     if(paddle2Height<50)paddle2Height=50;
     drawPaddle(paddle1Height,paddle2Height);
     usleep(15000);
     if((x<=17)&&(y<=paddle1Height)&&(y>=paddle1Height-50)){deltaX*=-1;deltaX+=abs(deltaX)/deltaX*0.1;deltaY+=abs(deltaY)/deltaY*0.1;}
     if((x>=xMax-17)&&(y<=paddle2Height)&&(y>=paddle2Height-50)){deltaX*=-1;deltaX+=abs(deltaX)/deltaX*0.1;deltaY+=abs(deltaY)/deltaY*0.1;}
     if((y<7)||(y>yMax-7))deltaY*=-1;
     if(x<=7){
        isPlaying=0;
        DrawIntL(++p2Score,xMax-30,30,20,10,1,1,1);
        DrawIntL(p1Score,10,30,20,10,1,1,1);
     }
     if(x>=xMax-7){
        isPlaying=0;
        DrawIntL(++p1Score,10,30,20,10,1,1,1);
        DrawIntL(p2Score,xMax-30,30,20,10,1,1,1);
     }
     glutSwapBuffers();

}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
   xMax=w;
   yMax=h;
}

void idle()
{
     if(isPlaying)
     glutPostRedisplay();
}

void key(unsigned char key, int x2, int y2)
{
    switch (key) 
    {
        case 27 :
            exit(0);
            break;
        case 'r':
             x=xMax/2;
             y=rand()%(yMax-14)+7;
             paddle1Height=275;
             paddle2Height=275;
             deltaX=6;
             deltaY=6;
             isPlaying=1;
             break;
    }

    glutPostRedisplay();
}

void keyboardSpecial (int key, int x, int y)
{
  switch (key) {
		case GLUT_KEY_UP:
		  paddle1Height+=9;
		  if(paddle1Height>yMax)paddle1Height=yMax;
		  break;
        case GLUT_KEY_DOWN:
		  paddle1Height-=9;
		  if(paddle1Height<50)paddle1Height=50;
		  break;
  }
}

void motion(int x, int y)
{
     paddle1Height=yMax-y+30;
}

void mouse(int button, int state, int x2, int y2)
{
     if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && !isPlaying){
        x=xMax/2;
        y=rand()%(yMax-14)+7;
        paddle1Height=275;
        paddle2Height=275;
        deltaX=6;
        deltaY=6;
        isPlaying=1;
     }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (600, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Pong");
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
