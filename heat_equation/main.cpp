#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <string>
#include <unistd.h>

#define PI 3.14159295358979
#define e 2.718281828
#define N 100
#define NCOLORS 6

using namespace std;

float width,height;
float temp[N+1][N+1];
float nTemp[N][N];  
int prevX,prevY;
float colors[NCOLORS*3]={1,0,0, 1,0.5,0, 1,1,0, 0,1,0, 0,0,1, 0,0,0}; 
bool paused=false,heater=true,blend=true;  
bool leftDown=false; 
bool advanced=false;
float t=0;    

void init(void) 
{
   glClearColor (0,0,0,0);
   srand(time(NULL));
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glShadeModel (GL_SMOOTH);
   glClear (GL_COLOR_BUFFER_BIT);
   for(int a=0; a<N; a++){
     for(int b=0; b<N; b++){
       temp[a][b]=0;
     }
   }

}

void drawCircle(float x, float y, float r)
{
     glBegin(GL_LINE_LOOP);
       for(float angle=0; angle<=2*PI; angle+=PI/20)
         glVertex2f(x+5*cos(angle),y+10*sin(angle));
     glEnd();
}

void setColor(float t)
{
   t=1-t;
   int ipart=(int)(t*(NCOLORS-1));
   if(t==0) ipart-=1;
   float fpart=t*(NCOLORS-1)-ipart;
   float r=(1-fpart)*colors[3*ipart]+fpart*colors[3*ipart+3];
   float g=(1-fpart)*colors[3*ipart+1]+fpart*colors[3*ipart+4];
   float b=(1-fpart)*colors[3*ipart+2]+fpart*colors[3*ipart+5];
   glColor3f(r,g,b);
}
     

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f(1,1,1);
   
   if(!paused || advanced){
       advanced=false;
       for(int a=0; a<N; a++){
         for(int b=0; b<N; b++){
           float c=0.15;
           //float c=((sin(3*(float)a/N*2*PI)+1.1)/2.1)*((sin(3*(float)b/N*2*PI)+1.1)/2.1);
           //float c=pow(e,-(a-N/2)*(a-N/2)/1000)/4;
           nTemp[a][b]=temp[a][b];
           if(b>0) nTemp[a][b]+=c/4*(temp[a][b-1]-temp[a][b]);
           if(b<N-1) nTemp[a][b]+=c/4*(temp[a][b+1]-temp[a][b]);
           if(a>0) nTemp[a][b]+=c/4*(temp[a-1][b]-temp[a][b]);
           if(a<N-1) nTemp[a][b]+=c/4*(temp[a+1][b]-temp[a][b]);
         }
       }
   
       for(int a=0; a<N; a++){
         for(int b=0; b<N; b++){
           temp[a][b]=nTemp[a][b];
           if((b==0 || b==N-1) && heater) temp[a][b]=1;
         }
       }
   }
   
//   glBegin(GL_LINES);
//   glLoadIdentity();
//   glRotatef(90,0,1,0);
//   glScalef(0.5,0.5,0.5);
//   for(int a=0; a<N; a++){
//     for(int b=0; b<N-1; b++){
//       setColor(temp[a][b]);
//       glVertex3f((float)a/(N-1),temp[a][b],-(float)b/(N-1));
//       if(blend) setColor(temp[a][b+1]);
//       glVertex3f((float)a/(N-1),temp[a][b+1],-(float)(b+1)/(N-1));
//     }
//   }
//   glEnd();

   
   glBegin(GL_QUADS);
   for(int a=0; a<N-1; a++){
     for(int b=0; b<N-1; b++){
       setColor(temp[a][b]);
       glVertex2f((float)a/(N-1),(float)b/(N-1));
       if(blend) setColor(temp[a+1][b]);
       glVertex2f((float)(a+1)/(N-1),(float)b/(N-1));
       if(blend) setColor(temp[a+1][b+1]);
       glVertex2f((float)(a+1)/(N-1),(float)(b+1)/(N-1));
       if(blend) setColor(temp[a][b+1]);
       glVertex2f((float)a/(N-1),(float)(b+1)/(N-1));
     }
   }
   glEnd();

   
   t+=.1;

   glFlush();
}

void idle()
{
     usleep(5000);
     if(paused)
       usleep(15000);
     glutPostRedisplay();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glOrtho(0,1,0,1,0,5);
   width=w;
   height=h;
}

void motion(int x, int y)
{
   if(x<width && x>0 && y>0 && y<height){
     temp[(int)(x/width*N)][(int)((height-y)/height*N)]=leftDown ? 1 : 0;
     temp[(int)(x/width*N)+1][(int)((height-y)/height*N)]=leftDown ? 1 : 0;
     temp[(int)(x/width*N)+1][(int)((height-y)/height*N)+1]=leftDown ? 1 : 0;
     temp[(int)(x/width*N)][(int)((height-y)/height*N)+1]=leftDown ? 1 : 0;
   }
   prevX=x;
   prevY=y;
}

void keyboard(unsigned char key, int x, int y)
{
     advanced=true;
     switch(key){
       case 'p':
       case 'P':
            paused=!paused;
            break;
       case 'H':
       case 'h':
            heater=!heater;
            break;
       case 'b':
       case 'B':
            blend=!blend;
            break;
       case 'c':
       case 'C':
            for(int a=0; a<N; a++){
              for(int b=0; b<N; b++){
                temp[a][b]=0;
              }
            }
            break;
     }
}

void mouse(int button, int state, int x, int y)
{
     leftDown=false;
     if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
       leftDown=true;
     prevX=x;
     prevY=y;
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (600,600); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("Gravity");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);
   glutMotionFunc(motion);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMainLoop();
   return 0;
}
