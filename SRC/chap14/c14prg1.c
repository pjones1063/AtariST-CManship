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


#include "SAMPLE.H"
#include <OSBIND.H>

#define FMD_START  0
#define FMD_GROW   1
#define FMD_SHRINK 2
#define FMD_FINISH 3
#define R_TREE     0
#define FINGER     3

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

int dial_x,  /* Dialog x coordinate.    */
    dial_y,  /* Dialog y coordinate.    */
    dial_w,  /* Dilaog width.           */
    dial_h,  /* Dialog height.          */
    num,     /* Value of number option. */
    n_x,     /* NUMBERS object x coord. */
    n_y;     /* NUMBERS object y coord. */

char number_str[13] = "    0000    ";  /* NUMBERS string. */

char *find_str();  /* Function declaration. */

   /* Structure to hold an object's description. */
typedef struct object
{
   int           ob_next;  /* Next sibling of object.              */
   int           ob_head;  /* Head of object's children.           */
   int           ob_tail;  /* Tail of object's children.           */
   unsigned int  ob_type;  /* Type of object.                      */
   unsigned int  ob_flags; /* Flags.                               */
   unsigned int  ob_state; /* State of object.                     */
   char          *ob_spec; /* Miscellaneous information.           */
   int           ob_x;     /* x coord of object upper left corner. */
   int           ob_y;     /* y coord of object upper left corner. */
   int           ob_w;     /* Width of object.                     */
   int           ob_h;     /* Height of object.                    */
} OBJECT;

OBJECT *tree_addr;  /* Pointer to our object structure. */

   /* Structure to hold object text information. */
typedef struct text_edinfo
{
   char *te_ptext;    /* Pointer to text.             */
   char *te_ptmplt;   /* Pointer to template.         */
   char *te_pvalid;   /* Pointer to validation chars. */
   int  te_font;      /* Font.                        */
   int  te_junk1;     /* Unused.                      */
   int  te_just;      /* Justification.               */
   int  te_color;     /* Color information.           */
   int  te_junk2;     /* Unused.                      */
   int  te_thickness; /* Border thickness.            */
   int  te_txtlen;    /* length of text string.       */
   int  te_tmplen;    /* length of template string.   */
} TEDINFO;

main ()
{
   appl_init ();              /* Initialize application.        */
   open_vwork ();             /* Set up workstation.            */
   do_dialog();               /* Go do the dialog box.          */
   button_wait();             /* Wait for mouse button.         */
   rsrc_free ();              /* Release resource memory.       */
   v_clsvwk (handle);         /* Close virtual workstation.     */
   appl_exit ();              /* Back to the desktop.           */
}

open_vwork ()
{
   int i;

   /* Get graphics handle, initialize the GEM arrays and open  */
   /* a virtual workstation.                                   */

   handle = graf_handle (&dum,&dum,&dum,&dum);
   for (i=0; i<10; work_in[i++] = 1);
   work_in[10] = 2;
   v_opnvwk (work_in, &handle, work_out);
}

