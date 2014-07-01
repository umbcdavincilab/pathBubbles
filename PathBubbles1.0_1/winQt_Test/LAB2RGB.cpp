


#include "LAB2RGB.h"
/*
 Input is the Lab( L(0,100), a(-100, 100), b(-100, 100) ) color and the output is the RGB color.
 */
LAB2RGB::LAB2RGB( float* RGB, double L, double a, double b)
{
   double T1 = 0.008856;
   double T2 = 0.206893;
   double X, Y, Z;


   double F_y=pow((L+16)/116,3) ;
   if(F_y<T1)
   {
       F_y = L/903.3;
   }
   Y=F_y;

   if(F_y>T1)
   {
       F_y = pow(F_y, 1.0/3.0);
   }
   else
   {
       F_y = 7.787*F_y + (16.0/116.0);
   }

   double F_x = a/500 + F_y;
   if(F_x < T2)
   {
       X = (F_x - 16.0/116.0) / 7.787;
   }
   else
   {
       X = pow(F_x, 3);
   }

   double F_z = F_y - b / 200;
   if(F_z< T2)
   {
       Z = (F_z - 16.0/116.0)/7.787;
   }
   else
   {
       Z = pow(F_z, 3);
   }

   X=X*0.950456;
   Z=Z*1.088754;

   double R= 3.240479*X - 1.537150*Y - 0.498535*Z;
   double G=-0.969256*X + 1.875992*Y + 0.041556*Z;
   double B= 0.055648*X - 0.204043*Y + 1.057311*Z;

   if(R>1)
   {
       R=1;
   }
   if(R<0)
   {
       R=0;
   }
   if(G>1)
   {
       G=1;
   }
   if(G<0)
   {
       G=0;
   }
   if(B>1)
   {
       B=1;
   }
   if(B<0)
   {
       B=0;
   }

   //RGB[0]=R*255;
  // RGB[1]=G*255;
  // RGB[2]=B*255;
	RGB[0] = R;
	RGB[1] = G;
	RGB[2] = B;
   //return RGB;
}
