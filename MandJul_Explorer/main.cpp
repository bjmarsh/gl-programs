/************************************************************************
  MANDELBROT/JULIA SET INTERACTIVE EXPLORER
  PROGRAMMED BY BENNETT MARSH
  (C) 2009
  
  THIS PROGRAM USES OPENGL TO SHOW THE RELATIONSHIP BETWEEN THE 
  MANDELBROT SET AND THE JULIA SET.  EVERY POINT ON THE COMPLEX PLANE
  REPRESENTS A VALUE FOR C THAT WHEN PLUGGED INTO THE
  ITERATING FUNCTION F(Z)=Z^A+C CREATE DIFFERENT JULIA SETS, WHERE A IS
  AN INTEGER GREATER THAN 1.  THE RELATIONSHIP IS THAT EVERY POINT IN 
  THE MANDELBROT SET GENERATES A CONNECTED JULIA SET, AND EVERY POINT 
  NOT IN THE MANDELBROT SET GENERATES AN UNCONNECTED JULIA SET.
  
  CONTROLS:
    LEFT CLICK AND DRAG: GENERATE JULIA SETS IN REAL TIME FOR THE
                         C VALUE REPRESENTED BY THE CURSOR LOCATION
    LEFT CLICK ON JULIA SIDE: ZOOM TO THAT AREA
    RIGHT CLICK ON MANDELBROT SIDE: ZOOM TO THAT AREA
    'R' KEY: RESET ORIGINAL VIEWING RANGES FOR BOTH PLOTS
    'D' KEY: TURN HIGH DETAIL MODE ON/OFF (DEFAULT IS OFF)
    'A' KEY: TOGGLE ANIMATION MODE ON/OFF
    ARROW KEYS: CHANGE THE C VALUE (MOVES GREEN CROSS)
    + or -: change power of z in iterating function f(z)=z^(power)+c. Starts at 2.
    ESC KEY: EXIT
***********************************************************************/


#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <unistd.h>

#define PI 3.1415926535898

float GetXPoint( float x );
float GetYPoint( float y );
inline void GenerateJulia( float zr, float zi, float cr, float ci, int maxIter );
inline void GenerateMandelbrot( float cr, float ci, int maxIter );
void DrawAxes();
void Animate();
void ClearCross();
void DrawCross();
void GetRanges();

//variables to hold the width & height of the window in pixels.  Upadated in reshape()
float width,height;
//variables to hold the current viewing range for the Mandelbrot/Julia plots
float xMinM=-2.1;
float xMaxM=0.7;
float yMinM=-1.4;
float yMaxM=1.4;
float xMinJ=-1.5;
float xMaxJ=1.5;
float yMinJ=-1.5;
float yMaxJ=1.5;
//used to hold current c value for generating Julia set
float cr,ci;
//holds the current detail
int detail=20;
//used to tell if Animation mode is on
int isAuto=0;
//holds a in f(z)=z^a+c
int power=2;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   cr=-0.75;
   ci=0;
   srand(time(NULL));
}

//converts an x coordinate relative to Mandelbrot window to a float between -1 and 0
float GetXPoint(float x)
{
      x=(x-xMinM)/(xMaxM-xMinM);
      return(x-1);
}

//converts an y coordinate relative to Mandelbrot window to a float between -1 and 1
float GetYPoint(float y)
{
      y=(y-yMinM)/(yMaxM-yMinM);
      return(2*y-1);
}

void ClearCross()
{
     for(float i=cr-0.08; i<=cr+0.08; i+=(xMaxM-xMinM)/width*2){
            GenerateMandelbrot(i,ci,400);
            GenerateMandelbrot(i,ci+(yMaxM-yMinM)/height,400);
            GenerateMandelbrot(i,ci-(yMaxM-yMinM)/height,400);
     }
     for(float i=ci-0.08; i<=ci+0.08; i+=(yMaxM-yMinM)/height){
            GenerateMandelbrot(cr-(xMaxM-xMinM)/width*2,i,400);
            GenerateMandelbrot(cr+(xMaxM-xMinM)/width*2,i,400);
            GenerateMandelbrot(cr,i,400);
     }
}

