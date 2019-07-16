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

open_acct ( file )
char *file;
{
   int x, len;
   char zip[10], buf[25];

   if ( ( acctfile = fopen ( file, "br" )) == 0 )
      form_alert ( 1, "[1][Can't open the file][CONTINUE]" );
   else {
      fread ( chkname, 1, 26, acctfile );
      fread ( chkstreet, 1, 26, acctfile );
      fread ( chkcity, 1, 16, acctfile );
      fread ( buf, 1, 10, acctfile);
      strcpy ( &chkcity[strlen(chkcity)], ", " );
      fread ( &chkcity[strlen(chkcity)], 1, 3, acctfile );
      strcpy ( &chkcity[strlen(chkcity)], "  " );
      fread ( zip, 1, 10, acctfile );
      len = strlen ( chkcity );
      if ( strlen ( zip ) > 5 ) {
         strncpy ( &chkcity[len], zip, 5 );
         chkcity[len+5] = 0;
         strcpy ( &chkcity[strlen(chkcity)], "-" );
         strcpy ( &chkcity[strlen(chkcity)], &zip[5] );
      }
      else
         strcpy ( &chkcity[strlen(chkcity)], zip );
      fread ( &balance, 4, 1, acctfile );
      if ( fclose ( acctfile ) != 0 )
            form_alert ( 1, "[1][File close error!][OKAY]");
      check_addr[CHKNAME].ob_spec = chkname;
      check_addr[CHKSTREE].ob_spec = chkstreet;
      check_addr[CHKCITY].ob_spec = chkcity;
      do_new_mnth ();
      if ( loaded )
         set_menu_entries ();
      else
         balance = 0;
   }
}


do_new_mnth ()
{
   int choice;

   cancdial_addr[CANCSTRG].ob_spec = newm;
   choice = get_month ();
   if ( choice == CANCOK ) {
      if ( !saved )
         save_month ( monthfile );
      open_new_month ();
   }
}


save_month ( file )
char *file;
{
   char newmfile[64];
   int x;

   strcpy ( newmfile, file );
   strcpy ( &newmfile[strlen(newmfile)-3], "BAK" );
   Fdelete ( newmfile );
   if ( Frename ( 0, file, newmfile ) == FAILED )
      form_alert ( 1, "[1][Error creating .BAK file!][OK]" );
   if ( ( mfile = fopen ( file, "bw" )) == 0 ) {
      form_alert ( 1, "[1][Disk Error!|Cannot save file.][CONTINUE]" );
      Frename ( 0, newmfile, file );
   }
   else {
      fwrite ( &num_trans, 2, 1, mfile );
      for ( x=0; x<num_trans; ++x )
         save_check ( x, mfile );
      if ( fclose ( mfile ) != 0 )
         form_alert ( 1, "[1][File close error!][OKAY]");
      else
         saved = TRUE;
      if ( ( mfile = fopen ( filename, "br+" )) == NULL )
         form_alert ( 1, 
                "[1][Error opening .MCK file!|Cannot update balance.][OK]" );
      else {
         fseek ( mfile, 91L, FROM_BEG );
         fwrite ( &balance, 4, 1, mfile );
         if ( fclose ( mfile ) != 0 )
            form_alert ( 1, "[1][File close error!][OK]");
      }
   }
}


open_new_month ()
{
   int mnth, x;

   for ( x=JAN; x<=MZERO; ++x )
      if ( cancdial_addr[x].ob_state == SELECTED )
         if ( x == MZERO )
            mnth = 0;
         else
            mnth = x-JAN+1;
   sprintf ( cancmnth, "%d", mnth );
   open_month ( acct_name, cancmnth );
}


