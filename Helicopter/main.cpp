#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include "../GLText/GLText.cpp"

using namespace std;

#define NUM_BOXES 25

float forceGrav=150;              //the acceleration speed of the helicopter
float width,height;               //width/height of the window
bool isStart=true;                //used to tell if game is starting
float boxWidth;                   //width of each green box in pixels
float boxHeights[NUM_BOXES];      //array to hold the y location of each of the 20 green boxes
float maxVel=10;                  //maximum vertical speed of the helicopter
bool stopped=false;               //used to tell if game is stopped or not
int maxChange=15;                 //the maximum number of pixels the green boxes can move up or down each time
int lastChange;                   //the last time that the game got harder (to prevent it from changing more than once every 3 seconds)
int score=0;                      //current score
int highScore=0;                  //high score
time_t start,current;             //used to keep track of time to tell the game when to get harder (every 3 secs)
char type=2;                      //used to keep track of current tunnel type (0=boxes,1=line)
fstream myfile;

class Helicopter{
      public:
         float height,velocity;  //y position and vertical speed of copter
         float upwardThrust;     //the force that it is pushing up with
         Helicopter(float _height, float _vel);
         void Draw();   //draws to screen
}heli(350,0);

Helicopter::Helicopter(float _height, float _vel)
{
   height=_height;
   velocity=_vel;
   upwardThrust=0;
}

void Helicopter::Draw()
{
    glColor3f(1,1,1);
    
    //body
    glBegin(GL_LINE_LOOP);
     glVertex2f(100,height-20);
     glVertex2f(120,height-5);
     glVertex2f(150,height-5);
     glVertex2f(170,height-15);
     glVertex2f(175,height-25);
     glVertex2f(170,height-35);
     glVertex2f(150,height-45);
     glVertex2f(120,height-45);
     glVertex2f(100,height-30);
     glVertex2f(85,height-30);
     glVertex2f(80,height-45);
     glVertex2f(75,height-45);
     glVertex2f(75,height-5);
     glVertex2f(80,height-5);
     glVertex2f(85,height-20);
    glEnd();
    
    //blade
    glBegin(GL_LINE_STRIP);
     glVertex2f(133.5,height-5);
     glVertex2f(133.5,height);
     glVertex2f(90,height);
     glVertex2f(90,height+5);
     glVertex2f(180,height+5);
     glVertex2f(180,height);
     glVertex2f(137.5,height);
     glVertex2f(137.5,height-5);
    glEnd();
    
    //window
    glBegin(GL_LINE_STRIP);
     glVertex2f(150,height-5);
     glVertex2f(167,height-25);
     glVertex2f(175,height-25);
    glEnd();
    
    //bottom rail
    glBegin(GL_LINE_STRIP);
     glVertex2f(120,height-45);
     glVertex2f(120,height-50);
     glVertex2f(115,height-50);
     glVertex2f(110,height-45);
     glVertex2f(105,height-50);
     glVertex2f(110,height-55);
     glVertex2f(160,height-55);
     glVertex2f(165,height-50);
     glVertex2f(160,height-45);
     glVertex2f(155,height-50);
     glVertex2f(150,height-50);
     glVertex2f(150,height-45);
     glVertex2f(147,height-45);
     glVertex2f(147,height-50);
     glVertex2f(123,height-50);
     glVertex2f(123,height-45);
    glEnd();
}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   srand(time(NULL));
}

void InitializeBoxes()    //does everything that needs to be done each time game restarts
{ 
     time(&start);        //stores start time to start
     maxChange=15;
     lastChange=0;
     score=0;
     heli.height=height/2+25;
     stopped=false;
     heli.velocity=0;
     boxWidth=width/NUM_BOXES;
     for(int i=0; i<NUM_BOXES; i++)    //sets all of the green box positions equal
       boxHeights[i]=height/2+175;
}

