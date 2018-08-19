#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>

/*
#define PI 3.1415926535898

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}
*/

void DrawDigitL(int __number, float __posX, float __posY, float __height, float __width, float __red, float __green, float __blue)
{
     glColor3f(__red,__green,__blue);
     glBegin(GL_LINE_STRIP);
       switch(__number){
         case 0:
              glVertex2f(__posX,__posY);
              glVertex2f(__posX+__width,__posY);
              glVertex2f(__posX+__width,__posY-__height);
              glVertex2f(__posX,__posY-__height);
              glVertex2f(__posX,__posY);
              break;
         case 1:
              glVertex2f(__posX,__posY);
              glVertex2f(__posX+__width/2,__posY);
              glVertex2f(__posX+__width/2,__posY-__height);
              glVertex2f(__posX+__width,__posY-__height);
              glVertex2f(__posX,__posY-__height);
              break;
         case 2:
              glVertex2f(__posX,__posY);
              glVertex2f(__posX+__width,__posY);
              glVertex2f(__posX+__width,__posY-__height/2);
              glVertex2f(__posX,__posY-__height/2);
              glVertex2f(__posX,__posY-__height);
              glVertex2f(__posX+__width,__posY-__height);
              break;  
         case 3:
              glVertex2f(__posX,__posY);
              glVertex2f(__posX+__width,__posY);
              glVertex2f(__posX+__width,__posY-__height/2);
              glVertex2f(__posX,__posY-__height/2);
              glVertex2f(__posX+__width,__posY-__height/2);
              glVertex2f(__posX+__width,__posY-__height);
              glVertex2f(__posX,__posY-__height);
              break;
         case 4:
              glVertex2f(__posX,__posY);
              glVertex2f(__posX,__posY-__height/2);
              glVertex2f(__posX+__width,__posY-__height/2);
              glVertex2f(__posX+__width,__posY);
              glVertex2f(__posX+__width,__posY-__height);
              break;
         case 5:
              glVertex2f(__posX+__width,__posY);
              glVertex2f(__posX,__posY);
              glVertex2f(__posX,__posY-__height/2);
              glVertex2f(__posX+__width,__posY-__height/2);
              glVertex2f(__posX+__width,__posY-__height);
              glVertex2f(__posX,__posY-__height);
              break;
         case 6:
              glVertex2f(__posX+__width,__posY);
              glVertex2f(__posX,__posY);
              glVertex2f(__posX,__posY-__height/2);
              glVertex2f(__posX+__width,__posY-__height/2);
              glVertex2f(__posX+__width,__posY-__height);
              glVertex2f(__posX,__posY-__height);
              glVertex2f(__posX,__posY-__height/2);
              break;
         case 7:
              glVertex2f(__posX,__posY);
              glVertex2f(__posX+__width,__posY);
              glVertex2f(__posX+__width,__posY-__height);
              break;  
         case 8:
              glVertex2f(__posX,__posY);
              glVertex2f(__posX+__width,__posY);
              glVertex2f(__posX+__width,__posY-__height);
              glVertex2f(__posX,__posY-__height);
              glVertex2f(__posX,__posY);
              glVertex2f(__posX,__posY-__height/2);
              glVertex2f(__posX+__width,__posY-__height/2);
              break;
         case 9:
              glVertex2f(__posX+__width,__posY-__height/2);
              glVertex2f(__posX+__width,__posY);
              glVertex2f(__posX,__posY);
              glVertex2f(__posX,__posY-__height/2);
              glVertex2f(__posX+__width,__posY-__height/2);
              glVertex2f(__posX+__width,__posY-__height);
              glVertex2f(__posX,__posY-__height);
              break;       
       }
     glEnd();
}

