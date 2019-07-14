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
#define MAX 10

/*************************************************
* main ()
*
* Main program
*************************************************/
main ()
{
   int num;
   int val[MAX];
   char ch;

   num = how_many ();
   get_nums ( num, val );
   sort ( num, val );
   output ( num, val );
   Cconin ();
}

/*************************************************
* how_many ()
*
* Retrieves from user the number of values to be
* sorted and returns that value to main ().
*************************************************/
how_many ()
{
   int n;

   n = MAX +1;
   while ( n > MAX ) {
      printf ( "How many numbers? " );
      scanf ( "%d", &n );
      printf( "\n\n" );
   }
   return ( n );
}

/*************************************************
* get_nums ()
*
* Retrieves from user the values to be sorted 
* and stores those values in the array v[]. 
* Input to the function is the number of values 
* to be sorted and the address of the array in 
* which to store the values.
*************************************************/
get_nums ( n, v )
int n;
int v[];
{
   int x, num;

   for ( x=0; x<n; ++x ) {
      printf ( "Enter number %d: ", x+1 );
      scanf ( "%d", &num );
      v[x] = num;
      printf ( "\n" );
   }
   return;
}

/*************************************************
* sort ()
*
* Uses a bubble sort to sort the #'s stored in 
* the input array.  Input to the function is the 
* number of values to be sorted and the address 
* of the array in which the values are stored.
*************************************************/
sort ( n, v )
int n;
int v[];
{
   int swtch, x, temp;

   swtch = 1;
   while ( swtch == 1 )
   {
      swtch = 0;
      for ( x=0; x<n-1; ++x )
         if ( v[x] > v[x+1] )
         {
            temp = v[x];
            v[x] = v[x+1];
            v[x+1] = temp;
            swtch = 1;
         }
   }
   return;
}

/*************************************************
* output ()
*
* Prints the sorted values to the screen.  The
* input to the function is the number of values to 
* print and the address of the array where the
* values are stored.
*************************************************/
output ( n,  v )
int n;
int v[];
{
   int x;

   printf ( "Sort complete!\n\n" );
   for ( x=0; x<=n-1; ++x )
      printf( "%d ", v[x] );
   printf( "\n\n" );
   return;
}


