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
#include <gemdefs.h>
#include <obdefs.h>
#include <fcntl.h>
#include "microchk.h"

#define WA_UPPAGE    0
#define WA_DNPAGE    1
#define WA_UPLINE    2
#define WA_DNLINE    3
#define WA_LFPAGE    4
#define WA_RTPAGE    5
#define BOLD         1
#define LIGHT        2
#define TRUE         1
#define FALSE        0
#define YES          1
#define NO           2
#define LEFT_BUTTON  0x0001
#define BUTTON_DOWN  0x0001
#define NUM_CLICKS   2
#define PARTS        NAME|INFO|UPARROW|DNARROW|VSLIDE|FULLER|CLOSER|HSLIDE
#define NUM_COLUMNS  93
#define MED          1
#define MATCH        0
#define REC_LENGTH   117
#define FROM_BEG     0
#define FROM_CUR_POS 1
#define FAILED       (-1)
#define DFLT_DRV     0
#define VISIBLE      1
#define MEDIUM       1
#define HIGH         2
#define CHAR_AVAIL   -1
#define CONSOLE      2
#define ESCAPE       27
#define CNTL_A       0x1e01
#define CNTL_B       0x3002
#define CNTL_C       0x2e03
#define CNTL_D       0x2004
#define CNTL_E       0x1205
#define CNTL_G       0x2207
#define CNTL_I       0x1709
#define CNTL_M       0x320d
#define CNTL_N       0x310e
#define CNTL_O       0x180f
#define CNTL_P       0x1910
#define CNTL_Q       0x1011
#define CNTL_R       0x1312
#define CNTL_S       0x1f13
#define CNTL_W       0x1117
#define CNTL_Y       0x1519

int work_in[11], work_out[57], contrl[12],
    intin[128], ptsin[128], intout[128], ptsout[128];

int msg_buf[8];

long pwrs[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000 };

int handle, dum, file, key,
    fullx, fully, fullw, fullh, wrkx, wrky, wrkw, wrkh, 
    w_h1, w_h2, res, full, num_trans, charw, charh, curchknum,
    num_deps, num_chks, loaded, all_done, mouse_x, mouse_y,
    num_clicks, edit_top, left, start_mnth, end_mnth, mnth, srch_trans,
    start_num, end_num, cur_count, cur_top, search, saved, canceling,
    month, full_draw, oldcolr;

int zero = 0;

char filename[64], chkname[30], chkstreet[30], chkcity[50],
     date_but[10], bal_but[10], trans_but[4],
     check_but[4], dep_but[4], mnth_but[10], acct_name[64],
     monthfile[64], cur_chk_num[6], cur_date[7], future_use[40],
     cancmnth[5], chtot[20], dptot[20], chcnt[10], dpcnt[10];

char windname[64];
char noacct[] = "No account opened";

char canc[] = "CANCEL CHECKS";
char newm[] = "  NEW MONTH  ";

char *months[] = { "Month 0", "January", "February", "March", "April",
                   "May", "June", "July", "August", "September", "October",
                   "November", "December" };

char spaces[] = "                              ";
char infotext[] = "  Number   Amount    Payee                           Memo                            Date";
char *string, *srch_payee, *srch_memo;
char rule[] = "------------------------------------------------------------------------------";

long balance, start_amnt, end_amnt;

OBJECT *menu_addr, *check_addr, *newacct_addr, *newfile_addr, 
       *newdate_addr, *srchdial_addr, *cancdial_addr, *recndial_addr,
       *rprtdial_addr, *lkmndial_addr, *srtodial_addr;

FILE *acctfile, *mfile;

char *get_tedinfo_str ();
FILE *opn_nw_auto ();
long str_to_long ();

struct check {
   char number[5];
   char payee[31];
   char memo[31];
   char date[9];
   long amount;
   char cancel[2];
};

struct check checks[500];
struct check srch_checks[1000];
struct check *cur_chk_strc;

TEDINFO *ob_tedinfo;