void DrawCross()
{
     glColor3f(0,1,0);
     glBegin(GL_LINES);
        glVertex2f(GetXPoint(cr-0.0185*(xMaxM-xMinM)),GetYPoint(ci));
        glVertex2f(GetXPoint(cr+0.0185*(xMaxM-xMinM)),GetYPoint(ci));
        glVertex2f(GetXPoint(cr),GetYPoint(ci-0.017857*(yMaxM-yMinM)));
        glVertex2f(GetXPoint(cr),GetYPoint(ci+0.017857*(yMaxM-yMinM)));
     glEnd();
}

void GetRanges()
{
    switch(power){
       case 2:xMinM=-2.1;
              xMaxM=0.7;
              yMinM=-1.4;
              yMaxM=1.4;
              break;
       default:xMinM=-1.5;
               xMaxM=1.5;
               yMinM=-1.5;
               yMaxM=1.5;
               break;
       }
                
}

void Animate()
{  
     detail=20;
     
     float newCr=rand()%500;
     newCr/=500;
     newCr*=(xMaxM-xMinM);
     newCr+=xMinM;
     float newCi=rand()%500;
     newCi/=500;
     newCi*=(yMaxM-yMinM);
     newCi+=yMinM;
     
     float dist=sqrt(pow(newCr-cr,2)+pow(newCi-ci,2));
     
     float deltaCr=(newCr-cr)/dist/30;
     float deltaCi=(newCi-ci)/dist/30;
     
     for(float a=0; a<=dist*30; a++){
         DrawCross(); 
         glFlush();    
         cr+=deltaCr;
         ci+=deltaCi;
         for(float i=xMinJ; i<=xMaxJ; i+=(xMaxJ-xMinJ)/width*2){
           for(float c=yMinJ; c<=yMaxJ; c+=(yMaxJ-yMinJ)/height){        
             GenerateJulia( i, c, cr, ci, detail);
           }
         }\
         cr-=deltaCr;
         ci-=deltaCi;
         ClearCross();DrawAxes();
         cr+=deltaCr;
         ci+=deltaCi;
      glFlush(); 
     }
}

//determines each pixel color for Mandelbrot set
inline void GenerateMandelbrot(float cr, float ci, int maxIter)
{
   float zr=0;
   float zi=0;
   float temp;
   int count=0;
   do{
    switch(power){
     case 1:
          temp=cos(zr)*cosh(zi)*cr+sin(zr)*sinh(zi)*ci;
      zi=-sin(zr)*sinh(zi)*cr+cos(zr)*cosh(zi)*ci;
      zr=temp;
      break;
     case 2:
      temp=zr*zr-zi*zi+cr;
      zi=2*zr*zi+ci;
      zr=temp;
      break;
     case 3:
      temp=zr*zr*zr-3*zr*zi*zi+cr;
      zi=3*zr*zr*zi-zi*zi*zi+ci;
      zr=temp;
      break;
     case 4:
      temp=zr*zr*zr*zr-6*zr*zr*zi*zi+zi*zi*zi*zi+cr;
      zi=4*zr*zr*zr*zi-4*zr*zi*zi*zi+ci;
      zr=temp;
      break;
     case 5:
      temp=zr*zr*zr*zr*zr-10*zr*zr*zr*zi*zi+5*zr*zi*zi*zi*zi+cr;
      zi=5*zr*zr*zr*zr*zi-10*zr*zr*zi*zi*zi+zi*zi*zi*zi*zi+ci;
      zr=temp;
      break;
     case 6:
      temp=zr*zr*zr*zr*zr*zr-15*zr*zr*zr*zr*zi*zi+15*zr*zr*zi*zi*zi*zi-zi*zi*zi*zi*zi*zi+cr;
      zi=6*zr*zr*zr*zr*zr*zi-20*zr*zr*zr*zi*zi*zi+6*zr*zi*zi*zi*zi*zi+ci;
      zr=temp;
      break;
     default:
      temp=zr;
      float a=zr,b=zi;
      float zrtemp,zitemp;
      for(int d=1; d<=power-1; d++){
             zrtemp=a*zr-b*zi;
             zitemp=a*zi+b*zr;
             a=zrtemp;
             b=zitemp;
      }
      zr=a+cr;
      zi=b+ci;
      break;
    }
      count++;
   }while((zr*zr+zi*zi<4)&&(count<maxIter));
   float red,blue,green;
   red=0;
   blue=0;
   green=0;
   if(count<maxIter){
      blue=count;
      blue=log(blue)/3;
      red=count;
      red/=24;
      red*=red*red;
      green=count;
      green/=24;
      green*=green*green;
   }
   glBegin(GL_POINTS);
     glColor3f(red,green,blue);
     glVertex2f((cr-xMinM)/(xMaxM-xMinM)-1,(ci-yMinM)/(yMaxM-yMinM)*2-1);
   glEnd();
}

