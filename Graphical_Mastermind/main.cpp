#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "../GLText/GLText.cpp"

#define PI 3.14159265358979
#define REVEAL false
#define MULT 0.065*height
#define RAD 0.029*height

using namespace std;

float boardWidth,actWidth,height;
float rowHeight, columnboardWidth;
int currentRow=0;
int pegsInCurrentRow=0;
int numPegs=6;
string solution="000000";
string rows[10];
string info[10];
string ref="0123456";
bool gameIsOver=false;
int mouseX, mouseY;
int maxRows=10;
int currentPeg=0;
int highlight[2]={0,0};
bool isWinner;
float animationFrames=4, curFrame=0, initX, initY, finalX, finalY;
bool isAnimated=false;
int tookFromRow=0;
string winners[3]={"222200","222220","222222"};
bool isAtMenu=true;
int menuCircleColors[10];
int lastTime;
int gameStartTime, gameTime, bestTime=0;

void init(void) 
{
   glClearColor (0.8, 0.7, 0.55, 1.0);
   glShadeModel (GL_FLAT);
   srand(time(NULL));
   for(int i=0; i<10; i++){
     rows[i]="000000";
     info[i]="000000";
   }
   for(int i=0; i<=numPegs; i++)
     solution[i]=ref[rand()%6+1];
   for(int i=0; i<10; i++){
     do{
       menuCircleColors[i]=rand()%6+1;
     }while(menuCircleColors[i]==2);
   }
   pegsInCurrentRow=0;
   currentRow=0;
   gameIsOver=false;
   currentPeg=0;
   isWinner=false;
   columnboardWidth=boardWidth/((float)numPegs+1);
   rowHeight=(height-50-MULT)/11;
   gameStartTime=glutGet(GLUT_ELAPSED_TIME);
}

void setColor(int n)
{
     switch(n){
       case 1:
            glColor3f(1,1,1);
            break;
       case 2:
            glColor3f(0,0,0);
            break;
       case 3:
            glColor3f(1,0,0);
            break;
       case 4:
            glColor3f(0,1,0);
            break;
       case 5:
            glColor3f(0,0,1);
            break;
       case 6:
            glColor3f(1,1,0);
            break;
       default:
            break;
     }
}

void drawRect(float x, float y, float w, float h, int n)
{
     setColor(n);
     glBegin(GL_POLYGON);
       glVertex2f(x,y);
       glVertex2f(x,y-h);
       glVertex2f(x+w,y-h);
       glVertex2f(x+w,y);
     glEnd();
}

void drawCircle(float x, float y, float r, int n)
{
     setColor(n);
     glBegin(GL_POLYGON);
       for(float a=0; a<=2*PI; a+=PI/20)
         glVertex2f(x+r*cos(a),y+r*sin(a));
     glEnd();
}

int getMouseRow()
{
     int row=(int)floor((height-(float)mouseY)/rowHeight);
     if(mouseY<50)row=-1;
     if(mouseY<50+MULT && mouseY>50)row=11;
     return row;
}

int getMouseColumn()
{
     return (int)floor((float)mouseX/columnboardWidth);
}