main ()
{
   appl_init ();            /* Initialize application.    */
   open_vwork ();           /* Set up workstation.        */
   do_mcheck();             /* Go do MicroCheck.          */
   v_clsvwk (handle);       /* Close virtual workstation. */
   Setcolor ( 2, oldcolr ); /* Reset color register.      */
   appl_exit ();            /* Back to the desktop.       */
}


do_mcheck()
{
   oldcolr = Setcolor ( 2, -1 );
   Setcolor ( 2, 0x005 );
   if ( (res = Getrez ()) != HIGH && res != MEDIUM )
      form_alert(1,"[0][MicroCheck ST runs|only in high or medium |resolution.][OK]");
   else {
      graf_mouse ( ARROW, &dum );
      strcpy ( acct_name, "NONE" );
      strcpy ( cur_chk_num, "0000" );
      balance = 0;
      month = -1;
      edit_top = cur_top = num_trans = num_chks = num_deps = 0;
      left = saved = TRUE;
      search = canceling = full_draw = FALSE;
      cur_chk_strc = checks;
      get_date ();

      if ( !rsrc_load ( "\MICROCHK.RSC" ) )
         form_alert ( 1, "[1][MICROCHK.RSC missing!][Okay]" );
      else {
         rsrc_gaddr ( R_TREE, MENUBAR, &menu_addr );
         rsrc_gaddr ( R_TREE, CHEKDIAL, &check_addr );
         rsrc_gaddr ( R_TREE, NEWADIAL, &newacct_addr );
         rsrc_gaddr ( R_TREE, FILEDIAL, &newfile_addr );
         rsrc_gaddr ( R_TREE, DATEDIAL, &newdate_addr );
         rsrc_gaddr ( R_TREE, SRCHDIAL, &srchdial_addr );
         rsrc_gaddr ( R_TREE, CANCDIAL, &cancdial_addr );
         rsrc_gaddr ( R_TREE, RECNDIAL, &recndial_addr );
         rsrc_gaddr ( R_TREE, RPRTDIAL, &rprtdial_addr );
         rsrc_gaddr ( R_TREE, LKMNDIAL, &lkmndial_addr );
         rsrc_gaddr ( R_TREE, SRTODIAL, &srtodial_addr );
         menu_bar ( menu_addr, TRUE );
         set_menu_entries ();
         wind_get ( 0, WF_WORKXYWH, &fullx, &fully, &fullw, &fullh );
         w_h1 = wind_create ( 0, fullx, fully, fullw, fullh );
         w_h2 = wind_create ( PARTS, fullx, fully, fullw, fullh );
         wind_set ( w_h2, WF_NAME, noacct, 0, 0 );
         wind_set ( w_h2, WF_INFO, infotext, 0, 0 );
         wind_open ( w_h1, fullx, fully, fullw, fullh );
         wind_open ( w_h2, fullx, fully, fullw, 316 - 162*(res==MED) );
         calc_vslid ( 1 );
         calc_hslid ( NUM_COLUMNS );
         full = FALSE;
         loaded = FALSE;

         get_event ();

         menu_bar ( menu_addr, FALSE );
         wind_close ( w_h2 );
         wind_delete ( w_h2 );
         wind_close ( w_h1 );
         wind_delete ( w_h1 );
         rsrc_free ();
      }
   }
}


get_event ()
{
   int h, event;

   all_done = FALSE;

   while ( !all_done ) {
      event = evnt_multi ( MU_KEYBD|MU_MESAG|MU_BUTTON, NUM_CLICKS, 
                         LEFT_BUTTON, BUTTON_DOWN,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, msg_buf, 0, 0,
                         &mouse_x, &mouse_y, &dum, &dum, &key, &num_clicks );

      if ( event & MU_KEYBD )
         handle_keys ();

      if ( event & MU_MESAG )
         handle_messages ();

      if ( event & MU_BUTTON )
         handle_button ();
   }
}