//determines each pixel color for Julia set
inline void GenerateJulia( float zr, float zi, float cr, float ci, int maxIter )
{
   float xx=zr;
   float yy=zi;
   float temp;
   int count=0;
   do{
    switch(power){
     case 1:
      temp=cos(zr)*cosh(zi)*cr+sin(zr)*sinh(zi)*ci;
      zi=-sin(zr)*sinh(zi)*cr+cos(zr)*cosh(zi)*ci;
      zr=temp;
      break;
     case 2:
      temp=zr*zr-zi*zi+cr;
      zi=2*zr*zi+ci;
      zr=temp;
      break;
     case 3:
      temp=zr*zr*zr-3*zr*zi*zi+cr;
      zi=3*zr*zr*zi-zi*zi*zi+ci;
      zr=temp;
      break;
     case 4:
      temp=zr*zr*zr*zr-6*zr*zr*zi*zi+zi*zi*zi*zi+cr;
      zi=4*zr*zr*zr*zi-4*zr*zi*zi*zi+ci;
      zr=temp;
      break;
     case 5:
      temp=zr*zr*zr*zr*zr-10*zr*zr*zr*zi*zi+5*zr*zi*zi*zi*zi+cr;
      zi=5*zr*zr*zr*zr*zi-10*zr*zr*zi*zi*zi+zi*zi*zi*zi*zi+ci;
      zr=temp;
      break;
     case 6:
      temp=zr*zr*zr*zr*zr*zr-15*zr*zr*zr*zr*zi*zi+15*zr*zr*zi*zi*zi*zi-zi*zi*zi*zi*zi*zi+cr;
      zi=6*zr*zr*zr*zr*zr*zi-20*zr*zr*zr*zi*zi*zi+6*zr*zi*zi*zi*zi*zi+ci;
      zr=temp;
      break;
     default:
      temp=zr;
      float a=zr,b=zi;
      float zrtemp,zitemp;
      for(int d=1; d<=power-1; d++){
             zrtemp=a*zr-b*zi;
             zitemp=a*zi+b*zr;
             a=zrtemp;
             b=zitemp;
      }
      zr=a+cr;
      zi=b+ci;
      break;
    }
      count++;
   }while((zr*zr+zi*zi<4)&&(count<maxIter));
   float red=0,blue=0,green=0;
   if((count<maxIter)&&(detail<2000)){
   blue=count%2;
   blue/=2;
   green=count%9;
   green/=9;
   red=count%13;
   red/=13;
   }
   if(detail==2000){
   red=pow(log(count)/5,3);
   green=pow(log(count)/5,3);
   blue=pow(log(count)/3,3);
   }
   glBegin(GL_POINTS);
     glColor3f(red,green,blue);
     glVertex2f((xx-xMinJ)/(xMaxJ-xMinJ),(yy-yMinJ)/(yMaxJ-yMinJ)*2-1);
   glEnd();
}