open_month ( file, mnth )
char *file, *mnth;
{
   int x, len, button, do_it, trans_cnt, old_dep_cnt, old_chk_cnt;
   char a[20], new_mfile[64];

   saved = TRUE;
   old_dep_cnt = num_deps;
   old_chk_cnt = num_chks;
   num_chks = num_deps = 0;
   strcpy ( new_mfile, filename );
   strcpy ( &new_mfile[strlen(new_mfile)-4], mnth );
   strcpy ( &new_mfile[strlen(new_mfile)], ".DAT" );
   if ( ( mfile = fopen ( new_mfile, "br" )) == 0 )
      form_alert ( 1, "[1][Can't open the file][CONTINUE]" );
   else {
      do_it = TRUE;
      fread ( &trans_cnt, 2, 1, mfile );
      if ( trans_cnt == 0 ) {
         button = form_alert ( 1, "[2][The data file for|this month is \
empty.|Do you want to start|a new month?][YES|NO]" );
         if ( button == YES ) {
            num_trans = load_auto ();
         }
         else {
            do_it = FALSE;
            num_chks = old_chk_cnt;
            num_deps = old_dep_cnt;
         }
      }
      else
         num_trans = trans_cnt;
      if ( do_it ) {
         clear_window ();
         loaded = TRUE;
         if ( balance < 0 && balance > (-100) )
            sprintf ( bal_but, "$-%ld.%02ld", balance/100,labs(balance%100) );
         else
            sprintf ( bal_but, "$%ld.%02ld", balance/100,labs(balance%100) );
         strcpy ( monthfile, new_mfile );
         strcpy ( acct_name, file );
         month = atoi ( mnth );
         x = 0;
         while ( x < trans_cnt ) {
            read_check ( x, mfile );
            if ( strcmp ( checks[x].number, "9999" ) == MATCH )
               num_deps += 1;
            else
               num_chks += 1; 
            ++x;
         }
         if ( x > 0 ) {
            strcpy ( cur_chk_num, checks[x-1].number );
            curchknum = atoi ( cur_chk_num );
            if ( strcmp ( cur_chk_num, "9999" ) != MATCH ) {
               curchknum += 1;
               sprintf ( a, "%d", curchknum );
               len = strlen ( a );
               strcpy ( &cur_chk_num[4-len], a );
            }
         }
         cur_top = edit_top = 0;
         cur_count = num_trans;
         cur_chk_strc = checks;
         strcpy ( windname, acct_name );
         strcpy ( &windname[strlen(windname)], ": Edit mode" );
         wind_set ( w_h2, WF_NAME, windname, 0, 0 );
         full_draw = TRUE;
         if ( fclose ( mfile ) != 0 )
            form_alert ( 1, "[1][File close error!][OKAY]");
      }
   }
}


load_auto ()
{
   char autoname[64];
   FILE *autofile;
   int x, count;

   count = 0;
   strcpy ( autoname, filename );
   strcpy ( &autoname[strlen(autoname)-4], ".AUT" );
   if ( ( autofile = fopen ( autoname, "br" )) != NULL ) {
      fread ( &count, 2, 1, autofile );
      x = 0;
      while ( x < count ) {
         read_check ( x, autofile );
         if ( strcmp ( checks[x].number, "9999" ) == MATCH ) {
            num_deps += 1;
            balance += checks[x].amount;
         }
         else {
            num_chks += 1; 
            balance -= checks[x].amount;
         }
         ++x;
      }
      saved = FALSE;
      if ( fclose ( autofile ) == FAILED )
         form_alert ( 1, "[1][Error closing AUTO file!][CONTINUE]" );
   }
   return ( count );
}


save_check ( i, f )
int i;
FILE *f;
{
   fwrite ( checks[i].number, 1, 4, f );
   fwrite ( checks[i].payee, 1, 30, f );
   fwrite ( checks[i].memo, 1, 30, f );
   fwrite ( checks[i].date, 1, 8, f );
   fwrite ( &checks[i].amount, 4, 1, f );
   fwrite ( checks[i].cancel, 1, 1, f );
   fwrite ( "THIS SPACE FOR POSSIBLE FUTURE EXPANSION", 1, 40, f );
}


read_check ( i, f )
int i;
FILE f;
{
   fread ( checks[i].number, 1, 4, f );
   fread ( checks[i].payee, 1, 30, f );
   fread ( checks[i].memo, 1, 30, f );
   fread ( checks[i].date, 1, 8, f );
   fread ( &checks[i].amount, 4, 1, f );
   fread ( checks[i].cancel, 1, 1, f );
   fread ( future_use, 1, 40, f );
}