set_menu_entries ()
{
   menu_ienable ( menu_addr, CLOSEMBR, loaded );
   menu_ienable ( menu_addr, OPENMBR, !loaded );
   menu_ienable ( menu_addr, NEWACCNT, !loaded );
   menu_ienable ( menu_addr, QUIT, TRUE );
   menu_ienable ( menu_addr, ENTER, loaded );
   menu_ienable ( menu_addr, SEARCH, loaded );
   menu_ienable ( menu_addr, CHKCAN, loaded );
   menu_ienable ( menu_addr, NEWMNTH, loaded );
   menu_ienable ( menu_addr, RECONCIL, loaded );
   menu_ienable ( menu_addr, PRNTWIND, loaded );
   menu_ienable ( menu_addr, PRNTREG, loaded );
   menu_ienable ( menu_addr, NEWYEAR, !loaded );
   menu_ienable ( menu_addr, CHKAUTO, loaded );
   menu_ienable ( menu_addr, NEWDATE, TRUE );
   menu_ienable ( menu_addr, IMPORT, !loaded );
}


calc_vslid ( line_cnt )
int line_cnt;
{
   int lines_avail, vslid_siz, pos;

   if ( line_cnt == 0 ) line_cnt = 1;
   wind_get ( w_h2, WF_WORKXYWH, &wrkx, &wrky, &wrkw, &wrkh );
   lines_avail = wrkh / charh;
   vslid_siz = 1000 * lines_avail / line_cnt;
   wind_set ( w_h2, WF_VSLSIZE, vslid_siz, 0, 0, 0 );
   pos = (int) ( (float)(cur_top) ) / 
         ( (float)(line_cnt - lines_avail) ) * 1000;
   wind_set ( w_h2, WF_VSLIDE, pos, 0, 0, 0 );
}


calc_hslid ( col_cnt )
int col_cnt;
{
   int cols_avail, hslid_siz, pos, lft;

   if ( left )
      lft = 0;
   else
      lft = 16;
   wind_get ( w_h2, WF_WORKXYWH, &wrkx, &wrky, &wrkw, &wrkh );
   cols_avail = wrkw / charw;
   hslid_siz = (int) ((1000L * (long) cols_avail) / (long) col_cnt);
   wind_set ( w_h2, WF_HSLSIZE, hslid_siz, 0, 0, 0 );
   pos = (int) ( (float)(lft) ) / ( (float)(col_cnt - cols_avail) ) * 1000;
   wind_set ( w_h2, WF_HSLIDE, pos, 0, 0, 0 );
}


open_vwork ()
{
   int i;

   handle = graf_handle ( &charw, &charh, &dum, &dum);
   for ( i=0; i<10; work_in[i++] = 1 );
   work_in[10] = 2;
   v_opnvwk ( work_in, &handle, work_out );
}


get_date ()
{
   int date, day, mnth, year;
   char d[3], m[3], y[4];

   date = Tgetdate ();
   day = date & 0x001f;
   mnth = (date >> 5) & 0x000f;
   year = ((date >> 9) & 0x007f) + 80;
   year = year % 100;
   sprintf ( d, "%d", day );
   sprintf ( m, "%d", mnth );
   sprintf ( y, "%d", year );
   if ( mnth < 10 ) {
      date_but[0] = '0';
      cur_date[0] = '0';
      strcpy ( &date_but[1], m );
      strcpy ( &cur_date[1], m );
   }
   else {
      strcpy ( date_but, m );
      strcpy ( cur_date, m );
   }
   date_but[2] = '/';
   if ( day < 10 ) {
      date_but[3] = '0';
      cur_date[2] = '0';
      strcpy ( &date_but[4], d );
      strcpy ( &cur_date[3], d );
   }
   else {
      strcpy ( &date_but[3], d );
      strcpy ( &cur_date[2], d );
   }
   date_but[5] = '/';
   if ( year < 10 ) {
      date_but[6] = '0';
      cur_date[4] = '0';
      strcpy ( &date_but[7], y );
      strcpy ( &cur_date[5], y );
   }
   else {
      strcpy ( &date_but[6], y );
      strcpy ( &cur_date[4], y );
   }
}


handle_keys ()
{}

handle_messages ()
{}

handle_button ()
{}

