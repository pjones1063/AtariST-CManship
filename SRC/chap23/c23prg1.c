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

#include <osbind.h>
#include <gemdefs.h>
#include <obdefs.h>
#include "date.h"

#define TRUE   1
#define FALSE  0
#define MATCH  0

/* GEM arrays */
int work_in[11],
    work_out[57], 
    contrl[12],
    intin[128], 
    ptsin[128], 
    intout[128], 
    ptsout[128];

extern int gl_apid; /* Global application ID. */

int handle,  /* Application handle. */
    dum,     /* Dummy storage.      */
    menu_id; /* Our accessory's ID. */

char *get_tedinfo_str ();

int msg_buf[8]; /* Message buffer. */

OBJECT *datedial_addr; /* Pointer to dialog box. */

/*****************************************************
*   Resource tree
*****************************************************/

char *rs_strings[] = {
"DATE AND TIME",
"",
"",
"000000AM",
"Time: __:__:__ __",
"999999AA",
"000000",
"Date: __/__/__",
"999999",
"OK",
"CANCEL"};

TEDINFO rs_tedinfo[] = {
0L, 1L, 2L, 3, 6, 2, 0x1180, 0x0, -1, 14,1,
3L, 4L, 5L, 3, 6, 0, 0x1180, 0x0, -1, 9,18,
6L, 7L, 8L, 3, 6, 0, 0x1180, 0x0, -1, 7,15};

OBJECT rs_object[] = {
-1, 1, 5, G_BOX, NONE, 0x30, 0x21121L, 0,0, 41,12,
2, -1, -1, G_BOXTEXT, NONE, SHADOWED, 0x0L, 2,1, 37,1,
3, -1, -1, G_FTEXT, EDITABLE, NORMAL, 0x1L, 12,3, 17,1,
4, -1, -1, G_FTEXT, EDITABLE, NORMAL, 0x2L, 13,5, 14,1,
5, -1, -1, G_BUTTON, 0x5, SHADOWED, 0x9L, 2,9, 17,1,
0, -1, -1, G_BUTTON, 0x27, SHADOWED, 0xAL, 22,9, 17,1};

long rs_trindex[] = {
0L};

#define NUM_OBS 6

/******************************************************
* Main program.
******************************************************/
main ()
{
   appl_init ();           /* Init application.         */
   open_vwork ();          /* Open virtual workstation. */
   do_acc ();              /* Go do our thing.          */
}


/******************************************************
* do_acc ()
* Initialize and handle desk accessory.
******************************************************/
do_acc ()
{
   int x;   /* Loop variable. */

   /* Place our accessory on the menu bar. */
   menu_id = menu_register ( gl_apid, "  Date/Time " );

   /* Initialize resource tree. */
   rs_trindex[0] = (long) rs_object;
   datedial_addr = (OBJECT *) rs_trindex[0];
   rs_object[1].ob_spec = (char *) &rs_tedinfo[0];
   rs_object[2].ob_spec = (char *) &rs_tedinfo[1];
   rs_object[3].ob_spec = (char *) &rs_tedinfo[2];
   rs_tedinfo[0].te_ptext = rs_strings[0];
   rs_tedinfo[0].te_ptmplt = rs_strings[1];
   rs_tedinfo[0].te_pvalid = rs_strings[2];
   rs_tedinfo[1].te_ptext = rs_strings[3];
   rs_tedinfo[1].te_ptmplt = rs_strings[4];
   rs_tedinfo[1].te_pvalid = rs_strings[5];
   rs_tedinfo[2].te_ptext = rs_strings[6];
   rs_tedinfo[2].te_ptmplt = rs_strings[7];
   rs_tedinfo[2].te_pvalid = rs_strings[8];
   rs_object[4].ob_spec = rs_strings[9];
   rs_object[5].ob_spec = rs_strings[10];

   /* Set all the objects' coordinates. */
   for ( x=0; x<NUM_OBS; ++x )
      rsrc_obfix ( datedial_addr, x );

   /* Wait forever for messages. */
   while ( 1 ) {
      evnt_mesag ( msg_buf );

      switch ( msg_buf[0] ) {  /* msg_buf[0] is message type. */

         /* Open our accessory. */
         case AC_OPEN:
            if ( msg_buf[4] == menu_id )
               do_date ();
      }
   }
}