clear_window ()
{
   GRECT r;

   wind_get ( w_h2, WF_WORKXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h );
   draw_rec ( r, 2, 8, WHITE );
}


get_month ()
{
   int choice;
   int dial_x, dial_y, dial_w, dial_h;

   clear_cancdial ();
   form_center ( cancdial_addr, &dial_x, &dial_y, &dial_w, &dial_h );
   form_dial ( FMD_START, 0, 0, 10, 10, dial_x, dial_y, dial_w, dial_h );
   objc_draw ( cancdial_addr, 0, 8, dial_x, dial_y, dial_w, dial_h );

   choice = form_do ( cancdial_addr, 0 );
   cancdial_addr[choice].ob_state = SHADOWED;

   form_dial ( FMD_FINISH, 0, 0, 10, 10, dial_x, dial_y, dial_w, dial_h );
   return ( choice );
}

clear_cancdial ()
{
   int x;

   for ( x=JAN; x<=MZERO; cancdial_addr[x++].ob_state = NORMAL ) ;
   if ( month != -1 )
      if ( month == 0 )
         cancdial_addr[MZERO].ob_state = SELECTED;
      else
         cancdial_addr[month+JAN-1].ob_state = SELECTED;
}

get_new_date ()
{
   int choice, okay;
   int dial_x, dial_y, dial_w, dial_h;

   string = get_tedinfo_str ( newdate_addr, NWDATE );
   string[0] = 0;
   form_center ( newdate_addr, &dial_x, &dial_y, &dial_w, &dial_h );
   form_dial ( FMD_START, 0, 0, 10, 10, dial_x, dial_y, dial_w, dial_h );
   objc_draw ( newdate_addr, 0, 8, dial_x, dial_y, dial_w, dial_h );

   okay = FALSE;

   do {
      choice = form_do ( newdate_addr, NWDATE );
      newdate_addr[choice].ob_state = SHADOWED;

      switch ( choice ) {

         case DATEOK:
            okay = chk_date ();
            if ( !okay )
               objc_draw ( newdate_addr, 0, 8, 
                   dial_x, dial_y, dial_w, dial_h );
            else {
               strcpy ( cur_date, string );
               format_date ( date_but, cur_date );
               updte_buttons ();
            }
            break;

         case DATECANC:
            string = get_tedinfo_str ( newdate_addr, NWDATE );
            string[0] = '@';
      }
   }
   while ( okay == FALSE && choice != DATECANC );

   form_dial ( FMD_FINISH, 0, 0, 10, 10, dial_x, dial_y, dial_w, dial_h );
}


chk_date ()
{
   int mnth, day, year, okay;
   char m[3], d[3], y[3];

   string = get_tedinfo_str ( newdate_addr, NWDATE );
   if ( strlen ( string ) == 6 ) {
      strncpy ( m, string, 2 );
      m[2] = 0;
      strncpy ( d, &string[2], 2 );
      d[2] = 0;
      strncpy ( y, &string[4], 2 );
      mnth = atoi ( m );
      day = atoi ( d );
      year = atoi ( y );
      if ( mnth < 0 | mnth >12 | day < 1 | day > 31 
           | year < 0 | year > 99 ) {
         okay = FALSE;
      }
      else
         okay = TRUE;
   }
   else
      okay = FALSE;
   if ( !okay ) {
      form_alert ( 1, "[1][Not a valid date!][CONTINUE]" );
      string[0] = 0;
   }
   return ( okay );
}

updte_buttons ()
{
   if ( !full ) {
      set_buttons ();
      center_butstring ( bal_but, 35, 194 );
      center_butstring ( trans_but, 131, 194 );
      center_butstring ( check_but, 227, 194 );
      center_butstring ( dep_but, 323, 194 );
      center_butstring ( mnth_but, 419, 194 );
      center_butstring ( date_but, 515, 194 );
   }
}

