#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include "../ComplexClass/complex.cpp"
#include "quantum_functions.h"
#include "../GLText/GLText.cpp"
#include <unistd.h>

#ifndef PI
#define PI 3.1415926535898
#endif
#define MAXXRES 200
#define MAXYRES 200

int prevX,prevY;
bool leftButtonDown = 0;
float w,h;
float scaleFactor = 0.75;
float t=0;
const int ztrans=-5;
float colors [MAXXRES][MAXYRES][3];
int xres = 80;
int yres = 80;
int zres = 20;
int n=5,l=2,m=0;
float radMultMult = 4.5;
float radMult = pow(n,1.65)*radMultMult;
float colorMult = 3.0;
bool blendOn = 1, colorPhi = 1;
bool paused = 1;
int plane = 0;

int frameCount=0, FPS = 0;
float prevTime = 0;

float rot[16], invRot[16];

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glShadeModel (GL_SMOOTH);
   srand(time(NULL));
  
   //load the identity into the rotation matrix
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glRotatef(90,1,0,0);
   glGetFloatv(GL_MODELVIEW_MATRIX, rot);
   glRotatef(-180,1,0,0);
   glGetFloatv(GL_MODELVIEW_MATRIX, invRot);
}


void setup()
{
   glLoadIdentity();
  
   glTranslatef(-w/h+.15,-.85,ztrans);
   glMultMatrixf(rot);
   glScalef(0.1,0.1,0.1);
  
   glBegin(GL_LINES);
   glColor3f(1,0,0);
   glVertex3f(0,0,0);
   glVertex3f(1,0,0);
    
   glColor3f(0,1,0);
   glVertex3f(0,0,0);
   glVertex3f(0,1,0);
    
   glColor3f(0,0,1);
   glVertex3f(0,0,0);
   glVertex3f(0,0,1);
   glEnd();
  
   glColor3f(1,1,1);
   glLoadIdentity();
   glTranslatef(0,0,ztrans);
   glBegin(GL_LINE_LOOP);
   for(float a=0; a<2*PI; a+=PI/20)
     glVertex3f(scaleFactor*cos(a),scaleFactor*sin(a),0);
   glEnd();

   


   glLoadIdentity();
   glColor3f(1,1,1);
   DrawText(-w/h+0.30,-.75,ztrans,"n = ", GLUT_BITMAP_HELVETICA_12);
   DrawInt(n, GLUT_BITMAP_HELVETICA_12);
   DrawText(-w/h+0.30,-.80,ztrans,"l = ", GLUT_BITMAP_HELVETICA_12);
   DrawInt(l, GLUT_BITMAP_HELVETICA_12);
   DrawText(-w/h+0.30,-.85,ztrans,"m = ", GLUT_BITMAP_HELVETICA_12);
   DrawInt(m, GLUT_BITMAP_HELVETICA_12);

   DrawText(w/h-0.2, -.95, ztrans, "FPS = ", GLUT_BITMAP_HELVETICA_12);
   DrawInt(FPS, GLUT_BITMAP_HELVETICA_12);
}

void GetColor(float x, float y, float z, float c[3])
{
   x=x/scaleFactor;
   y=y/scaleFactor;
   z=z/scaleFactor;
   float rho = sqrt(x*x+y*y+z*z);
   float theta = acos(z/rho);
   float phi=0; 
   if(colorPhi) phi = atan2(y,x);
   
   Complex i (0,1);
    
   Complex value = WaveFunction(n,l,m,rho*radMult,theta,phi,t);
    
   SetPhaseColor(value.cangle(), 1, c);
    
   float mag = value.cabs()*value.cabs();
   c[0]*=mag; c[1]*=mag; c[2]*=mag;
    
}

