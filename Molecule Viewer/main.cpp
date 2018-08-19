#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>
#include <unistd.h>
#include "../GLText/GLText.cpp"

#define PI 3.1415926535898

using namespace std;

int numAtoms=0, numConnections;
string *x, *y, *z, *types, *cons, name="";
float width,height;
string files[]={"Glucose.pdb","Testosterone.pdb","Water.pdb","Caffeine.pdb","Cholesterol.pdb","Nicotine.pdb",
                "Adenosine Triphosphate.pdb","Morphine.pdb","Aspartame.pdb","Vitamin-B12.pdb","Teflon.pdb",
                "Yellow Food Coloring.pdb","Saturated Fat.pdb","Heme.pdb","Tetradecahelicene.pdb","Fat.pdb",
                "Prozac.pdb","Polystyrene.pdb","Chlorophyll.pdb","Methyl Methacrylate.pdb","PVC.pdb",
                "Monosodium Glutamate.pdb","Polydichloroethene.pdb","Polyisobutene.pdb","Synthetic Detergent.pdb",
                "TNT.pdb","Mustard Gas.pdb","Buckminsterfullerene.pdb","Taxol.pdb","Valium.pdb","Glycogen.pdb",
                "NAD+.pdb"};
float xRot=0, yRot=0, zRot=0;
int prevX,prevY;
bool rightIsPressed=false, showCons=true, showAtoms=true, showOutline=false, isRotating=false;
float scaleFactor=2;
int numFiles=32;
int currentFile=1;
float minX=-20,minY=-20,minZ,maxX=20,maxY=20,maxZ;
float xMin,yMin,xMax,yMax;
int currentCons[5];
int consToAtom=0;
float rad=0.2,alpha=0.5;
string mode="Ball-and-Stick";
int detail=16;