void DrawDigit3D(int __number, float __posX, float __posY, float __posZ, float __height, float __width, float __red, float __green, float __blue)
{
     glColor3f(__red,__green,__blue);
     glBegin(GL_LINE_STRIP);
       switch(__number){
         case 0:
              glVertex3f(__posX,__posY, __posZ);
              glVertex3f(__posX+__width,__posY, __posZ);
              glVertex3f(__posX+__width,__posY-__height, __posZ);
              glVertex3f(__posX,__posY-__height, __posZ);
              glVertex3f(__posX,__posY, __posZ);
              break;
         case 1:
              glVertex3f(__posX,__posY, __posZ);
              glVertex3f(__posX+__width/2,__posY, __posZ);
              glVertex3f(__posX+__width/2,__posY-__height, __posZ);
              glVertex3f(__posX+__width,__posY-__height, __posZ);
              glVertex3f(__posX,__posY-__height, __posZ);
              break;
         case 2:
              glVertex3f(__posX,__posY, __posZ);
              glVertex3f(__posX+__width,__posY, __posZ);
              glVertex3f(__posX+__width,__posY-__height/2, __posZ);
              glVertex3f(__posX,__posY-__height/2, __posZ);
              glVertex3f(__posX,__posY-__height, __posZ);
              glVertex3f(__posX+__width,__posY-__height, __posZ);
              break;  
         case 3:
              glVertex3f(__posX,__posY, __posZ);
              glVertex3f(__posX+__width,__posY, __posZ);
              glVertex3f(__posX+__width,__posY-__height/2, __posZ);
              glVertex3f(__posX,__posY-__height/2, __posZ);
              glVertex3f(__posX+__width,__posY-__height/2, __posZ);
              glVertex3f(__posX+__width,__posY-__height, __posZ);
              glVertex3f(__posX,__posY-__height, __posZ);
              break;
         case 4:
              glVertex3f(__posX,__posY, __posZ);
              glVertex3f(__posX,__posY-__height/2, __posZ);
              glVertex3f(__posX+__width,__posY-__height/2, __posZ);
              glVertex3f(__posX+__width,__posY, __posZ);
              glVertex3f(__posX+__width,__posY-__height, __posZ);
              break;
         case 5:
              glVertex3f(__posX+__width,__posY, __posZ);
              glVertex3f(__posX,__posY, __posZ);
              glVertex3f(__posX,__posY-__height/2, __posZ);
              glVertex3f(__posX+__width,__posY-__height/2, __posZ);
              glVertex3f(__posX+__width,__posY-__height, __posZ);
              glVertex3f(__posX,__posY-__height, __posZ);
              break;
         case 6:
              glVertex3f(__posX+__width,__posY, __posZ);
              glVertex3f(__posX,__posY, __posZ);
              glVertex3f(__posX,__posY-__height/2, __posZ);
              glVertex3f(__posX+__width,__posY-__height/2, __posZ);
              glVertex3f(__posX+__width,__posY-__height, __posZ);
              glVertex3f(__posX,__posY-__height, __posZ);
              glVertex3f(__posX,__posY-__height/2, __posZ);
              break;
         case 7:
              glVertex3f(__posX,__posY, __posZ);
              glVertex3f(__posX+__width,__posY, __posZ);
              glVertex3f(__posX+__width,__posY-__height, __posZ);
              break;  
         case 8:
              glVertex3f(__posX,__posY, __posZ);
              glVertex3f(__posX+__width,__posY, __posZ);
              glVertex3f(__posX+__width,__posY-__height, __posZ);
              glVertex3f(__posX,__posY-__height, __posZ);
              glVertex3f(__posX,__posY, __posZ);
              glVertex3f(__posX,__posY-__height/2, __posZ);
              glVertex3f(__posX+__width,__posY-__height/2, __posZ);
              break;
         case 9:
              glVertex3f(__posX+__width,__posY-__height/2, __posZ);
              glVertex3f(__posX+__width,__posY, __posZ);
              glVertex3f(__posX,__posY, __posZ);
              glVertex3f(__posX,__posY-__height/2, __posZ);
              glVertex3f(__posX+__width,__posY-__height/2, __posZ);
              glVertex3f(__posX+__width,__posY-__height, __posZ);
              glVertex3f(__posX,__posY-__height, __posZ);
              break;       
       }
     glEnd();
}

