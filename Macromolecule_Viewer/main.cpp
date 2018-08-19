#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>
#include "../GLText/GLText.cpp"

#define PI 3.1415926535898

using namespace std;

int numAtoms=0, numConnections;
string *x, *y, *z, *types, name="";
float width,height;
string files[]={"DNA.pdb","tRNA.pdb","Hemoglobin.pdb","Insulin.pdb"};
float xRot=0, yRot=0;
int prevX,prevY;
bool rightIsPressed=false;
float scaleFactor=1;
int numFiles=4;
int currentFile=1;
float minX,minY,minZ,maxX,maxY,maxZ;
float xMin,yMin,xMax,yMax;
bool showHetatms=true;

string removeSpace(string s)
{
    if(s[0]==' ')
     s=s.substr(1,s.length()-1);
    if(s[0]==' ')
     s=removeSpace(s);
    return s;
}

int stringToInt(string s)
{
    string ref="0123456789";
    int num=0;
    for(int i=0; i<s.length(); i++)
        num=10*num+ref.find(s[i]);
    return num;
}

float stringToFloat(string s)
{
    bool isNegative=false;
    float ans;
    if(s[0]=='-'){
      s=s.substr(1,s.length()-1);
      isNegative=true;
    }
    int decPos=s.find(".");
    string iPart=s.substr(0,decPos);
    string fPart=s.substr(decPos+1,3);
    ans=(stringToInt(iPart)+(float)stringToInt(fPart)/1000);
    if(isNegative)ans*=-1.0;
    return ans;
}

float getMin(string* x)
{
      float min=stringToFloat(x[0]);
      for(int i=1;i<numAtoms; i++){
        if(stringToFloat(x[i])<min)
          min=stringToFloat(x[i]);
      }
      return min;
}

float getMax(string* x)
{
      float max=stringToFloat(x[0]);
      for(int i=1;i<numAtoms; i++){
        if(stringToFloat(x[i])>max)
          max=stringToFloat(x[i]);
      }
      return max;
}

void getCoordinates()
{
    numAtoms=0;
    scaleFactor=1;
    string line;
    name="";
    ifstream myfile (files[currentFile-1].c_str());
    if (myfile.is_open())
    {
        while (! myfile.eof() )
        {
          getline (myfile,line);
          if(line.find("ATOM")==0 || (line.find("HETATM")==0 && showHetatms)){
            numAtoms++;
          }
        }
        myfile.close();
        int counter=0,counter2=0,counter3=0;
        x = new string[numAtoms];
        y = new string[numAtoms];
        z = new string[numAtoms];
        types = new string[numAtoms];
        ifstream myfile(files[currentFile-1].c_str());
        while (! myfile.eof() )
        {
          getline (myfile,line);
          if(line.find("ATOM")==0 || (line.find("HETATM")==0 && showHetatms)){
            x[counter]=removeSpace(line.substr(31,7));
            y[counter]=removeSpace(line.substr(39,7));
            z[counter]=removeSpace(line.substr(47,7));
            if(line.length()>=77)
              types[counter]=removeSpace(line.substr(76,2));
            else 
              types[counter]=removeSpace(line.substr(12,2));
            counter++;
          }
          if(line.find("COMPND   2")==0)
            name=line.substr(21);
        }
        myfile.close();
        minX=getMin(x);
        maxX=getMax(x);
        minY=getMin(y);
        maxY=getMax(y);
        minZ=getMin(z);
        maxZ=getMax(z);
        if(name=="")
          name=files[currentFile-1].substr(0,files[currentFile-1].length()-4);
        if(name.find(';')>=0)
          name=name.substr(0,name.find(';'));
    }
}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   getCoordinates();
}

