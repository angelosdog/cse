/*
* Test if a point is inside a triangle.
* Dumvid Collien, 2009
*
* Given Three points of a triangle, and another arbitrary point this program determines if that point lies inside the triangle.
*
* This is determined by satisfying the following rule:
* A point P(x,y) is inside triangle A(xA,yA), B(xB,yB), C(xC,yC)
* iff
* P is on the same side of the line AB as C
* P is on the same side of the line BC as A
* and
* P is on the same side of the line AC as B
*
* A special case exits for a vertical line (inf gradient) when testing the side of the line
*/

#include <stdio.h>
#include <assert.h>
int test1( double xP, double yP, double m,double b, double lx,double ly) {
int test2( double xP, double yP, double m, double b ); 
int tritest(double xA,double yA,double xB,double yB,double xC,double yC,double xP, double yP);


int main(int argc, char* argv[]) {
//put in pairs?
    double xA, yA;
    double xB, yB;
    double xC, yC;
    double xP, yP;

    int totalNumberOfInputs = 0;

// whether or not to check if you got enough input values is debatable
    printf("Triangle Vertex A (x,y): "); 
    totalNumberOfInputs += scanf("%lf %lf", &xA,&xA);
        
    printf("Triangle Vertex  B (x,y): "); 
    totalNumberOfInputs += scanf("%lf %lf", &xB,&yB);
        
    printf("Triangle Vertex  C (x,y): "); 
    totalNumberOfInputs  += scanf("%lf %lf", &xC,&yC);
 
    printf("Test Point (x,y): "); 
    totalNumberOfInputs += scanf("%lf %lf", &xP,&yP);

    if(totalNumberOfInputs != 8 ) {
       printf("You're stupid and didn't put in the right inputs!\n");
       assert(0);
    }

    if(tritest(xA,yA,xB,yB,xC,yC,xP,yP) != 0){  
       printf("Point (%.2lf,%.2lf) is inside the Triangle\n ", xP,yP);
    } else {
       printf("Point (%.2lf,%.2lf) is outside the Triangle\n ", xP,yP);
    }
    return 0;
}

int tritest(double xA,double yA,double xB,double yB,double xC,double yC,double xP, double yP){

    int line1, line2, line3;
// line eqns
// b: y - yB = m( x - xB ), x = 0
    double gradientAB = (yB-yA)/(xB-xA);
    double gradientAC = (yC-yA)/(xC-xA);
    double gradientBC = (yC-yB)/(xC-xB);

   
    double yIntAB = gradientAB * -xB + yB;
    double yIntAC = gradientAC * -xC + yC;
    double yIntBC = gradientBC * -xC + yC;

// vertical line checks
    if( xB == xA ) {
       line1 = ( (xP <= xA) == (xC <= xA) );
    } else {
       line1 = test1( xP, yP, gradientAB, yIntAB,xC,yC);
    }

    if( xB == xC ) {
       line2 = ( (xP <= xC) == (xA <= xC) );
    } else {
       line2 = test1(xP,yP, gradientBC, yIntBC,xA,yA);
    }

    if( xC == xA ) {
        line3 = ( (xP <= xA ) == (xB <= xA) );
    } else {
        line3 = test1(xP, yP, gradientAC,yIntAC,xB,yB);
    }

    return line1 && line2 && line3;

}

// two points lie on the same side of a line
int test1( double xP, double yP, double m,double b, double lx,double ly) {
    return (test2( xP,yP, m,b )==test2(lx,ly,m,b));
}

int test2( double xP, double yP, double m, double b ){
    if( yP < m * xP + b ) {
       return -1; // point is under line
    } else if ( yP == m * xP + b ){
       return 0;
    } else {
       return 1; // over
    }
}
