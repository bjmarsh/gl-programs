#include <GL/gl.h>
#include <GL/glut.h>
#include <cstdlib>
#include <string>
#include "shapes.cpp"
#include "../GLText/GLText.cpp"

using namespace std;

class BUIComponent
{
     protected:
      bool visible;
      int stackPosition;
      float ph,pw;
      BColor primaryColor;
      BColor secondaryColor;
     
     public:
      bool isVisible(){
        if(visible)return true;
        else return false;
      }
      
      BColor getPrimaryColor(){return primaryColor;}
      BColor getSecondaryColor(){return secondaryColor;}
      
      void setVisible(bool v){
        visible=v;
      }
      void setPrimaryColor(BColor c){
        primaryColor=c;
      }
      void setSecondaryColor(BColor c){
        secondaryColor=c;
      }
      void setStackPosition(int p){
        stackPosition=p;
      }
      void setPDs(float w, float h){
        pw=w;
        ph=h;
      }
      
      virtual void Draw(){}
};

class BUICanvas
{
      private:
       float pixelWidth, pixelHeight;
       BUIComponent *stack[50];
       float dimensions[4];  //[xmin, xmax, ymin, ymax]
       float windowDimensions[2];  //[width,height]
       int numComponents;
       
      public:
       BUICanvas(){
        pixelWidth=pixelHeight=1;
        numComponents=0;
       }
       
       float getPixelWidth(){return pixelWidth;}
       float getPixelHeight(){return pixelHeight;}
       float getNumberOfComponents(){return numComponents;}
       
       void setPixelWidth(float w){
         pixelWidth=w;
         if(numComponents>0){
           for(int i=0; i<numComponents; i++)
             stack[i]->setPDs(pixelWidth,pixelHeight);
         }
       }
       
       void setPixelHeight(float h){
         pixelHeight=h; 
         if(numComponents>0){
           for(int i=0; i<numComponents; i++)
             stack[i]->setPDs(pixelWidth,pixelHeight);
         }  
       } 
       
       void setWindowDimensions(float w, float h){
         windowDimensions[0]=w;
         windowDimensions[1]=h;
         setPixelWidth((dimensions[1]-dimensions[0])/w);
         setPixelHeight((dimensions[3]-dimensions[2])/h);
       }
       
       void setDimensions(float xmin, float xmax, float ymin, float ymax){
         dimensions[0]=xmin;
         dimensions[1]=xmax;
         dimensions[2]=ymin;
         dimensions[3]=ymax;
         setPixelWidth((dimensions[1]-dimensions[0])/windowDimensions[0]);
         setPixelHeight((dimensions[3]-dimensions[2])/windowDimensions[1]);
       }
       
       BPoint convertMouseCoordinates(float x, float y){
         return BPoint(x/windowDimensions[0]*(dimensions[1]-dimensions[0])+dimensions[0],(windowDimensions[1]-y)/windowDimensions[1]*(dimensions[3]-dimensions[2])+dimensions[2]);
       }
       
       BUIComponent add(BUIComponent& c){
         stack[numComponents]=&c;
         c.setStackPosition(numComponents);
         c.setPDs(pixelWidth,pixelHeight);
         numComponents++;
       }
       
       void update(){
         if(numComponents>0){
           for(int i=0; i<numComponents; i++)
             if(stack[i]->isVisible())stack[i]->Draw();
         } 
       } 
       
};

class BButton : public BUIComponent
{
     private:
      BRectangle button;
      string text;
      void *font;
      int size;
      
     public: 
      bool isClicked;     
      BButton(float x, float y, float width, float height, string t){
         font=GLUT_BITMAP_HELVETICA_12;
         size=1;
         button.setVertex(BPoint(x,y));
         button.setWidth(width);
         button.setHeight(height);
         text=t;
         setPrimaryColor(BColor(1,1,1,1));
         setSecondaryColor(BColor(0,0,0,1));
         visible=true;
         isClicked=false;
      }
      
      string getText(){return text;}
      int getSize(){return size;}
      float getWidth(){return button.getWidth();}
     float getHeight(){return button.getHeight();}
      float getX(){return button.getVertex().getX();}
      float getY(){return button.getVertex().getY();}
      
      void setText(string t){text=t;}
      void setWidth(float w){button.setWidth(w);}
      void setHeight(float h){button.setHeight(h);}
      void setX(float x){button.setVertex(BPoint(x,button.getVertex().getY()));}
      void setY(float y){button.setVertex(BPoint(button.getVertex().getX(),y));}
      
      void setSize(int s){
        if(s>2)s=2;
        if(s<0)s=0;
        size=s;
        if(s==0)font=GLUT_BITMAP_HELVETICA_10;
        if(s==1)font=GLUT_BITMAP_HELVETICA_12;
        if(s==2)font=GLUT_BITMAP_HELVETICA_18;
      }
      
      bool isInButton(BPoint p){
        if(p.getX()>button.getVertex().getX() && 
           p.getY()<button.getVertex().getY() && 
           p.getX()<button.getVertex().getX() + button.getWidth() &&
           p.getY()>button.getVertex().getY() - button.getHeight())
             return true;
        else return false;
      }
      
      virtual void Draw(){
         glColor4f(primaryColor.getRed(),primaryColor.getGreen(),primaryColor.getBlue(),primaryColor.getAlpha());
         button.Draw();
         glColor4f(secondaryColor.getRed(),secondaryColor.getGreen(),secondaryColor.getBlue(),secondaryColor.getAlpha());
         DrawText(button.getVertex().getX()+4*pw,button.getVertex().getY()-button.getHeight()+2*ph,0,text,font);
      }
};

// BUICanvas canvas;
// BButton b1(100,200,60,15,"Click Me");    

// void init(void) 
// {
//    glClearColor (0.0, 0.0, 0.0, 0.0);
//    glShadeModel (GL_FLAT);
//    canvas.add(b1);
// }

// void display(void)
// {
//    glClear (GL_COLOR_BUFFER_BIT);
//    glColor3f (1.0, 1.0, 1.0);
   
//    canvas.update();

//    glutSwapBuffers ();
// }

// void reshape (int w, int h)
// {
//    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
//    glMatrixMode (GL_PROJECTION);
//    glLoadIdentity ();
//    gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
// }

// void mouse(int button, int state, int x, int y)
// {
//      if(b1.isInButton(BPoint(x,500-y)) && state==GLUT_DOWN)
//        b1.setPrimaryColor(BColor(0.5,0.5,1));
//      if(state==GLUT_UP)
//        b1.setPrimaryColor(BColor(1,1,1));
//      glutPostRedisplay();
// }

// int main(int argc, char** argv)
// {
//    glutInit(&argc, argv);
//    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
//    glutInitWindowSize (500, 500); 
//    glutInitWindowPosition (0, 0);
//    glutCreateWindow ("GLUT Demo");
//    init ();
//    glutDisplayFunc(display); 
//    glutReshapeFunc(reshape);
//    glutMouseFunc(mouse);
//    glutMainLoop();
//    return 0;
// }
