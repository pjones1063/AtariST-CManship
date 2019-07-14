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


#include <osbind.h>

#define TRUE        1
#define FALSE       0
#define O_BINARY    8192
#define QUIT        3
#define LEFT_BUTTON 1
#define DOWN        1

/* The usual required GEM global arrays */
int work_in[11], 
    work_out[57], 
    pxyarray[10],
    contrl[12], 
    intin[128], 
    ptsin[128], 
    intout[128], 
    ptsout[128];

/* Global variables */
int handle, dum;

long pic[2],  /* Pointers to logical screens. */
     scrn;    /* Pointer to physical screen.  */

int desk_palette[16];   /* Desktop color palette.  */
int pic_palette[2][16]; /* Picture color palettes. */


main ()
{
   appl_init ();       /* Initialize application.        */
   open_vwork ();      /* Set up workstation.            */
   do_pictures ();     /* Go do the picture stuff.       */
   clean_up ();        /* Get everything back to normal. */
   appl_exit ();       /* Back to the desktop.           */
}


open_vwork ()
{
   int i;

   /* Get graphics handle, initialize the GEM arrays and open  */
   /* a virtual workstation.                                   */

   handle = graf_handle ( &dum, &dum, &dum, &dum);
   for ( i=0; i<10; work_in[i++] = 1 );
   work_in[10] = 2;
   v_opnvwk ( work_in, &handle, work_out );
}


do_pictures ()
{
   /* If the pictures are loaded okay, */
   /* then allow user to view them.    */

   if ( init_screens () )
      flip_screens ();
}


init_screens ()
{
   int x,       /* Index variable.              */
       okay;    /* File load flag.              */

   /* Store the desktop's color palette. */
   for ( x=0; x<16; desk_palette[x++]=Setcolor (x, -1) );

   /* Store the address of the desktop's screen. */
   scrn = Physbase ();

   /* Reserve memory for pictures and load them */
   /* into the allotted space, storing pointers */
   /* to them in the pic[] array.               */

   okay = TRUE;
   x = 0;
   while ( (okay == TRUE) && (x < 2) ) {
      pic[x] = ( Malloc (32768L) & 0xffffff00 ) + 0x0100;
      okay = get_pic ( x++ );
   }
   return ( okay );
}


flip_screens ()
{
   int choice; /* Button number clicked in alert box. */

   choice = 1;

   /* View pictures until QUIT button is clicked. */
   while ( choice != QUIT ) {

      /* Call up alert box to get user's picture choice. */
      choice = form_alert ( 0, "[2][Choose picture to view][One|Two|Quit]" );

      /* We only want to show a picture if the */
      /* QUIT button hasn't been clicked.      */

      if ( choice != QUIT ) {

         /* Set the screen to show the chosen picture. */
         Setscreen ( pic[choice-1], pic[choice-1], -1 );

         /* Set the palette to the picture's settings. */
         Setpalette ( &pic_palette[choice-1][0] );

         /* Wait for a button click. */
         evnt_button ( 1, LEFT_BUTTON, DOWN, &dum, &dum, &dum, &dum );
      }
   }
}


get_pic ( num )
int num; /* Number of picture to load. */
{
   char path[64],   /* Storage for picture's pathname.       */
        file[13],   /* Storage for picture's filename.       */
        pictype[6]; /* Storage for default picture filename. */

   /* Build default picture filename. */
   strcpy ( pictype, "*.PI " );
   pictype[4] = Getrez () + '1';

   /* If file selector CANCEL button wasn't clicked, */
   /* read the chosen DEGAS file into memory.  If an */
   /* error is returned, the program will abort.     */

   if ( select_file ( path, file, pictype, FALSE ) )
      if ( read_degas ( num, path ) )
         return ( TRUE );
      else
         return ( FALSE );
   else
      return ( FALSE );
}


read_degas ( num, pathname )
int num;        /* Picture number to read. */
char *pathname; /* Picture's pathname.     */
{
   int f_h,     /* File handle.                  */
       buf[10]; /* Temp buffer for unused bytes. */

   /* Process file only if no error is returned when opening. */
   if ( (f_h = open ( pathname, O_BINARY )) != -1 ) {

      /* First two bytes is resolution data. */
      read ( f_h, buf, 2 );

      /* Next 32 bytes (16 words) is the color palette. */
      read ( f_h, &pic_palette[num][0], 32 );

      /* Finally, we have 32K of picture data. */
      read ( f_h, pic[num], 32000 );

      /* Close file and tell calling function */
      /* that everything went all right.      */

      close ( f_h );
      return ( TRUE );
   }

   /* In case of error opening the file. */
   else 
      return ( FALSE );
}


select_file ( path, fnme, deflt, display)
char *path,  /* Address for path storage.     */
     *fnme,  /* Address for filename storage. */
     *deflt; /* Address of default filename.  */
int display;    /* Display default filename?     */
{
   int x,      /* Loop variable.                         */
       choice, /* Button clicked from file selector box. */
       len;    /* String length.                         */
   char ch;    /* Temp character storage.                */

   /* Clear filename string if not to be displayed. */
   if ( display == FALSE )
      for ( x=0; x<13; fnme[x++] = '\0' );

   /* Build file selector box pathname. */
   Dgetpath ( path, 0 );
   len = strlen ( path );
   path[len] = '\\';
   strcpy ( &path[ len + 1 ], deflt );

   /* Call up file selector box to get user's choice. */
   fsel_input ( path, fnme, &choice );

   /* Find last significant character in pathname in    */
   /* order to delete the filename portion of the path. */

   len = strlen ( path );
   x = len-1;
   while ( path[x] != '\\' && path[x] != ':' && x > 0 )
      --x;
   strcpy ( &path[x+1], fnme );

   return ( choice );
}


clean_up ()
{
   /* Setscreen back to desktop. */
   Setscreen ( scrn, scrn, -1 );

   /* Restore original color palette. */
   Setpalette ( desk_palette );

   /* Return the reserved memory back to the system. */
   Mfree ( pic[1] );
   Mfree ( pic[0] );

   /* Close virtual workstation. */
   v_clsvwk ();
}


