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
#define START {
#define STOP }
#define INPUT scanf
#define OUTPUT printf
#define TIMES *
#define EQUALS =
#define SQUARES main()
#define WAIT ch=getchar()

SQUARES
START
   int num,ans;
   char ch;

   OUTPUT ( "Enter a number: " );
   INPUT ( "%d", &num );
   OUTPUT ( "\n\n" );
   ans EQUALS num TIMES num;
   OUTPUT ( "The square of %d is %d.", num, ans );
   WAIT;
   WAIT;
STOP