string analyzeGuess(string input)
{
    string result="";
    int solutionUsed[6]={0,0,0,0,0,0};
    for(int i=0; i<numPegs; i++){
      if(input[i]==solution[i]){
        result="2"+result;
        solutionUsed[i]=1;
      }else{
        for(int c=0; c<numPegs; c++){
          if(solutionUsed[c]==0 && solution[c]!=input[c] && input[i]==solution[c]){
            result+="1";
            solutionUsed[c]=1;
            break;
          }
        }
      }
    }
      if(result.length()==0)result+="000000";
      if(result.length()==1)result+="00000";
      if(result.length()==2)result+="0000";
      if(result.length()==3)result+="000";
      if(result.length()==4)result+="00";
      if(result.length()==5)result+="0";
      return result;
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (0.57,0.5,0.32);
   
   for(float i=1; i<12; i++){    //horizontal lines
     glBegin(GL_LINES);
       glVertex2f(0,height-i*rowHeight);
       glVertex2f(boardWidth,height-i*rowHeight);
     glEnd();
   }
   for(float i=1; i<=numPegs; i++){      //vertical lines
     glBegin(GL_LINES);
       glVertex2f(i*columnboardWidth,50+MULT);
       glVertex2f(i*columnboardWidth,height);
     glEnd();
   } 
   drawRect(0,height,columnboardWidth,rowHeight,0);   //tan box in upper left
   
   drawRect(0,height-(maxRows+1)*rowHeight,boardWidth,height-(maxRows+1)*rowHeight,0);  //bottom tan box with pegs
   
   for(float i=1; i<maxRows+1; i++){         //b/w peg holes
     drawCircle(columnboardWidth/6,height-i*rowHeight-rowHeight/4,RAD/9,0);
     drawCircle(columnboardWidth/6,height-i*rowHeight-3*rowHeight/4,RAD/9,0);
     drawCircle(columnboardWidth/2,height-i*rowHeight-rowHeight/4,RAD/9,0);
     drawCircle(columnboardWidth/2,height-i*rowHeight-3*rowHeight/4,RAD/9,0);
     drawCircle(5*columnboardWidth/6,height-i*rowHeight-rowHeight/4,RAD/9,0);
     drawCircle(5*columnboardWidth/6,height-i*rowHeight-3*rowHeight/4,RAD/9,0);
   }
   
   glColor3f(0.68,0.6,0.43);
   glBegin(GL_LINES);                    //vertical lines in leftmost column
     glVertex2f(columnboardWidth/3,height-rowHeight);
     glVertex2f(columnboardWidth/3,height-(maxRows+1)*rowHeight);
     glVertex2f(2*columnboardWidth/3,height-rowHeight);
     glVertex2f(2*columnboardWidth/3,height-(maxRows+1)*rowHeight);
   glEnd();
   for(float i=1; i<maxRows+1; i++){          //mini horizontal lines in leftmost column
     glBegin(GL_LINES);
       glVertex2f(0,height-i*rowHeight-rowHeight/2);
       glVertex2f(columnboardWidth,height-i*rowHeight-rowHeight/2);
     glEnd();
   } 
   
   
   for(float i=1; i<7; i++)      //row of colored pegs on bottom
     drawCircle((i-1)/6*boardWidth+boardWidth/12,50+MULT/2,RAD,(int)i);
     
   for(float i=0; i<=currentRow; i++){  //draws pegs in rows
     for(float c=0; c<numPegs; c++){
       if(rows[(int)i][(int)c]!='0')
         drawCircle((c+1)*columnboardWidth+columnboardWidth/2, height-(i+1)*rowHeight-rowHeight/2, RAD, ref.find(rows[(int)i][(int)c]));
     }
   }
   
   if(gameIsOver || REVEAL){
     for(float c=0; c<numPegs; c++)     //draws pegs in solution row
       drawCircle((c+1)*columnboardWidth+columnboardWidth/2, height-rowHeight/2, RAD, ref.find(solution[(int)c]));
   }else{
     glColor3f (0.57,0.5,0.32);          //covers computers pegs
     drawRect(0,height,boardWidth,rowHeight,0);
     setColor(1);
     DrawText((boardWidth-columnboardWidth)/2-60+columnboardWidth, height-rowHeight+20, 0, "MASTERMIND", GLUT_BITMAP_HELVETICA_18);
   }    
   setColor(2);
   glBegin(GL_LINES);    //black lines saparating different areas
     glVertex2f(columnboardWidth, height);
     glVertex2f(columnboardWidth, 50+MULT);
     glVertex2f(0, height-rowHeight);
     glVertex2f(boardWidth, height-rowHeight);
     glVertex2f(0, 50+MULT);
     glVertex2f(boardWidth, 50+MULT);
     glVertex2f(0, 50+MULT+rowHeight*(10-maxRows));
     glVertex2f(boardWidth, 50+MULT+rowHeight*(10-maxRows));
     glVertex2f(0, 0);
     glVertex2f(0, height);
     glVertex2f(boardWidth, 0);
     glVertex2f(boardWidth, height);
   glEnd();
   
   if(currentRow>0){                      //b/w pegs on left
     for(int i=0; i<currentRow; i++){
       if(info[i][0]!='0')
         drawCircle(columnboardWidth/6,height-((float)i+1)*rowHeight-rowHeight/4,RAD*2/9,ref.find(info[i][0]));
       if(info[i][1]!='0')
         drawCircle(columnboardWidth/2,height-((float)i+1)*rowHeight-rowHeight/4,RAD*2/9,ref.find(info[i][1]));
       if(info[i][2]!='0')
         drawCircle(5*columnboardWidth/6,height-((float)i+1)*rowHeight-rowHeight/4,RAD*2/9,ref.find(info[i][2]));
       if(info[i][3]!='0')
         drawCircle(columnboardWidth/6,height-((float)i+1)*rowHeight-3*rowHeight/4,RAD*2/9,ref.find(info[i][3]));
       if(info[i][4]!='0')
         drawCircle(columnboardWidth/2,height-((float)i+1)*rowHeight-3*rowHeight/4,RAD*2/9,ref.find(info[i][4]));
       if(info[i][5]!='0')
         drawCircle(5*columnboardWidth/6,height-((float)i+1)*rowHeight-3*rowHeight/4,RAD*2/9,ref.find(info[i][5]));
     }
   }
   glColor3f(0.4,0.35,0.27); 
   if(highlight[0])       //highlight box on mouse over
     drawRect((float)highlight[1]*columnboardWidth, height-(float)(currentRow+1)*rowHeight, columnboardWidth,rowHeight,0);
   
   if(gameIsOver){   //sets bottom text box color
     if(isWinner)
       setColor(4);
     else
       setColor(3);
   }else{
     setColor(2);
   }
   int pos=50;
   if(gameIsOver)pos=(int)(height/2+25);
   drawRect(0,pos,boardWidth,50,0);    //black box on bottom
   
   setColor(1);    //bottom box text
   if(!gameIsOver){
     DrawText(boardWidth/2-70,pos-15,0,"Welcome to Mastermind!",GLUT_BITMAP_HELVETICA_12);
     DrawText(boardWidth/2-122,pos-30,0,"Drag pegs from the bottom row to get started.",GLUT_BITMAP_HELVETICA_12);
     DrawText(boardWidth/2-85,3,0,"-Programmed By Bennett Marsh-",GLUT_BITMAP_HELVETICA_10);
     gameTime=glutGet(GLUT_ELAPSED_TIME)-gameStartTime;
   }else{
     if(info[currentRow]==winners[numPegs-4])
       DrawText(boardWidth/2-75,pos-12,0,"You are a true mastermind!",GLUT_BITMAP_HELVETICA_12);
     else 
       DrawText(boardWidth/2-115,pos-12,0,"You were outsmarted by the computer :-(",GLUT_BITMAP_HELVETICA_12);
     DrawText(boardWidth/2-85,pos-27,0,"Click anywhere to play again.",GLUT_BITMAP_HELVETICA_12);
     DrawText(boardWidth/2-87,pos-42,0,"Press \'m\' to return to the menu.",GLUT_BITMAP_HELVETICA_12);
     if((gameTime<bestTime || bestTime==0) && isWinner)bestTime=gameTime;
   }
   //draws time info
   DrawText(5,height-10,0,"Time:",GLUT_BITMAP_HELVETICA_10);
   DrawInt(5, height-21, 0, (int)floor((float)gameTime/1000),GLUT_BITMAP_HELVETICA_10);
   DrawText(".",GLUT_BITMAP_HELVETICA_10);
   DrawInt((int)(((float)gameTime/1000-floor((float)gameTime/1000))*100),GLUT_BITMAP_HELVETICA_10);
   
   DrawText(5,height-33,0,"Best:",GLUT_BITMAP_HELVETICA_10);
   DrawInt(5, height-44, 0, (int)floor((float)bestTime/1000),GLUT_BITMAP_HELVETICA_10);
   DrawText(".",GLUT_BITMAP_HELVETICA_10);
   DrawInt((int)(((float)bestTime/1000-floor((float)bestTime/1000))*100),GLUT_BITMAP_HELVETICA_10);

   if(currentPeg!=0)       //draws peg as it is being dragged
     drawCircle(mouseX, mouseY, RAD, currentPeg);
     
   if(isAtMenu){     //draws the menu
     drawRect(0,height,boardWidth,height,2);
     setColor(1);
     DrawText(boardWidth/2-50,height-170,0,"Mastermind!",GLUT_BITMAP_HELVETICA_18);
     DrawText(boardWidth/2-17,height-185,0,"v1.0",GLUT_BITMAP_HELVETICA_12);
     DrawText(boardWidth/2-87,height-215,0,"Programmed by Bennett Marsh",GLUT_BITMAP_HELVETICA_12);
     DrawText(boardWidth/2-143,height-295,0,"Type a number to select an option:",GLUT_BITMAP_HELVETICA_18);
     DrawText(boardWidth/2-120,height-330,0,"1: Beginner (Sequence of 4)",GLUT_BITMAP_HELVETICA_18);
     DrawText(boardWidth/2-130,height-360,0,"2: Intermediate (Sequence of 5)",GLUT_BITMAP_HELVETICA_18);
     DrawText(boardWidth/2-127,height-390,0,"3: Mastermind (Sequence of 6)",GLUT_BITMAP_HELVETICA_18);
     DrawText(boardWidth/2-127,height-420,0,"Press \'m\' at any time to return to this screen.",GLUT_BITMAP_HELVETICA_12);
     if(glutGet(GLUT_ELAPSED_TIME)>lastTime+1000){
       lastTime=glutGet(GLUT_ELAPSED_TIME);
       for(int i=0; i<10; i++){      //updates circles on menu
         do{
           menuCircleColors[i]=rand()%6+1;
         }while(menuCircleColors[i]==2);
       }
     }
     for(int i=1; i<6; i++){     //circles on menu
       drawCircle((float)i*boardWidth/6, height-25, RAD, menuCircleColors[i-1]);
       drawCircle((float)i*boardWidth/6, 25, RAD, menuCircleColors[i+4]);
     }
   }

   glutSwapBuffers ();
}

