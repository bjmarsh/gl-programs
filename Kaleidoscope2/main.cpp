/*
Kaleidoscope v3.5
By Bennett Marsh
(C)2009

This program uses semi-transparent circles and geometric transformations
to produce beautiful kaleidoscope-like images.  In non-screensaver mode,
you can draw your own images.  In screensaver mode, the program uses
gravity-like physics to move the brush around automatically.  In screensaver
mode,  the color of the brush gradually fades between the colors of the rainbow
and the brush size is varied randomly.  All of the parameters are randomized
each time the program is run so that it is nearly impossible to get the
same image twice.
*/

#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

#define PI 3.1415926535898
#define SIN sin(45*PI/180)       //used to rotate image 45 degrees with rotation matrices
#define COS cos(45*PI/180)
#define SCREENSAVER false      //toggles whether or not to compile in screensaver mode

float height,width;    //window dimensions
float x2=-2,y2,x=0,y=0;   //used for storing mouse coordinates
int startX=-1, startY;   //hold initial mouse coordinates to detect movement

//colors[] holds the RGB values for all 7 colors of the rainbow.  They are in 7 sets of 3.
float colors[21]={1.0,0.0,0.0,1.0,0.5,0.0,1.0,1.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0,0.3,0.0,0.5,1.0,0.0,1.0};
//color1[] and color2[] hold the RGB values for the two colors that the current color is fading between
float color1[3]={1,0,0};
float color2[3]={1,0.5,0};
float fadeColors[3];    //the RGB values of the currentColor
int currentColor=1;   //the position in colors[] of color2[]
int colorCount=0;      //the number of frames since the last color change
int maxColorCount=50;  //the number of frames it takes to fade from one color to another
bool isAuto=SCREENSAVER, isPaused=false;  //whether or not it is in animation mode
float radius=0.1,radiusIncrement;    //the radius of the brush & the radius increment for each frame in animation mode
float accel, velX=0, velY=0;  //acceleration, vertical velocity, and horizontal velocity of the brush in animation mode
float attractX, attractY, locX=0, locY=0;   //the location of the attraction point and the location of the brush
int frameCount=0, frameMax=20, maxWidth=15;  //the number of frames in the current image, the number of frames in between attraction point switches, and the max width of the brush
bool symmetry=true;  //whether or not symmetry is on
float alpha;   //the current alpha value


void init(void)   //called when program starts
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear (GL_COLOR_BUFFER_BIT);
}

void display(void)  //called when program starts & when the display needs to be updated.
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   glutSwapBuffers();
}

void reshape (int w, int h)  //called when program starts & when window is resized
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   height=h;
   width=w;
   gluOrtho2D (-width/height,width/height,-1,1);
}

void drawCircle(float x, float y)  //draws the "glowing" circle (circle gets more transparent near edge)
{
     //the colored circle
     alpha=0.005*(10-radius*40)*((frameCount<450)?1:(500-frameCount)/50);
     glBegin(GL_POLYGON);
       glColor4f(fadeColors[0],fadeColors[1],fadeColors[2],alpha);
       glVertex2f(x,y);
       glColor4f(fadeColors[0],fadeColors[1],fadeColors[2],0.0);
       for(float angle=0; angle<=PI+PI/20; angle+=PI/20)
         glVertex2f(radius*cos(angle)+x,radius*sin(angle)+y);
      glEnd();
      glBegin(GL_POLYGON);
       glColor4f(fadeColors[0],fadeColors[1],fadeColors[2],alpha);
       glVertex2f(x,y);
       glColor4f(fadeColors[0],fadeColors[1],fadeColors[2],0.0);
       for(float angle=PI; angle<=2*PI+PI/20; angle+=PI/20)
         glVertex2f(radius*cos(angle)+x,radius*sin(angle)+y);
      glEnd();
      
      //the white circle in the middle of the colored one
      alpha=(frameCount<450) ? 0.3 : (550-frameCount)/50*0.3;
      glBegin(GL_POLYGON);
       glColor4f(1,1,1,alpha);
       glVertex2f(x,y);
       glColor4f(1,1,1,0.0);
       for(float angle=0; angle<=PI+PI/20; angle+=PI/20)
         glVertex2f(radius*cos(angle)/4+x,radius*sin(angle)/4+y);
      glEnd();
      glBegin(GL_POLYGON);
       glColor4f(1,1,1,alpha);
       glVertex2f(x,y);
       glColor4f(1,1,1,0.0);
       for(float angle=PI; angle<=2*PI+PI/20; angle+=PI/20)
         glVertex2f(radius*cos(angle)/4+x,radius*sin(angle)/4+y);
      glEnd();
      
}

