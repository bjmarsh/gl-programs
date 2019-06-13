/* WAVE POOL
 * By Bennett Marsh
 * March 2012
 *
 * Implements the 2-dimensional wave equation to simulate a wave pool on
 * the screen. By cycling through the various source, wall, and medium 
 * setups, a variety of wave phenomena can be observed, including diffraction,
 * refraction, reflection, interference, beat frequencies, and the Doppler
 * effect.  By clicking and dragging you can interact with the waves and create
 * ripples.  Using the right mouse button allows you to draw walls.
 *
 * CONTROLS
 * Left click and drag - create ripples
 * Right click and drag - draw walls, or, if probe is on, 
 *                        move the probe around screen
 * 'S' key - turn source on/off
 * + and - keys - cycle through source setups
 * [ and ] keys - cycle through wall setups
 * < and > keys - cycle through medium setups (lenses/prisms)
 * 'G' key - turn probe on/off
 * 'C' key - clear all waves
 * 'K' key - cycle through color schemes
 * 'W' key - clear all walls
 * 'E' key - turn borders on/off
 * 'B' key - toggle blending mode on/off.  
 *           Blending looks better but runs slightly slower.
 * 'P' key - pause
 * Space - advance one frame when paused
*/

#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <string>
#include <unistd.h>

#define PI 3.14159295358979
#define N 231                  //number of data points in 1 direction. Should be odd
#define BUF 75                 //size of buffer to dissipate energy
#define TOT BUF+N+BUF
#define NDPTS 500
#define LEN (N>NDPTS)?N:NDPTS

float width,height;     //screen dimensions
float temp[TOT][TOT];   //node positions
float nTemp[TOT][TOT];  //node velocities
float maxT[N][N];       //saves max value at each node to visualize intensity
bool wall[TOT][TOT];    //wall on/off
int prevX,prevY;
bool paused=false,source=true,blend=true;  
bool leftDown=false; 
bool advanced=false;
float t=0;    
int scheme=0;
int sourceType=0;
int wallType=0;
int mediumType=0;
int probe=0;
float probeData[LEN];
int probeX,probeY,probeY2;
int view=0;

void resetProbe(bool q)   //set probe vals to 0
{
     if(!(!q && probe==1)){
       for(int i=0; i<NDPTS; i++)
         probeData[i]=0;
     }
}

void resetView()
{
     for(int a=0; a<N; a++)
         for(int b=0; b<N; b++)
           maxT[a][b]=0;
}

void init(void) 
{
   glClearColor (0,0,0,0);
   srand(time(NULL));
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glShadeModel (GL_SMOOTH);
   glClear (GL_COLOR_BUFFER_BIT);
   for(int a=0; a<TOT; a++){
     for(int b=0; b<TOT; b++){
       temp[a][b]=0;
       nTemp[a][b]=0;
       wall[a][b]==false;
     }
   }
   resetProbe(1);
   probeX=(TOT-1)/2;
   probeY=BUF+3*N/4;
   probeY2=BUF+3*N/4;

}

float setMedium(int a, int b)
{
   float c=2;
   int p=30;
   switch(mediumType){
     case 0:
       break;
     case 1: //diagonal prism
       if((b-BUF)>(1.2*N-(a-BUF)) && (b-BUF)<(1.5*N-(a-BUF)))
         c=0.5;
       break;
     case 2:  //circular prism
       if((b-BUF-N/2)*(b-BUF-N/2)+(a-BUF-N/2)*(a-BUF-N/2)<900)
         c=0.5;
       break;
     case 3:  //biconvex lens
       if(((b-BUF-N/2-p+30)*(b-BUF-N/2-p+30)+(a-BUF-N/2)*(a-BUF-N/2)<2500) &&
          ((b-BUF-N/2+p+30)*(b-BUF-N/2+p+30)+(a-BUF-N/2)*(a-BUF-N/2)<2500))
         c=0.5;
       break;
     case 4:  //weird doughnut shaped thing
       if(((b-BUF-N/2)*(b-BUF-N/2)+(a-BUF-N/2)*(a-BUF-N/2)<1600) &&
          ((b-BUF-N/2)*(b-BUF-N/2)+(a-BUF-N/2)*(a-BUF-N/2)>400))
         c=0.5;
       break;
   }
   return c;
}