void DrawAxes()
{
   glColor3f(1,0,0);
   glBegin(GL_LINES);
     if(GetXPoint(xMinM)<=0)glVertex2f(GetXPoint(xMinM),GetYPoint(0));
     if(GetXPoint(xMaxM)<=0)glVertex2f(GetXPoint(xMaxM),GetYPoint(0));
     if(GetXPoint(0)<=0)glVertex2f(GetXPoint(0),GetYPoint(yMinM));
     if(GetXPoint(0)<=0)glVertex2f(GetXPoint(0),GetYPoint(yMaxM));
     if(GetXPoint(-2)<=0)glVertex2f(GetXPoint(-2),GetYPoint(0.05));
     if(GetXPoint(-2)<=0)glVertex2f(GetXPoint(-2),GetYPoint(-0.05));
     if(GetXPoint(-1.5)<=0)glVertex2f(GetXPoint(-1.5),GetYPoint(0.025));
     if(GetXPoint(-1.5)<=0)glVertex2f(GetXPoint(-1.5),GetYPoint(-0.025));
     if(GetXPoint(-1)<=0)glVertex2f(GetXPoint(-1),GetYPoint(0.05));
     if(GetXPoint(-1)<=0)glVertex2f(GetXPoint(-1),GetYPoint(-0.05));
     if(GetXPoint(-0.5)<=0)glVertex2f(GetXPoint(-0.5),GetYPoint(0.025));
     if(GetXPoint(-0.5)<=0)glVertex2f(GetXPoint(-0.5),GetYPoint(-0.025));
     if(GetXPoint(0.5)<=0)glVertex2f(GetXPoint(0.5),GetYPoint(0.025));
     if(GetXPoint(0.5)<=0)glVertex2f(GetXPoint(0.5),GetYPoint(-0.025));
     if(GetXPoint(0.05)<=0){glVertex2f(GetXPoint(0.05),GetYPoint(-1));
                            glVertex2f(GetXPoint(-0.05),GetYPoint(-1));}
     if(GetXPoint(0.025)<=0){glVertex2f(GetXPoint(0.025),GetYPoint(-0.5));
                             glVertex2f(GetXPoint(-0.025),GetYPoint(-0.5));}
     if(GetXPoint(0.05)<=0){glVertex2f(GetXPoint(0.05),GetYPoint(1));
                            glVertex2f(GetXPoint(-0.05),GetYPoint(1));}
     if(GetXPoint(0.025)<=0){glVertex2f(GetXPoint(0.025),GetYPoint(0.5));
                             glVertex2f(GetXPoint(-0.025),GetYPoint(0.5));}
   glEnd();
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   
   //generates the Mandelbrot set
   for(float x=xMinM; x<=xMaxM; x+=(xMaxM-xMinM)/width*2){
     for(float y=yMinM; y<=yMaxM; y+=(yMaxM-yMinM)/height){
       GenerateMandelbrot(x,y,400); 
     }
   }
   
   //draws red axes on Mandelbrot plot
   DrawAxes();
   
   //draws green cross to mark current position
   glColor3f(0,1,0);
   glBegin(GL_LINES);
     glVertex2f(GetXPoint(cr-0.0185*(xMaxM-xMinM)),GetYPoint(ci));
     glVertex2f(GetXPoint(cr+0.0185*(xMaxM-xMinM)),GetYPoint(ci));
     glVertex2f(GetXPoint(cr),GetYPoint(ci-0.017857*(yMaxM-yMinM)));
     glVertex2f(GetXPoint(cr),GetYPoint(ci+0.017857*(yMaxM-yMinM)));
   glEnd();
   
   //generates Julia set
   for(float i=xMinJ; i<=xMaxJ; i+=(xMaxJ-xMinJ)/width*2){
     for(float c=yMinJ; c<=yMaxJ; c+=(yMaxJ-yMinJ)/height){        
       GenerateJulia( i, c, cr, ci, detail);
     }
   }
   
   glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   width=w;
   height=h;
   gluOrtho2D(-1,1,-1,1);
}

void mouse(int button, int state, int x, int y){
     //if user clicks left button on Mandelbrot side, generates the Julia set for the corresponding c values
     if((button==GLUT_LEFT_BUTTON)&&(state==GLUT_DOWN)&&!isAuto){
       if(x<width/2){
         ClearCross();
         DrawAxes();
         cr=(x/(width/2))*(xMaxM-xMinM)+xMinM;
         ci=((height-y)/height)*(yMaxM-yMinM)+yMinM;
         for(float i=xMinJ; i<=xMaxJ; i+=(xMaxJ-xMinJ)/width*2){
           for(float c=yMinJ; c<=yMaxJ; c+=(yMaxJ-yMinJ)/height){        
             GenerateJulia( i, c, cr, ci, detail);
           }
         }
         DrawCross();
         glFlush();
       }else if(!isAuto){
         //if user clicks left button on Julia side, zoom to that area
         x-=width/2;
         float dif=xMaxJ-xMinJ;
         float temp=x/width*2;
         temp=temp*dif+xMinJ;
         xMinJ=temp-0.25*dif;
         xMaxJ=temp+0.25*dif;
         dif=yMaxJ-yMinJ;
         temp=height-y+30;
         temp/=width/2;
         temp=temp*dif+yMinJ;
         yMinJ=temp-0.25*dif;
         yMaxJ=temp+0.25*dif;
         for(float i=xMinJ; i<=xMaxJ; i+=(xMaxJ-xMinJ)/width*2){
           for(float c=yMinJ; c<=yMaxJ; c+=(yMaxJ-yMinJ)/height){        
             GenerateJulia( i, c, cr, ci, detail);
           }
         }
         glFlush();
       }
     }
     ////if user clicks right button on Mandelbrot side, zoom to that area
     if((button==GLUT_RIGHT_BUTTON)&&(state==GLUT_DOWN)&&(x<width/2)&&!isAuto){
       float dif=xMaxM-xMinM;
       float temp=x/width*2;
       temp=temp*dif+xMinM;
       xMinM=temp-0.25*dif;
       xMaxM=temp+0.25*dif;
       dif=yMaxM-yMinM;
       temp=height-y+30;
       temp/=width/2;
       temp=temp*dif+yMinM;
       yMinM=temp-0.25*dif;
       yMaxM=temp+0.25*dif;
       glutPostRedisplay();
     }
}

