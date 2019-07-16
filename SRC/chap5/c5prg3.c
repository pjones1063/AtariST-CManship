/***********************************************/
/* CManship Example Source                     */
/*      For the Atari ST 16/32 bit             */
/*       Developed with Megamax C              */
/*       & Laser C (on ST TOS 1.02 only)       */
/*                                             */
/* https://www.gnu.org/licenses/agpl-3.0.txt   */
/*                                             */
/* http://13leader.net                         */
/*                                             */
/***********************************************/


#include <stdio.h>

int weeks[2][7] = {
   { 3, 6, 7, 4, 3, 8, 9 },
   { 5, 3, 7, 9, 3, 2, 6}
      };

main()
{
   int w,    /* Loop variable--weeks. */
       d,    /* Loop variable--days.  */
       mtot, /* Weekly total.         */
       wtot, /* Monthly total.        */
       ch;   /* Character storage.    */

   for ( w=0, mtot=0; w<2; w++ ) {
      for ( d=0, wtot=0; d<7; d++ ) {
         wtot += weeks[w][d];
         printf ( "Sales for day %d: %d\n", d+1, weeks[w][d] );
         }
      printf ( "\n" );
      printf( "Sales for week %d: %d\n\n", w+1, wtot );
      mtot += wtot;
      }
   printf ( "\n\n" );
   printf ( "Total sales for month: %d\n", mtot );
   ch = getchar ();
}


