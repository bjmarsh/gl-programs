#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <unistd.h>
#include "../GLText/GLText.cpp"

#define PI 3.14159265358908
#define NUM 625
#define INTERVALS 40

float width=400,height=400;
float friction=0.00;
float wallCushiness=0.0;
float upperKE=45;
float heaterVal=0.0;
float coolerVal=0.0;
float gravity=0.0;
float slowTime=1.0;
int shift=20, curTime, prevTime=0;
int energies[INTERVALS];
float pressure=0;
float prevPressure;
bool isBrownian=false;

struct Ball{
  float x,y,vx,vy,rad,mass,ke,momentum;
};

Ball balls[NUM];

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   glEnable (GL_BLEND);
   srand(time(NULL));
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   for(int i=0; i<25; i++){
     for(int c=0; c<25; c++){
       balls[i*25+c].x=rand()%(int)width;
       balls[i*25+c].y=rand()%(int)height;
       balls[i*25+c].rad=4;
       balls[i*25+c].vx=balls[i*25+c].vy=(float)(rand()%81-40)/10;
       balls[i*25+c].mass=2;
     }
   }
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   float totalKE=0;
   for(int i=0; i<NUM; i++){
     float speed=sqrt(balls[i].vx*balls[i].vx+balls[i].vy*balls[i].vy);
     balls[i].ke=.5*balls[i].mass*speed*speed;
     balls[i].momentum=balls[i].mass*speed;
     totalKE+=balls[i].ke;
     glColor3f(balls[i].ke/upperKE,1-fabs(upperKE/2-balls[i].ke)/upperKE*2,1-balls[i].ke/upperKE);
     glBegin(GL_POLYGON);
       for(float angle=0; angle<=2*PI; angle+=PI/10)
         glVertex2f(balls[i].x+balls[i].rad*cos(angle),balls[i].y+balls[i].rad*sin(angle));
     glEnd();
   }
   glColor4f(3*heaterVal,0,3*coolerVal,10*(heaterVal+coolerVal));
   glBegin(GL_POLYGON);
     glVertex2f(0,shift);
     glVertex2f(width,shift);
     glColor4f(5*heaterVal,0,5*coolerVal,0);
     glVertex2f(width,shift+20);
     glVertex2f(0,shift+20);
   glEnd();
   glColor3f(1,1,1);
   glBegin(GL_LINES);
     glVertex2f(0,shift);
     glVertex2f(width,shift);
   glEnd();
   DrawText(5,3,0,"T=", GLUT_BITMAP_HELVETICA_12);
   DrawFloat(10*totalKE/NUM, 3, GLUT_BITMAP_HELVETICA_12);
   curTime=time(NULL);
   if(curTime!=prevTime){
     prevPressure=pressure;
     pressure=0;
     prevTime=curTime;
   }
   DrawText("   P=", GLUT_BITMAP_HELVETICA_12);
   DrawFloat(prevPressure/10, 3, GLUT_BITMAP_HELVETICA_12);
   DrawText("   n=", GLUT_BITMAP_HELVETICA_12);
   DrawInt(NUM, GLUT_BITMAP_HELVETICA_12);
   DrawText("   V=", GLUT_BITMAP_HELVETICA_12);
   DrawFloat(height*width/1000, 3, GLUT_BITMAP_HELVETICA_12);
   
   int pos=300, w=3;
   for(int i=0; i<INTERVALS; i++){
     int h=10*shift*energies[i]/NUM;
     float ke=(float)(i*i)/balls[0].mass/2;
     glColor3f(ke/upperKE,1-fabs(upperKE/2-ke)/upperKE*2,1-ke/upperKE);
     glBegin(GL_QUADS);
       glVertex2f(w*i+pos,0);
       glVertex2f(w*i+pos+w,0);
       glVertex2f(w*i+pos+w,h);
       glVertex2f(w*i+pos,h);
     glEnd();
   }

       
   
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluOrtho2D (0,w,0,h);
   width=w;
   height=h-shift;
}

void mouse(int button, int state, int x, int y)
{
     balls[0].mass=2;
     balls[0].vx=5;
     balls[0].vy=5;
     if(button==GLUT_RIGHT_BUTTON)init();
}

void motion(int x, int y)
{
     
}

