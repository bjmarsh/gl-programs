#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <unistd.h>
#include "../GLText/GLText.cpp"

#define PI 3.1415926535898
#define G 6.673e-11            //universal gravitational constant
#define MAX_NUM_SATELLITES 25
#define MOONVIEW 122
#define CLOSEVIEW 8

float ConvertKmToPixel(float km);

int numSatellites=0;
float width,height;
float kmPerPixel;
float initX,initY,curX,curY;
bool mouseIsDown=false, trailIsOn=false, isRed=false;
int currentView=CLOSEVIEW;
int currentSpeedMult=20;

class Body{
      public:
         float posx,posy,velx,vely,radius,mass;
         
         void SetValues(float _posx, float _posy, float _velx, float _vely, float _radius, float _mass)
         {
            posx=_posx;
            posy=_posy;
            velx=_velx;
            vely=_vely;
            radius=_radius;
            mass=_mass;
         }
         
         Body(float _posx, float _posy, float _velx, float _vely, float _radius, float _mass)
         {
            SetValues(_posx,_posy,_velx,_vely,_radius,_mass);
         }
         
         Body(){}
         void Draw(float red, float green, float blue);
         void operator = (Body);
         
}satellites[MAX_NUM_SATELLITES];

Body earth(0, 0, 0, 0, 6378.1, 5.9742e24);

void Body::Draw(float red, float green, float blue)
{
     glColor3f(red,green,blue);
     glBegin(GL_POLYGON);
       for(float angle=0; angle<=2*PI; angle+=PI/25)
         glVertex2f(ConvertKmToPixel(radius*cos(angle)+posx),ConvertKmToPixel(radius*sin(angle)+posy));
     glEnd();
}

void Body::operator=(Body body)
{
     posx=body.posx;
     posy=body.posy;
     velx=body.velx;
     vely=body.vely;
     radius=body.radius;
     mass=body.mass;
}

float ConvertKmToPixel(float km)
{
     return km/kmPerPixel;
}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   numSatellites=1;
   satellites[0].SetValues(0,8000,7035*20,0,204,7.36e22);
}

void display(void)
{
   if(!trailIsOn)glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   
   earth.Draw(0,0,1);
   
   if(numSatellites>0){
     for(int s=0; s<numSatellites; s++){
        float distFromEarth=sqrt(pow(satellites[s].posx,2)+pow(satellites[s].posy,2));
        float Fg=G*earth.mass*satellites[s].mass/pow(distFromEarth*1000,2);
        float acceleration=Fg/satellites[s].mass;
        float slope,accelX,accelY;
        if(satellites[s].posx!=0){
           slope=satellites[s].posy/satellites[s].posx;
           accelX=acceleration/sqrt(slope*slope+1)*(-fabs(satellites[s].posx)/satellites[s].posx);
           accelY=fabs(accelX*slope)*(-fabs(satellites[s].posy)/satellites[s].posy);
        }else{
           accelX=0;
           accelY=acceleration*(-fabs(satellites[s].posy)/satellites[s].posy);
        }
        float div=1.0/currentSpeedMult/currentSpeedMult;
        satellites[s].velx+=accelX/div;
        satellites[s].vely+=accelY/div;
        satellites[s].posx+=satellites[s].velx/1000;
        satellites[s].posy+=satellites[s].vely/1000;
        if(distFromEarth<earth.radius+satellites[s].radius){
          for(int i=s; i<=numSatellites-1; i++)
               satellites[i]=satellites[i+1];
          numSatellites--;
        }else{
           satellites[s].Draw(1,1,1);
        } 
     }
   }
   
   glColor3f(1,1,1);
   if(isRed  && mouseIsDown)glColor3f(1,0,0);
   
   float dist=sqrt(initX*initX+initY*initY)-earth.radius;
   float fpartDist=(dist-floor(dist))*1000;
   if(dist>=0 && !trailIsOn){
      DrawText(-width/2+2,-height/2+5,0,"Altitude: ",GLUT_BITMAP_HELVETICA_18);
      DrawInt((int)dist,GLUT_BITMAP_HELVETICA_18);
      DrawText(".",GLUT_BITMAP_HELVETICA_18);
      DrawInt((int)fpartDist,GLUT_BITMAP_HELVETICA_18);
      DrawText(" km",GLUT_BITMAP_HELVETICA_18);
   }
   
   if(mouseIsDown && !trailIsOn){
        glBegin(GL_LINES);
          glVertex2f(ConvertKmToPixel(initX),ConvertKmToPixel(initY));
          glVertex2f(curX,curY);
        glEnd();
        float velocity=sqrt(pow((curX*kmPerPixel-initX)*15,2)+pow((curY*kmPerPixel-initY)*15,2))/1000/currentSpeedMult;
        float fpartVelocity=(velocity-floor(velocity))*1000;
        DrawText(-width/2+2,-height/2+22,0,"Velocity: ",GLUT_BITMAP_HELVETICA_18);
        DrawInt((int)velocity,GLUT_BITMAP_HELVETICA_18);
        DrawText(".",GLUT_BITMAP_HELVETICA_18);
        DrawInt((int)fpartVelocity,GLUT_BITMAP_HELVETICA_18);
        DrawText(" km/s",GLUT_BITMAP_HELVETICA_18);
     }

   glutSwapBuffers ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   height=h;
   width=w;
   gluOrtho2D (-width/2,width/2,-height/2,height/2);
   kmPerPixel = earth.radius * currentView / height;
   glClear(GL_COLOR_BUFFER_BIT);
}

