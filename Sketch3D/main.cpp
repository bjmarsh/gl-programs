#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "../GLText/GLText.cpp"

#define PI 3.14159265359
#define NUM 10000
#define COLORS 11

float width,height;
float xRot=0, yRot=0;
float scale=1;
int prevX,prevY;
bool rightDown=false;
bool leftDown=false;
int rotateState=0;
int rightClick=0;
bool axesOn=true;
bool symmetryOn=false;
bool showInfo=true;
float coords[NUM][5];
int num=0;
int clickInColor=-1;
int currentColor=8;
int lineWidth=2;
float aspectRatio=1.0;
float transx=0, transy=0;
float colors[COLORS][3]={{1,0,0},{1,0.5,0},{1,1,0},{0,1,0},{0,0,1},{0.29,0,0.51},{0.5,0,1},{0.93,0.51,0.93},{1,1,1},{0.5,0.5,0.5},{0.59,0.29,0}};
string colorNames[COLORS]={"Red","Orange","Yellow","Green","Blue","Indigo","Violet","Pink","White","Gray","Brown"};
void* font=GLUT_BITMAP_HELVETICA_12;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glShadeModel (GL_SMOOTH);
   srand(time(NULL));
   for(int i=0; i<NUM; i++){
     for(int c=0; c<5; c++)
       coords[i][c]=0;
   }
}

float* transform(float x, float y, float c)
{
   float coord[3];
   float r1=sqrt(y*y+c*c);
   float txz=atan(y/c);
   if(c<0)txz+=PI;
   float z2=r1*cos(txz+xRot*PI/180);
   float r2=sqrt(x*x+z2*z2);
   float tx=atan(z2/x);
   if(x<0)tx+=PI;
   coord[0]=r2*cos(tx+yRot*PI/180);
   coord[1]=r1*sin(txz+xRot*PI/180);
   coord[2]=r2*sin(tx+yRot*PI/180);
   return coord;
}

void drawStrip(int a, int b, int c)
{
     int signs[3]={a/abs(a),b/abs(b),c/abs(c)};
     a=abs(a)-1;
     b=abs(b)-1;
     c=abs(c)-1;
     glLineWidth((int)coords[0][4]);
     glBegin(GL_LINE_STRIP);
         for(int i=0; i<num; i++){
           if(coords[i][0]!=0 && coords[i][1]!=0 && coords[i][2]!=0){
             glColor3f(colors[(int)coords[i][3]][0],colors[(int)coords[i][3]][1],colors[(int)coords[i][3]][2]);
             glVertex3f(signs[0]*coords[i][a],signs[1]*coords[i][b],signs[2]*coords[i][c]);
           }else{
             glEnd();
             glLineWidth((int)coords[i+1][4]);
             glBegin(GL_LINE_STRIP);
           }
         }
     glEnd();
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);

   glLoadIdentity();   
   glTranslatef(transx,transy,-5);
   glRotatef(xRot,1,0,0);
   glRotatef(yRot,0,1,0);
   glScalef(scale,scale,scale);

   glColor3f(1,1,1);
   glLineWidth(1);
   if(axesOn){
     glBegin(GL_LINES);
       glVertex3f(-0.5,0,0);
       glVertex3f(0.5,0,0);
       glVertex3f(0,-0.5,0);
       glVertex3f(0,0.5,0);
       glVertex3f(0,0,-0.5);
       glVertex3f(0,0,0.5);
     glEnd();
     glColor4f(0.7,0.7,0.7,0.05);
     glutSolidSphere(0.5/scale,16,16);
   }
   
   if(symmetryOn){
     for(int a=1; a<=3; a++){
       for(int b=1; b<=3; b++){
         if(a!=b){
           int c=6-a-b;
           drawStrip(a,b,c);
           drawStrip(-a,b,c);
           drawStrip(-a,-b,c);
           drawStrip(-a,b,-c);
           drawStrip(a,-b,c);
           drawStrip(a,-b,-c);
           drawStrip(a,b,-c);
           drawStrip(-a,-b,-c);
         }
       }
     }
   }else drawStrip(1,2,3);
   
   glLoadIdentity();
   for(int c=0; c<COLORS; c++){
     glColor3f(colors[c][0],colors[c][1],colors[c][2]);
     glBegin(GL_QUADS);
       glVertex3f(-1+c*2.0/COLORS,-1,-2);
       glVertex3f(-1+(c+1)*2.0/COLORS,-1,-2);
       glVertex3f(-1+(c+1)*2.0/COLORS,-0.95,-2);
       glVertex3f(-1+c*2.0/COLORS,-0.95,-2);
     glEnd();
   }
   
   glColor3f(1,1,1);
   if(showInfo){
     DrawText(-aspectRatio,0.95,-2,"Points: ",font);
     DrawInt(num,font);
     DrawText("/10000",font);
     DrawText(-aspectRatio,0.95-2/height*12,-2,"Color: ",font);
     glColor3f(colors[currentColor][0],colors[currentColor][1],colors[currentColor][2]);
     DrawText(-aspectRatio+2/height*37,0.95-2/height*12,-2,colorNames[currentColor],font);
     glColor3f(1,1,1);
     DrawText(-aspectRatio,0.95-2/height*24,-2,"Scale: ",font);
     DrawFloat(scale,3,font);
     DrawText(-aspectRatio,0.95-2/height*36,-2,"Auto-Rotate Mode (r): ",font);
     DrawText((rotateState==0)?"Off":(rotateState==1)?"Fast":"Slow",font);
     DrawText(-aspectRatio,0.95-2/height*48,-2,"Symmetry (s): ",font);
     DrawText((symmetryOn)?"On":"Off",font);
     DrawText(-aspectRatio,0.95-2/height*60,-2,"Axes (a): ",font);
     DrawText((axesOn)?"On":"Off",font);
     DrawText(-aspectRatio,0.95-2/height*72,-2,"Line width (+/-): ",font);
     DrawInt(lineWidth,font);
     DrawText(-aspectRatio,0.95-2/height*84,-2,"Right button (m): ",font);
     DrawText((rightClick==2)?"Translate":(rightClick==0)?"Rotate":"Scale",font);
   }

   glutSwapBuffers ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   width=w;
   height=h;
   aspectRatio=width/height;
   glOrtho(-aspectRatio,aspectRatio,-1.0,1.0,2.0,10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity() ;
}