void reshape (int w, int h)
{
   actWidth=w;
   height=h;
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   boardWidth=300;
   gluOrtho2D (-(float)w/2+150, (float)w/2+150, 0.0, (GLdouble) h);
   rowHeight=(height-50-MULT)/11;
   columnboardWidth=boardWidth/((float)numPegs+1);
}

void idle()
{
     usleep(25000);
     if(pegsInCurrentRow>=numPegs){
       info[currentRow]=analyzeGuess(rows[currentRow]);
       if(info[currentRow]==winners[numPegs-4] || currentRow==maxRows-1)
         gameIsOver=true;
       else
         currentRow++;
       pegsInCurrentRow=0;
     }
     if(info[currentRow]==winners[numPegs-4])isWinner=true;
     if(isAnimated){
        curFrame++;
        mouseX=(int)(initX+(finalX-initX)*curFrame/animationFrames);
        mouseY=(int)(initY+(finalY-initY)*curFrame/animationFrames);
        if(curFrame==animationFrames){
          if(tookFromRow){
            rows[currentRow][tookFromRow-1]=ref[currentPeg];
            tookFromRow=0;
            pegsInCurrentRow++;
          }
          curFrame=0;
          isAnimated=false;
          currentPeg=0;
          
        }
     }
     if(currentRow==0 && rows[0]=="000000")pegsInCurrentRow=0;
     glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
     mouseX=x+(-actWidth/2+150);
     mouseY=(int)height-y;
     int row=getMouseRow(); 
     int column=getMouseColumn();
     if(state==GLUT_DOWN){
        if(row==11){
          column=(int)floor((float)mouseX/boardWidth*6)+1;
          currentPeg=column;
        } 
        if(row==currentRow+1 && column!=0){
          currentPeg=ref.find(rows[currentRow][column-1]);
          rows[currentRow][column-1]='0'; 
          pegsInCurrentRow--; 
          tookFromRow=column;
        }  
        finalX=mouseX;
        finalY=mouseY;         
     }else{
        if(currentPeg!=0 && row==currentRow+1 && rows[currentRow][column-1]=='0' && column<=numPegs){
          rows[currentRow][column-1]=ref[currentPeg];
          pegsInCurrentRow++;
          currentPeg=0;
        }else if(row<=maxRows){
          isAnimated=true;
          initX=mouseX;
          initY=mouseY;
        }else{
          currentPeg=0;
          tookFromRow=0;
        }
     }
     if(gameIsOver && isWinner){
       if(maxRows>1)maxRows--;
       init();
     }
     if(gameIsOver && !isWinner){
       maxRows=10;
       init();
     }
     highlight[0]=0;
}