void setColor(int A, int  B)
{
   float r=0,g=0,b=0;
   float t=temp[A][B];
   if(view==1)
     t=maxT[A][B];
   
   if(scheme==0){  //red/black/cyan
       t*=2;
       r=t;
       g=-t;
       b=-t;
   }else if(scheme==1){  //black/cyan
       t*=2;
       r=0;
       g=(t+1)/2;
       b=(t+1)/2;
   }else if(scheme==2){  //yellow/black/blue
       t*=3;
       t=fabs(t);
       r=t*t/4;
       g=t;
       b=(1.5-t);
   }else if(scheme==3){  //green/black/red
       t*=2;
       r=-t;
       g=t;
       b=0;
   }else if(scheme==4){  //blue/yellow/green
       t*=2;
       r=t;
       g=t;
       b=-t;
   }else if(scheme==5){  //bright blue
       t*=2;
       t=fabs(t);
       r=0;
       g=1-t;
       b=1-t*t*t;
   }else if(scheme==6){  //bright blue
       b=g=r=exp(-100*t*t);
   }
   
   glColor3f(r,g,b);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f(1,1,1);
   
   if(!paused || advanced){
       advanced=false;
         for(int iter=0; iter<2; iter++){  
           for(int a=0; a<TOT; a++){      //update velocities according to wave equation
             for(int b=0; b<TOT; b++){
               float c=setMedium(a,b);
               if(b>0 && !wall[a][b-1]) nTemp[a][b]+=c/4*(temp[a][b-1]-temp[a][b]);
               if(b<TOT && !wall[a][b+1]) nTemp[a][b]+=c/4*(temp[a][b+1]-temp[a][b]);
               if(a>0 && !wall[a-1][b]) nTemp[a][b]+=c/4*(temp[a-1][b]-temp[a][b]);
               if(a<TOT && !wall[a+1][b]) nTemp[a][b]+=c/4*(temp[a+1][b]-temp[a][b]);
               float d=0.0075;
               if(b>N+BUF)   //increase damping factor if in buffer
                 d=(float)(b-N-BUF)/(BUF);
               else if(a>N+BUF)
                 d=(float)(a-N-BUF)/(BUF);
               else if (b<BUF)
                 d=(float)(BUF-b)/BUF;
               else if (a<BUF)
                 d=(float)(BUF-a)/BUF;

               nTemp[a][b]-=d*nTemp[a][b];
             }
           }
           for(int a=0; a<TOT; a++){   //update positions and source values
             for(int b=0; b<TOT; b++){
               temp[a][b]+=nTemp[a][b];
               
               if(sourceType==0){  //centered pt source
                 if(a==(TOT-1)/2 && b==(TOT-1)/2 && source) temp[a][b]=2*sin(3*t);
               }else if(sourceType==1){   //plane wave, low freq
                 if(b-BUF==1 && source) temp[a][b]=sin(3*t);
               }else if(sourceType==2){   //plane wave, high freq
                 if(b-BUF==1 && source) temp[a][b]=sin(9*t);
               }else if(sourceType==3){  //two sources, same frequency, in phase
                 if(a==(TOT-1)/2-20 && b==(TOT-1)/2 && source) temp[a][b]=2*sin(5*t);
                 if(a==(TOT-1)/2+20 && b==(TOT-1)/2 && source) temp[a][b]=2*sin(5*t);
               }else if(sourceType==4){  //two sources, slightly diff frequency
                 if(a==(TOT-1)/2-20 && b==(TOT-1)/2 && source) temp[a][b]=2*sin(3.4*t);
                 if(a==(TOT-1)/2+20 && b==(TOT-1)/2 && source) temp[a][b]=2*sin(4*t);
               }else if(sourceType==5){  //pt souce from bottom
                 if(a==(TOT-1)/2 && b-BUF==1 && source) temp[a][b]=5*sin(5*t);
               }else if(sourceType==6){   //pulse plane wave
                 if(b-BUF==1 && source && t/20-floor(t/20)<=.03) temp[a][b]=(int)(t/20)%2*2-1;
               }else if(sourceType==7){   //tripole
                 if(a==(TOT-1)/2-10 && b==(TOT-1)/2-(int)(10/sqrt(3)) && source) temp[a][b]=2*sin(5*t);
                 if(a==(TOT-1)/2+10 && b==(TOT-1)/2-(int)(10/sqrt(3)) && source) temp[a][b]=2*sin(5*t);
                 if(a==(TOT-1)/2 && b==(TOT-1)/2+(int)(20/sqrt(3)) && source) temp[a][b]=2*sin(5*t);
               }else if(sourceType==8){   //parallel semi-plane waves
                 if((b==(TOT-1)/2-20 && fabs(a-BUF-N/2)<30) && source) temp[a][b]=sin(3*t);
                 if((b==(TOT-1)/2+20 && fabs(a-BUF-N/2)<30) && source) temp[a][b]=sin(3*t);
               }else if(sourceType==9){   //pulse point source, center 
                 if(a==(TOT-1)/2 && b==(TOT-1)/2 && source && t/20-floor(t/20)<=.03) temp[a][b]=2*((int)(t/20)%2*2-1);
               }else if(sourceType==10){   //parallel semi-plane waves
                 if(a==(TOT-1)/2-45 && b==(TOT-1)/2 && source) temp[a][b]=2*sin(5*t);
               }
             }
           }
           t+=.1;
       }
       //collect data
     if(probe==1){  //point probe
       for(int i=0; i<NDPTS-1; i++)
         probeData[i]=probeData[i+1];
       probeData[NDPTS-1]=temp[probeX][probeY];
     }else if(probe==2){  //cross sectional view
       for(int i=0; i<N; i++)
         probeData[i]=temp[BUF+i][probeY2];
     }else if(probe==3){     //intensity plot
       for(int i=0; i<N; i++){
         if(pow(temp[BUF+i][probeY2],2) > probeData[i])
           probeData[i]=pow(temp[BUF+i][probeY2],2);
       }
     }
     if(view==1){  //update max vals for intensity visualization
       for(int a=0; a<N; a++){
         for(int b=0; b<N; b++){
           if(fabs(temp[a+BUF][b+BUF]) > maxT[a][b])
             maxT[a][b]=fabs(temp[a+BUF][b+BUF]);
         }
       }
     }
   }
   
   glBegin(GL_TRIANGLES);   //draw to screen
   for(int at=BUF; at<N+BUF-1; at++){
     for(int bt=BUF; bt<N+BUF-1; bt++){
       float a=at-BUF;
       float b=bt-BUF;
       if(view==0)
         setColor(at,bt);
       else if(view==1)
         setColor((int)a,(int)b);
       glVertex2f((float)a/(N-1),(float)b/(N-1));
       if(blend && view==0) setColor(at+1,bt);
       glVertex2f((float)(a+1)/(N-1),(float)b/(N-1));
       if(blend && view==0) setColor(at,bt+1);
       glVertex2f((float)(a)/(N-1),(float)(b+1)/(N-1));
       
       glVertex2f((float)(a)/(N-1),(float)(b+1)/(N-1));
       if(blend && view==0) setColor(at+1,bt+1);
       glVertex2f((float)(a+1)/(N-1),(float)(b+1)/(N-1));
       if(blend && view==0) setColor(at+1,bt);
       glVertex2f((float)(a+1)/(N-1),(float)b/(N-1));
     }
   }
   glEnd();
   
   for(int at=BUF; at<N+BUF-1; at++){     //draw prisms/lenses
     for(int bt=BUF; bt<N+BUF-1; bt++){
       if(setMedium(at,bt)<2){
         float a=at-BUF;
         float b=bt-BUF;
         glColor4f(.15,.15,.5,0.5);
         glRectf((float)a/(N-1),(float)b/(N-1),(float)(a+1)/(N-1),(float)(b+1)/(N-1)); 
       }
     }
   }
   
   for(int at=BUF; at<N+BUF-1; at++){   //draw walls
     for(int bt=BUF; bt<N+BUF-1; bt++){
       if(wall[at][bt]){
           temp[at][bt]=0;
           float a=at-BUF;
           float b=bt-BUF;
           glColor3f(.5,.5,.5);
           glRectf((float)a/(N-1),(float)b/(N-1),(float)(a+1)/(N-1),(float)(b+1)/(N-1));    
       }
     }
   }
   
   if(probe){
     glColor4f(1,1,0,1);
     if(probe==1){       //draw point probe
         glBegin(GL_LINE_LOOP);
           for(float t=0; t<2*PI; t+=PI/20)
             glVertex2f(.01*cos(t)+(float)(probeX-BUF)/(N-1),.01*sin(t)+(float)(probeY-BUF)/(N-1));
         glEnd();
         float k=.01*sqrt(2)/2;
         glBegin(GL_LINES);
           glVertex2f((float)(probeX-BUF)/(N-1)+k,(float)(probeY-BUF)/(N-1)+k);
           glVertex2f((float)(probeX-BUF)/(N-1)-k,(float)(probeY-BUF)/(N-1)-k);
           glVertex2f((float)(probeX-BUF)/(N-1)+k,(float)(probeY-BUF)/(N-1)-k);
           glVertex2f((float)(probeX-BUF)/(N-1)-k,(float)(probeY-BUF)/(N-1)+k);
         glEnd();
     }else if(probe==2 || probe==3){   //draw cross-sectional probe
         glBegin(GL_LINE_LOOP);
           for(float t=PI/2; t<=3*PI/2; t+=PI/30)
             glVertex2f(.004+.004*cos(t),(float)(probeY2-BUF)/N+.004*sin(t));
           for(float t=PI/2; t<=3*PI/2; t+=PI/30)
             glVertex2f(1-.004-.004*cos(t),(float)(probeY2-BUF)/N-.004*sin(t));
         glEnd();
     }
   }
   
   if(probe){   //set up probe info area
     glColor4f(0,0,0,0.8);
     glRectf(0,0,1,0.2);
     glColor4f(1,1,1,1);
     glBegin(GL_LINES);
       glVertex2f(0,.2);
       glVertex2f(1,.2);
     glEnd();
     glColor4f(1,1,0,1);
     if(probe==1){  //draw data to screen
         glBegin(GL_LINE_STRIP);
           for(int a=0; a<NDPTS; a++)
             glVertex2f((float)a/(NDPTS-1),probeData[a]/10+.1);
         glEnd();
     }else if(probe==2){
         glBegin(GL_LINE_STRIP);
           for(int a=0; a<N; a++)
             glVertex2f((float)a/(N-1),probeData[a]/10+.1);
         glEnd();
     }else if(probe==3){
         float maxv=0;
         for(int a=0; a<N; a++){
           if(probeData[a]>maxv)
             maxv=probeData[a];
           glColor4f(probeData[a]*2,probeData[a]*2,probeData[a]*2,0.6);
           float xc=(float)a/(N);
           glRectf(xc,0,xc+1.0/N,0.2);
         }
         glColor4f(1,1,1,0.4);
         glBegin(GL_LINE_STRIP);
           for(int a=0; a<N; a++)
             glVertex2f((float)a/(N-1),probeData[a]/maxv*0.15);
         glEnd();
     }
   }

   glFlush();
}

