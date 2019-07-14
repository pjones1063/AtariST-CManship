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
   char ch;
   int num1,num2,ans;
 
   printf ( "Enter two numbers: " );
   scanf ( "%d %d", &num1, &num2 );
   ans = num1 + num2;
   printf ( "\n\n" );
   printf ( "The sum of %d and %d is %d.\n\n", num1, num2, ans );
   printf ( "Press return\n" );
   ch = getchar();
   ch = getchar();
}