do_dialog ()
{
   int choice; /* Button choice from dialog. */

   /* Here we load the resource file.  If the file is missing,  */
   /* we warn the user with an alert box then terminate the     */
   /* program by skipping the code following the else.          */
 
   if (! rsrc_load ("\SAMPLE.RSC"))
      form_alert (1, "[1][SAMPLE.RSC missing!][I'll do better!]");

   /* If the resource file loads OK, we get the address of the  */
   /* tree, get the coords for centering the dialog, save the   */
   /* portion of the screen that'll be covered by the dialog,   */
   /* and draw the dialog.  The mouse pointer is changed to     */
   /* pointing finger.                                          */

   else {
      rsrc_gaddr (R_TREE, SAMPLE, &tree_addr);
      form_center (tree_addr, &dial_x, &dial_y, &dial_w, &dial_h);
      objc_offset (tree_addr, NUMBERS, &n_x, &n_y);
      form_dial (FMD_START, 0, 0, 10, 10, dial_x, dial_y, dial_w, dial_h);
      form_dial (FMD_GROW, 0, 0, 10, 10, dial_x, dial_y, dial_w, dial_h);
      objc_draw (tree_addr, 0, 2, dial_x, dial_y, dial_w, dial_h);
      graf_mouse (FINGER,&dum);

   /* Here we allow the user to interact with the dialog then,  */
   /* based on the chosen button, perform the necessary action. */
   /* The form_do function is repeated until the user chooses   */
   /* either the OK button or the CANCEL button.                */

      num = 0;
      do {
         choice = form_do (tree_addr, NAME);
         if (choice == RADIO1) v_gtext (handle,160,20,"Radio 1   ");
         if (choice == RADIO2) v_gtext (handle,160,20,"Radio 2   ");
         if (choice == RADIO3) v_gtext (handle,160,20,"Radio 3   ");
         if (choice == RADIO4) v_gtext (handle,160,20,"Radio 4   ");
         if (choice == RADIO5) v_gtext (handle,160,20,"Radio 5   ");
         if (choice == RADIO6) v_gtext (handle,160,20,"Radio 6   ");
         if (choice == OPTION1) v_gtext (handle,160,20,"Option 1  ");
         if (choice == OPTION2) v_gtext (handle,160,20,"Option 2  ");
         if (choice == UPARROW) do_up();
         if (choice == DWNARROW) do_down();
      }
      while (choice != CANCEL && choice != OK);

   /* Once the CANCEL or OK buttons have been pressed, we clean  */
   /* up after ourselves by performing the "shrinking box" and   */
   /* then redrawing the screen.                                 */

      form_dial (FMD_SHRINK, 0, 0, 10, 10, dial_x, dial_y, dial_w, dial_h);
      form_dial (FMD_FINISH, 0, 0, 10, 10, dial_x, dial_y, dial_w, dial_h);
      print_results(tree_addr);  /* Print user's choices.          */
  }
}

do_up ()
{

   /* First we increment our value and make sure it stays in    */
   /* range.  If the value has become larger than 9999, we must */
   /* also reinitialize display string for the object NUMBERS.  */
   /* We then call our function to update the NUMBERS object.   */

   num += 1;
   if (num > 9999) {
      num = 0;
      strcpy (number_str,"    0000    ");
   }
   edit_object ();
}

do_down ()
{

   /* Here we decrement the value and check for its range, after */
   /* which we update the NUMBERS object.                        */

   num -= 1;
   if (num < 0) num = 9999;
   edit_object ();
}

edit_object ()
{
   TEDINFO *ob_tedinfo;
   char temp_str[10];

   /* Here we edit the string we're using for the text display  */
   /* in the object NUMBERS so that it reflects the new value.  */

   sprintf (temp_str,"%d",num);
   strcpy (&number_str[8 - strlen (temp_str)],temp_str);
   strcpy (&number_str[8],"    ");

   /* Then we find the object NUMBERS' TEDINFO and point the    */
   /* te_ptext member to our updated string, after which we     */
   /* redraw the object NUMBERS.                                */

   ob_tedinfo = (TEDINFO *) tree_addr[NUMBERS].ob_spec;
   ob_tedinfo -> te_ptext = number_str;

   /* For high resolution, change the 16 below to 32. */
   objc_draw (tree_addr, NUMBERS, 1, n_x, n_y, 96, 16);
}

print_results (tree_addr)
OBJECT tree_addr[];
{
   char *string;

   /* Here we call the function that locates the string, then  */
   /* print the user's input to the screen.                    */

   string = find_str (NAME, string);
   v_gtext (handle, 160, 20, "Your name is ");
   v_gtext (handle, 264, 20, string);
   string = find_str (AGE, string);
   v_gtext (handle, 160, 36, "Your age is ");
   v_gtext (handle, 264, 36, string);
   string = find_str (NUMBERS, string);
   v_gtext (handle, 160, 52, "Final number value: ");
   v_gtext (handle, 320, 52, &string[4]);
}

char *find_str (object, string)
int object;
char *string;
{
   TEDINFO *ob_tedinfo;

   /* In this function, we locate the object's TEDINFO structure */
   /* then set our string pointer to the pointer found in the    */
   /* te_ptext member.                                           */

   ob_tedinfo = (TEDINFO *) tree_addr[object].ob_spec;
   string = ob_tedinfo -> te_ptext;
   return (string);
}
   
/* Waits for left button to be pressed and released. */
button_wait()
{
    evnt_button (1,1,1,&dum,&dum,&dum,&dum);
    evnt_button (1,1,0,&dum,&dum,&dum,&dum);
}

