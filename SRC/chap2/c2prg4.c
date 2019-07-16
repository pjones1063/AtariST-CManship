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


/#include <stdio.h>
#define TEXT "strings"
main()
{
   char ch;

   printf ( ">%s<\n", "strings" );
   printf ( ">%10s<\n", TEXT );
   printf ( ">%-10s<\n", "strings" );
   printf ( ">%10.5s<\n", TEXT );
   printf ( ">%-10.5s<\n", "strings" );
   ch = getchar();
}

