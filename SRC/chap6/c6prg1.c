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

#define RETURN 13
#define BACKSP 8
#define MAX 78
#define NOFILE -1
#define CTRL_Z 26

int code;

/******************************************************
* MAIN PROGRAM
******************************************************/
main()
{
   char filename[15], /* Filename for text file. */
        text[MAX];    /* Text entered by user.   */
   int  file;         /* File ID.                */

   file = start_file ( filename );
   get_text ( file, text );
   close ( file );
}

/******************************************************
* start_file ()
*
* Gets the filename from the user, and then opens the
* file.  The input to the function is the address of
* storage for the filename.  The function's output is 
* the open file's file ID.
******************************************************/
start_file ( filename )
char filename[];
{
   int file, /* File ID.           */
       ch;   /* Character storage. */

   /* Initialize file ID to error condition. */
   file = NOFILE;

   /* Continue trying to open a file until successful. */
   while (file == NOFILE) {

      /* Get filename from user. */
      printf ( "Filename: \n" );
      get_str ( filename );

      /* Check if file already exists. */
      if ( ( file = open(filename,2) ) == NOFILE )

         /* If it doesn't exist, create it. */
         file = creat ( filename, 0 );

      /* The file the user wants to open already exists. */
      else {

         /* Reset file ID to error condition in case user */
         /* doesn't want to delete already existing file. */
         file = NOFILE;

         /* Find out if user wants to delete the existing */
         /* file and create a new one.                    */
         printf ( "File already exists!  Delete it? " );
         if ( (ch = getchar() ) == 'Y' || ch == 'y' )
            file = creat ( filename, 0 );
      }
   }
   printf ( "\n" );
   return ( file );
}

/******************************************************
* get_text ()
*
* Get the text to be stored in the file from the user.
* The inputs to the function are the file's ID and
* the address of string storage.
******************************************************/
get_text ( file, text )
int file;
char text[];
{
   int num_char; /* Number of characters in string. */

   printf( "Type your message:\n\n" );
   code = 0;

   /* Get text from keyboard until user wants to exit. */
   while ( code != CTRL_Z ) {

      /* Get a string of text. */
      num_char = get_str ( text );

      /* Add a LF and a null to the string. */
      text[num_char++] = '\n';
      text[num_char] = '\0';

      /* Save the text string to the file. */
      write ( file, text, num_char );
   }
}


/******************************************************
* get_str ()
*
* Gets each of the strings that make up the text file.
* The input is the address of the string storage.  The
* output is the number of characters in the string.
******************************************************/
get_str ( text )
char text[];
{
   int n; /* Character count. */

   n = 0;

   /* Get character code from keyboard. */
   code = Cconin();

   /* Check if end of string or end of text. */
   while ( code != RETURN && code != CTRL_Z && n <= MAX ) {

      /* Add character to string if not a backspace. */
      if ( code != BACKSP ) {
         text[n++] = code;
      }

      /* Handle backspace character. */
      else if ( n > 0 ) {

         /* Shorten string by one character. */
         text[--n] = '\0';

         /* Erase character on screen. */
         Cconout ( ' ' );

         /* Move cursor back one space. */
         Cconout ( BACKSP );
      }

      /* Get next character code. */
      code = Cconin ();
   }
   printf ( "\n" );
   return ( n );
}

      

