#include <cmath>
#include "../ComplexClass/complex.cpp"
#define PI 3.14159265359

Complex SphericalHarmonic(int l, int m, float theta, float phi)
{
   Complex Y (0,0);
   Complex i (0,1);
   switch(l){
     case 0:
       switch(m){
         case 0:
           Y = 0.5*sqrt(1/PI);
           break;
       }
       break;
       
     case 1:
       switch(m){
         case -1:
           Y = 0.5*sqrt(1.5/PI) * cexp(-1*i*phi) * sin(theta);
           break;
         case 0:
           Y = 0.5*sqrt(3/PI) * cos(theta);
           break;
         case 1:
           Y = -0.5*sqrt(1.5/PI) * cexp(i*phi) * sin(theta);
           break;
       }
       break;
       
     case 2:
       switch(m){
         case -2:
           Y = 0.25*sqrt(7.5/PI) * cexp(-2*i*phi) * sin(theta)*sin(theta);
           break;
         case -1:
           Y = 0.5*sqrt(7.5/PI) * cexp(-1*i*phi) * sin(theta)*cos(theta);
           break;
         case 0:
           Y = 0.25*sqrt(5/PI) * (3*cos(theta)*cos(theta) - 1);
           break;
         case 1:
           Y = -0.5*sqrt(7.5/PI) * cexp(1*i*phi) * sin(theta)*cos(theta);
           break;
         case 2:
           Y = 0.25*sqrt(7.5/PI) * cexp(2*i*phi) * sin(theta)*sin(theta);
           break;
       }
       break;
   }
   
   return Y;
}

float RadialHydrogen(int n, int l, float r)
{
   float R = 0;
   
   switch(n){
     case 1:
       switch(l){
         case 0:
           R = 2*exp(-r);
           break;
       }
       break;
       
     case 2:
       switch(l){
         case 0:
           R = 1/sqrt(8)*(2-r)*exp(-r/2);
           break;
         case 1:
           R = 1/sqrt(8*3)*r*exp(-r/2);
           break;
       }
       break;
       
     case 3:
       switch(l){
         case 0:
           R = 2/sqrt(27) * (1 - 2*r/3 + 2*r*r/27) * exp(-r/3);
           break;
         case 1:
           R = 4*sqrt(2.0/27)/3 * (1-r/6) * r * exp(-r/3);
           break;
         case 2:
           R = 2*sqrt(2.0/27/5)/27 * r * r * exp(-r/3);
           break;
       }
       break;
   }
   
   return R;
           
}
