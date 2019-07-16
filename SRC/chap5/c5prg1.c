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
#include <osbind.h>
#define TRUE 1
#define FALSE 0

/*************************************************
*  MAIN PROGRAM
*************************************************/
main ()
{
   int num,   /* Number to guess.   */
       guess, /* Player's guess.    */
       win,   /* Game end flag.     */
       turns, /* Number of guesses. */
       play;  /* Repeat game flag.  */

   play = TRUE;
   while ( play ) {
      turns = 0; 
      win = FALSE;
      num = get_num ();
      while ( !win ) {
         ++turns;
         guess = get_guess ();
         win = check_guess ( num, guess );
      }
      printf ( "It took you %d turns.\n\n", turns );
      play = play_again ();
   }
}

/*************************************************
* get_num()
*
* Returns a random number from 1 to 100.
*************************************************/
int get_num ()
{
   int n;

   n = (int) Random ();
   n = abs(n) % 99 + 1;
   return ( n );
}

/*************************************************
* get_guess ()
*
* Retrieve a number from 1 to 100 from the
* keyboard.
*************************************************/
int get_guess ()
{
   int g;
   g = 0;
   while ( g<1 || g>100 ) {
      printf( "Enter a number from 1 to 100: " );
      scanf ( "%d", &g );
      printf ( "\n\n" );
   }
   return ( g );
}

/*************************************************
* check_guess()
*
* Compare the player's guess with the random
* number, and print the appropriate message.  The
* input to the function is the original number
* and the player's guess.  This function returns 
* a value of TRUE if the number has been guessed, 
* and FALSE otherwise.
*************************************************/
int check_guess ( num, guess )
int num, guess;
{
   int wn = FALSE;

   if ( guess < num )
      printf ( "Too low\n\n" );
   else if ( guess > num )
      printf ( "Too high\n\n" );
   else {
      printf ( "You guessed it!\n" );
      wn = TRUE;
   }
   return ( wn );
}

/*************************************************
* play_again()
*
* Asks the player if he wishes to play again and
* returns a value of TRUE if he does or FALSE if
* he doesn't.
*************************************************/
int play_again ()
{
   int ch, p;

   p = -1;
   ch = getchar ();
   while ( ( p!=TRUE ) && ( p!=FALSE ) ) {
      printf( "Play again? " );
      if ( ( ch=getchar () ) == 'y' || ch == 'Y')
         p = TRUE;
      else if ( ch == 'n' || ch == 'N' )
         p = FALSE;
   }
   printf ( "\n\n" );
   return ( p );
}


