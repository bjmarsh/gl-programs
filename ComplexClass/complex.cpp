#include <cstdlib>
#include <iostream>
#include <cmath>

#ifndef __COMPLEXCLASS
#define __COMPLEXCLASS 1

using namespace std;

class Complex{
public:
    //real and imaginary components of complex number
    double real;
    double imag;
         
    //constructors
    Complex(double,double);
    Complex();
         
    //write complex number to screen
    void Write();
         
    //overload operators
    void operator = (Complex);
    void operator = (double);
    Complex operator + (Complex);
    Complex operator + (double);
    Complex operator - (Complex);
    Complex operator - (double);
    Complex operator * (Complex);
    Complex operator * (double);
    Complex operator / (Complex);
    Complex operator / (double);
    Complex operator ^ (int);
    bool operator == (Complex);
    bool operator != (Complex);
    void operator *= (Complex);
    void operator *= (double);
    void operator /= (Complex);
    void operator /= (double);
    void operator -= (Complex);
    void operator -= (double);
    void operator += (Complex);
    void operator += (double);
         
    //complex math
    double cabs();
    Complex cconj();
    double cangle();
    Complex csin();
    Complex ccos();
    Complex csinh();
    Complex ccosh();
    Complex cpow(float);
};

Complex::Complex(double x, double y){
    real=x;
    imag=y;
}

Complex::Complex(){
    real=0;
    imag=0;
}

void Complex::operator= (Complex param){
    real=param.real;
    imag=param.imag;
}

void Complex::operator= (double param){
    real=param;
    imag=0;
}

Complex Complex::operator+ (Complex param) {
    Complex temp;
    temp.real = real + param.real;
    temp.imag = imag + param.imag;
    return (temp);
}

Complex Complex::operator+ (double param) {
    Complex temp;
    temp.real = real + param;
    temp.imag = imag;
    return (temp);
}

Complex Complex::operator- (Complex param) {
    Complex temp;
    temp.real = real - param.real;
    temp.imag = imag - param.imag;
    return (temp);
}

Complex Complex::operator- (double param) {
    Complex temp;
    temp.real = real - param;
    temp.imag = imag;
    return (temp);
}

Complex Complex::operator* (Complex param) {
    Complex temp;
    temp.real = real*param.real - imag*param.imag;
    temp.imag = imag*param.real + real*param.imag;
    return (temp);
}

Complex Complex::operator* (double param) {
    Complex temp;
    temp.real = real * param;
    temp.imag = imag * param;
    return (temp);
}

Complex Complex::operator/ (Complex param) {
    Complex temp;
    temp.real = (real*param.real+imag*param.imag)/(param.real*param.real+param.imag*param.imag);
    temp.imag = (imag*param.real-real*param.imag)/(param.real*param.real+param.imag*param.imag);
    return (temp);
}

Complex Complex::operator/ (double param) {
    Complex temp;
    temp.real = real / param;
    temp.imag = imag / param;
    return (temp);
}

Complex operator/(double x, Complex param){
    return ((Complex(param.real,-param.imag)*x)/(param.real*param.real+param.imag*param.imag));
}

Complex operator+(double x, Complex param){
    return Complex(param.real+x,param.imag);
}

Complex operator-(double x, Complex param){
    return Complex(x-param.real,-param.imag);
}

Complex operator*(double x, Complex param){
    return Complex(x*param.real, x*param.imag);
}

Complex Complex::operator^ (int param) {
    if(param<0)return(1/(Complex(real,imag)^(-param)));
    if(param==0)return (Complex(1,0));
    if(param==1)return (Complex(real,imag));
    Complex temp(real,imag);
    for(int i=0; i<param-1; i++)
        temp=Complex(real,imag)*temp;
    return (temp);
}

bool Complex::operator== (Complex param){
    if((real==param.real)&&(imag==param.imag))
        return true;
    else
        return false;
}

bool Complex::operator!= (Complex param){
    if((real==param.real)&&(imag==param.imag))
        return false;
    else
        return true;
}

void Complex::operator*= (Complex param){
    Complex temp;
    temp.real=real*param.real-imag*param.imag;
    temp.imag=real*param.imag+imag*param.real;
    real=temp.real;
    imag=temp.imag;
}

void Complex::operator*= (double param){
    real*=param;
    imag*=param;
}

