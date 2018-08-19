#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>

#define NUM 10
#define PI 3.14159265358979
#define SPACING 8

float width=900,height=600;
int selected=-1;
float xstart=350,ystart=350;
bool moveCharges=true;
int visible=7;
bool drawLines=true;

struct Charge{
     float charge,x,y,radius; 
};  

Charge charges[NUM];

void randomize()
{
   for(int c=0;c<NUM; c++){
     charges[c].charge=rand()%1+1;
     if(rand()%2)charges[c].charge*=-1;
     charges[c].x=rand()%((int)width-100)+50;
     charges[c].y=rand()%((int)height-100)+50;
     charges[c].radius=10;
   }
} 

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   srand(time(NULL));
   randomize();
}

float* getNetVector(float x, float y, float *vec)
{
      float mag=0, dir=0;
      for(int c=0; c<visible; c++){
        float dist=sqrt((x-charges[c].x)*(x-charges[c].x)+(y-charges[c].y)*(y-charges[c].y));
        float force=100000*charges[c].charge/dist/dist;
        float angle=atan((charges[c].y-y)/(charges[c].x-x));
        if(charges[c].x<x)angle+=PI;
        float vecx=mag*cos(dir)+force*cos(angle);
        float vecy=mag*sin(dir)+force*sin(angle);
        mag=sqrt(vecx*vecx+vecy*vecy);
        dir=atan(vecy/vecx);
        if(vecx<0)dir+=PI;
      }
      vec[0]=mag;
      vec[1]=dir;
      return vec;
}
       
void drawFieldLine(float x, float y)
{
     float xc=x,yc=y,xp,yp;
     for(int i=0; i<300; i++){
       xp=xc;
       float xt=xc;
       yp=yc;
       float *vec = new float[2];
       getNetVector(xc,yc,vec);
       xc=xp+5*cos(vec[1]);
       getNetVector(xt,yc,vec);
       yc=yp+5*sin(vec[1]);
       glColor3f(1,1,1);
       glBegin(GL_LINES);
         glVertex2f(xp,yp);
         glVertex2f(xc,yc);
       glEnd();
       delete vec;
     }
     xc=x,yc=y;
     for(int i=0; i<300; i++){
       xp=xc;
       float xt=xc;
       yp=yc;
       float* vec = new float[2];
       getNetVector(xc,yc, vec);
       xc=xp-5*cos(vec[1]);
       getNetVector(xt,yc, vec);
       yc=yp-5*sin(vec[1]);
       glColor3f(1,1,1);
       glBegin(GL_LINES);
         glVertex2f(xp,yp);
         glVertex2f(xc,yc);
       glEnd();
       delete vec;
     }
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   
   for(float x=SPACING; x<=width-SPACING; x+=SPACING){
     for(float y=SPACING; y<=height-SPACING; y+=SPACING){
       float* vec = new float[2];
       getNetVector(x,y, vec);
       float mag=vec[0];
       float dir=vec[1];
       glBegin(GL_LINES);
         glColor3f(0,sqrt(fabs(mag))/4,0);
         glVertex2f(x-(SPACING/2.5)*cos(dir),y-(SPACING/2.5)*sin(dir));
         glColor3f(sqrt(fabs(mag))/4,0,0);
         glVertex2f(x+(SPACING/2.5)*cos(dir),y+(SPACING/2.5)*sin(dir));
       glEnd();
       delete vec;
     }
   }
   
   if(!moveCharges)drawFieldLine(xstart,ystart);
   if(drawLines){
     for(float i=0; i<10; i++)drawFieldLine(width/2,10+i*(height-20)/9);
   }
   
   for(int c=0; c<visible; c++){
     float r=0,g=0;
     if(charges[c].charge<0)r=0.75-0.25*charges[c].charge;
     else g=0.75+0.25*charges[c].charge;
     glBegin(GL_POLYGON);
       glColor3f(r,g,0);
       glVertex2f(charges[c].x,charges[c].y);
       glColor3f(0.3*r,0.3*g,0);
       for(float angle=0; angle<=PI+PI/20; angle+=PI/20)
         glVertex2f(charges[c].x+charges[c].radius*cos(angle),charges[c].y+charges[c].radius*sin(angle));
     glEnd();
     glBegin(GL_POLYGON);
       glColor3f(r,g,0);
       glVertex2f(charges[c].x,charges[c].y);
       glColor3f(0.3*r,0.3*g,0);
       for(float angle=PI; angle<=2*PI+PI/20; angle+=PI/20)
         glVertex2f(charges[c].x+charges[c].radius*cos(angle),charges[c].y+charges[c].radius*sin(angle));
     glEnd();
     glColor3f(1,1,1);
     glBegin(GL_LINES);
       glVertex2f(charges[c].x-5,charges[c].y);
       glVertex2f(charges[c].x+5,charges[c].y);
       if(charges[c].charge>0){
         glVertex2f(charges[c].x,charges[c].y-5);
         glVertex2f(charges[c].x,charges[c].y+5);
       }
     glEnd();
   }

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

void mouse(int button, int state, int x, int y)
{
     y=height-y;
     if(state==GLUT_DOWN){
       for(int c=0; c<NUM; c++){
         float dist=sqrt((x-charges[c].x)*(x-charges[c].x)+(y-charges[c].y)*(y-charges[c].y));
         if(dist<charges[c].radius && button==GLUT_LEFT_BUTTON)selected=c;
         else if(dist<charges[c].radius)charges[c].charge*=-1;
       }
       }else selected=-1;
     glutPostRedisplay();
}

void motion(int x, int y)
{
     y=height-y;
     if(selected!=-1 && moveCharges){
       charges[selected].x=x;
       charges[selected].y=y;
     }
     xstart=x;
     ystart=y;
     glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
     switch(key){
       case 'm':
       case 'M':
         moveCharges=!moveCharges;
         break;
       case ',':
       case '<':
         if(visible>1)visible--;
         break;
       case '.':
       case '>':
         if(visible<NUM)visible++;
         break;
       case 27:
         exit(0);
         break;
       case 'r':
       case 'R':
         randomize();
         break;
       case 'l':
       case 'L':
         drawLines=!drawLines;
         break;
     }
     glutPostRedisplay();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (900, 600); 
   glutInitWindowPosition (50, 50);
   glutCreateWindow ("Electric Fields");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
