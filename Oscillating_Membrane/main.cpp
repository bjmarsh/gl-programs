#include <GL/glut.h>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <unistd.h>
#include "../ComplexClass/complex.cpp"

#ifndef PI
#define PI 3.1415926535898
#endif

using namespace std;

float xRot=0, yRot=0;
float w,h;
int prevX,prevY;
float detail=55;
bool isRotating=false;
bool isPaused=false;
bool leftButtonDown=false;
bool is3d=true;
float coeffs[9]={1,0,0,0,0,0,0,0,0};
float etime;
float zoomFactor=1;
int mode=1;

float GetY(float x, float z)
{
      if(!isPaused)etime=(float)glutGet(GLUT_ELAPSED_TIME)/1000;
      float y=0;
      for(int i=0; i<9; i++){
        if(mode==1){
          int m=(i)%3+1;
          int n=(int)ceil((float)(i+1)/3);
          float freq=sqrt(m*m+n*n);
          y+=coeffs[i]*sin(etime*freq)*sin(m*x)*sin(n*z);
        }
        else{
          int m=(i)%3;
          int n=(int)ceil((float)(i+1)/3)-1;
          float freq=sqrt(m*m+n*n);
          y+=coeffs[i]*sin(etime*freq)*cos(m*x)*cos(n*z);
        }
      }


//       Complex psi = Complex(0,0);
//       for(int i=0; i<9; i++){
//          int m=i%3+1;
//          int n=i/3+1;
//          float freq=sqrt(m*m+n*n);
//          psi += coeffs[i]*sin(n*x)*sin(m*z)*cexp(Complex(0,-1)*freq*etime);
//       }
//       y=psi.cabs();
       return y;
}

static void 
resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    w=width;
    h=height;
}