void motion(int x, int y)
{
     mouseX=x+(-actWidth/2+150);
     mouseY=(int)height-y;
     int row=getMouseRow(); 
     int column=getMouseColumn();
     if(row==currentRow+1 && currentPeg!=0 && rows[currentRow][column-1]=='0' && column<=numPegs){
       highlight[0]=1;
       highlight[1]=column;
     }else{
       highlight[0]=0;
     }
}

void keyboard(unsigned char key, int x, int y)
{
     if(ref.find(key)>0 && ref.find(key)<7  && !gameIsOver && !isAtMenu){
       int count=-1;
       do{
           count++;
       }while(rows[currentRow][count]!='0');
       
         rows[currentRow][count]=key;
         pegsInCurrentRow++;
       
     }
     if(gameIsOver && isWinner){
       if(maxRows>1)maxRows--;
       init();
     }
     if(gameIsOver && !isWinner){
       maxRows=10;
       init();
     }
     if(isAtMenu  && (key=='1' || key=='2' || key=='3')){
       numPegs=ref.find(key)+3;
       bestTime=0;
       isAtMenu=false;
       maxRows=10;
       init();
     }
     if(key=='m' || key=='M')isAtMenu=true;
     if(key==27)exit(0);
      
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (300, 615); 
   glutInitWindowPosition (100, 50);
   glutCreateWindow ("Mastermind");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
