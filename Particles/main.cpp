#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <unistd.h>
#include "../GLText/GLText.cpp"

#define PI 3.14159265359
#define NUM 500

float width,height,mx=0,my=0;
float bounciness=0.4;
float resistance=0.008;
int numVisible=0;
int curPar=0;
int mode=2;
bool mouseIsDown=false;
bool isPaused=false;
int colorScheme;

class Particle{
      public:
        float x,y,vx,vy,rad,r,g,b,mass;
        Particle(float xt, float yt, float vxt, float vyt, float radt, float rt, float gt, float bt, float masst)
        {x=xt;y=yt;vx=vxt;vy=vyt;rad=radt;r=rt;g=gt;b=bt;mass=masst;}
        Particle()
        {x=0;y=0;vy=0;vx=0;rad=4;r=1;b=1;g=1;mass=1;}
        void Draw();
               
}particles[NUM];

void Particle::Draw()
{
  glBegin(GL_POLYGON);
    glColor4f(r,g,b,0.8);
    glVertex2f(x,y);
    glColor4f(r,g,b,0);
    for(float angle=0; angle<=PI+PI/20; angle+=PI/20)
      glVertex2f(x+rad*cos(angle),y+rad*sin(angle));
  glEnd();
  glBegin(GL_POLYGON);
    glColor4f(r,g,b,0.8);
    glVertex2f(x,y);
    glColor4f(r,g,b,0);
    for(float angle=PI; angle<=2*PI+PI/20; angle+=PI/20)
      glVertex2f(x+rad*cos(angle),y+rad*sin(angle));
  glEnd();
}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glShadeModel (GL_SMOOTH);
   srand (time(NULL));
   colorScheme=rand()%7+1;
}

void attractToMouse(int i)
{
     if(particles[i].x==mx)particles[i].x+=0.1;
     float theta=atan((particles[i].y-my)/(particles[i].x-mx));
     if(particles[i].x<mx)theta+=PI;
     float dist=sqrt((particles[i].x-mx)*(particles[i].x-mx)+(particles[i].y-my)*(particles[i].y-my));
     if(mode==1){
       particles[i].vx-=dist*cos(theta)/100/particles[i].mass/particles[i].rad;
       particles[i].vy-=dist*sin(theta)/100/particles[i].mass/particles[i].rad;
     }
     if(mode==2){
       if(dist<15)dist=15;
       particles[i].vx-=10*cos(theta)/particles[i].mass/sqrt(dist);
       particles[i].vy-=10*sin(theta)/particles[i].mass/sqrt(dist);
     }
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);

   DrawText(-width/2+2,-height/2+5,0,"Mode: ",GLUT_BITMAP_HELVETICA_12);
   DrawInt(mode,GLUT_BITMAP_HELVETICA_12);
   DrawText("  Particles: ",GLUT_BITMAP_HELVETICA_12);
   DrawInt(numVisible,GLUT_BITMAP_HELVETICA_12);

   for(int i=0; i<numVisible; i++){
     particles[i].x+=particles[i].vx;
     particles[i].y+=particles[i].vy;
     attractToMouse(i);
     particles[i].vx*=1-resistance;
     particles[i].vy*=1-resistance;
     particles[i].vy-=particles[i].y*fabs(particles[i].y)/height/300/particles[i].mass;
     particles[i].vx-=particles[i].x*fabs(particles[i].x)/width/300/particles[i].mass;
     if(particles[i].x>width/2){particles[i].x=width/2;particles[i].vx*=-bounciness;}
     if(particles[i].x<-width/2){particles[i].x=-width/2;particles[i].vx*=-bounciness;}
     if(particles[i].y>height/2){particles[i].y=height/2;particles[i].vy*=-bounciness;}
     if(particles[i].y<-height/2){particles[i].y=-height/2;particles[i].vy*=-bounciness;}
     particles[i].Draw();
   }


   glutSwapBuffers ();
}

void reshape (int w, int h)
{
   width=w;
   height=h;
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluOrtho2D (-width/2,width/2,-height/2,height/2);
}

void randomize(int i, bool a)
{
     particles[i].r=(float)(rand()%100+1)/100+((colorScheme==1||colorScheme==4||colorScheme==5||colorScheme==7)?0.5:0);
     particles[i].g=(float)(rand()%100+1)/100+((colorScheme==2||colorScheme==4||colorScheme==6||colorScheme==7)?0.5:0);
     particles[i].b=(float)(rand()%100+1)/100+((colorScheme==3||colorScheme==5||colorScheme==6||colorScheme==7)?0.5:0);
     particles[i].rad=(float)(rand()%31+10)/10;
     particles[i].mass=(float)(rand()%91+10)/10;
     float speed=rand()%10+1;
     float theta=(float)(rand()%100)/100*2*PI;
     if(a)theta=(float)i/NUM*2*PI;
     particles[i].vx=speed*cos(theta);
     particles[i].vy=speed*sin(theta);
}

void idle()
{
     usleep(10000);
     if(mouseIsDown){
       if(numVisible<NUM)numVisible++;
       int i=curPar;
       randomize(i,true);
       particles[i].x=mx;
       particles[i].y=my;
       curPar++;
       if(curPar==NUM)curPar=0;
     }
     if(!isPaused)glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
     mouseIsDown=true;
     if(state==GLUT_UP)mouseIsDown=false;
     if(state==GLUT_UP && button==GLUT_RIGHT_BUTTON)
       for(int i=0; i<numVisible; i++)randomize(i,false);
}

void motion(int x, int y)
{
     mx=x-width/2;
     my=(height-y)-height/2;
}

void passive(int x, int y)
{
     mx=x-width/2;
     my=(height-y)-height/2;
}

void keyboard(unsigned char key, int x, int y)
{
     switch(key){
       case 'p':
       case 'P':
         isPaused=!isPaused;
         break;
       case '1':
         mode=1;
         break;
       case '2':
         mode=2;
         break;
       case 'c':
       case 'C':
         numVisible=0;
         curPar=0;
         break;
     }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (900, 650); 
   glutInitWindowPosition (20, 20);
   glutCreateWindow ("Particles");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(passive);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
