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
#define BACKSPACE 8
#define MAX 8
#define PRINTER_OFF 0
#define NOFILE ((FILE *)0)

FILE *fopen();

int work_in[11];
int work_out[57];
int handle;

int contrl[12];
int intin[128];
int ptsin[128];
int intout[128];
int ptsout[128];

struct name {
            char fname[30];
            char lname[30];
    };
struct rec {
        struct name names;
        char street[30];
        char city[30];
    };

/*****************************************************
* MAIN PROGRAM
******************************************************/
main ()
{
   int num_recs, /* Number of addresses in the file. */
       load;     /* File flag.                       */
   char ch;
   struct rec address[MAX];

   /* Open virtual workstation. */
   open_vwork ();

   /* Get the address data from disk or keyboard. */
   num_recs = get_data ( address, &load );

   /* Convert integer flag to character-type data. */
   ch=load;

   /* Output addresses to screen or printer. */
   output ( address, num_recs );

   /* Save address data to disk if it was entered */
   /* from the keyboard rather than from disk.    */
   if ( ch=='N' || ch=='n')
      save_file( address, num_recs );

   /* Wait for a key press. */
   printf ( "Press key\n" );
   Cconin ();

   /* Close the virtual workstation. */
   v_clsvwk(handle);
}


/*****************************************************
* open_vwork ()
*
* Initializes a virtual workstation.
******************************************************/
open_vwork ()
{
    int i;
    for (i=0; i<10; work_in[i++] = 1);
    work_in[10] = 2;
    v_opnvwk(work_in, &handle, work_out);
}


/*****************************************************
* get_data ()
*
* Allows the user to choose to enter address records 
* either from disk or from the keyboard.  The inputs 
* are a pointer to the array of structures that will 
* hold the data and a pointer to the flag that will
* tell the program whether the data has been loaded
* from disk or typed from the keyboard.  The output
* is the total number of records entered.
/*****************************************************/
get_data ( recp, load )
struct rec *recp;
int *load;
{
   int num_recs;

   /* Print the prompt. */
   Cconws ( "Load file? " );

   /* Loop until we get a proper keystroke. */
   while ( (*load=Cconin())!='Y' && *load!='y' && *load!='N' && *load!='n' );

   printf("\n\n");

   /* If the user answered "N" to the prompt, get the addresses   */
   /* from the keyboard, or else get the addresses from the disk. */
   if ( *load == 'N' || *load == 'n' )
      num_recs = get_records ( recp );
   else
      num_recs = disk_file ( recp );

   /* Return the number of addresses that were entered. */
   return ( num_recs );
}


/*****************************************************
* get_records ()
*
* Retrieves address data from the keyboard.  The
* input is a pointer to the structure that will hold
* the address data.  The output is the total number
* of records entered.
/*****************************************************/
get_records ( recp )
struct rec *recp;
{
   int ans, /* Character storage. */
       i;   /* Record counter.    */

   /* Initialize our variables. */
   ans = 'y';
   i = -1;

   /* Keep getting addresses until the user indicates  */
   /* that he is finished or until we run out of room. */
   while ( (ans=='Y' || ans=='y') && i+1<MAX ) {
      ++i;
      Cconws ( "FIRST NAME: " );
      get_str ( recp->names.fname, 29 );
      Cconws ( "\n LAST NAME: " );
      get_str ( recp->names.lname, 29 );
      Cconws ( "\n    STREET: " );
      get_str ( recp->street, 29 );
      Cconws ( "\n      CITY: " );
      get_str ( recp->city, 29 );
      Cconws ( "\n\nAnother (y/n)? " );
      ans = Cconin ();
      printf ( "\n\n" );
      ++recp;
   }

   /* Return the record count. */
   return ( i+1 );
}


/*****************************************************
* disk_file ()
*
* Reads address records from a disk file.  The input
* is a pointer to the structure in which to store
* the records.  The output is the number of records
* read.
******************************************************/
disk_file(recp)
struct rec *recp;
{
   FILE *p_file;
   char filename[15];
   int num_recs, x, l;

   p_file = NOFILE;

   /* Get valid filename. */
   while (p_file == NOFILE) {
      Cconws("Filename: ");
      get_str(filename,14);
      printf("\n\n");
      p_file = fopen(filename, "r");
      if (p_file == NOFILE)
         printf("No such file!\n\n");
   }

   /* Read in number of records in file. */
   num_recs = getw(p_file);

   /* Read in all address records. */
   for (x=0; x<num_recs; ++x) {
      fgets(recp->names.fname, 30, p_file);
      l = strlen ( recp->names.fname );
      recp->names.fname[l-1] = 0;
      fgets(recp->names.lname, 30, p_file);
      fgets(recp->street, 30, p_file);
      fgets(recp->city, 30, p_file);
      ++recp;
   }

   /* Return number of records read. */
   return(num_recs);
}


/*****************************************************
* output ()
*
* Asks the user if the address records should be
* sent to the printer or to the screen.  The inputs
* are pointer to the structure holding the records
* and the number of records in the structure.
******************************************************/
output(recp, num_recs)
struct rec *recp;
int num_recs;
{
   int status, device;

   /* Initialize loop variable. */
   status = PRINTER_OFF;

   /* Loop until records have been output. */
   while (status==PRINTER_OFF) {

      /* Get device from user. */
      Cconws("Print to screen or printer (s/p)? ");
      device = Cconin();
      printf("\n\n");

      /* Send address records to requested device. */
      if (device == 'p' || device == 'P')
         status = printer(recp, num_recs);
      else {     
         screen(recp, num_recs);
         status = -1;
      }
   }
}