void DrawBoxes(char type)
{
    glColor3f(0,1,0);
    if(type==1){
     for(int i=0; i<NUM_BOXES; i++){
        glBegin(GL_LINE_LOOP);              //top row of boxes
          glVertex2f(i*boxWidth,height);
          glVertex2f(i*boxWidth+boxWidth,height);
          glVertex2f(i*boxWidth+boxWidth,boxHeights[i]);
          glVertex2f(i*boxWidth,boxHeights[i]);
        glEnd();
        glBegin(GL_LINE_LOOP);                //lower row of boxes
          glVertex2f(i*boxWidth,40);
          glVertex2f(i*boxWidth+boxWidth,40);
          glVertex2f(i*boxWidth+boxWidth,boxHeights[i]-350);
          glVertex2f(i*boxWidth,boxHeights[i]-350);
        glEnd();
     }
    }else if(type==2){
        glBegin(GL_LINE_STRIP);
          for(int i=0; i<NUM_BOXES; i++)
            glVertex2f(i*boxWidth,boxHeights[i]);
          glVertex2f(width,boxHeights[NUM_BOXES-1]);
        glEnd();
        glBegin(GL_LINE_STRIP);
          for(int i=0; i<NUM_BOXES; i++)
            glVertex2f(i*boxWidth,boxHeights[i]-350);
          glVertex2f(width,boxHeights[NUM_BOXES-1]-350);
        glEnd();
    }else if(type==3){
        for(int i=0; i<NUM_BOXES; i++){
        glBegin(GL_POLYGON);              //top row of boxes
          glVertex2f(i*boxWidth,height);
          glVertex2f(i*boxWidth+boxWidth,height);
          glVertex2f(i*boxWidth+boxWidth,boxHeights[i]);
          glVertex2f(i*boxWidth,boxHeights[i]);
        glEnd();
        glBegin(GL_POLYGON);                //lower row of boxes
          glVertex2f(i*boxWidth,40);
          glVertex2f(i*boxWidth+boxWidth,40);
          glVertex2f(i*boxWidth+boxWidth,boxHeights[i]-350);
          glVertex2f(i*boxWidth,boxHeights[i]-350);
        glEnd();
        }
    }   
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   
   time(&current);   //updates the current time
   
   if(isStart){
      InitializeBoxes();
      isStart=false;
   }
   
   DrawBoxes(type);
   
   //calculate the new velocity and height of the copter
   heli.velocity+=forceGrav/100-heli.upwardThrust/100;
   heli.height-=heli.velocity;
   
   //limits the copters vertical speed
   if(heli.velocity>maxVel)heli.velocity=maxVel;
   if(heli.velocity<-maxVel)heli.velocity=-maxVel;
   
   //makes the game harder every 3 secs
   if((current-start)%3==0 && current-start!=lastChange){
      maxChange+=5;
      lastChange=(current-start);
   }
   
   heli.Draw();
   
   //shift box positions to the left
   for(int i=0; i<NUM_BOXES-1; i++)
           boxHeights[i]=boxHeights[i+1];
   
   //randomize position of rightmost box within the window ranges        
   do{
    boxHeights[NUM_BOXES-1]+=rand()%(maxChange*2+1)-maxChange;
   }while((boxHeights[NUM_BOXES-1]>height) || (boxHeights[NUM_BOXES-1]<390));
   
   //white line separating game from stats
   glColor3f(1,1,1);
   glBegin(GL_LINES);
     glVertex2f(0,40);
     glVertex2f(width,40);
   glEnd();
   
   //draws current score to screen
   DrawInt(10,13,0,++score,GLUT_BITMAP_TIMES_ROMAN_24);
   
   //updates high score
   if(score>highScore)highScore=score;
   DrawInt(width-70,13,0,highScore,GLUT_BITMAP_TIMES_ROMAN_24);
   
   //draws current playing time to screen
   DrawInt(width/2,13,0,(current-start),GLUT_BITMAP_TIMES_ROMAN_24);
   
   usleep(30000);
   
   //stop game if copter hits a green box
   if(heli.height>boxHeights[(int)floor(100/boxWidth)]-5 || heli.height<boxHeights[(int)floor(100/boxWidth)]-295)
     stopped=true;
   
   glutSwapBuffers();
   
   if(!stopped)
   glutPostRedisplay();

}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   width=w;
   height=h;
   gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
}

void mouse(int button, int state, int x, int y)
{
     if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
        heli.upwardThrust=2*forceGrav;
     else if(button==GLUT_LEFT_BUTTON)
        heli.upwardThrust=0;
        
     if(button==GLUT_RIGHT_BUTTON && stopped){
        isStart=true;
        glutPostRedisplay();
     }
}

void keyboard(unsigned char key, int x, int y)
{
     switch(key){
        case 27:
             exit(0);
             break;
        case 'R':
        case 'r':
             isStart=true;
             glutPostRedisplay();
        case 'p':
        case 'P':
             if(stopped)
               stopped=false;
             else
               stopped=true;
             glutPostRedisplay();
             break;
        case 't':
        case 'T':
             if(++type==4)type=1;
             break;
     }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (1017, 700); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("Tunnel");
   glutFullScreen();
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