/*****************************************************
* do_date ()
* Loads the resource file and handles the dialog box.
*****************************************************/
do_date ()
{
   int dial_x, /* Dialog's X coord.  */
       dial_y, /* Dialog's Y coord.  */
       dial_w, /* Dialog's width.    */
       dial_h, /* Dialog's height.   */
       choice, /* Exit button clicked from dialog.       */
       okay;   /* Flag indicating if entered date valid. */


   char date_str[8],  /* String to hold date. */
        time_str[10]; /* String to hold time. */

   char *string; /* Temp string pointer. */


   graf_mouse ( ARROW, 0L );
         
   get_time ( time_str );
   get_date ( date_str );

   /* Copy system time and date into dialog box, */
   string = get_tedinfo_str ( datedial_addr, TIMEFLD );
   strcpy ( string, time_str );
   string = get_tedinfo_str ( datedial_addr, DATEFLD );
   strcpy ( string, date_str );

   /* Prepare dialog box for drawing, and init flag. */
   form_center ( datedial_addr, &dial_x, &dial_y, &dial_w, &dial_h );
   form_dial ( FMD_START, 0, 0, 10, 10, dial_x, dial_y, dial_w, dial_h );
   okay = TRUE;

   /* This loop repeats until the user clicks the CANCEL button       */
   /* or until the user enters a valid date and clicks the OK button. */
   do {
      /* Draw dialog and allow user to manipulate it. */
      objc_draw ( datedial_addr, 0, 8, dial_x, dial_y, dial_w, dial_h );
      choice = form_do ( datedial_addr, TIMEFLD );

      /* Reset the state of the chosen button. */
      datedial_addr[choice].ob_state = SHADOWED;

      /* If OK button clicked, check entered date and set system */
      /* date if date entered is valid,                          */
      if ( choice == OKBUTN ) {
         okay = chk_date ( datedial_addr );
         if ( okay )
            set_date ( datedial_addr );
      }
   }
   while ( okay == FALSE && choice == OKBUTN );

   /* Get rid of the dialog box. */
   form_dial ( FMD_FINISH, 0, 0, 10, 10, dial_x, dial_y, dial_w, dial_h );
}


/*****************************************************
* chk_date ()
* Examines the strings in dialog for a valid date
* and valid time.
*****************************************************/
chk_date ( dial_addr )
OBJECT *dial_addr; /* Address of dialog box. */
{
   int mnth, day, year, /* Date and time broken down into integer portions.*/
       hour, min, sec, 
       space,           /* Flag indicating non-valid chars in time string. */
       okay,            /* Flag indicating valid time and date. */
       x;               /* Loop variable. */

   char m[3], d[3], y[3], /* Date and time as character arrays. */
        h[3], mn[3], s[3], 
        ap[3];            /* "AM" or "PM" */

   char *date_str, /* Pointer to string containing date. */
        *time_str; /* Pointer to string containing time. */

   /* Init date and time integers to error condition. */
   mnth = day = year = hour = min = sec = -1;

   /* Get address of string containing date. */
   date_str = get_tedinfo_str ( dial_addr, DATEFLD );

   /* Convert date string to integer format. */
   if ( strlen ( date_str ) == 6 ) {
      strncpy ( m, date_str, 2 );
      m[2] = 0;
      strncpy ( d, &date_str[2], 2 );
      d[2] = 0;
      strncpy ( y, &date_str[4], 2 );
      y[2] = 0;
      mnth = atoi ( m );
      day = atoi ( d );
      year = atoi ( y );
   }

   /* Get address of string containing time. */
   time_str = get_tedinfo_str ( dial_addr, TIMEFLD );

   /* Check for spaces in time string. */
   space = FALSE;
   for ( x=0; x<6; ++x )
      if ( time_str[x] == ' ' )
         space = TRUE;

   /* Convert time string to integer format. */
   if ( (strlen ( time_str ) == 8) && !space ) {
         strncpy ( h, time_str, 2 );
         h[2] = 0;
         strncpy ( mn, &time_str[2], 2 );
         mn[2] = 0;
         strncpy ( s, &time_str[4], 2 );
         s[2] = 0;
         hour = atoi ( h );
         min = atoi ( mn );
         sec = atoi ( s );
         strcpy ( ap, &time_str[6] );
   }

   /* Examine time and date for validity. */
   if ( mnth < 1 | mnth >12 | day < 1 | day > 31 
        | year < 0 | year > 99 | hour < 0 | hour > 23 | min < 0
         | min > 59 | sec < 0 | sec > 59 | 
        ( (strcmp (ap,"AM")!=MATCH) && (strcmp (ap,"PM")!=MATCH) ) ) {
      okay = FALSE;
      form_alert ( 1, "[1][Date or time not valid!][CONTINUE]" );
   }
   else
      okay = TRUE;
      
   return ( okay );
}


