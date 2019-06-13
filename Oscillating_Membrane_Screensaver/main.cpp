#include <GL/glut.h>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <unistd.h>
#include "../GLText/GLText.cpp"

#define PI 3.1415926535898

using namespace std;

float detail=60;
bool isPaused=false;
float coeffs[16];
float targets[16];
float colors[3];
int scheme=1;
float b=1;
bool isFading=false, showStats=false;
int counter=0;
float etime;
int mode=1;
int initX=-1;
int lastTime=0;
float fps=0;

float GetY(float x, float z)
{
      if(!isPaused)etime=(float)glutGet(GLUT_ELAPSED_TIME)/5000;
      float y=0;
      for(int i=0; i<16; i++){
        if(mode==1){
          int m=i%4+1;
          int n=(int)ceil((float)(i+1)/4);
          float freq=sqrt(m*m+n*n);
          y+=coeffs[i]*sin(etime*freq)*sin(m*x)*sin(n*z);
        }
        else{
          int m=i%4;
          int n=(int)ceil((float)(i+1)/4)-1;
          float freq=sqrt(m*m+n*n);
          y+=coeffs[i]*sin(etime*freq)*cos(m*x)*cos(n*z);
        }
      }
      return y;
}

static void 
resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, PI, 0, PI);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

void randomize()
{
    for(int i=0;i<16;i++){
      coeffs[i]=((float)(rand()%200)/100-1);
      targets[i]=((float)(rand()%200)/100-1);
    }
    int temp=scheme;
    do{
      scheme=rand()%14+1;
    }while(scheme==temp);
    mode=rand()%2+1;
    for(int i=0; i<3; i++)
      colors[i]=rand()%10+1;
}

void setColor(float p)
{
     switch(scheme){
       case 1: //rainbow
         glColor3f(b*(cos(colors[0]*p)+1)/2,b*(sin(-colors[1]*p)+1)/2,b*(sin(colors[2]*p)+1)/2);
         break;
       case 2:   //g/r
         glColor3f(b*p,b*-p,0);
         break;
       case 3:  //b/r
         glColor3f(b*p,0,b*-p);
         break;
       case 4:   //g/b
         glColor3f(0,b*p,b*-p);
         break;
       case 5:  //b/y
         glColor3f(b*sin(p),b*p,b*-p);
         break;
       case 6:  //r/y
         glColor3f(b*fabs(p),b*p,0);
         break;
       case 7:  //cold colors
         glColor3f(0,b*(sin(5*p)+1)/2,b*(sin(-7*p)+1)/2);
         break;
       case 8:   //rwb
         glColor3f(b*(sin(5*p)+1)/2,b*(sin(-3*p)+1)/2,b*(sin(-3*p)+1)/2);
         break;
       case 9:   //g/y/r
         glColor3f(b*(sin(5*p)+1)/2,b*(sin(-7*p)+1)/2,0);
         break;
       case 10:  //g/p
         glColor3f(-p*b,p*b,-p*b);
         break;
       case 11:   //y/p
         glColor3f(fabs(p)*b,p*b,-p*b);
         break;
       case 12:  //g/y
         glColor3f(p*b,fabs(p)*b,0);
         break;
       case 13:  //b/o
         glColor3f(-p*b,-p*0.5*b,p*b);
         break;
       case 14:   //blue/w
         glColor3f(-p*b,-p*b,fabs(p)*b);
         break;
     }
}