void idle()
{
     usleep(20000);
     if(rotateState){
       xRot+=1.5-(float)(rotateState==2)*1.2;
       yRot+=1-(float)(rotateState==2)*0.8;
       if(leftDown && clickInColor==-1){
         float x=((float)prevX-width/2)*(2*aspectRatio/width);
         float y=-((float)prevY-height/2)*(2/height);
         x-=transx;
         y-=transy;
         x/=scale;
         y/=scale;
         float c=0.5/scale;
         float* trans=transform(x,y,c);
         coords[num][0]=trans[0];
         coords[num][1]=trans[1];
         coords[num][2]=trans[2];
         coords[num][3]=currentColor;
         coords[num][4]=lineWidth;
         num++;
         if(num>NUM-1)num=0;
       }
     }
     glutPostRedisplay();
}

void motion(int xm, int ym)
{
     if(rightDown && rightClick==0){
       if(xm>prevX)yRot+=abs(xm-prevX);
       if(xm<prevX)yRot-=abs(xm-prevX);
       if(ym>prevY)xRot+=abs(ym-prevY);
       if(ym<prevY)xRot-=abs(ym-prevY);
     }
     if(rightDown && rightClick==1){
       scale-=(float)(ym-prevY)/10;
       if(scale<=0.1)scale=0.1;
     }
     if(rightDown && rightClick==2){
       transx+=(float)(xm-prevX)/50;
       transy+=(float)(prevY-ym)/50;
     }
     if(leftDown && !rotateState && clickInColor==-1){
       float x=((float)xm-width/2)*(2*aspectRatio/width);
       float y=-((float)ym-height/2)*(2/height);
       float c=0.5/scale;
       x-=transx;
       y-=transy;
       x/=scale;
       y/=scale;
       float* trans=transform(x,y,c);
       coords[num][0]=trans[0];
       coords[num][1]=trans[1];
       coords[num][2]=trans[2];
       coords[num][3]=currentColor;
       coords[num][4]=lineWidth;
       num++;
       if(num>NUM-1)num=0;
     }
     prevX=xm;
     prevY=ym;
}

void mouse(int button, int state, int xm, int ym)
{
     prevX=xm;
     prevY=ym;
     rightDown=false;
     leftDown=false;
     float x=((float)xm-width/2)*(2*aspectRatio/width);
     float y=-((float)ym-height/2)*(2/height);
     x-=transx;
     y-=transy;
     x/=scale;
     y/=scale;
     if(state==GLUT_UP && button==GLUT_LEFT_BUTTON){
       if(y*scale<-0.95 && x*scale<1 && x*scale>-1){
         if((int)((x*scale+1)/(2.0/COLORS))==clickInColor)
           currentColor=clickInColor;
       }else{
         coords[num][0]=0;
         coords[num][1]=0;
         coords[num][2]=0;
         coords[num][3]=currentColor;
         coords[num][4]=lineWidth;
         num++;
         if(num>NUM-1)num=0;
       
       }
       clickInColor=-1;
     }
     if(state==GLUT_DOWN && button==GLUT_RIGHT_BUTTON)
       rightDown=true;
     if(state==GLUT_DOWN && button==GLUT_LEFT_BUTTON){
       leftDown=true;
       if(y*scale<-0.95 && x*scale<1 && x*scale>-1){
         clickInColor=(int)((x*scale+1)/(2.0/COLORS));
       }
     }
       
}

void keyboard(unsigned char key, int x, int y)
{
     switch(key){
       case 'r':
       case 'R':
         rotateState=(rotateState+1)%3;
         break;
       case 'c':
       case 'C':
         for(int i=0; i<NUM; i++){
           for(int c=0; c<5; c++)
             coords[i][c]=0;
         }
         num=0;
         break;
       case 'a':
       case 'A':
         axesOn=!axesOn;
         break;
       case 'm':
       case 'M':
         rightClick=(rightClick+1)%3;
         break;
       case 's':
       case 'S':
         symmetryOn=!symmetryOn;
         break;
       case '+':
       case '=':
         lineWidth++;
         break;
       case '-':
       case '_':
         lineWidth-=(lineWidth>1);
         break;
       case 'z':
       case 'Z':
         xRot=yRot=0;
         scale=1;
         transx=0;
         transy=0;
         break;
       case 'i':
       case 'I':
         showInfo=!showInfo;
         break;
     }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("Doodle3D");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMotionFunc(motion);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutIdleFunc(idle);
   glutMainLoop();
   return 0;
}
