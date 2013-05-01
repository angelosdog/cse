/*
 * Test if a point is inside a triangle.
 * Dumvid Collien, 2009
 *
 * Given Three points of a triangle, and another arbitrary point this program determines if that point lies inside the triangle.
 *
 * This is determined by satisfying the following rule:
 * A point P(x,y) is inside triangle A(x0,y0), B(x1,y1), C(x2,y2)
 * iff
 * P is on the same side of the line AB as C
 * P is on the same side of the line BC as A
 * and
 * P is on the same side of the line AC as B
 *
 * A special case exits for a vertical line (inf gradient) when testing the side of the line
 */

#include <stdio.h>

int test2( double px, double py, double m, double b ){   
	if( m >= 0){
if( py < m * px + b ) {
   return -1; // point is under line
} else if (py == m * px + b){
	return 0;
} else {
   return 1; // over
}
} else {
if( py <  m * px + b ) {
   return -1; // point is under line
}  else if (py == m * px + b){
	return 0;
}else {
   return 1; // over
}
}
}
int // two points lie on the same side of a line

test1( double px, double py, double m,double b, double lx,double ly) {
   	if (test2(px,py, m, b) == 0){
		return 1;
	}else{
	return (test2( px,py, m,b )==test2(lx,ly,m,b));
	}
}

int tritest(double x0,double y0,double x1,double y1,double x2,double y2,double px, double py){
   int line1, line2, line3;
   // line eqns

   double m01 = (y1-y0)/(x1-x0);
   // b: y - y1 = m( x - x1 ), x = 0

   double b01 = m01 * -x1 + y1;
   double m02, m12, b02, b12;
   m02 = (y2-y0)/(x2-x0);
   m12 = (y2-y1)/(x2-x1);
   b02 = m02 * -x2 + y2;
   b12 = m12 * -x2 + y2;

   // vertical line checks
if((px == x0) && (py == y0)){
	return 1;  
} else if ((px == x1) && (py == y1)){
	return 1;
} else if ((px == x2) && (py == y2)){
	return 1;	
} else { 
   if( x1 == x0 ) {
	if (px == x0){
           line1 = (px == x0);
	} else {
           line1 = ( (px <= x0) == (x2 <= x0) );
	}
   } else {
      line1 = test1( px, py, m01, b01,x2,y2);
   }

   if( x1 == x2 ) {
	if (px == x1){
	   line2 = (px == x1);
	} else{
           line2 = ( (px <= x2) == (x0 <= x2) );
        }
   } else {
      line2 = test1(px,py, m12, b12,x0,y0);
   }
  
   if( x2 == x0 ) {
	if (px == x0){
	   line3 = (px == x0);
      	} else { 
          line3 = ( (px <= x0 ) == (x1 <= x0) );
        }
   } else {
      line3 = test1(px, py, m02,b02,x1,y1);
   }
}
   return line1 && line2 && line3;
}

int main(int argc, char* argv[]) {
   double x0,y0,x1,y1,x2,y2,px;
   double py;

   int scanfsReturnValueAggregatedOverAllScanfs = 0;

   // get input
   printf("Triangle Vertex A (enter x,y): "); scanfsReturnValueAggregatedOverAllScanfs += scanf("%lf,%lf", &x0,&y0);
   printf("\nTriangle Vertex B (enter x,y): "); scanfsReturnValueAggregatedOverAllScanfs += scanf("%lf,%lf", &x1,&y1);
   printf("\nTriangle Vertex C (enter x,y): "); scanfsReturnValueAggregatedOverAllScanfs += scanf("%lf,%lf", &x2,&y2);
   printf("\nTest Point (enter x,y): "); scanfsReturnValueAggregatedOverAllScanfs += scanf("%lf,%lf", &px,&py);

   // print error
   if( scanfsReturnValueAggregatedOverAllScanfs != 8 ) {
      printf("You're stupid and didn't put in the right inputs!\n");
      return 1;
   }

   // print answer
   printf("\nThe point is ");
   if (tritest(x0,y0,x1,y1,x2,y2,px,py)) {
      printf("INSIDE");
   } else {
      printf("OUTSIDE");
   }
   printf(" the Triangle\n");

   // return 0
   return 0;
}