static void 
display(void)
{
    float rot=glutGet(GLUT_ELAPSED_TIME);
    rot=rot/1000*90;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(0,1,0);
    
    float y,x2,y2,z2;
    
    glLoadIdentity();
    
    if(is3d){
      glTranslatef(0,0,-5);
      if(isRotating)
        glRotatef(rot,1,1,1);
      glRotatef(xRot,1,0,0);
      glRotatef(yRot,0,1,0);
      glScalef(zoomFactor,0.5*zoomFactor,zoomFactor);
      for(float x=0; x<=PI; x+=PI/detail){
        for(float z=0; z<=PI-PI/detail; z+=PI/detail){
           y=GetY(x,z);
           z2=z+PI/detail;
           y2=GetY(x,z2);
           glBegin(GL_LINES);
             glColor3f(1-1.5*y,1+1.5*y,1-fabs(1.5*y));
             glVertex3f(x/PI*2-1,y,z/PI*2-1);
             glVertex3f(x/PI*2-1,y2,z2/PI*2-1);
           glEnd();
        }
      }
      for(float z=0; z<=PI; z+=PI/detail){
        for(float x=0; x<=PI-PI/detail; x+=PI/detail){
           y=GetY(x,z);
           x2=x+PI/detail;
           y2=GetY(x2,z);
           glBegin(GL_LINES);
             glColor3f(1-1.5*y,1+1.5*y,1-fabs(1.5*y));
             glVertex3f(x/PI*2-1,y,z/PI*2-1);
             glVertex3f(x2/PI*2-1,y2,z/PI*2-1);
           glEnd();
        }
      }
    }else{
      glScalef(0.35,0.35,1);
      glTranslatef(-PI/2,-0.75,-2);
      for(float x=0; x<=PI-PI/detail; x+=PI/detail){
        for(float z=0; z<=PI-PI/detail; z+=PI/detail){
           y=GetY(x,z);
           glColor3f(-y,y,0);
           glBegin(GL_POLYGON);
             glVertex3f(x,z,0);
             glVertex3f(x+PI/detail,z,0);
             glVertex3f(x+PI/detail,z+PI/detail,0);
             glVertex3f(x,z+PI/detail,0);
           glEnd();
        }
      }
      glColor3f(1,1,1);
      glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0,PI,0);
        glVertex3f(0,PI,0);
        glVertex3f(PI,PI,0);
        glVertex3f(PI,PI,0);
        glVertex3f(PI,0,0);
        glVertex3f(PI,0,0);
        glVertex3f(0,0,0);
      glEnd();
    }
    glLoadIdentity();
    glTranslatef(0,0,-2);
    for(int n=1; n<=3; n++){
      for(int m=1; m<=3; m++){
        float ce=coeffs[3*(n-1)+m-1];
        glColor3f(-ce,ce,0);
        glBegin(GL_POLYGON);
          glVertex3f(-0.25+(float)(m-1)*0.5/3,-0.45-(float)(n-1)*0.5/3,0);
          glVertex3f(-0.25+(float)(m-1)*0.5/3,-0.45-(float)(n)*0.5/3,0);
          glVertex3f(-0.25+(float)(m)*0.5/3,-0.45-(float)(n)*0.5/3,0);
          glVertex3f(-0.25+(float)(m)*0.5/3,-0.45-(float)(n-1)*0.5/3,0);
        glEnd();
        float freq=sqrt(m*m+n*n);
        freq*=etime*180/PI;
        freq=(float)((int)freq%360)/360;
        glColor3f(0,0,1);
        if(ce!=0){
          glBegin(GL_LINES);
            glVertex3f(-0.25+(float)(m-1)*0.5/3+freq*0.5/3,-0.45-(float)(n-1)*0.5/3,0);
            glVertex3f(-0.25+(float)(m-1)*0.5/3+freq*0.5/3,-0.45-(float)(n)*0.5/3,0);
          glEnd();
        }
      }
    }
    glColor3f(1,1,1);
    for(int i=0; i<4; i++){
      glBegin(GL_LINES);
        glVertex3f(-0.25,-0.95+(float)i*0.5/3,0);
        glVertex3f(0.25,-0.95+(float)i*0.5/3,0);
      glEnd();
    }
    for(int i=0; i<4; i++){
      glBegin(GL_LINES);
        glVertex3f(-0.25+(float)i*0.5/3,-0.95,0);
        glVertex3f(-0.25+(float)i*0.5/3,-0.45,0);
      glEnd();
    }

    glutSwapBuffers();
}


static void 
key(unsigned char key, int x, int y)
{
    switch (key) 
    {
      case '=':
      case '+':
           detail++;
           break;
      case '-':
      case '_':
           if(detail>1)detail--;
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
           for(int i=1;i<9;i++)
             coeffs[i]=0;
           break;
      case 'r':
      case 'R':
           for(int i=0;i<9;i++)
             coeffs[i]=((float)(rand()%200)/100-1)*(rand()%2);
           break;
      case 'v':
      case 'V':
           is3d=!is3d;
           break;
    }
    
    string numbers="123456789";
    int pos=numbers.find(key);
    if(pos>=0 && coeffs[pos]!=0)
      coeffs[pos]=0;
    else if(pos>=0 && coeffs[pos]==0)
      coeffs[pos]=1;
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
     prevX=x;
     prevY=y;
     if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
       leftButtonDown=true;
     else
       leftButtonDown=false;
       
}

void motion(int x, int y)
{
     if(leftButtonDown){
       if(x>prevX)yRot+=abs(x-prevX);
       if(x<prevX)yRot-=abs(x-prevX);
       if(y>prevY)xRot+=abs(y-prevY);
       if(y<prevY)xRot-=abs(y-prevY);
     }else{
       zoomFactor+=(float)(prevY-y)/10;
       if(zoomFactor<=0.1)zoomFactor=0.1;
     }
     prevX=x;
     prevY=y;
}

void idle()
{
     usleep(10000);
     glutPostRedisplay();
}


int 
main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(700,700);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Oscillating Membrane");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);

    glClearColor(0,0,0,0);
    srand (time(NULL));

    glutMainLoop();

    return EXIT_SUCCESS;
}