string removeSpace(string s)
{
    if(s[0]==' ')
     s=s.substr(1,s.length()-1);
    if(s[s.length()-1]==' ')
     s=s.substr(0,s.length()-1);
    if(s[0]==' ' || s[s.length()-1]==' ')
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
    numConnections=0;
    string line;
    name="";
    ifstream myfile (files[currentFile-1].c_str());
    if (myfile.is_open())
    {
        while (! myfile.eof() )
        {
          getline (myfile,line);
          if(line.find("ATOM")==0 || line.find("HETATM")==0){
            numAtoms++;
          }
          if(line.find("CONECT")==0)
            numConnections++;
        }
        myfile.close();
        int counter=0,counter2=0,counter3=0;
        x = new string[numAtoms];
        y = new string[numAtoms];
        z = new string[numAtoms];
        types = new string[numAtoms];
        cons = new string[numConnections];
        ifstream myfile(files[currentFile-1].c_str());
        while (! myfile.eof() )
        {
          getline (myfile,line);
          if(line.find("ATOM")==0 || line.find("HETATM")==0){
            x[counter]=removeSpace(line.substr(31,7));
            y[counter]=removeSpace(line.substr(39,7));
            z[counter]=removeSpace(line.substr(47,7));
            if(line.length()>=77 && line.substr(76,2)!="  ")
              types[counter]=removeSpace(line.substr(76,2));
            else 
              types[counter]=removeSpace(line.substr(12,2));
            counter++;
          }
          if(line.find("CONECT")==0){
            cons[counter2]=removeSpace(line);
            counter2++;
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
    if(numAtoms<400)
      detail=16;
    else
      detail=8;
}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   getCoordinates();
   glLineWidth(2);
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void setColor(int atom)
{
     if(types[atom]=="C")
       glColor4f(0.2,0.2,0.2,alpha);
     else if(types[atom]=="O")
       glColor4f(1.0,0.0,0.0,alpha);
     else if(types[atom]=="H")
       glColor4f(1.0,1.0,1.0,alpha);
     else if(types[atom]=="N")
       glColor4f(0.0,0.0,1.0,alpha);
     else if(types[atom]=="P")
       glColor4f(1.0,0.5,0.0,alpha);
     else if(types[atom]=="S")
       glColor4f(1.0,1.0,0.0,alpha);
     else if(types[atom]=="CO" || types[atom]=="Co")
       glColor4f(0.94,0.56,0.63,alpha);
     else if(types[atom]=="CL" || types[atom]=="Cl")
       glColor4f(0.12,0.94,0.12,alpha);
     else if(types[atom]=="NA" || types[atom]=="Na")
       glColor4f(0.67,0.36,0.95,alpha);
     else if(types[atom]=="ZN" || types[atom]=="Zn")
       glColor4f(0.49,0.5,0.69,alpha);
     else if(types[atom]=="F")
       glColor4f(0.56,0.88,0.31,alpha);
     else if(types[atom]=="FE" || types[atom]=="Fe")
       glColor4f(0.88,0.4,0.2,alpha);
     else if(types[atom]=="MG" || types[atom]=="Mg")
       glColor4f(0.54,1.0,0.0,alpha);
     else
       glColor4f(1.0,1.0,1.0,alpha);
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
     else if(types[atom]=="CO" || types[atom]=="Co")
       return 2.0;
     else if(types[atom]=="ZN" || types[atom]=="Zn")
       return 1.39;
     else if(types[atom]=="NA" || types[atom]=="Na")
       return 2.27;
     else if(types[atom]=="CL" || types[atom]=="Cl")
       return 1.75;
     else if(types[atom]=="F")
       return 1.47;
     else if(types[atom]=="FE" || types[atom]=="Fe")
       return 2.0;
     else if(types[atom]=="MG" || types[atom]=="Mg")
       return 1.73;
     else
       return 1.5;
}

void processConnections(int c)
{
     consToAtom=0;
     currentCons[0]=stringToInt(removeSpace(cons[c].substr(6,5)));
     for(int i=0; i<4; i++){
       if(cons[c].length()>=5*i+16){
         if(stringToInt(removeSpace(cons[c].substr(5*i+11,5)))!=0){
           consToAtom++;
           currentCons[consToAtom]=stringToInt(removeSpace(cons[c].substr(5*i+11,5)));
         }
       }
     }
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   float offset=0;
   if(isRotating){
     xRot=glutGet(GLUT_ELAPSED_TIME);
     xRot=xRot/1000*45;
     yRot=glutGet(GLUT_ELAPSED_TIME);
     yRot=yRot/1000*45;
   }
   for(int i=0; i<numAtoms; i++){
     glLoadIdentity();
     glTranslatef(0,0,-4);
     glScalef(scaleFactor,scaleFactor,0.1*scaleFactor);
     glTranslatef(0,0,-20);
     glRotatef(xRot,1,0,0);
     glRotatef(yRot,0,1,0);
     glTranslatef(-(minX+maxX)/2,-(minY+maxY)/2,(minZ+maxZ)/2);
     glTranslatef(stringToFloat(x[i]),stringToFloat(y[i]),-stringToFloat(z[i]));
     alpha=1.0;
     setColor(i);
     if(showAtoms)
       glutSolidSphere(rad*getSize(i),detail,detail);
   }
   glColor3f(1,1,1);
   glDisable(GL_LIGHTING);
   if(showCons){
     glLoadIdentity();
     glTranslatef(0,0,-4);
     glScalef(scaleFactor,scaleFactor,0.1*scaleFactor);
     glTranslatef(0,0,-20);
     glRotatef(xRot,1,0,0);
     glRotatef(yRot,0,1,0);
     glTranslatef(-(minX+maxX)/2,-(minY+maxY)/2,(minZ+maxZ)/2);
     for(int i=0; i<numConnections; i++){
       processConnections(i); 
       for(int c=0; c<consToAtom; c++){
         if(c<consToAtom-1){
           if(currentCons[c+1]==currentCons[c+2])
             offset=0.15;
         }
         if(currentCons[c+1]==currentCons[c])
           offset=-0.15;
         if(!showAtoms)offset=0;
         glBegin(GL_LINES);
           setColor(currentCons[0]-1);
           glVertex3f(stringToFloat(x[currentCons[0]-1]),stringToFloat(y[currentCons[0]-1])+offset,-stringToFloat(z[currentCons[0]-1]));
           setColor(currentCons[c+1]-1);
           glVertex3f(stringToFloat(x[currentCons[c+1]-1]),stringToFloat(y[currentCons[c+1]-1])+offset,-stringToFloat(z[currentCons[c+1]-1]));
         glEnd();
         offset=0;
       }
     }
   }
   if(showOutline){
     for(int i=0; i<numAtoms; i++){
       glLoadIdentity();
       glTranslatef(0,0,-4);
       glScalef(scaleFactor,scaleFactor,0.1*scaleFactor);
       glTranslatef(0,0,-20);
       glRotatef(xRot,1,0,0);
       glRotatef(yRot,0,1,0);
       glTranslatef(-(minX+maxX)/2,-(minY+maxY)/2,(minZ+maxZ)/2);
       glTranslatef(stringToFloat(x[i]),stringToFloat(y[i]),-stringToFloat(z[i]));
       rad=1.0;
       alpha=0.2;
       setColor(i);
       glutSolidSphere(rad*getSize(i),detail,detail);
       alpha=1.0;
       rad=0.2;
     }
   }
   glLoadIdentity();
   glColor3f(1,1,1);
   DrawText(xMin+3*(xMax-xMin)/width,yMin+45*(yMax-yMin)/height,-2.01,"Name: ",GLUT_BITMAP_HELVETICA_18);
   DrawText(name,GLUT_BITMAP_HELVETICA_18);
   DrawText(xMin+3*(xMax-xMin)/width,yMin+25*(yMax-yMin)/height,-2.01,"Number of Atoms: ",GLUT_BITMAP_HELVETICA_18);
   DrawInt(numAtoms,GLUT_BITMAP_HELVETICA_18);
   DrawText(xMin+3*(xMax-xMin)/width,yMin+5*(yMax-yMin)/height,-2.01,"Mode: ",GLUT_BITMAP_HELVETICA_18);
   DrawText(mode,GLUT_BITMAP_HELVETICA_18);
   glEnable(GL_LIGHTING);

   glutSwapBuffers ();
   
   if(isRotating){
     usleep(5000);
     glutPostRedisplay();
   }
}

void reshape (int w, int h)
{
    width=w;
    height=h;                       
    const float ar = (float) width / (float) height;
    glViewport(0, 0, (int)width, (int)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float dim=max((maxX-minX)/2,(maxY-minY)/2);
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
            scaleFactor=2;
            break;
       case '+':
       case '=':
            if(currentFile<numFiles)
              currentFile++;
            scaleFactor=2;
            break;
       case 'M':
       case 'm':
            if(mode=="Ball-and-Stick"){
              showAtoms=false;
              mode="Sticks";
            }else if(mode=="Sticks"){
              showAtoms=true;
              showCons=false;
              rad=0.7;
              mode="Spacefilling";
            }else if(mode=="Spacefilling"){
              showCons=true;
              showOutline=true;
              rad=0.2;
              mode="Combination";
            }else if(mode=="Combination"){
              showOutline=false;
              mode="Ball-and-Stick";
            }
            break;
       case 'r':
       case 'R':
            isRotating=!isRotating;
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

    glutCreateWindow("Molecule Viewer");

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