void SetBasisVectors(float e1[3], float e2[3], float e3[3])
{
   e1[0] = invRot[0];
   e1[1] = invRot[1];
   e1[2] = invRot[2];
    
   e2[0] = invRot[4];
   e2[1] = invRot[5];
   e2[2] = invRot[6];
    
   e3[0] = invRot[8];
   e3[1] = invRot[9];
   e3[2] = invRot[10];
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);  
  
   float sq = scaleFactor; 
    
   glLoadIdentity();
   glTranslatef(0,0,-2);
   
   float x,y,z,X,Y,Z;
   float e1[3], e2[3], e3[3];
   SetBasisVectors(e1,e2,e3);

   float dx = 2*sq/(xres-1);
   float dy = 2*sq/(yres-1);

   if(plane==0) {
       for(int i=0; i<xres; i++){
             x = -sq+i*dx;
             for(int j=0; j<yres; j++){
                 y = -sq+j*dy;
                 colors[i][j][0]=colors[i][j][1]=colors[i][j][2] = 0.0;
                 float mag = x*x+y*y;
                 if(mag > scaleFactor*scaleFactor)
                     continue;
                 float zr = sqrt(scaleFactor*scaleFactor-mag);
                 float dz = 2*zr/(zres-1);
                 for(int k=0; k<zres; k++){
                     z = -zr+k*dz; 
                     if(x*x + y*y + z*z > scaleFactor*scaleFactor)
                         continue;      
                     //coordinates in atom's frame
                     X = x*e1[0] + y*e2[0] + z*e3[0];
                     Y = x*e1[1] + y*e2[1] + z*e3[1];
                     Z = x*e1[2] + y*e2[2] + z*e3[2];
            
                     float c[3];
                     GetColor(X,Y,Z,c);
                     colors[i][j][0] += (((float)k/zres*2-1)*0.6+1) * c[0]*colorMult;
                     colors[i][j][1] += (((float)k/zres*2-1)*0.6+1) * c[1]*colorMult;
                     colors[i][j][2] += (((float)k/zres*2-1)*0.6+1) * c[2]*colorMult;
                 }
            }
       }
   }else{
       for(int i=0; i<xres; i++){
             x = -sq+i*dx;
             for(int j=0; j<yres; j++){
                 y = -sq+j*dy;
                 float c[3];
                 if(plane==3)
                   GetColor(x,y,0,c);
                 else if(plane==2)
                   GetColor(x,0,y,c);
                 else
                   GetColor(0,x,y,c);
                 colors[i][j][0] = c[0]*colorMult*2;
                 colors[i][j][1] = c[1]*colorMult*2;
                 colors[i][j][2] = c[2]*colorMult*2;
            }
       }
   }
   
   if(blendOn){
       for(int i=0; i<xres-1; i++){
       x = -sq+i*dx;
           for(int j=0; j<yres-1; j++){
               y = -sq+j*dy;
               float avgr = (colors[i][j][0]+colors[i+1][j][0]+colors[i+1][j+1][0]+colors[i][j+1][0])/4;
               float avgg = (colors[i][j][1]+colors[i+1][j][1]+colors[i+1][j+1][1]+colors[i][j+1][1])/4;
               float avgb = (colors[i][j][2]+colors[i+1][j][2]+colors[i+1][j+1][2]+colors[i][j+1][2])/4;
               glBegin(GL_TRIANGLE_FAN);
                 glColor3f(avgr,avgg,avgb);
                 glVertex3f(x+dx/2,y+dy/2,0);
                 glColor3f(colors[i][j][0],colors[i][j][1],colors[i][j][2]);
                 glVertex3f(x,y,0);
                 glColor3f(colors[i+1][j][0],colors[i+1][j][1],colors[i+1][j][2]);
                 glVertex3f(x+dx,y,0);
                 glColor3f(colors[i+1][j+1][0],colors[i+1][j+1][1],colors[i+1][j+1][2]);
                 glVertex3f(x+dx,y+dy,0);
                 glColor3f(colors[i][j+1][0],colors[i][j+1][1],colors[i][j+1][2]);
                 glVertex3f(x,y+dy,0);
                 glColor3f(colors[i][j][0],colors[i][j][1],colors[i][j][2]);
                 glVertex3f(x,y,0);
               glEnd();
           }
       }
   } else {
       for(int i=0; i<xres; i++){
           x = -sq+i*dx;
           for(int j=0; j<yres; j++){
               y = -sq+j*dy;
               glColor3f(colors[i][j][0],colors[i][j][1],colors[i][j][2]);
               glRectf(x-dx/2, y-dy/2, x+dx/2, y+dy/2);
           }
       }
   }
  
   setup();
           
   glutSwapBuffers();
}

void reshape (int width, int height)
{
   const float ar = (float) width / (float) height;
   w=width;
   h=height;
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-ar, ar, -1.0, 1.0, 2.0, 100.0);
   
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity() ;
}


void mouse(int button, int state, int x, int y)
{
   prevX=x;
   prevY=y;
   leftButtonDown = 0;
   if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
       leftButtonDown = 1;
   glutPostRedisplay();
}

void motion(int x, int y)
{
   float rotX, rotY, totRot;
    
   if(leftButtonDown) {
       rotY=.8*(x-prevX);
       rotX=.8*(y-prevY);
       totRot = sqrt(rotX*rotX+rotY*rotY);
        
       glMatrixMode(GL_MODELVIEW);
       glLoadIdentity();
       glRotatef(totRot,rotX,rotY,0);
       glMultMatrixf(rot);
       glGetFloatv(GL_MODELVIEW_MATRIX, rot);
        
       glLoadMatrixf(invRot);
       glRotatef(-totRot,rotX,rotY,0);
       glGetFloatv(GL_MODELVIEW_MATRIX, invRot);
   } else {
       scaleFactor *= 1-(y-prevY)/100.0;
   }
     
   prevX=x;
   prevY=y;
   glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
   switch(key){
   case 'q':
   case 'Q':
     if(n<6)n++;
     break;
   case 'a':
   case 'A':
        if(n>1){
           n--;
           if(l>n-1) l=n-1;
           if(m>l) m=l;
           if(m<-l) m=-l;
        }
         break;
         
   case 'w':
   case 'W':
        if(l<n-1)l++;
        break;
   case 's':
   case 'S':
        if(l>0){
           l--;
           if(m>l) m=l;
           if(m<-l) m=-l;
        }
   break;
         
   case 'e':
   case 'E':
        if(m<l)m++;
        break;
   case 'd':
   case 'D':
        if(m>-l) m--;
        break;

   case '+':
   case '=':
        colorMult+=0.1;
        break;
   case '-':
   case '_':
        if(colorMult>0) colorMult-=0.1;
        break;
   case '[':
   case '{':
        radMultMult+=0.2;
        break;
   case ']':
   case '}':
        if(radMultMult>0) radMultMult-=0.2;
        break;
   case 'b':
   case 'B':
        blendOn = !blendOn;
        break;
   case 'C':
   case 'c':
        colorPhi = !colorPhi;
        break;
   case 'P':
   case 'p':
        paused = !paused;
        break;
   case '0':
   case '1':
   case '2':
   case '3':
        plane=key-'0';
        break;
   }
    
   radMult = pow(n,1.65)*radMultMult;
   glutPostRedisplay();
}

void idle()
{
   if(!paused){
       t=glutGet(GLUT_ELAPSED_TIME)/30000.0;
       if( t*30000 - prevTime > 1000 ) {
           prevTime = t*30000;
           FPS = frameCount;
           frameCount=0;
       }
       frameCount++;
   }
   glutPostRedisplay();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (700, 700); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("Hydrogen Atom");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutKeyboardFunc(keyboard);
   glutIdleFunc(idle);
   glutMainLoop();
   return 0;
}