void Complex::operator/= (Complex param){
    Complex temp;
    temp.real = (real*param.real+imag*param.imag)/(param.real*param.real+param.imag*param.imag);
    temp.imag = (imag*param.real-real*param.imag)/(param.real*param.real+param.imag*param.imag);
    real=temp.real;
    imag=temp.imag;
}

void Complex::operator/= (double param){
    real/=param;
    imag/=param;
}

void Complex::operator-= (double param){
    real-=param;
}

void Complex::operator-= (Complex param){
    real-=param.real;
    imag-=param.imag;
}

void Complex::operator+= (double param){
    real+=param;
}

void Complex::operator+= (Complex param){
    real+=param.real;
    imag+=param.imag;
}

double Complex::cabs(){
    return (sqrt(real*real+imag*imag));
}

Complex Complex::cconj(){
    return (Complex(real,-imag));
}

double Complex::cangle(){
    double angle=atan2(imag,real);
    if(angle<0) angle+=atan(1)*8;   //2PI
    return angle;
}

Complex Complex::csin(){
    Complex temp;
    temp.real=std::sin(real)*std::cosh(imag);
    temp.imag=std::cos(real)*std::sinh(imag);
    return temp;
}

Complex Complex::ccos(){
    Complex temp;
    temp.real=std::cos(real)*std::cosh(imag);
    temp.imag=-std::sin(real)*std::sinh(imag);
    return temp;
}

Complex Complex::csinh(){
    return (Complex(std::sinh(real)*std::cos(imag), std::cosh(real)*std::sin(imag)));
}

Complex Complex::ccosh(){
    return (Complex(std::cosh(real)*std::cos(imag), std::sinh(real)*std::sin(imag)));
}

Complex cexp(Complex z){
    return exp(z.real)*Complex(cos(z.imag),sin(z.imag));
}

Complex Complex::cpow(float p){
    float r = sqrt(real*real+imag*imag);
    float phi = atan2(imag,real);
    phi *= p;
    r = pow(r,p);
    return Complex(r*cos(phi),r*sin(phi));
}

void Complex::Write(){
    if(imag>0)
        cout << real << "+" << imag << "i" << endl;
     else if(imag<0)
       cout << real << "-" << -imag << "i" << endl;
     else
       cout << real << endl;
}

/* set the color base on the phase of a comlex number. Phase is between 0
   and 2PI */
void SetPhaseColor(float phase, float bright, float c[3])
{
     float r=0,g=0,b=0;
     phase = Complex(cos(phase),sin(phase)).cangle();
     float h = phase/(atan(1)*8);
     if(h<=1./6){
       r=1;
       g=h*6;
     }else if(h<=2./6){
       r=1-(h-1./6)*6;
       g=1;
     }else if(h<=3./6){
       g=1;
       b=(h-2./6)*6;
     }else if(h<=4./6){
       b=1;
       g=1-(h-3./6)*6;
     }else if(h<=5./6){
       r=(h-4./6)*6;
       b=1;
     }else{
       r=1;
       b=1-(h-5./6)*6;
     }
     //r=g=b=1;
     if(bright>1)
       bright=1;
     if(bright<0)
       bright=0;
     
     bright=bright*bright*bright;
     
     c[0]=r*bright; c[1]=g*bright; c[2]=b*bright;
}


#endif
/*
int main(int argc, char *argv[])
{
    Complex x(-1,5);
    Complex y(2,-3);
    cout << "x = ";
    x.Write();
    cout << "y = ";
    y.Write();
    cout << "x+y = ";
    (x+y).Write();
    cout << "x+1 = ";
    (x+1).Write();
    cout << "x-y = ";
    (x-y).Write();
    cout << "x-1 = ";
    (x-1).Write();
    cout << "x*y = ";
    (x*y).Write();
    cout << "x*2 = ";
    (x*2).Write();
    cout << "x/y = ";
    (x/y).Write();
    cout << "x/4 = ";
    (x/4).Write();
    cout << "1/x = ";
    (1/x).Write();
    cout << "x^2 = ";
    (x^2).Write();
    cout << "sinh(x)=";
    (x.csinh()).Write();
    cout << "angle(x)=" << x.cangle() << endl;
    cout << "2*x=";
    (2*x).Write();
    cout << "4-x=";
    (4-x).Write();
    system("PAUSE");
    return EXIT_SUCCESS;
}
*/
