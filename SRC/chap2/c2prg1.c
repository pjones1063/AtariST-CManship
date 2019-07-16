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
#define TEXT "Your full name is"

main()
{
   char ch;
   char fname [20], lname [20];

   printf ( "Enter your first name: " );
   scanf ( "%s", fname );
   printf ( "\n\n" );
   printf ( "Hi, %s!  Enter your last name: ", fname );
   scanf ( "%s", lname );
   printf ( "\n\n" );
   printf ( "%s %s %s.\n\n", TEXT, fname, lname );
   ch = getchar();
   ch = getchar();
}


