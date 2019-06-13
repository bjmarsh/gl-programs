#include <GL/glut.h>
#include <cmath>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
#include "../BUI/BUI.cpp"

#define PI 3.1415926535898

float ballColor[4]={0.8,0.8,0.8,1};
float loc[3]={0,0,2.2};
float delta[3]={0,0,0};
float curve[2]={0,0};
float speedMult=2;
float width,height;
float px=0,py=0,px2=0,py2=0;
float compSpeed=0.009;
float curveFactor=0.000003;
float pColor=0,pColor2=0;
float prevX,prevY,xSpeed,ySpeed;
bool gameOver=false;
bool usingPerspective=true;
float scores[2]={0,0};

BUICanvas canvas;
BButton resetButton(0.95,-0.85,0.15,0.05,"Reset");

void resize(int w, int h)
{
    width=w;
    height=h;       
                      
    const float ar = (float) width / (float) height;
    
    glViewport(0, 0, (int)width, (int)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(usingPerspective)glFrustum(-1, 2*ar-1, -1.0, 1.0, 2.0, 10.1);
    else glOrtho(-1, 2*ar-1, -1.0, 1.0, 2.0, 10.1);
    canvas.setWindowDimensions(w,h);
    canvas.setDimensions(-1,2*ar-1,-1,1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

void DrawPaddle(float x, float y, float z, float red, float green, float blue, float alpha)
{
    glLoadIdentity();
    glColor3f(1,1,1);
    
    //two lines in middle of paddle
    glBegin(GL_LINES);
      glVertex3f(x,y+0.15,z);
      glVertex3f(x,y-0.15,z);
      glVertex3f(x-0.2,y,z);
      glVertex3f(x+0.2,y,z);
    glEnd();
    
    //middle circle of paddle
    glBegin(GL_LINE_LOOP);
      for(float i=0; i<=2*PI; i+=2*PI/40)
        glVertex3f(0.03*cos(i)+x,0.03*sin(i)+y,z);
    glEnd();
    
    //border of paddle
    if(z>-10)glLineWidth(3);
    glBegin(GL_LINE_LOOP);
      for(float angle=PI/2; angle<=PI; angle+=PI/20)
        glVertex3f(0.05*cos(angle)+x-0.15, 0.05*sin(angle)+y+0.1, z);
      for(float angle=PI; angle<=3*PI/2; angle+=PI/10)
        glVertex3f(0.05*cos(angle)+x-0.15, 0.05*sin(angle)+y-0.1, z);
      for(float angle=3*PI/2; angle<=2*PI; angle+=PI/10)
        glVertex3f(0.05*cos(angle)+x+0.15, 0.05*sin(angle)+y-0.1, z);
      for(float angle=0; angle<=PI/2; angle+=PI/10)
        glVertex3f(0.05*cos(angle)+x+0.15, 0.05*sin(angle)+y+0.1, z);
    glEnd();
    glLineWidth(1);
    
    //filled part of paddle
    glColor4f(red,green,blue,alpha);
    glBegin(GL_POLYGON);
      for(float angle=PI/2; angle<=PI; angle+=PI/20)
        glVertex3f(0.05*cos(angle)+x-0.15, 0.05*sin(angle)+y+0.1, z);
      for(float angle=PI; angle<=3*PI/2; angle+=PI/10)
        glVertex3f(0.05*cos(angle)+x-0.15, 0.05*sin(angle)+y-0.1, z);
      for(float angle=3*PI/2; angle<=2*PI; angle+=PI/10)
        glVertex3f(0.05*cos(angle)+x+0.15, 0.05*sin(angle)+y-0.1, z);
      for(float angle=0; angle<=PI/2; angle+=PI/10)
        glVertex3f(0.05*cos(angle)+x+0.15, 0.05*sin(angle)+y+0.1, z);
    glEnd();
}
     
void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor4f(0.5,0.5,0.5,1);

    glLoadIdentity();
    
    for(float s=2.1; s<=10; s+=7.9/15){
       glBegin(GL_LINE_LOOP);
         glVertex3f(-1,1,-s);
         glVertex3f(-1,-1,-s);
         glVertex3f(1,-1,-s);
         glVertex3f(1,1,-s);
       glEnd();
    }

    glBegin(GL_POLYGON);
      glColor4f(0,0,0,1.2-(loc[1]+1)/2.5);
      for(float angle=2*PI; angle>=0; angle-=2*PI/20)
        glVertex3f(0.1*cos(angle)+loc[0],-1,0.1*sin(angle)-loc[2]);
    glEnd();
 
    glColor4f(0.5,0.5,0.5,1);
    glBegin(GL_LINES);
      glVertex3f(-1,-1,-2.1);
      glVertex3f(-1,-1,-10);
      glVertex3f(-1,1,-2.1);
      glVertex3f(-1,1,-10);
      glVertex3f(1,-1,-2.1);
      glVertex3f(1,-1,-10);
      glVertex3f(1,1,-2.1);
      glVertex3f(1,1,-10);
    glEnd();
    
    loc[0]+=delta[0]*speedMult;
    loc[1]+=delta[1]*speedMult;
    loc[2]+=delta[2]*speedMult;
    
    delta[0]+=curve[0];
    delta[1]+=curve[1];
    
    if(loc[0]>0.9){
      loc[0]=0.9;
      delta[0]*=-1;
      curve[0]*=0.5;
      curve[1]*=0.8;
    }
    if(loc[0]<-0.9){
      loc[0]=-0.9;
      delta[0]*=-1;
      curve[0]*=0.5;
      curve[1]*=0.8;
    }
    if(loc[1]>0.9){
      loc[1]=0.9;
      delta[1]*=-1;
      curve[0]*=0.8;
      curve[1]*=0.5;
    }
    if(loc[1]<-0.9){
      loc[1]=-0.9;
      delta[1]*=-1;
      curve[0]*=0.8;
      curve[1]*=0.5;
    }
    if(loc[2]>9.9){
      loc[2]=9.9;
      delta[2]*=-1;
      curve[0]*=0.5;
      curve[1]*=0.5;
      if(loc[0]>px2-0.3 && loc[0]<px2+0.3 && loc[1]>py2-0.25 && loc[1]<py2+0.25){
        pColor2=1;
        speedMult+=0.01;
      }else{
        gameOver=true;
        ballColor[0]=0;
        ballColor[1]=1;
        ballColor[2]=0;
        scores[0]++;
      }
    }
    if(loc[2]<2.2){
      loc[2]=2.2;
      delta[2]*=-1;
      if(loc[0]>px-0.3 && loc[0]<px+0.3 && loc[1]>py-0.25 && loc[1]<py+0.25){
        pColor=1;
        speedMult+=0.01;
        curve[0]=-curveFactor*xSpeed;
        curve[1]=-curveFactor*ySpeed;
      }else{
        gameOver=true;
        ballColor[0]=1;
        ballColor[1]=0;
        ballColor[2]=0;
        scores[1]++;
      }
    }
    
    if(pColor>0)pColor-=0.005;
    if(pColor2>0)pColor2-=0.005;
    
    float slope=delta[0]/fabs(delta[2]);
    slope=loc[0]+(10-loc[2])*slope;
    if(slope<px2 && delta[2]>0 && fabs(slope-px2)>0.05)px2-=compSpeed;
    else if(delta[2]>0  && fabs(slope-px2)>0.05) px2+=compSpeed;
    if(px2>0.8)px2=0.8;
    if(px2<-0.8)px2=-0.8;
    
    slope=delta[1]/fabs(delta[2]);
    slope=loc[1]+(10-loc[2])*slope;
    if(slope<py2 && delta[2]>0 && fabs(slope-py2)>0.05)py2-=compSpeed;
    else if(delta[2]>0 && fabs(slope-py2)>0.05)py2+=compSpeed;
    if(py2>0.8)py2=0.8;
    if(py2<-0.8)py2=-0.8;
    
    if(delta[2]<0 && (fabs(px2)>0.05 || fabs(py2)>0.05)){
      px2-=0.006*px2/fabs(px2);
      py2-=0.006*py2/fabs(py2);
    }

    glColor4fv(ballColor);
    glTranslatef(loc[0],loc[1],-loc[2]);
    glRotatef(10,0,1,0);
    glRotatef(a,1,1,1);
    glEnable(GL_LIGHTING);
    glutSolidSphere(0.1,16,16);
    glDisable(GL_LIGHTING);
    
    glLoadIdentity();
    DrawText(1.02,-0.5,-2.1,"PLAYER:",GLUT_BITMAP_HELVETICA_18);
    DrawInt(scores[0],GLUT_BITMAP_HELVETICA_18);
    DrawText(1.02,0.5,-2.1,"COMPUTER:",GLUT_BITMAP_HELVETICA_18);
    DrawInt(scores[1],GLUT_BITMAP_HELVETICA_18);
    
    DrawPaddle(px2,py2,-10,0.8,pColor2,pColor2,0.4);
    DrawPaddle(px,py,-2.1,pColor,pColor,0.8,0.4);
    
    glLoadIdentity();
    glTranslatef(0,0,-2.0);
    canvas.update();
    
    glutSwapBuffers();
    
    if(gameOver){
      time_t start,current;
      time(&start);
      do{
         time(&current);
      }while(current-start<=1);
      gameOver=false;
      delta[0]=0;
      delta[1]=0;
      delta[2]=0;
      loc[0]=0;
      loc[1]=0;
      loc[2]=2.2;
      curve[0]=0;
      curve[1]=0;
      ballColor[0]=0.8;
      ballColor[1]=0.8;
      ballColor[2]=0.8;
      compSpeed+=0.001;
      curveFactor+=0.000001;
      speedMult=2;
      px2=0;
      py2=0;
   }
}


void key(unsigned char key, int x, int y)
{
    switch (key) 
    {
        case 27 : 
        case 'q':
            exit(0);
            break;
        case 'r':
        case 'R':
            delta[0]=0;
            delta[1]=0;
            delta[2]=0;
            loc[0]=0;
            loc[1]=0;
            loc[2]=2.2;
            curve[0]=0;
            curve[1]=0;
            break;
        case 'V':
        case 'v':
            float ar=width/height;  
            usingPerspective=!usingPerspective; 
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity(); 
            if(usingPerspective)glFrustum(-1, 2*ar-1, -1.0, 1.0, 2.0, 10.1);
            else glOrtho(-1, 2*ar-1, -1.0, 1.0, 2.0, 10.1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity(); 
            break;      
    }

    glutPostRedisplay();
}

void passive(int x, int y)
{
     px=x;
     py=height-y;
     py=py/height*2-1;
     float ar=width/height;
     px=px/height*2-1;
     if(px<-0.8)px=-0.8;
     if(px>0.8)px=0.8;
     if(py<-0.85)py=-0.85;
     if(py>0.85)py=0.85;
     xSpeed=x-prevX;
     ySpeed=-(y-prevY);
     prevX=x;
     prevY=y;
}

void motion(int x, int y)
{
     px=x;
     py=height-y;
     py=py/height*2-1;
     float ar=width/height;
     px=px/height*2-1;
     if(px<-0.8)px=-0.8;
     if(px>0.8)px=0.8;
     if(py<-0.85)py=-0.85;
     if(py>0.85)py=0.85;
     xSpeed=x-prevX;
     ySpeed=-(y-prevY);
     prevX=x;
     prevY=y;
}

static void 
idle(void)
{
     usleep(3000);
     glutPostRedisplay();
}

void BUIonClick(BUIComponent c)
{
     scores[0]=0;
     scores[1]=0;
}

void mouse(int button, int state, int x, int y)
{
     if(loc[0]==0 && loc[1]==0 && loc[2]<2.3 && fabs(px)<0.25 && fabs(py)<0.2 && state==GLUT_DOWN){
        delta[0]=xSpeed/3000;
        delta[1]=ySpeed/3000;
        curve[0]=-curveFactor*xSpeed;
        curve[1]=-curveFactor*ySpeed;
        delta[2]=0.016;
        
        //for some unknown reason, if either curve value is zero the computer paddle disappears, so this prevents that
        if(curve[0]==0)curve[0]=0.000001;
        if(curve[1]==0)curve[1]=0.000001;
     }
     
     // resetButton.detectClick(x,y,state,canvas);
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 1.0f, 1.0f, 5.0f, 0.0f };
const GLfloat light1_position[] = { 0.0f, 0.0f, 0.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int 
main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(830,700);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Pong3D");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(passive);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    
    canvas.add(resetButton);
    resetButton.setSize(2);
    resetButton.setSecondaryColor(BColor(1,1,1,1));

    glClearColor(0.1,0.1,0.1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //glEnable(GL_LINE_SMOOTH);

    glutMainLoop();

    return EXIT_SUCCESS;
}