void idle()
{      
     glutPostRedisplay();
     usleep(500);
}

void mouse(int button, int state, int x, int y)
{
     if(state==GLUT_DOWN){
       initX=(x-width/2)*kmPerPixel;
       initY=(height-y-height/2)*kmPerPixel;
       curX=ConvertKmToPixel(initX);
       curY=ConvertKmToPixel(initY);
       mouseIsDown=true;
     }else{
           float finalY=(height-(float)y-height/2)*kmPerPixel;
           float finalX=((float)x-width/2)*kmPerPixel;
           if(numSatellites<MAX_NUM_SATELLITES){
             numSatellites++;
             satellites[numSatellites-1].SetValues(initX,initY,(finalX-initX)*15,(finalY-initY)*15,2*kmPerPixel,7.36e22);
           }else{
             for(int i=0; i<=MAX_NUM_SATELLITES-2; i++)
               satellites[i]=satellites[i+1];
             satellites[MAX_NUM_SATELLITES-1].SetValues(initX,initY,(finalX-initX)*15,(finalY-initY)*15,2*kmPerPixel,7.36e22);
           }
           mouseIsDown=false;
     }
}

void motion(int x, int y)
{
     curX=x-width/2;
     curY=height-y-height/2;
     isRed=false;
     if(sqrt(pow((float)((curX*kmPerPixel-initX)*15),2)+pow((float)((curY*kmPerPixel-initY)*15),2))>=sqrt(2*G*earth.mass*pow((float)currentSpeedMult,2)/sqrt(initX*initX+initY*initY)/1000))
       isRed=true;
}

void passive(int x, int y)
{
     initX=(x-width/2)*kmPerPixel;
     initY=(height-y-height/2)*kmPerPixel;
}

void keyboard(unsigned char key, int x, int y)
{
     switch(key){
       case 'c':
       case 'C':
            glClear(GL_COLOR_BUFFER_BIT);
            break;
       case 't':
       case 'T':
            trailIsOn=!trailIsOn;
            glClear(GL_COLOR_BUFFER_BIT);
            break;
       case 'r':
       case 'R':
            numSatellites=1;
            if(currentView==CLOSEVIEW){
              satellites[0].SetValues(0,8000,7035*currentSpeedMult,0,2*kmPerPixel,7.36e22);
            }else{
              satellites[0].SetValues(0,384405,1022*currentSpeedMult,0,1737.4,7.36e22);
            }
            glClear(GL_COLOR_BUFFER_BIT);
            break;
       case 'V':
       case 'v':
            if(currentView==MOONVIEW){
              currentView=CLOSEVIEW;
              kmPerPixel = earth.radius * currentView / height;
              currentSpeedMult=20;
              satellites[0].SetValues(0,8000,7035*currentSpeedMult,0,2*kmPerPixel,7.36e22);
            }else{
              currentView=MOONVIEW;
              currentSpeedMult=500;
              satellites[0].SetValues(0,384405,1022*currentSpeedMult,0,1737.4,7.36e22);
            }
            kmPerPixel = earth.radius * currentView / height;
            glClear(GL_COLOR_BUFFER_BIT);
            break;
     }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("Orbits");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutKeyboardFunc(keyboard);
   glutPassiveMotionFunc(passive);
   glutMainLoop();
   return 0;
}
