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

int week[] = { 5, 7, 2, 10, 7, 1, 6 };

main ()
{
   int i,     /* Loop variable.       */
       total, /* Sum of weekly sales. */
       ch;    /* Character storage.   */

   for ( i=0, total=0; i<7; i++ ) {
      total += week[i];
      printf ( "Sales for day %d: %d\n", i+1, week[i] );
      }
   printf ( "\n" );
   printf ( "Total sales: %d", total );
   ch = getchar ();
}


