//#include <GL/gl.h>
//#include <GL/glut.h>
//#include <ctime>
//#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

void DrawText(float x, float y, float z, string text, void *font)
{
     glRasterPos3f(x,y,z);
     for(int i=0; i<text.length(); i++)
       glutBitmapCharacter(font, text[i]);
}

void DrawText(string text, void *font)
{
     for(int i=0; i<text.length(); i++)
       glutBitmapCharacter(font, text[i]);
}

void DrawInt(float x, float y, float z, int num, void *font)
{
     glRasterPos3f(x,y,z);
     string s="";
     bool isNeg=false;
     if(num==0){
        glutBitmapCharacter(font, '0');
        return;
     }
     if(num<0){
	isNeg=true;
	num*=-1;
     }
     string ref="0123456789";
     while(num>0){
	s=ref[num%10]+s;
	num/=10;
     }
     if(isNeg)s='-'+s;
     DrawText(x,y,z,s,font);
}

void DrawInt(int num, void *font)
{
     string s="";
     bool isNeg=false;
     if(num==0){
        glutBitmapCharacter(font, '0');
        return;
     }
     if(num<0){
	isNeg=true;
	num*=-1;
     }
     string ref="0123456789";
     while(num>0){
	s=ref[num%10]+s;
	num/=10;
     }
     if(isNeg)s='-'+s;
     DrawText(s,font);
}

void DrawFloat(int x, int y, int z, float num, int decimalDigits, void *font)
{
	glRasterPos3f(x,y,z);
	float fpart=fabs(num)-floor(fabs(num));
	DrawInt(x,y,z,(int)(num>0)?floor(num):ceil(num),font);
	DrawText(".",font);
	DrawInt((int)(fpart*pow(10.0,decimalDigits)),font);
}

void DrawFloat(float num, int decimalDigits, void *font)
{
	float fpart=fabs(num)-floor(fabs(num));
	DrawInt((int)(num>0)?floor(num):ceil(num),font);
	DrawText(".",font);
	DrawInt((int)(fpart*pow(10.0,decimalDigits)),font);
}

/*
void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   
   DrawText(100,100,0,"Hello World!",GLUT_BITMAP_HELVETICA_18);
   DrawInt(100,300,0,1234,GLUT_BITMAP_HELVETICA_18);
   
   glutSwapBuffers ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow ("GLUT Demo");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;
}
*/