void idle()
{
     usleep(5000);
     if(paused)
       usleep(30000);
     glutPostRedisplay();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluOrtho2D (0,1,0,1);
   width=w;
   height=h;
}

void motion(int x, int y)
{
   if(x<width && x>0 && y>0 && y<height && leftDown){
     float xstep=(float)(x-prevX)/50;
     float ystep=(float)(y-prevY)/50;
     float yt=prevY;
     float xt=prevX;
     for(int i=0; i<50; i++){
         temp[(int)(xt/width*N)+BUF][(int)((height-yt)/height*N)+BUF]=1;
         yt+=ystep;
         xt+=xstep;
     }
   }
   if(x<width && x>0 && y>0 && y<height && !leftDown){
      if(!probe){
         float xstep=(float)(x-prevX)/100;
         float ystep=(float)(y-prevY)/100;
         float yt=prevY;
         float xt=prevX;
         for(int i=0; i<100; i++){
             wall[(int)(xt/width*N)+BUF][(int)((height-yt)/height*N)+BUF]=true;
             yt+=ystep;
             xt+=xstep;
         }
      }else if(probe==1){
        probeX=(int)(x/width*N)+BUF;
        probeY=(int)((height-y)/height*N)+BUF;
      }else if(probe==2 || probe==3){
        probeY2=(int)((height-y)/height*N)+BUF;
        if(probeY2>BUF+N-2) probeY2=BUF+N-2;
        if(probeY2<BUF+1) probeY2=BUF+1;
        resetProbe(0);
      }
   }
   prevX=x;
   prevY=y;
}


