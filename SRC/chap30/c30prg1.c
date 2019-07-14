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

handle_keys ()
{
   int button;

   if ( loaded && !search && !canceling )
      switch ( key ) {

         case CNTL_A:
            menu_tnormal ( menu_addr, CHECKS, FALSE );
            do_auto ();
            menu_tnormal ( menu_addr, CHECKS, TRUE );
            break;

         case CNTL_E:
            menu_tnormal ( menu_addr, CHECKS, FALSE );
            do_enter ();
            menu_tnormal ( menu_addr, CHECKS, TRUE );
            break;

         case CNTL_M:
            menu_tnormal ( menu_addr, FILEBAR, FALSE );
            do_new_mnth ();
            menu_tnormal ( menu_addr, FILEBAR, TRUE );
            break;

         case CNTL_P:
            menu_tnormal ( menu_addr, CHECKS, FALSE );
            do_check_canc ();
            menu_tnormal ( menu_addr, CHECKS, TRUE );
            break;

         case CNTL_R:
            menu_tnormal ( menu_addr, CHECKS, FALSE );
            do_reconcil ();
            menu_tnormal ( menu_addr, CHECKS, TRUE );
            break;
      }

   if ( !loaded )
      switch ( key ) {

         case CNTL_N:
            menu_tnormal ( menu_addr, FILEBAR, FALSE );
            do_newacct ();
            menu_tnormal ( menu_addr, FILEBAR, TRUE );
            break;

         case CNTL_O:
            menu_tnormal ( menu_addr, FILEBAR, FALSE );
            button = get_acct ();
            if ( button )
               open_acct ( filename );
            menu_tnormal ( menu_addr, FILEBAR, TRUE );
            break;

         case CNTL_Y:
            menu_tnormal ( menu_addr, UTILITY, FALSE );
            do_new_year ();
            menu_tnormal ( menu_addr, UTILITY, TRUE );
            break;

         case CNTL_I:
            menu_tnormal ( menu_addr, UTILITY, FALSE );
            do_import ();
            menu_tnormal ( menu_addr, UTILITY, TRUE );
            break;
      }

   switch ( key ) {

      case CNTL_Q:
         menu_tnormal ( menu_addr, FILEBAR, FALSE );
         do_quit ();
         menu_tnormal ( menu_addr, FILEBAR, TRUE );
         break;

      case CNTL_S:
        if ( loaded && !canceling ) {
           menu_tnormal ( menu_addr, CHECKS, FALSE );
           do_search ();
           menu_tnormal ( menu_addr, CHECKS, TRUE );
        }
        break;

      case CNTL_C:
         if ( loaded ) {
            menu_tnormal ( menu_addr, FILEBAR, FALSE );
            do_wind_close ();
            menu_tnormal ( menu_addr, FILEBAR, TRUE );
         }
         break;

      case CNTL_D:
         menu_tnormal ( menu_addr, UTILITY, FALSE );
         get_new_date ();
         menu_tnormal ( menu_addr, UTILITY, TRUE );
         break;

      case CNTL_W:
         if ( loaded ) {
            menu_tnormal ( menu_addr, PRINT, FALSE );
            print_wind ();
            menu_tnormal ( menu_addr, PRINT, TRUE );
         }
         break;

      case CNTL_G:
         if ( loaded ) {
            menu_tnormal ( menu_addr, PRINT, FALSE );
            print_reg ();
            menu_tnormal ( menu_addr, PRINT, TRUE );
         }
         break;

   }
}


do_newacct ()
{
   int choice, okay;
   int dial_x, dial_y, dial_w, dial_h;

   clear_newacct ();
   form_center ( newacct_addr, &dial_x, &dial_y, &dial_w, &dial_h );
   form_dial ( FMD_START, 0, 0, 10, 10, dial_x, dial_y, dial_w, dial_h );
   objc_draw ( newacct_addr, 0, 8, dial_x, dial_y, dial_w, dial_h );

   do {
      choice = form_do ( newacct_addr, NEWNAME );
      newacct_addr[choice].ob_state = SHADOWED;

      switch ( choice ) {

         case NEWOK:
            okay = check_newacct ();
            if ( !okay )
               objc_draw ( newacct_addr, 0, 8, 
                           dial_x, dial_y, dial_w, dial_h );
            else
               newacct_file ();
            break;
            
         case NEWCANCL:
            clear_newacct ();
      }
   }
   while ( okay == FALSE && choice != NEWCANCL );

   form_dial ( FMD_FINISH, 0, 0, 10, 10, dial_x, dial_y, dial_w, dial_h );
}


