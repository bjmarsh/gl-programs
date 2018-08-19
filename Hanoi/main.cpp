#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <unistd.h>

#define NUM_DISKS 7
#define MAX_NUM_DISKS 10
#define PI 3.1415926535898


float width,height;
bool diskIsClicked=false;
int clickedTower=0;
int moves=0;


class Disk{
      public:
        int size;
        Disk(int _size){size=_size;}
        Disk(){size=0;}
        void SetValues(int _size);
        void Draw(float x, float y);
        void operator = (Disk);
}clickedDisk;

void Disk::operator=(Disk disk)
{
     size=disk.size;
}

class Tower{
      public:
        float pos;
        int numDisks;
        Tower(){ numDisks=0; }
        Disk stack[NUM_DISKS];
        void Push(Disk);
        Disk Pop();
        void Draw(float bottom, float top);
}towers[3];

void Disk::SetValues(int _size)
{
   size=_size;
}

void Tower::Push(Disk disk)
{        
    stack[numDisks]=disk;
    numDisks++;
}

Disk Tower::Pop()
{
     return stack[--numDisks];
}

void Tower::Draw(float bottom, float top)
{
     glLoadIdentity();
     glColor4f(1,1,1,0.4);
     glTranslatef(pos,-1,-2.5);
     glRotatef(15,1,0,0);
     glBegin(GL_POLYGON);
       for(float ang=0; ang<=2*PI; ang+=PI/20)
          glVertex3f(0.02*cos(ang),top+1,0.02*sin(ang));
     glEnd();
     glBegin(GL_POLYGON);
       for(float ang=0; ang<=2*PI; ang+=PI/20)
          glVertex3f(0.02*cos(ang),bottom+1,0.02*sin(ang));
     glEnd();
     glBegin(GL_QUADS);
       for(float ang=0; ang<=2*PI; ang+=PI/20){
          glVertex3f(0.02*cos(ang),top+1,0.02*sin(ang));
          glVertex3f(0.02*cos(ang),bottom+1,0.02*sin(ang));
          glVertex3f(0.02*cos(ang+PI/20),bottom+1,0.02*sin(ang+PI/20));
          glVertex3f(0.02*cos(ang+PI/20),top+1,0.02*sin(ang+PI/20));
       }
     glEnd();
}

void ClickTower(int tower)
{
     if(!diskIsClicked && tower[towers].numDisks>0){
        clickedTower=tower;
        clickedDisk=towers[tower].Pop();
        diskIsClicked=true;
     }else if(diskIsClicked){
        towers[tower].Push(clickedDisk);
        moves++;
        if(towers[tower].numDisks>1 && clickedDisk.size>towers[tower].stack[towers[tower].numDisks-2].size){
           towers[clickedTower].Push(towers[tower].Pop());
           moves--;
        }
        diskIsClicked=false;
     }
     glutPostRedisplay();
}

void Restart()
{
     towers[0].numDisks=0;
     towers[1].numDisks=0;
     towers[2].numDisks=0;
     for(int i=NUM_DISKS-1; i>=0; i--)
       towers[0].Push(Disk(i+1));
     diskIsClicked=false;
     moves=0;
     glutPostRedisplay();
}

void Solve(int n, int from, int to, int by)
{
     if(n>0){
        Solve(n-1, from, by, to);
        towers[to].Push(towers[from].Pop());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(int b=0; b<3; b++){
          towers[b].Draw(-1,towers[b].numDisks*1/(float)MAX_NUM_DISKS-1);
          if(towers[b].numDisks>0){
            for(float d=0; d<towers[b].numDisks; d++)
              towers[b].stack[(int)d].Draw(towers[b].pos,(d+1)*1/MAX_NUM_DISKS);
          }
          towers[b].Draw(towers[b].numDisks*1/(float)MAX_NUM_DISKS-1,0);
        }
        moves++;
        glutSwapBuffers();
        usleep(20000);
        Solve(n-1, by, to, from);
     }
}