void setColor(int atom)
{
     if(types[atom]=="C")
       glColor3f(0.3,0.3,0.3);
     else if(types[atom]=="O")
       glColor3f(1.0,0.0,0.0);
     else if(types[atom]=="H")
       glColor3f(1.0,1.0,1.0);
     else if(types[atom]=="N")
       glColor3f(0.0,0.0,1.0);
     else if(types[atom]=="P")
       glColor3f(1.0,0.5,0.0);
     else if(types[atom]=="S")
       glColor3f(1.0,1.0,0.0);
     else if(types[atom]=="CO")
       glColor3f(0.94,0.56,0.63);
     else if(types[atom]=="CL")
       glColor3f(0.12,0.94,0.12);
     else if(types[atom]=="NA")
       glColor3f(0.67,0.36,0.95);
     else if(types[atom]=="ZN")
       glColor3f(0.49,0.5,0.69);
     else
       glColor3f(1.0,1.0,1.0);
}

float getSize(int atom)
{
     if(types[atom]=="C")
       return 1.7;
     else if(types[atom]=="O")
       return 1.52;
     else if(types[atom]=="H")
       return 1.09;
     else if(types[atom]=="N")
       return 1.55;
     else if(types[atom]=="P")
       return 1.8;
     else if(types[atom]=="S")
       return 1.8;
     else if(types[atom]=="CO")
       return 2.0;
     else if(types[atom]=="ZN")
       return 1.39;
     else if(types[atom]=="NA")
       return 2.27;
     else if(types[atom]=="CL")
       return 1.75;
     else
       return 1.5;
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   for(int i=0; i<numAtoms; i++){
     glLoadIdentity();
     glTranslatef(0,0,-4);
     glScalef(scaleFactor,scaleFactor,0.1*scaleFactor);
     glTranslatef(0,0,-20);
     glRotatef(yRot,0,1,0);
     glRotatef(xRot,cos(yRot*PI/180),0,sin(yRot*PI/180));
     glTranslatef(-(minX+maxX)/2,-(minY+maxY)/2,(minZ+maxZ)/2);
     glTranslatef(stringToFloat(x[i]),stringToFloat(y[i]),-stringToFloat(z[i]));
     setColor(i);
     glutSolidSphere(0.5*getSize(i),6,6);
   }
   glColor3f(1,1,1);
   glDisable(GL_LIGHTING);
   glLoadIdentity();
   DrawText(xMin,yMin,-2,name,GLUT_BITMAP_HELVETICA_18);
   glEnable(GL_LIGHTING);

   glutSwapBuffers ();
}

void reshape (int w, int h)
{
    width=w;
    height=h;                       
    const float ar = (float) width / (float) height;
    glViewport(0, 0, (int)width, (int)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float dim=(maxX-minX)/2;
    xMin=-dim*ar;
    xMax=dim*ar;
    yMin=-dim;
    yMax=dim;
    glFrustum(xMin,xMax,yMin,yMax, 2.0, 135.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

void mouse(int button, int state, int x, int y)
{
     prevX=x;
     prevY=y;
     if(state==GLUT_DOWN && button==GLUT_RIGHT_BUTTON)
       rightIsPressed=true;
     else
       rightIsPressed=false;
}

void motion(int x, int y)
{
     if(rightIsPressed){
       scaleFactor+=(float)(prevY-y)/20;
       if(scaleFactor<0)scaleFactor=0;
     }else{
       if(x>prevX)yRot+=abs(x-prevX);
       if(x<prevX)yRot-=abs(x-prevX);
       if(y>prevY)xRot+=abs(y-prevY);
       if(y<prevY)xRot-=abs(y-prevY);
     }
     prevX=x;
     prevY=y;
     glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
     switch(key){
       case '-':
       case '_':
            if(currentFile>1)
              currentFile--;
            break;
       case '+':
       case '=':
            if(currentFile<numFiles)
              currentFile++;
            break;
       case 'h':
       case 'H':
            showHetatms=!showHetatms;
            break;
     }
     getCoordinates();
     reshape(width,height);
     glutPostRedisplay();
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

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Macromolecule Viewer");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
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
    
    init();

    glutMainLoop();

    return EXIT_SUCCESS;
}