check_newacct ()
{
   int x, okay;

   okay = TRUE;
   for ( x=NEWNAME; x<=NEWBALNC; ++x ) {
      string = get_tedinfo_str ( newacct_addr, x );
      if ( string[0] == '@' )
         okay = FALSE;
   }
   if ( !okay )
      form_alert(1,"[1][You must complete|the form to start|a new account!]\
[OK]");
   return ( okay );
}
   
      
newacct_file ()
{
   int choice, okay, x;
   int dial_x, dial_y, dial_w, dial_h;

   string = get_tedinfo_str ( newfile_addr, FILENAME );
   string[0] = 0;
   for ( x=0; x<64; filename[x++]=0 );
   newfile_addr[NEWOK].ob_state = SHADOWED;
   form_center ( newfile_addr, &dial_x, &dial_y, &dial_w, &dial_h );
   form_dial ( FMD_START, 0, 0, 10, 10, dial_x, dial_y, dial_w, dial_h );
   objc_draw ( newfile_addr, 0, 8, dial_x, dial_y, dial_w, dial_h );

   do {
      choice = form_do ( newfile_addr, FILENAME );
      newfile_addr[choice].ob_state = SHADOWED;

      switch ( choice ) {

         case FILEOK:
            okay = check_file ();
            if ( !okay )
               objc_draw ( newfile_addr, 0, 8, 
                           dial_x, dial_y, dial_w, dial_h );
            else {
               string = get_tedinfo_str ( newfile_addr, FILENAME );
               strcpy ( acct_name, string );
               filename[0] = Dgetdrv () + 'a';
               filename[1] = ':';
               Dgetpath ( &filename[2], DFLT_DRV );
               filename[strlen(filename)] = '\\';
               strcpy ( &filename[strlen(filename)], string );
               strcpy ( &filename[strlen(filename)], ".MCK" );
               acctfile = fopen ( filename, "bw" );
               if ( acctfile != 0 ) {
                  write_new_info ();
                  open_acct ( filename );
               }
            }
            break;

         case FILECANC:
            string = get_tedinfo_str ( newfile_addr, FILENAME );
            string[0] = 0;
      }
   }
   while ( !okay && choice != FILECANC );

   form_dial ( FMD_FINISH, 0, 0, 10, 10, dial_x, dial_y, dial_w, dial_h );
}

clear_newacct ()
{
   int x;

   for ( x=NEWNAME; x<=NEWBALNC; ++x ) {
      string = get_tedinfo_str ( newacct_addr, x );
      string[0] = '@';
   }
   newacct_addr[NEWCANCL].ob_state = SHADOWED;
}


char *get_tedinfo_str ( tree, object )
OBJECT *tree;
int object;
{
   TEDINFO *ob_tedinfo;

   ob_tedinfo = (TEDINFO *) tree[object].ob_spec;
   return ( ob_tedinfo->te_ptext );
}


check_file ()
{
   int okay;

   okay = TRUE;
   string = get_tedinfo_str ( newfile_addr, FILENAME );
   if ( strlen ( string ) == 0 ) {
      form_alert(1,"[1][Invalid filename!][OK]");
      okay = FALSE;
   }
   return ( okay );
}

no_decimal ( s )
char *s;
{
   int x, i, d, len;
   char s2[20], s3[20];

   strcpy ( s2, s );
   len = strlen ( s2 );
   i = 0;
   d = FALSE;
   for ( x=0; x<len; ++x )
      if ( d && s2[x] != ' ' )
         ++i;
      else
         if ( s2[x] == ' ' )
            d = TRUE;

   if ( i == 0 && len < 6 )
      strcpy ( &s2[len], "00" );
   else
      if ( i == 1 || len == 6 )
         strcpy ( &s2[len], "0" );

   i = 0;
   for ( x=0; x<strlen(s2); ++x )
      if ( s2[x] != ' ' & s2[x] != '.' )
         s3[i++] = s2[x];
   s3[i] = 0;
   strcpy ( s, s3 );
}


long str_to_long ( s )
char *s;
{
   int x, len, factor;
   long num;

   num = 0;
   len = strlen ( s );
   factor = len - 1;
   for ( x=0; x<len; ++x )
      num += (long) ( s[x] - '0' ) * pwrs[factor--];
   return ( num );
}


write_new_info ()
{
   int len, x;
   char s[10], tmpfile[64];
   FILE *f;

   string = get_tedinfo_str ( newacct_addr, NEWNAME );
   fwrite ( string, 1, 26, acctfile );
   string = get_tedinfo_str ( newacct_addr, NEWADDR );
   fwrite ( string, 1, 26, acctfile );
   string = get_tedinfo_str ( newacct_addr, NEWCITY );
   fwrite ( string, 1, 26, acctfile );
   string = get_tedinfo_str ( newacct_addr, NEWSTATE );
   fwrite ( string, 1, 3, acctfile );
   string = get_tedinfo_str ( newacct_addr, NEWZIP );
   fwrite ( string, 1, 10, acctfile );
   string = get_tedinfo_str ( newacct_addr, NEWBALNC );
   no_decimal ( string );
   balance = str_to_long ( string );
   fwrite ( &balance, 1, 4, acctfile );
   if ( fclose ( acctfile ) != 0 )
      form_alert ( 1, "[1][File close error!][OKAY]");
   for ( x=0; x<13; ++x ) {
      sprintf ( s, "%d", x );
      strcpy ( &s[strlen(s)], ".dat" );
      ob_tedinfo = (TEDINFO *) newfile_addr[FILENAME].ob_spec;
      tmpfile[0] = Dgetdrv () + 'a';
      strcpy ( &tmpfile[1], ":" );
      Dgetpath ( &tmpfile[strlen(tmpfile)], DFLT_DRV );
      strcpy ( &tmpfile[strlen(tmpfile)], "\\" );
      strcpy ( &tmpfile[strlen(tmpfile)], ob_tedinfo->te_ptext );
      strcpy ( &tmpfile[strlen(tmpfile)], s );
      if ( ( f = fopen ( tmpfile, "bw" ) ) == NULL )
         form_alert ( 1, "[1][Error creating file!][OK]" );
      else
         fwrite ( &zero, 2, 1, f );
      if ( fclose ( f ) != 0 )
            form_alert ( 1, "[1][File close error!][OK]");
   }
}