void setEdges(bool on)
{
     for(int a=0; a<TOT; a++){
        wall[a][BUF]=on;
        wall[a][BUF+N-2]=on;
     }
     for(int b=0; b<TOT; b++){
        wall[BUF][b]=on;
        wall[BUF+N-2][b]=on;
     }
     
}

void setWalls(){
    for(int a=0; a<TOT; a++){
      for(int b=0; b<TOT; b++){
        wall[a][b]=false;
      }
    }
    if(wallType==1){  //single small slit
      for(int a=0; a<TOT; a++){
        if(fabs(a-BUF-N/2)>10)wall[a][BUF+N/6]=true;
      }
    }else if(wallType==2){  //single large slit
      for(int a=0; a<TOT; a++){
        if(fabs(a-BUF-N/2)>25)wall[a][BUF+N/6]=true;
      }
    }else if(wallType==3){   //double slit
      for(int a=0; a<TOT; a++){
        if(fabs(a-BUF-N/3)>5 && fabs(a-BUF-2*N/3)>5)wall[a][BUF+N/6]=true;
      }
    }else if(wallType==4){   //diffraction grating
      for(int a=0; a<TOT; a+=1){
        if(((a-BUF)/10)%2==0 || a<BUF || a>BUF+N)
          wall[a][BUF+N/6]=true;
      }
    }else if(wallType==5){    //square obstacle
      for(int a=0; a<TOT; a++){
        for(int b=0; b<TOT; b++){
          if(fabs(a-BUF-N/2)<7 && fabs(b-BUF-N/4)<7)wall[a][b]=true;
        }
      }
    }else if(wallType==6){    //line obstacle
      for(int a=0; a<TOT; a++){
        if(fabs(a-BUF-N/2)<25)wall[a][BUF+N/6]=true;
      }
    }else if(wallType==7){    //half plane
      for(int a=0; a<TOT; a++){
        if(a<BUF+N/2)wall[a][BUF+N/6]=true;
      }
    }else if(wallType==8){   //circle
      for(float t=0; t<=2*PI; t+=PI/300){
        int a=(int)(80*cos(t))+BUF+N/2;
        int b=(int)(80*sin(t))+BUF+N/2;
        wall[a][b]=true;
      }
    }else if(wallType==9){   //ellipse
      for(float t=0; t<=2*PI; t+=PI/300){
        int a=(int)(75*cos(t))+BUF+N/2;
        int b=(int)(60*sin(t))+BUF+N/2;
        wall[a][b]=true;
      }
    }else if(wallType==10){   //parabola
      for(float a=BUF+N/4; a<BUF+3*N/4; a+=0.3){
        wall[(int)a][(int)(-(a-BUF-N/2)*(a-BUF-N/2)/40+BUF+4*N/5-57)]=true;
      }
    }
}

     
void keyboard(unsigned char key, int x, int y)
{
     switch(key){
       case 'p':
       case 'P':
            paused=!paused;
            break;
       case 'S':
       case 's':
            source=!source;
            resetProbe(0);
            break;
       case 'b':
       case 'B':
            blend=!blend;
            break;
       case 'k':
       case 'K':
            scheme=(scheme+1)%7;
            break;
       case 'r':
       case 'R':
            resetProbe(1);
            resetView();
            break;
       case 'v':
       case 'V':
            view=(view+1)%2;
            resetProbe(1);
            resetView();
            break;
       case '-':
       case '_':
            if(sourceType>0)
              sourceType--;
            resetProbe(0);
            break;
       case '=':
       case '+':
            if(sourceType<10)
              sourceType++;
            resetProbe(0);
            break;
       case '[':
       case '{':
            if(wallType>0)
              wallType--;
            setWalls();
            resetProbe(0);
            break;
       case ']':
       case '}':
            if(wallType<10)
              wallType++;
            setWalls();
            resetProbe(0);
            break;
       case ',':
       case '<':
            if(mediumType>0)
              mediumType--;
            resetProbe(0);
            break;
       case '>':
       case '.':
            if(mediumType<4)
              mediumType++;
            resetProbe(0);
            break;
       case 'e':
       case 'E':
            setEdges(!wall[BUF][BUF]);
            resetProbe(0);
            break;
       case 'g':
       case 'G':
            probe=(probe+1)%4;
            resetProbe(1);
            break;
       case ' ':
            advanced=true;
            break;
       case 'c':
       case 'C':
            for(int a=0; a<TOT; a++){
              for(int b=0; b<TOT; b++){
                temp[a][b]=0;
                nTemp[a][b]=0;
              }
            }
            resetProbe(0);
            break;
       case 'w':
       case 'W':
            for(int a=0; a<TOT; a++){
              for(int b=0; b<TOT; b++){
                wall[a][b]=false;
              }
            }
            break;
     }
}

void mouse(int button, int state, int x, int y)
{
     leftDown=false;
     if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
       if(probe==1){
         probeX=(int)(x/width*N)+BUF;
         probeY=(int)((height-y)/height*N)+BUF;
       }else if(probe==2 || probe==3){
         probeY2=(int)((height-y)/height*N)+BUF;
         if(probeY2>BUF+N-2) probeY2=BUF+N-2;
         if(probeY2<BUF+1) probeY2=BUF+1;
         resetProbe(0);           
       }
     }
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
   glutCreateWindow ("Wave Pool");
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