/*****************************************************
* set_date ()
* Sets the system time and date to the values
* entered into the dialog box.
*****************************************************/
set_date ( dial_addr )
OBJECT *dial_addr; /* Address of dialog box. */
{
   char *string; /* Temporary string pointer. */
   char s[3];    /* Temporary string storage. */
   int h,        /* Work variable.            */
       time,     /* Time in system format.    */
       date;     /* Date in system format.    */

   /* Get address of string containing time. */
   string = get_tedinfo_str ( dial_addr, TIMEFLD );

   /* Extract "hours" portion and convert to integer. */
   strncpy ( s, string, 2 );
   h = atoi ( s );

   /* Adjust hour to the 24-hour clock format. */
   if ( (strcmp ( &string[6], "PM" ) == MATCH) && (h != 12) )
      h += 12;
   if ( (strcmp ( &string[6], "AM" ) == MATCH) && (h == 12) )
      h = 0;

   /* Shift bits into the proper position and place them */
   /* into the time integer.                             */
   h = h << 11;
   time = h;

   /* Get the "minutes" portion, convert to integer,     */
   /* shift bits and place them into the time integer.   */
   strncpy ( s, &string[2], 2 );
   h = atoi ( s );
   h = h << 5;
   time = time | h;

   /* Process the "seconds" portion of the time. */
   strncpy ( s, &string[4], 2 );
   h = atoi ( s ) / 2;
   time = time | h;

   /* Set the system clock to the new time. */
   Tsettime ( time );

   /* Get the address of the string containing the date. */
   string = get_tedinfo_str ( dial_addr, DATEFLD );

   /* Process the "month" portion. */
   strncpy ( s, string, 2 );
   h = atoi ( s );
   h = h << 5;
   date = h;

   /* Process the "day" portion. */
   strncpy ( s, &string[2], 2 );
   h = atoi ( s );
   date = date | h;

   /* Process the "year" portion. */
   strncpy ( s, &string[4] );
   h = atoi ( s ) - 80;
   h = h << 9;
   date = date | h;

   /* Set the system to clock to the new date. */
   Tsetdate ( date );
}


/*****************************************************
* get_time ()
* Gets system time and converts it to string format.
*****************************************************/
get_time ( string )
char *string; /* Pointer to string in which to store time. */
{
   int time,           /* Time in system format.               */
       hour, min, sec; /* Time broken down into separate ints. */

   char s[3];          /* "AM" or "PM" */

   /* Get system time and break down into individual components. */
   time = Tgettime ();
   sec = ( time & 0x001f ) * 2;
   min = ( time >> 5 ) & 0x003f;
   hour = ( time >> 11 ) & 0x001f;

   /* Convert system 24-hour format to regular 12-hour format. */
   if ( hour > 11 ) {
      strcpy ( s, "PM" );
      if ( hour > 12 )
         hour -= 12;
   }
   else {
      strcpy ( s, "AM" );
      if ( hour == 0 )
         hour = 12;
   }

   /* Convert and add hours to time string. */
   if ( hour < 10 ) {
      string[0] = '0';
      sprintf ( &string[1], "%d", hour );
   }
   else
      sprintf ( string, "%d", hour );

   /* Convert and add minutes to time string. */
   if ( min < 10 ) {
      string[2] = '0';
      sprintf ( &string[3], "%d", min );
   }
   else
      sprintf ( &string[2], "%d", min );

   /* Convert and add seconds to time string. */
   if ( sec < 10 ) {
      string[4] = '0';
      sprintf ( &string[5], "%d", sec );
   }
   else
      sprintf ( &string[4], "%d", sec );

   /* Add "AM" or "PM" to time string. */
   strcpy ( &string[6], s );
}


/*****************************************************
* get_date ()
* Gets system date and converts it to string format.
*****************************************************/
get_date ( string )
char *string; /* Pointer to string that will contain the date. */
{
   int date,            /* Date in system format.       */
       day, mnth, year; /* Date broken into components. */

   /* Get system date and convert to individual components. */
   date = Tgetdate ();
   day = date & 0x001f;
   mnth = (date >> 5) & 0x000f;
   year = ((date >> 9) & 0x007f) + 80;
   year = year % 100;

   /* Convert and add "months" portion to date string. */
   if ( mnth < 10 ) {
      string[0] = '0';
      sprintf ( &string[1], "%d", mnth );
   }
   else
      sprintf ( string, "%d", mnth );

   /* convert and add "days" portion to date string. */
   if ( day < 10 ) {
      string[2] = '0';
      sprintf ( &string[3], "%d", day );
   }
   else
      sprintf ( &string[2], "%d", day );

   /* Convert and add "year" portion to date string. */
   sprintf ( &string[4], "%d", year );
}


/*****************************************************
* get_tedinfo_str ()
* Returns a pointer to an editable string in a 
* dialog box.
*****************************************************/
char *get_tedinfo_str ( tree, object )
OBJECT *tree; /* Address of dialog box.           */
int object;   /* Object that contains the string. */
{
   TEDINFO *ob_tedinfo; /* Pointer to a tedinfo structure. */

   ob_tedinfo = (TEDINFO *) tree[object].ob_spec;
   return ( ob_tedinfo->te_ptext );
}


/*****************************************************
* open_vwork ()
* Opens a virtual workstation.
*****************************************************/
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