/*****************************************************
* save_file ()
*
* Writes the address records out to a disk file.  The
* inputs are a pointer to the structure holding the
* records and the number of records in the structure.
******************************************************/
save_file(recp, num_recs)
struct rec *recp;
int num_recs;
{
   FILE *p_file;
   char r,x;
   char filename[15];

   /* Ask if user wants to save file. */
   Cconws("Save file? ");
   while ((r=Cconin)!='Y' && r!='y' && r!='N' && r!='n');
   printf("\n\n");

   if (r == 'Y' || r == 'y') {
      p_file = NOFILE;

      /* Loop until we get a valid filename. */
      while (p_file == NOFILE) {
         Cconws("Filename: ");
         get_str(filename, 14);
         printf("\n\n");

         /* If file doesn't exist. open it. */
         if ((p_file=fopen(filename, "r")) == NOFILE)
            p_file = fopen(filename, "w");

         /* If file does exist, check if okay to delete. */
         else {
            p_file = NOFILE;
            Cconws("File already exists.  Delete it? ");
            if ((r=Cconin()) == 'Y' || r == 'y')
               p_file = fopen(filename, "w");
               printf("\n\n");
         }
      }

      /* Write out the number of address records. */
      putw(num_recs, p_file);

      /* Write out all the address records. */
      for (x=0; x<num_recs; ++x) {
         fprintf(p_file, "%s\n", recp->names.fname);
         fprintf(p_file, "%s\n", recp->names.lname);
         fprintf(p_file, "%s\n", recp->street);        
         fprintf(p_file, "%s\n", recp->city);
         ++recp;
      }
      fclose(p_file);
   }
}


/*****************************************************
* screen ()
*
* Writes the address records out to the screen.  The
* input is a pointer to the structure holding the
* records and the number of records in the structure.
******************************************************/
screen(recp, num_recs)
struct rec *recp;
int num_recs;
{
   int x;

   /* Enter alphanumeric screen mode. */
   v_enter_cur(handle);

   /* Write out each line of each record. */
   for (x=0; x<=num_recs-1; ++x) {
      pos_cur(x,0);
      printf("Record #%d\n", x+1);
      pos_cur(x,1);
      printf("%s %s\n", recp->names.fname, recp->names.lname);
      pos_cur(x,2);
      printf("%s\n", recp->street);
      pos_cur(x,3);
      printf("%s\n\n", recp->city);
      ++recp;
   }
}


/*****************************************************
* printer ()
*
* Writes the address records out to a printer.  recp
* is a pointer to the structure holding the
* records and num_recs is the number of records stored
* in the structure.
******************************************************/
printer(recp, num_recs)
struct rec *recp;
int num_recs;
{
   int x, status, i;
   FILE *p_file;

   /* Wait for printer to be turned on. */
   status = Cprnout(0);
   if (status == PRINTER_OFF) {
      printf("Turn on printer!\n");
      return(status);
   }

   /* Send each line of each record to the printer. */
   for ( x=0; x<num_recs; ++x ) {
      for ( i=0; i<strlen(recp->names.fname); ++i )
         Cprnout(recp->names.fname[i]);
      Cprnout ( ' ' );
      for ( i=0; i<strlen(recp->names.lname); ++i )
         Cprnout(recp->names.lname[i]);
      Cprnout ( '\n' );
      Cprnout ( '\r' );
      for ( i=0; i<strlen(recp->street); ++i )
         Cprnout(recp->street[i]);
      Cprnout ( '\n' );
      Cprnout ( '\r' );
      for ( i=0; i<strlen(recp->city); ++i )
         Cprnout(recp->city[i]);
      Cprnout ( '\n' );
      Cprnout ( '\r' );
      Cprnout ( '\n' );
      Cprnout ( '\r' );
      Cprnout ( '\n' );
      Cprnout ( '\r' );
      Cprnout ( '\n' );
      Cprnout ( '\r' );
      ++recp;
   }
   return(status);
}


/*****************************************************
* pos_cur ()
*
* Positions the cursor on the screen.  i is
* the record number and l is the number of the line
* within the record being printed.
******************************************************/
pos_cur(i,l)
int i,l;
{
   int x, y;

   /* If even-numbered record, position on */
   /* right side of screen.                */
   if ((i+1)%2 == 0)
      x = 50;

   /* If odd-numbered record, position on */
   /* left side of screen.                */
   else
      x = 10;

   /* Calculate vertical position of line. */
   y = ((i/2)*5)+4+l;

   /* Position cursor. */
   vs_curaddress(handle,y,x);
} 


/*****************************************************
* get_str ()
*
* Gets a string from the keyboard.  s is a pointer
* to a character array and mx is the maximum allowable
* length of the string.
******************************************************/
int get_str(s, mx)
char s[];
int mx;
{
   int p, code;

   p = 0;

   /* Get character from console. */
   code = Cconin();

   /* Add character to string. */
   while (code != RETURN && p <= mx-1) {
      if (code != BACKSPACE) {
         s[p++] = code;
      }

      /* Handle backspace. */
      else if (p > 0) {
         s[--p] = '\0';
         putchar(BACKSPACE);
         putchar(' ');
         putchar(BACKSPACE);
      }

      /* Get next character. */
      code = Cconin();

      /* Add null to end of string. */
      s[p] = '\0';
   }
   if (p == mx)
      printf("\r\n");
}