void DrawIntL(double __number, float __posX, float __posY, float __height, float __width, float __red, float __green, float __blue)
{
     if(__number==0){
        DrawDigitL(0,__posX,__posY,__height,__width,__red,__green,__blue);
        return;
     }                     
     int digits=floor(log10(__number))+1;
     __number/=pow(10,digits-1);
     for(int i=0; i<digits; i++){
        double dig=floor(__number);
        DrawDigitL(dig,i*__width+__posX+3*i,__posY,__height,__width,__red,__green,__blue);
        __number-=dig;
        __number*=10;
     }
}

void DrawInt3D(double __number, float __posX, float __posY, float __posZ, float __height, float __width, float __red, float __green, float __blue)
{
     if(__number==0){
        DrawDigit3D(0,__posX,__posY, __posZ, __height,__width,__red,__green,__blue);
        return;
     }                     
     int digits=floor(log10(__number))+1;
     __number/=pow(10,digits-1);
     for(int i=0; i<digits; i++){
        double dig=floor(__number);
        DrawDigit3D(dig,i*__width+__posX+3*i,__posY, __posZ, __height,__width,__red,__green,__blue);
        __number-=dig;
        __number*=10;
     }
}


void DrawDecimalL(double __number, int __decSpots, float __posX, float __posY, float __height, float __width, float __red, float __green, float __blue)
{
     DrawIntL(floor(__number),__posX,__posY,__height,__width,__red,__green,__blue);
     int digits=floor(log10(__number))+1;
     if(__number<1)digits=1;
     __posX+=__width*digits+3*digits;
     glBegin(GL_POINTS);
       glVertex2f(__posX,__posY-__height);
       glVertex2f(__posX+1,__posY-__height);
       glVertex2f(__posX,__posY-__height+1);
       glVertex2f(__posX+1,__posY-__height+1);
     glEnd();
     __posX+=4;
     __number-=floor(__number);
     __number*=pow(10,__decSpots);
     DrawIntL(__number,__posX,__posY,__height,__width,__red,__green,__blue);
}

void DrawDecimal3D(double __number, int __decSpots, float __posX, float __posY, float __posZ, float __height, float __width, float __red, float __green, float __blue)
{
     DrawInt3D(floor(__number),__posX,__posY, __posZ, __height,__width,__red,__green,__blue);
     int digits=floor(log10(__number))+1;
     if(__number<1)digits=1;
     __posX+=__width*digits+3*digits;
     glBegin(GL_POINTS);
       glVertex2f(__posX,__posY-__height);
       glVertex2f(__posX+1,__posY-__height);
       glVertex2f(__posX,__posY-__height+1);
       glVertex2f(__posX+1,__posY-__height+1);
     glEnd();
     __posX+=4;
     __number-=floor(__number);
     __number*=pow(10,__decSpots);
     DrawInt3D(__number,__posX,__posY, __posZ,__height,__width,__red,__green,__blue);
}

/*     
void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);

   DrawDigitL(0,0,100,20,10,1,1,1);
   DrawDigitL(1,15,100,20,10,1,1,1);
   DrawDigitL(2,30,100,20,10,1,1,1);
   DrawDigitL(3,45,100,20,10,1,1,1);
   DrawDigitL(4,60,100,20,10,1,1,1);
   DrawDigitL(5,75,100,20,10,1,1,1);
   DrawDigitL(6,90,100,20,10,1,1,1);
   DrawDigitL(7,105,100,20,10,1,1,1);
   DrawDigitL(8,120,100,20,10,1,1,1);
   DrawDigitL(9,135,100,20,10,1,1,1);
   
   DrawDecimalL(0.367,3,0,300,20,10,1,1,1);

   glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (400, 400); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("__numbers");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;
}
*/
