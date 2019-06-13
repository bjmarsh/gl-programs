#include <GL/gl.h>
#include <GL/glut.h>
#include <cstdlib>


class BPoint   //a point located at (x,y)
{
     private:
      float x,y;
      
     public:
      BPoint(float x, float y){
        this->x=x;
        this->y=y;
      }
      
      BPoint(){
        x=0;
        y=0;
      }
      
      BPoint operator=(BPoint p){
            x=p.x;
            y=p.y;
            return *this;
      }
      
      float getX(){
            return x;
      }
      float getY(){
            return y;
      }
      void setX(float x){
            this->x=x;
      }
      void setY(float y){
           this->y=y;
      }
};

class BColor   //a RGBA color with values between 0.0 and 1.0
{
     private:
      float red,green,blue,alpha;
      
     public:
      BColor(float r, float g, float b, float a){
        red=r;
        green=g;
        blue=b;
        alpha=a;
      }
      
      BColor(float r, float g, float b){
        red=r;
        green=g;
        blue=b;
        alpha=1;
      }
      
      BColor(){
        red=1;
        blue=1;
        green=1;
        alpha=1;
      }
      
      float getRed(){
        return red;
      }
      float getGreen(){
        return green;
      }
      float getBlue(){
        return blue;
      }
      float getAlpha(){
        return alpha;
      }
      void set(float r, float g, float b, float a){
        red=r;
        green=g;
        blue=b;
        alpha=a;
      }
      
      void set(float r, float g, float b){
        red=r;
        green=g;
        blue=b;
        alpha=1;
      } 
};

class BRectangle   //a rectangle with top-left corner at BPoint vertex
{
     private:
      BPoint vertex;
      float width,height;
     
     public: 
      BRectangle(BPoint vertex, float width, float height){
        this->vertex = vertex;
        this->width=width;
        this->height=height;
      }
      
      BRectangle(float width, float height){
        this->width=width;
        this->height=height;
        vertex = BPoint(0,0);
      }
      
      BRectangle(){
        vertex = BPoint(0,0);
        width=1;
        height=1;
      }
      
      BPoint getVertex(){
            return vertex;
      }
      float getWidth(){
            return width;
      }
      float getHeight(){
            return height;
      }
      void setVertex(BPoint p){
           vertex=p;
      }
      void setWidth(float w){
           width=w;
      }
      void setHeight(float h){
           height=h;
      }
      
      void Draw(){
        glBegin(GL_POLYGON);
          glVertex2f(vertex.getX(),vertex.getY());
          glVertex2f(vertex.getX()+width,vertex.getY());
          glVertex2f(vertex.getX()+width,vertex.getY()-height);
          glVertex2f(vertex.getX(),vertex.getY()-height);
        glEnd();
      }
};

/*void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   
   BPoint p(100,200);
   BPoint p2;
   BRectangle rect (p,36,135);
   BRectangle rect2 (204,65);
   BRectangle rect3;
   
   rect.Draw();
   rect2.Draw();
   

   glutSwapBuffers ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluOrtho2D (-(float)w/2, (float)w/2, -(float)h/2, (float)h/2);
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
}*/
