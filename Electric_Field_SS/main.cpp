#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <unistd.h>

#define NUM 10
#define PI 3.14159265358979
#define SPACING 7
#define MAXL 20
#define SCREENSAVER true
#define MAXFRAMES 300

float width=900,height=600;
int visible=7;
int startx=-1,starty;
int frameCount=0;
float fade=1;
int NUML=15;

struct Charge{
     float charge,x,y,radius,velx,vely; 
};  

struct FieldLine{
     float x,y,velx,vely,r,g,b;
};

Charge charges[NUM];
FieldLine lines[MAXL];
FieldLine lines2[MAXL];
FieldLine lines3[MAXL];

void randomize()
{
   for(int c=0;c<NUM; c++){
     charges[c].charge=rand()%2+1;
     if(rand()%2)charges[c].charge*=-1;
     charges[c].x=rand()%((int)width-100)+50;
     charges[c].y=rand()%((int)height-100)+50;
     charges[c].radius=10;
     charges[c].velx=rand()%11-5;
     charges[c].vely=rand()%11-5;
   }
   for(int l=0;l<NUML; l++){
     lines[l].x=rand()%((int)width-100)+50;
     lines[l].y=rand()%((int)height-100)+50;
     lines[l].velx=rand()%11-5;
     lines[l].vely=rand()%11-5;
     lines[l].r=(float)(rand()%101)/100;
     lines[l].g=(float)(rand()%101)/100;
     lines[l].b=(float)(rand()%101)/100;
   }
   visible=rand()%9+2;
   NUML=rand()%(MAXL-4)+5;
} 

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   srand(time(NULL));
   randomize();
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glLineWidth(3);
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
       
void drawFieldLine(float x, float y,float r,float g,float b, float a)
{
     float xc=x,yc=y,xp,yp;
     for(int i=0; i<100; i++){
       xp=xc;
       yp=yc;
       float *vec = new float[2];
       getNetVector(xc,yc, vec);
       xc=xp+7*cos(vec[1]);
       yc=yp+7*sin(vec[1]);
       glColor4f(1-(1-(float)i/50)*r,1-(1-(float)i/50)*g,1-(1-(float)i/50)*b,(1-(float)i/100)*a*fade);
       glBegin(GL_LINES);
         glVertex2f(xp,yp);
         glVertex2f(xc,yc);
       glEnd();
       delete vec;
     }
     xc=x,yc=y;
     for(int i=0; i<100; i++){
       xp=xc;
       yp=yc;
       float *vec = new float[2];
       getNetVector(xc,yc, vec);
       xc=xp-7*cos(vec[1]);
       yc=yp-7*sin(vec[1]);
       glColor4f(1-(1-(float)i/50)*r,1-(1-(float)i/50)*g,1-(1-(float)i/50)*b,(1-(float)i/100)*a*fade);
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

   for(int i=0; i<NUML; i++){
     drawFieldLine(lines[i].x,lines[i].y,lines[i].r,lines[i].g,lines[i].b,1);
     drawFieldLine(lines2[i].x,lines2[i].y,lines2[i].r,lines2[i].g,lines2[i].b,0.2);
     drawFieldLine(lines3[i].x,lines3[i].y,lines3[i].r,lines3[i].g,lines3[i].b,0.1);
   }
   
   for(int c=0; c<visible; c++){
     glColor3f(0,0,0);
     glBegin(GL_POLYGON);
       for(float a=0; a<2*PI; a+=PI/5)
         glVertex2f(charges[c].x+8*cos(a),charges[c].y+8*sin(a));
     glEnd();
   }

   glutSwapBuffers ();
}

void idle()
{
     usleep((25-visible)*1000.0);
     for(int c=0; c<NUM; c++){
       charges[c].x+=charges[c].velx;
       charges[c].y+=charges[c].vely;
       if(charges[c].x<0){charges[c].x=0;charges[c].velx*=-1;}
       if(charges[c].y<0){charges[c].y=0;charges[c].vely*=-1;}
       if(charges[c].x>width){charges[c].x=width;charges[c].velx*=-1;}
       if(charges[c].y>height){charges[c].y=height;charges[c].vely*=-1;}
     }
     for(int l=0; l<NUML; l++){
       lines3[l]=lines2[l];
       lines2[l]=lines[l];
       lines[l].x+=lines[l].velx;
       lines[l].y+=lines[l].vely;
       if(lines[l].x<0){lines[l].x=0;lines[l].velx*=-1;}
       if(lines[l].y<0){lines[l].y=0;lines[l].vely*=-1;}
       if(lines[l].x>width){lines[l].x=width;lines[l].velx*=-1;}
       if(lines[l].y>height){lines[l].y=height;lines[l].vely*=-1;}
     }
     frameCount++;
     if(frameCount<=50)fade=(float)frameCount/50;
     if(frameCount>=MAXFRAMES-50)fade=1-((float)frameCount-MAXFRAMES+50)/50;
     if(frameCount==MAXFRAMES){
       frameCount=0;
       randomize();
     }
     glutPostRedisplay();
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

void keyboard(unsigned char key, int x, int y)
{
     switch(key){
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
     }
     glutPostRedisplay();
}

void passive(int x, int y)   //detects movement and exits if in scn svr mode
{
     if(startx==-1){
       startx=x;
       starty=y;
     }else if(SCREENSAVER){
       if(abs(x-startx)>=4 || abs(y-starty)>=4)
       exit(0);
     }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (900, 600); 
   glutInitWindowPosition (50, 50);
   glutCreateWindow ("Electric Fields");
   if(SCREENSAVER){
     glutFullScreen();
     glutSetCursor(GLUT_CURSOR_NONE);
   }
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutIdleFunc(idle);
   glutPassiveMotionFunc(passive);
   glutMainLoop();
   return 0;
}
