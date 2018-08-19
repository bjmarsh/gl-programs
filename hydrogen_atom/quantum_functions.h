#ifndef PI
#define PI 3.1415926535898
#endif

Complex SphericalHarmonic(int l, int m, float theta, float phi)
{
  Complex Y (0,0);
  Complex i (0,1);
  m = abs(m);
  float st=sin(theta), ct=cos(theta);
  
  switch(l){
    case 0:
      switch(m){
        case 0:
          Y = 2.0*sqrt(1/PI);
          break;
      }
      break;
      
    case 1:
      switch(m){
        case 0:
          Y = ct;
          break;
        case 1:
          Y = cexp(i*phi) * st;
          break;
      }
      break;
      
    case 2:
      switch(m){
        case 0:
          Y = (3*ct*ct - 1);
          break;
        case 1:
          Y = 2*cexp(1*i*phi) * st*ct;
          break;
        case 2:
          Y = cexp(2*i*phi) * st*st;
          break;
      }
      break;
      
      case 3:
      switch(m){
        case 0:
          Y = (5*ct*ct*ct-3*ct);
          break;
        case 1:
          Y = cexp(1*i*phi) * st *(5*ct*ct - 1);
          break;
        case 2:
          Y = 3*cexp(2*i*phi) * st*st*ct;
          break;
        case 3:
          Y = cexp(3*i*phi) * st*st*st;
          break;
      }
      break;
      
      case 4:
      switch(m){
        case 0:
          Y = (35*ct*ct*ct*ct-30*ct*ct + 3);
          break;
        case 1:
          Y = 5*cexp(1*i*phi) * st *(7*ct*ct*ct - 3*ct);
          break;
        case 2:
          Y = 3*cexp(2*i*phi) * st*st*(7*ct*ct-1);
          break;
        case 3:
          Y = 9*cexp(3*i*phi) * st*st*st*ct;
          break;
        case 4:
          Y = 4*cexp(4*i*phi) * st*st*st*st;
          break;
      }
      break;
      
      case 5:
      switch(m){
        case 0:
          Y = 0.03*(63*ct*ct*ct*ct*ct-70*ct*ct*ct + 15*ct);
          break;
        case 1:
          Y = 0.07*cexp(1*i*phi) * st *(21*ct*ct*ct*ct - 14*ct*ct+1);
          break;
        case 2:
          Y = 0.4*cexp(2*i*phi) * st*st*(3*ct*ct*ct-ct);
          break;
        case 3:
          Y = 0.08*cexp(3*i*phi) * st*st*st*(9*ct*ct-1);
          break;
        case 4:
          Y = 0.3*cexp(4*i*phi) * st*st*st*st*ct;
          break;
        case 5:
          Y = 0.1*cexp(5*i*phi) * st*st*st*st*st;
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
          R = 0.5*(2-r)*exp(-r/2);
          break;
        case 1:
          R = r*exp(-r/2);
          break;
      }
      break;
      
    case 3:
      switch(l){
        case 0:
          R = 2*(1 - 2*r/3 + 2*r*r/27) * exp(-r/3);
          break;
        case 1:
          R = (1-r/6) * r * exp(-r/3);
          break;
        case 2:
          R = 0.05 * r * r * exp(-r/3);
          break;
      }
      break;
      
      case 4:
      switch(l){
        case 0:
          R = 2*(1 - 0.75*r + r*r/8 - r*r*r/192) * exp(-r/4);
          break;
        case 1:
          R = (1 - 0.25*r + r*r/80) * r * exp(-r/4);
          break;
        case 2:
          R = 0.06 * (1 - r/12)*r*r * exp(-r/4);
          break;
        case 3:
          R = 0.002 * r * r * r * exp(-r/4);
          break;
      }
      break;
      
      case 5:
      switch(l){
        case 0:
          R = 2*(r*r*r*r*.000213-r*r*r*.0107+r*r*0.16-r*0.8+1) * exp(-r/5);
          break;
        case 1:
          R = 2*(1 - 0.3*r + .024*r*r - .000533*r*r*r) * r * exp(-r/5);
          break;
        case 2:
          R = .09 * (1 - r/7.5 + r*r/262.5)*r*r * exp(-r/5);
          break;
        case 3:
          R = .004 * (1-r/20) *r*r*r * exp(-r/5);
          break;
        case 4:
          R = .00002 *r*r*r*r * exp(-r/5);
          break;
      }
      break;
      
      case 6:
      switch(l){
        case 0:
          R = 3*(1 - r*.8333 + r*r*.1852 - r*r*r*.01543 + r*r*r*r*.0005144 - r*r*r*r*r*.000005716) * exp(-r/6);
          break;
        case 1:
          R = 2*(1 - r/3 + r*r/30 - r*r*r/810 + r*r*r*r*.00001470) * r * exp(-r/6);
          break;
        case 2:
          R = .13 * (1 - r/6 + r*r/126 - r*r*r*.0001102)*r*r * exp(-r/6);
          break;
        case 3:
          R = .008 * (1 - r/12 + r*r/648)*r*r*r * exp(-r/6);
          break;
        case 4:
          R = .00004 * (1-r/30) * r*r*r*r * exp(-r/6);
          break;
        case 5:
          R = .00002 * r*r*r*r*r * exp(-r/6);
          break;
      }
      break;
  }
  
  return R;
          
}

Complex WaveFunction(int n, int l, int m, float rho, float theta, float phi, float t)
{
   Complex i (0,1);
   m=abs(m);
   return RadialHydrogen(n,l,rho)*SphericalHarmonic(l,m,theta,phi)*cexp(i*n*n*t);    
}