void keyboard(unsigned char key, int x, int y)
{
     switch(key){
        //resets window ranges
        case 'r':
        case 'R':
             GetRanges();
             xMinJ=-1.5;
             xMaxJ=1.5;
             yMinJ=-1.5;
             yMaxJ=1.5;
             glutPostRedisplay();
             break;
        case 'd':
        case 'D':
             if((detail==20)&&!isAuto){
               detail=2000;
             }else{
               detail=20;
             }
             for(float i=xMinJ; i<=xMaxJ; i+=(xMaxJ-xMinJ)/width*2){
               for(float c=yMinJ; c<=yMaxJ; c+=(yMaxJ-yMinJ)/height){        
                 GenerateJulia( i, c, cr, ci, detail);
               }
             }
             glFlush();
             break;
        case 'a':
        case 'A':
             if(isAuto==0){
               isAuto=1;
               ClearCross();
               DrawAxes();
             }else{
               isAuto=0;
               for(float x=xMinM; x<=xMaxM; x+=(xMaxM-xMinM)/width*2){
                 for(float y=yMinM; y<=yMaxM; y+=(yMaxM-yMinM)/height){
                   GenerateMandelbrot(x,y,400); 
                 }
               }
               DrawAxes();
               DrawCross();
               glFlush();
             }
             break;
        case '=':
        case '+':
             power++;
             GetRanges();
             glutPostRedisplay();
             break;
        case '-':
        case '_':
             if(power>2)power--;
             GetRanges();
             glutPostRedisplay();
             break;
        case 27:
             exit(0);
             break;
     }
}

void motion( int x, int y )
{
     //detects the mouse being dragged and generates Julia plots as user rolls over Mandelbrot side
     if((x<width/2)&&!isAuto){
         ClearCross();
         DrawAxes();
         cr=(x/(width/2))*(xMaxM-xMinM)+xMinM;
         ci=((height-y)/height)*(yMaxM-yMinM)+yMinM;
         for(float i=xMinJ; i<=xMaxJ; i+=(xMaxJ-xMinJ)/width*2){
           for(float c=yMinJ; c<=yMaxJ; c+=(yMaxJ-yMinJ)/height){        
             GenerateJulia( i, c, cr, ci, detail);
           }
         }
         DrawCross();
         glFlush();
       }
}

void special(int key, int x, int y)
{
     if(!isAuto){
     ClearCross();
     switch(key){
        case GLUT_KEY_LEFT:
             cr-=(xMaxM-xMinM)/width*4;
             if(cr<xMinM)cr=xMinM;
             break;
        case GLUT_KEY_RIGHT:
             cr+=(xMaxM-xMinM)/width*4;
             if(cr>xMaxM)cr=xMaxM;
             break;
        case GLUT_KEY_UP:
             ci+=(yMaxM-yMinM)/height*2;
             if(ci>yMaxM)ci=yMaxM;
             break;
        case GLUT_KEY_DOWN:
             ci-=(yMaxM-yMinM)/height*2;
             if(ci<yMinM)ci=yMinM;
             break;
     }
     DrawAxes();
     for(float i=xMinJ; i<=xMaxJ; i+=(xMaxJ-xMinJ)/width*2){
       for(float c=yMinJ; c<=yMaxJ; c+=(yMaxJ-yMinJ)/height){        
             GenerateJulia( i, c, cr, ci, detail);
       }
     }
     DrawCross();
     glFlush();
     }
}

void idle()
{
     if(isAuto)Animate();
     usleep(15000);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   int test=1;
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (1000, 500); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("Mandelbrot/Julia Explorer");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutMotionFunc(motion);
   glutSpecialFunc(special);
   glutIdleFunc(idle);
   glutMainLoop();
   return 0;
}
