
#include <GL/glut.h>
#include <stdlib.h>
#include <cmath>
#include <unistd.h>

#define PI 3.1415926535898
#define AXES_ON 1
#define AXES_OFF 2
#define X_LINES 3
#define Z_LINES 4

#define LIN_1 1
#define LIN_2 2
#define CYC_1 3
#define MORE_1 4
#define CYC_2 5
#define MORE_2 6
#define MORE_3 7
#define MORE_4 8
#define CYC_3 9
#define LIN_3 10
#define LIN_4 11
#define LIN_5 12
#define LIN_6 13
#define LIN_7 14
#define LIN_8 15
#define CYC_4 16

float xRot=0, yRot=0;
int prevX,prevY;
float range=10;
float xMin=-range,xMax=range,zMin=-range, zMax=range;
float detail=50;
bool isRotating=false;
bool axesOn=true;
bool isAnimated=false;
bool xLines=true,zLines=true;
int currentEqu=6;

float constant=1;
float delta=-0.005;
float speed=0.0001;


void mainMenuCallback(int id)
{
     switch(id){
        case 1:
          glutChangeToMenuEntry(1,"*Axes On", AXES_ON);
          glutChangeToMenuEntry(2,"Axes Off", AXES_OFF);
          axesOn=true;
          break;
        case 2:
          glutChangeToMenuEntry(1,"Axes On", AXES_ON);
          glutChangeToMenuEntry(2,"*Axes Off", AXES_OFF);
          axesOn=false;
          break;
        case 3:
          if(xLines)
            glutChangeToMenuEntry(3,"x-lines", X_LINES);
          else
            glutChangeToMenuEntry(3,"*x-lines", X_LINES);
          xLines=!xLines;
          break;
        case 4:
          if(zLines)
            glutChangeToMenuEntry(4,"z-lines", Z_LINES);
          else
            glutChangeToMenuEntry(4,"*z-lines", Z_LINES);
          zLines=!zLines;
          break;
     }
     glutPostRedisplay();
}

void linearFunctionMenuCallback(int id)
{
     currentEqu=id;
     glutPostRedisplay();
}

void cyclicalFunctionMenuCallback(int id)
{
     currentEqu=id;
     glutPostRedisplay();
}

void moreFunctionMenuCallback(int id)
{
     currentEqu=id;
     glutPostRedisplay();
}

float GetY(float x, float z)
{
      float returnVal;
      switch(currentEqu){
         case 1:
              returnVal=(x+z);
              break;
         case 2:
              returnVal= (x*z);
              break;
         case 3:
              returnVal= (cos(x)+cos(z));
              break;
         case 4:
              returnVal= (-1/(x*x+z*z));
              break;
         case 5:
              returnVal= (pow(fabs(cos(x)+cos(z)),0.5));
              break;
         case 6:
              returnVal= ((x*x*x*z-x*z*z*z)/390);
              break;
         case 7:
              returnVal= (sin(fabs(x)-fabs(z)));
              break;
         case 8:
              returnVal= (cos(fabs(x)+fabs(z)));
              break;
         case 9:
              returnVal= (pow(fabs(cos(x*x+z*z)),0.125));
              break;
         case 10:
              returnVal= (x*x-z*z);
              break;
         case 11:
              returnVal= (x*x+z*z);
              break;
         case 12:
              returnVal= (x*x*x-z*z*z);
              break;
         case 13:
              returnVal= (x*x*x+z*z*z);
              break;
         case 14:
              returnVal= (x*x*x*x-z*z*z*z);
              break;
         case 15:
              returnVal= (x*x*x*x+z*z*z*z);
              break;
         case 16:
              returnVal= 0.5*pow(sin(x*x+z*z),3);
              break;
      }
      if(isAnimated)
        return (constant*returnVal);
      return returnVal;
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
}

float* crossProduct(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float *cp)
{
   x1-=x2;
   y1-=y2;
   z1-=z2;
   x3-=x2;
   y3-=y2;
   z3-=z2;
   cp[0]=(y1*z3-y3*z1);
   cp[1]=(x3*z1-x1*z3);
   cp[2]=(x1*y3-x3*y1);
   return cp;
}
   

