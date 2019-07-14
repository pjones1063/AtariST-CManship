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
#include <osbind.h>

#define PRINT(Z) printf("+%d  p=%ld   &p=%ld   *p=%d\n", x, Z, &Z, *Z)

char *p1, array1[] = "ABC";
int *p2, array2[] = {10, 11, 12};

main()
{
   int x = 0, ch;

   p1 = array1;
   PRINT ( p1 );
   printf ( "\n" );
   for ( x=0; x<3; ++x ) {
      p1 = &array1[x];
      PRINT ( p1 );
   }
   printf ( "\n" );
   for ( x=0, p2=array2; x<3; ++x ) {
      PRINT ( p2 );
      ++p2;
   }
   printf ( "\nPress any key\n" );
   Cconin ();
}
    