static void 
display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    float y,y2,y3,y4;
    
    glLoadIdentity();
      for(float x=0; x<=PI-PI/detail; x+=PI/detail){
        for(float z=0; z<=PI-PI/detail; z+=PI/detail){
           y=GetY(x,z);
           y2=GetY(x+PI/detail,z);
           y3=GetY(x,z+PI/detail);
           y4=GetY(x+PI/detail,z+PI/detail);
           glBegin(GL_POLYGON);
             setColor(y);
             glVertex2f(x,z);
             setColor(y2);
             glVertex2f(x+PI/detail,z);
             setColor(y4);
             glVertex2f(x+PI/detail,z+PI/detail);
             setColor(y3);
             glVertex2f(x,z+PI/detail);
           glEnd();
        }
      }
    if(showStats){
      glColor3f(1,1,1);
      for(int i=0; i<16; i++){
        DrawText(0,PI-(float)(i+2)/15,0,(coeffs[i])>=0 ? "0." : "-0.",GLUT_BITMAP_HELVETICA_12);
        if(fabs(coeffs[i])<0.1)
          DrawText("0", GLUT_BITMAP_HELVETICA_12);
        DrawInt((int)(fabs(coeffs[i])*100),GLUT_BITMAP_HELVETICA_12);  
        DrawText((targets[i])>=0 ? " (0." : " (-0.",GLUT_BITMAP_HELVETICA_12);
        if(fabs(targets[i])<0.1)
          DrawText("0", GLUT_BITMAP_HELVETICA_12);
        DrawInt((int)(fabs(targets[i])*100),GLUT_BITMAP_HELVETICA_12);
        DrawText(")", GLUT_BITMAP_HELVETICA_12);
      }
      DrawText(0,PI-0.07,0,"COEFFS (TARGETS):", GLUT_BITMAP_HELVETICA_12);
      DrawText(0,PI-1.3,0,"FPS=", GLUT_BITMAP_HELVETICA_12);
      DrawInt((int)floor(fps), GLUT_BITMAP_HELVETICA_12);
      DrawText(".", GLUT_BITMAP_HELVETICA_12);
      DrawInt((int)(1000*(fps-floor(fps))), GLUT_BITMAP_HELVETICA_12);
    }
    
    fps=glutGet(GLUT_ELAPSED_TIME)-lastTime;
    fps=1000/fps;
    lastTime=glutGet(GLUT_ELAPSED_TIME);
    
    glutSwapBuffers();
}


static void 
key(unsigned char key, int x, int y)
{
    switch (key) 
    {
      case '=':
      case '+':
           if(scheme<14)scheme++;
           break;
      case '-':
      case '_':
           if(scheme>1)scheme--;
           break;  
      case 'p':
      case 'P':
           isPaused=!isPaused;
           break; 
      case 'M':
      case 'm':
           mode=abs(mode-3);
           break; 
      case 'f':
      case 'F':
           coeffs[0]=1;
           for(int i=1;i<16;i++)
             coeffs[i]=0;
           break;
      case 'r':
      case 'R':
           randomize();
           break;
      case 's':
      case 'S':
           showStats=!showStats;
           break;
      case 27:
           exit(0);
           break;
    }
    glutPostRedisplay();
}

void idle()
{
     usleep(25000);
     counter++;
     for(int i=0; i<16; i++){
       if(fabs(coeffs[i]-targets[i])<0.02)
         targets[i]=(float)(rand()%200)/100-1;
       coeffs[i]+=-(coeffs[i]-targets[i])/fabs(coeffs[i]-targets[i])*0.01;
     }
     if(counter>=200){
       isFading=true;
       counter=0;
     }
     if(isFading)
       b-=0.05;
     if(b<=0 && isFading){
       isFading=false;
       randomize();
     }
     if(!isFading && b<1)
       b+=0.05;
     glutPostRedisplay();
}

void init()
{
    glClearColor(0,0,0,0);
    srand(time(NULL));
    randomize();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void passive(int x, int y)
{
     if(initX==-1){
       initX=x;
     }else if(abs(initX-x)>2){
       exit(0);
     }        
}

int 
main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(700,700);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Oscillating Membrane");
    glutFullScreen();
    glutSetCursor(GLUT_CURSOR_NONE);

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(passive);
    
    init();
    glutMainLoop();

    return EXIT_SUCCESS;
}