static void 
display(void)
{
    float rot=glutGet(GLUT_ELAPSED_TIME);
    rot=rot/1000*90;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(0,1,0);
    
    glLoadIdentity();
    
    float scaleFactor=1/fabs(xMax);
    glTranslatef(0,0,-5);
    if(isRotating)
      glRotatef(rot,1,1,1);
    glRotatef(xRot,1,0,0);
    glRotatef(yRot,0,1,0);
    glScalef(scaleFactor,scaleFactor,scaleFactor);
    
    float y,x2,y2,y3,y4,z2;
    /*for(float x=xMin; x<=xMax; x+=(xMax-xMin)/detail){
      for(float z=zMin; z<=zMax-(zMax-zMin)/detail; z+=(zMax-zMin)/detail){
         y=GetY(x,z);
         z2=z+(zMax-xMin)/detail;
         y2=GetY(x,z2);
         if(zLines){
           glBegin(GL_LINES);
             glVertex3f(x,y,z);
             glVertex3f(x,y2,z2);
           glEnd();
         }
      }
    }
    
    for(float z=zMin; z<=zMax; z+=(zMax-zMin)/detail){
      for(float x=xMin; x<=xMax-(xMax-xMin)/detail; x+=(xMax-xMin)/detail){
         y=GetY(x,z);
         x2=x+(xMax-xMin)/detail;
         y2=GetY(x2,z);
         if(xLines){
           glBegin(GL_LINES);
             glVertex3f(x,y,z);
             glVertex3f(x2,y2,z);
           glEnd();
         }
      }
    }*/
    
    for(float x=xMin; x<=xMax-(xMax-xMin)/detail; x+=(xMax-xMin)/detail){
      for(float z=zMin; z<=zMax-(zMax-zMin)/detail; z+=(zMax-zMin)/detail){
         y=GetY(x,z);
         z2=z+(zMax-zMin)/detail;
         x2=x+(xMax-xMin)/detail;
         y2=GetY(x,z2);
         y3=GetY(x2,z);
         y4=GetY(x2,z2);
         float *n1 = new float[3];
         crossProduct(x,y2,z2,x,y,z,x2,y3,z, n1);
         float *n2 = new float[3];
         crossProduct(x2,y3,z,x2,y4,z2,x,y2,z2, n2);
         if(zLines){
           glBegin(GL_TRIANGLES);
             glNormal3f(n1[0],n1[1],n1[2]);
             glVertex3f(x,y,z);
             glVertex3f(x,y2,z2);
             glVertex3f(x2,y3,z);
             
             glNormal3f(n2[0],n2[1],n2[2]);
             glVertex3f(x2,y4,z2);
             glVertex3f(x,y2,z2);
             glVertex3f(x2,y3,z);
           glEnd();
         }
         delete n1, n2;
      }
    }
    
    if(axesOn){
      glColor3f(1,1,1);
      glBegin(GL_LINES);
        glVertex3f(0,xMin,0);
        glVertex3f(0,xMax,0);
        glVertex3f(xMin,0,0);
        glVertex3f(xMax,0,0);
        glVertex3f(0,0,xMin);
        glVertex3f(0,0,xMax);
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
      case ',':
      case '<':
           if(range>1)range-=0.5;
           break; 
      case '.':
      case '>':
           range+=0.5;
           break; 
      case 'r':
      case 'R':
           isRotating=!isRotating;
           break;
      case 'a':
      case 'A':
           isAnimated=!isAnimated;
           constant=1;
           speed=0.0001;
           delta=-0.005;
           break;
      case 'd':
      case 'D':
           speed+=0.00002;
           break;
      case 's':
      case 'S':
           if(speed>0.00002)
             speed-=0.00002;
           else if(speed>0.00001)
             speed-=0.000002;
           break;    
    }
    xMin=-range;
    xMax=range;
    zMin=-range;
    zMax=range;
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
     prevX=x;
     prevY=y;
}

void motion(int x, int y)
{
     if(x>prevX)yRot+=abs(x-prevX);
     if(x<prevX)yRot-=abs(x-prevX);
     if(y>prevY)xRot+=abs(y-prevY);
     if(y<prevY)xRot-=abs(y-prevY);
     prevX=x;
     prevY=y;
}

static void 
idle(void)
{
    if(constant>0)delta-=speed;
    else delta+=speed;
    constant+=delta;
    usleep(15000);
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.1f, 0.1f, 0.1f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 0.0f, 0.0f, 1.0f, 0.0f };
const GLfloat light_position2[] = { 0.0f, 0.0f, -1.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 10.0f };

/* Program entry point */

int 
main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(700,700);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Graph3D");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);

    glClearColor(0,0,0,0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    
    int linearFunctionMenu=glutCreateMenu(linearFunctionMenuCallback);
    glutAddMenuEntry("x+z",LIN_1);
    glutAddMenuEntry("x*z",LIN_2);
    glutAddMenuEntry("x^2-z^2",LIN_3);
    glutAddMenuEntry("x^2+z^2",LIN_4);
    glutAddMenuEntry("x^3-z^3",LIN_5);
    glutAddMenuEntry("x^3+z^3",LIN_6);
    glutAddMenuEntry("x^4-z^4",LIN_7);
    glutAddMenuEntry("x^4+z^4",LIN_8);
    
    int cyclicalFunctionMenu=glutCreateMenu(cyclicalFunctionMenuCallback);
    glutAddMenuEntry("cos(x)+cos(z)",CYC_1);
    glutAddMenuEntry("|cos(x)+cos(z)|^(1/2)",CYC_2);
    glutAddMenuEntry("|cos(x^2+z^2)|^(1/8)",CYC_3);
    glutAddMenuEntry("sin(x^2+z^2)^3",CYC_4);
    
    int moreFunctionMenu=glutCreateMenu(moreFunctionMenuCallback);
    glutAddMenuEntry("-1/(x^2+z^2)",MORE_1);
    glutAddMenuEntry("(x^3*z-x*z^3)/390",MORE_2);
    glutAddMenuEntry("sin(|x|-|z|)",MORE_3);
    glutAddMenuEntry("cos(|x|+|z|)",MORE_4);
    
    glutCreateMenu(mainMenuCallback);
    glutAddMenuEntry("*Axes On",AXES_ON);
    glutAddMenuEntry("Axes Off",AXES_OFF);
    glutAddMenuEntry("*x-lines",X_LINES);
    glutAddMenuEntry("*z-lines",Z_LINES);
    glutAddSubMenu("Linear Functions",linearFunctionMenu);
    glutAddSubMenu("Cyclical Functions",cyclicalFunctionMenu);
    glutAddSubMenu("More Functions",moreFunctionMenu);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();

    return EXIT_SUCCESS;
}