void Disk::Draw(float x, float y)
{
     switch(size){
        case 1:
           glColor4f(1,0,0,0.4);
           break;
        case 2:
           glColor4f(1,0.5,0,0.4);
           break;
        case 3:
           glColor4f(1,1,0,0.4);
           break;
        case 4:
           glColor4f(0,1,0,0.4);
           break;
        case 5:
           glColor4f(0,0,1,0.4);
           break;
        case 6:
           glColor4f(0.5,0,1,0.4);
           break;
        case 7:
           glColor4f(1,0,1,0.4);
           break;
        case 8:
           glColor4f(1,0,0,0.4);
           break;
        case 9:
           glColor4f(1,0.5,0,0.4);
           break;
        case 10:
           glColor4f(1,1,0,0.4);
           break;
     }
     glLoadIdentity();
     glTranslatef(x,-1,-2.5);
     glRotatef(15,1,0,0);
     float diskWidth=0.4-0.03*(NUM_DISKS-size);
     
     glBegin(GL_POLYGON);
        for(float angle=0; angle<=2*PI; angle+=2*PI/100)
          glVertex3f(diskWidth/2*cos(angle),y-1/(float)MAX_NUM_DISKS,diskWidth/2*sin(angle));
     glEnd();
     
     glBegin(GL_QUADS);
        for(float angle=0; angle<=2*PI; angle+=2*PI/100){
          glVertex3f(diskWidth/2*cos(angle),y,diskWidth/2*sin(angle));
          glVertex3f(diskWidth/2*cos(angle),y-1/(float)MAX_NUM_DISKS,diskWidth/2*sin(angle));
          glVertex3f(diskWidth/2*cos(angle+PI/50),y-1/(float)MAX_NUM_DISKS,diskWidth/2*sin(angle+PI/50));
          glVertex3f(diskWidth/2*cos(angle+PI/50),y,diskWidth/2*sin(angle+PI/50));
        }
     glEnd();
     
     glBegin(GL_POLYGON);
        for(float angle=2*PI; angle>=0; angle-=2*PI/100)
          glVertex3f(diskWidth/2*cos(angle),y,diskWidth/2*sin(angle));
     glEnd();
}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   //glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
   for(int i=NUM_DISKS-1; i>=0; i--)
      towers[0].Push(Disk(i+1));
   
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   
   for(int b=0; b<3; b++){
      towers[b].Draw(-1,towers[b].numDisks*1/(float)MAX_NUM_DISKS-1);
      if(towers[b].numDisks>0){
         for(float d=0; d<towers[b].numDisks; d++)
           towers[b].stack[(int)d].Draw(towers[b].pos,(d+1)*1/MAX_NUM_DISKS);
      }
      towers[b].Draw(towers[b].numDisks*1/(float)MAX_NUM_DISKS-1,0);
   }
   if(diskIsClicked)
      clickedDisk.Draw(towers[clickedTower].pos, 1.5);

   glutSwapBuffers ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   //gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
   width=w;
   height=h;
   glFrustum(-1, 1, -1, 1, 2, 5);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity() ;
   towers[0].pos=width*0.25;
   towers[1].pos=width*0.5;
   towers[2].pos=width*0.75;
   towers[0].pos=-0.6;
   towers[1].pos=0;
   towers[2].pos=0.6;
}

void keyboard(unsigned char key, int x, int y)
{
     switch(key){
        case 's':
        case 'S':
             Restart();
             Solve(NUM_DISKS, 0, 2, 1);
             glutPostRedisplay();
             break;
        case 'r':
        case 'R':
             Restart();
             break;
        case '1':
             ClickTower(0);
             break;
        case '2':
             ClickTower(1);
             break;
        case '3':
             ClickTower(2);
             break;
     }
}

void mouse(int button, int state, int x, int y)
{
     y=height-y;
     if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && y<height/2){
        if(x>width/8 && x<3*width/8)
            ClickTower(0);
        if(x>3*width/8 && x<5*width/8)
            ClickTower(1);
        if(x>5*width/8 && x<7*width/8)
            ClickTower(2);
     }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("Towers of Hanoi");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