float getDist(float x1, float y1,float x2, float y2)
{
      return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

void idle()
{
     usleep(5000);
     if(isBrownian){
       balls[1].rad=20;
       balls[1].mass=70;
     }else{
       balls[1].rad=balls[0].rad;
       balls[1].mass=balls[0].mass;
     }
     for(int i=0; i<NUM; i++){
       balls[i].x+=balls[i].vx/slowTime;
       balls[i].y+=balls[i].vy/slowTime;
       balls[i].vx-=friction*balls[i].vx;
       balls[i].vy-=friction*balls[i].vy+gravity;
       if(balls[i].x<balls[i].rad){
         balls[i].x=balls[i].rad;
         balls[i].vx*=-(1-wallCushiness);
         pressure+=balls[i].vx*balls[i].mass;
       }
       if(balls[i].y<balls[i].rad+shift){
         balls[i].y=balls[i].rad+shift;
         balls[i].vy*=wallCushiness-1+coolerVal-((upperKE-balls[i].ke)*heaterVal>-1?(upperKE-balls[i].ke)*heaterVal:-heaterVal);
         pressure+=balls[i].vy*balls[i].mass;
       }
       if(balls[i].x>width-balls[i].rad){
         balls[i].x=width-balls[i].rad;
         balls[i].vx*=-(1-wallCushiness);
         pressure+=-balls[i].vx*balls[i].mass;
       }
       if(balls[i].y>height-balls[i].rad+shift){
         balls[i].y=height-balls[i].rad+shift;
         balls[i].vy*=-(1-wallCushiness);
         pressure+=-balls[i].vy*balls[i].mass;
       }
     }
     for(int i=0; i<NUM-1; i++){
       for(int c=i+1; c<NUM; c++){
         float dist=getDist(balls[i].x,balls[i].y,balls[c].x,balls[c].y);
         float prevFrame=getDist(balls[i].x-balls[i].vx,
                                 balls[i].y-balls[i].vy,
                                 balls[c].x-balls[c].vx,
                                 balls[c].y-balls[c].vy);
         if(dist<balls[i].rad+balls[c].rad && balls[i].x==balls[c].x)balls[i].x-=2;
         if(dist<balls[i].rad+balls[c].rad && balls[i].y==balls[c].y)balls[i].y-=2;
         if(dist<balls[i].rad+balls[c].rad && dist<prevFrame){
           float ax=balls[i].x-balls[c].x;
           float ay=balls[i].y-balls[c].y;
           float am=sqrt(ax*ax+ay*ay);
           float tx=-ay/ax;
           float ty=1;
           float tm=sqrt(tx*tx+ty*ty);
           float v1afx=((balls[i].vx*ax+balls[i].vy*ay)*(balls[i].mass-balls[c].mass)+2*balls[c].mass*(balls[c].vx*ax+balls[c].vy*ay))/(am*am*(balls[i].mass+balls[c].mass))*ax;
           float v1afy=v1afx/ax*ay;
           float v2afx=((balls[c].vx*ax+balls[c].vy*ay)*(balls[c].mass-balls[i].mass)+2*balls[i].mass*(balls[i].vx*ax+balls[i].vy*ay))/(am*am*(balls[i].mass+balls[c].mass))*ax;
           float v2afy=v2afx/ax*ay;
           float v1tfx=(balls[i].vx*tx+balls[i].vy*ty)/tm/tm*tx;
           float v1tfy=v1tfx/tx*ty;
           float v2tfx=(balls[c].vx*tx+balls[c].vy*ty)/tm/tm*tx;
           float v2tfy=v2tfx/tx*ty;
           balls[i].vx=v1afx+v1tfx;
           balls[i].vy=v1afy+v1tfy;
           balls[c].vx=v2afx+v2tfx;
           balls[c].vy=v2afy+v2tfy;
         }
       }
     }
     float maxP=30;
     float interval=maxP/INTERVALS;
     for(int c=0; c<INTERVALS; energies[c++]=0);
     for(int c=0; c<NUM; c++){
       int n=(int)(balls[c].momentum/interval);
       if(n<INTERVALS)
         energies[n]++;
     }
     glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
     switch(key){
       case 'h':
       case 'H':
            if(coolerVal<0.6)heaterVal=fabs(heaterVal-0.1);
            break;
       case 'c':
       case 'C':
            if(heaterVal<0.1)coolerVal=fabs(coolerVal-0.7);
            break;
       case 'g':
       case 'G':
            gravity=fabs(gravity-0.05);
            break;
       case 's':
       case 'S':
            for(int i=0; i<NUM; i++)
              balls[i].rad=fabs(balls[i].rad-6);
            break;
       case 'b':
       case 'B':
            isBrownian=!isBrownian;
            break;
     }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (400,420); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("Ideal Gas");
   
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   init ();
   glutMotionFunc(motion);
   glutMouseFunc(mouse);
   glutIdleFunc(idle);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
