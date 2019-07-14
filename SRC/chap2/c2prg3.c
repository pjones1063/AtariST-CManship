/***********************************************/
/* CManship Example Source                     */
/*      For the Atari ST 16/32 bit             */
/*       Developed with Megamax C              */
/*       & Lattice C for the Atari ST          */
/*                                             */
/* https://www.gnu.org/licenses/agpl-3.0.txt   */
/*                                             */
/* http://13leader.net                         */
/*                                             */
/***********************************************/


#include <stdio.h>
main()
{
   int num=5555;
   char ch;

   printf ( ">%d<\n", num );
   printf ( ">%10d<\n", num );
   printf ( ">%010d\n", num );
   printf ( ">%3d<\n", num );
   printf ( ">%-10d<\n", num );
   printf ( ">%f<\n", 3.14159 );
   printf ( ">%2.3f<\n", 3.14159 );
   printf ( ">%10.4f<\n", 3.14159 );
   printf ( ">%-10.4f<\n", 3.14159 );
   ch = getchar();
}


