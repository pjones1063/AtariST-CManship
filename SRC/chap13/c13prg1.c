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

/* Main program */
main ()
{
   appl_init ();      /* Initialize application.        */
   open_vwork ();     /* Set up workstation.            */
   sel_file ();       /* Go select file.                */
   button_wait ();    /* Wait for a mouse button press. */
   v_clsvwk (handle); /* Close virtual workstation.     */
   appl_exit ();      /* Back to the desktop.           */
}

/* Initialize a virtual worksation. */
open_vwork ()
{
    int i;

    handle = graf_handle (&dum,&dum,&dum,&dum); /* Get graphics handle. */
    for (i=0; i<10; work_in[i++] = 1);     /* Initialize GEM arrays.    */
    work_in[10] = 2;
    v_opnvwk (work_in, &handle, work_out); /* Open virtual workstation. */
}

/* Do file selector box. */
sel_file ()
{
  int button,    /* File selector button value. */
      i;         /* Loop variable.              */
  char path[50], /* Storage for filenames.      */
       file[13];

  for (i=0; i<20; path[i++]='\0'); /* Fill filename space with nulls. */
  for (i=0; i<13; file[i++]='\0');
  path[0] = Dgetdrv() + 65;        /* Convert default drive to char.  */
  strcpy (&path[1],":\*.*");       /* Complete the pathname.          */
  fsel_input (path,file,&button);  /* Open the file selector box.     */
  prnt_info (file,button);         /* Go print results.               */
}

/* Print out the user's choices. */
prnt_info (file,button)
char *file; /* Pointer to the chosen filename. */
int button; /* Value of the button pressed.    */
{
  v_gtext (handle,28,50,"The file you chose was: "); 
  v_gtext (handle,220,50,file);
  v_gtext (handle,28,66,"And you pressed the ");
  if (button == 0)
    v_gtext (handle,188,66,"CANCEL button.");
  else
    v_gtext (handle,188,66,"OK button.");
}

/* Waits for left button to be pressed and released. */
button_wait()
{
    evnt_button (1,1,1,&dum,&dum,&dum,&dum);
    evnt_button (1,1,0,&dum,&dum,&dum,&dum);
}
                                    
