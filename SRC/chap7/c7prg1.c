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

char *p1, array1[] = "ABC";
int *p2, array2[] = {10, 11, 12};

main ()
{
   int x, ch;

   p1 = array1;
   printf ( "+0  p1=%ld    &p1=%ld    *p1=%d\n\n", p1, &p1, *p1 );
   for ( x=0; x<3; ++x ) {
      p1 = &array1[x];
      printf ( "+%d  p1=%ld    &p1=%ld    *p1=%d\n", x, p1, &p1, *p1 );
   }
   printf( "\n" );
   for ( x=0, p2=array2; x<3; ++x ) {
      printf ( "+%d  p2=%ld    &p2=%ld    *p2=%d\n", x, p2, &p2, *p2 );
      ++p2;
   }
   printf ( "\nPress any key\n" );
   Cconin ();
}