void Draw( float x, float y )
{
     //determines current color
     float multiplier = colorCount;
     multiplier /= maxColorCount;
     fadeColors[0]=(color1[0]-color2[0])*multiplier;
     fadeColors[0]=color1[0]-fadeColors[0];
     fadeColors[1]=(color1[1]-color2[1])*multiplier;
     fadeColors[1]=color1[1]-fadeColors[1];
     fadeColors[2]=(color1[2]-color2[2])*multiplier;
     fadeColors[2]=color1[2]-fadeColors[2];
     
     //draws the circles to the screen witht he various transformations
     drawCircle(x,y);
     if(symmetry){
       drawCircle(-x,y);
       drawCircle(x,-y);     
       drawCircle(-x,-y);       
       drawCircle(y,x);       
       drawCircle(-y,x);       
       drawCircle(y,-x);       
       drawCircle(-y,-x);  
          
       //45 degree rotations
       drawCircle((COS*x-SIN*y),(SIN*x+COS*y));       
       drawCircle(-(COS*x-SIN*y),(SIN*x+COS*y));       
       drawCircle((COS*x-SIN*y),-(SIN*x+COS*y));       
       drawCircle(-(COS*x-SIN*y),-(SIN*x+COS*y));       
       drawCircle((SIN*x+COS*y),(COS*x-SIN*y));       
       drawCircle(-(SIN*x+COS*y),(COS*x-SIN*y));      
       drawCircle((SIN*x+COS*y),-(COS*x-SIN*y));       
       drawCircle(-(SIN*x+COS*y),-(COS*x-SIN*y));
     }
       //draws transparent black rectangles to get the fade out effect after 450 frames
       if(frameCount>=450){
          glColor4f(0,0,0,(float)(frameCount-450)/350);
          glBegin(GL_QUADS);
            glVertex2f(-width/height,-1);
            glVertex2f(-width/height,1);
            glVertex2f(width/height,1);
            glVertex2f(width/height,-1);
          glEnd();
       }
       //resets the animation once 550 frames is reached
       if(frameCount>=550){
          frameCount=0;
          velX=velY=locX=locY=0;
          radius=0.01;
          maxWidth=rand()%11+10;
          maxColorCount=rand()%141+10;
          frameMax=rand()%41+10;
          glClear(GL_COLOR_BUFFER_BIT);
       }
            
     
     glutSwapBuffers();  //display the image

     //changes colors when the fade is complete
     colorCount++;
     if(colorCount>=maxColorCount){
       colorCount=0;
       currentColor++;
       if(currentColor>6)currentColor=0;
         for(int i=0; i<=2; i++)
           color1[i]=color2[i];
         for(int i=3*currentColor; i<=3*currentColor+2; i++)
           color2[i-3*currentColor]=colors[i];
     }
     
}

void mouse(int button, int state, int x, int y)  //detects clicks
{
     if(state==GLUT_UP&&!isAuto)
        x2=-2;
}

void motion(int x, int y)  //detects mouse dragging & paints the screen as you drag
{
    if(x2==-2){
      x2=(float)x/height*2-width/height;
      y2=(height-(float)y)/height*2-1;
    }
    if(!isAuto)Draw((float)x/height*2-width/height,(height-(float)y)/height*2-1);
}

void special(int key, int x, int y)  //arrow keys for changing the brush width
{
     switch(key){
       case GLUT_KEY_UP:
            if(radius<0.15)
            radius+=0.01;
            break;
       case GLUT_KEY_DOWN:
            if(radius>0.02)
            radius-=0.01;
            break;
       }
}

void keyboard(unsigned char key, int x, int y)  //handles keyboard input
{
     switch(key){
        case 'c':
        case 'C':
             glutPostRedisplay();
             x2=-2;
             if(isAuto){
               x2=0;
               y2=0;
             }
             break;
        case 's':
        case 'S':
             symmetry=!symmetry;
             break;
     case 'a':
     case 'A':
            if(!isAuto){
             isAuto=true;
             x2=0;
             y2=0;
             isPaused=false;
            }else{
             isAuto=false;
             x2=-2;
             frameCount=0;
             velX=velY=locX=locY=0;
            }
             break;
     case 27:
          exit(0);
          break;
     case 'p':
     case 'P':
          isPaused=!isPaused;
          break;
     }
}

/*
This function controls the animation.  The brush starts in the middle of the screen at
(0,0) and a point is picked at random to which the bush is "attracted," kind of like gravity.
The brush will accelerate to this point for frameMax frames and then a new point is picked.
*/
void idle()
{
   if(isAuto && !isPaused){
    accel=0.00000004*(float)(rand()%4);
    if(frameCount%frameMax==0){
       attractX=rand()%100;    //picks a random attraction point
       attractY=rand()%100;
       attractX/=100;
       attractY/=100;
       attractX-=0.5;
       attractY-=0.5;
       radiusIncrement=rand()%maxWidth+1;   //determins new target radius and the increment per frame
       radiusIncrement/=100;
       radiusIncrement=(radiusIncrement-radius)/(float)frameMax;
    }
    ++frameCount;
    float slope=(attractY-locY)/(attractX-locX);  //the slope of the vector representing the attraction force
    if(attractX==locX)slope=0.1;   //prevents and undefined slope
    float dx=sqrt(accel/(slope*slope+1));  //the horizontal acceleratoin
    if(locX>attractX)dx*=-1;
    float dy=slope*dx;   //vertical acceleration
    velX+=dx;     //calculates new velocities
    velY+=dy;
    locX+=velX;   //calculates new coordinates
    locY+=velY;
    radius+=radiusIncrement;   //increases radius
    Draw(locX,locY);    //draws to screen 
   }
   usleep(15000);       //pause for 15 ms
}

void passive(int x, int y)   //detects movement and exits if in scn svr mode
{
     if(startX==-1){
       startX=x;
       startY=y;
     }else if(SCREENSAVER){
       if(abs(x-startX)>=4 || abs(y-startY)>=4)
       exit(0);
     }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("Kaleidoscope");
   if(SCREENSAVER){
     glutFullScreen();
     glutSetCursor(GLUT_CURSOR_NONE);
   }
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glShadeModel (GL_SMOOTH);
   srand (time(NULL));
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMotionFunc(motion);
   glutMouseFunc(mouse);
   glutSpecialFunc(special);
   glutKeyboardFunc(keyboard);
   glutIdleFunc(idle);
   glutPassiveMotionFunc(passive);
   glutMainLoop();
   return 0;
}
