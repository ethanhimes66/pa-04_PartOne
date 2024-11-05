/*----------------------------------------------------------------------------
pa-04_PartOne:  Intro to Enhanced Needham-Schroeder Key-Exchange with TWO-way Authentication

FILE:   kdc.c

Written By: 
     1- James Handlon
     2- Ethan Himes
Submitted on: 
----------------------------------------------------------------------------*/

#include <linux/random.h>
#include <time.h>
#include <stdlib.h>

#include "../myCrypto.h"

//*************************************
// The Main Loop
//*************************************
int main ( int argc , char * argv[] )
{
    int       fd_A2K , fd_K2A   ;
    FILE     *log ;
    
    char *developerName = "Code by Handlon, Himes" ;

    fprintf( stdout , "Starting the KDC's   %s\n"  , developerName ) ;

    if( argc < 3 )
    {
        printf("\nMissing command-line file descriptors: %s <readFrom Amal> "
               "<sendTo Amal>\n\n", argv[0]) ;
        exit(-1) ;
    }

    fd_A2K    = atoi ( argv[0] ) ;  // Read from Amal   File Descriptor
    fd_K2A    = atoi ( argv[1] ) ;  // Send to   Amal   File Descriptor

    log = fopen("kdc/logKDC.txt" , "w" );
    if( ! log )
    {
        fprintf( stderr , "The KDC's   %s. Could not create log file\n"  , developerName ) ;
        exit(-1) ;
    }

    BANNER( log ) ;
    fprintf( log , "Starting the KDC\n"  ) ;
    BANNER( log ) ;

    fprintf( log , "\n<readFrom Amal> FD=%d , <sendTo Amal> FD=%d\n\n" , fd_A2K , fd_K2A );

    // Get Amal's master keys with the KDC and dump it to the log
    myKey_t  Ka ;    // Amal's master key with the KDC

    // Use  getKeyFromFile( "kdc/amalKey.bin" , ....  )
	// On failure, print "\nCould not get Amal's Masker key & IV.\n" to both  stderr and the Log file
	// and exit(-1)
	// On success, print "Amal has this Master Ka { key , IV }\n" to the Log file
	// BIO_dump_fp the Key IV indented 4 spaces to the righ
    if (getKeyFromFile("kdc/amalKey.bin", &Ka) == -1)
    {
        fprintf( log , "\nCould not get Amal's Masker key & IV.\n");
        fprintf( stderr , "\nCould not get Amal's Masker key & IV.\n");
        exit(-1);
    } else {
        fprintf( log , "Amal has this Master Ka { key , IV }\n");
        BIO_dump_indent_fp( log , Ka.key, sizeof(Ka.key), 4);
    }
    fprintf( log , "\n" );
	// BIO_dump_fp the IV indented 4 spaces to the righ
    BIO_dump_indent_fp( log , Ka.iv, sizeof(Ka.iv), 4);

    fflush( log ) ;
    
    // Get Basim's master keys with the KDC
    myKey_t   Kb ;    // Basim's master key with the KDC    

    // Use  getKeyFromFile( "kdc/basimKey.bin" , .... ) )
	// On failure, print "\nCould not get Basim's Masker key & IV.\n" to both  stderr and the Log file
	// and exit(-1) 
	// On success, print "Basim has this Master Ka { key , IV }\n" to the Log file
	// BIO_dump_fp the Key IV indented 4 spaces to the righ
    if (getKeyFromFile("kdc/basimKey.bin", &Kb) == -1)
    {
        fprintf( log , "\nCould not get Basim's Masker key & IV.\n");
        fprintf( stderr , "\nCould not get Basim's Masker key & IV.\n");
        exit(-1);
    } else {
        fprintf( log , "Basim has this Master Ka { key , IV }\n");
        BIO_dump_indent_fp( log , Kb.key, sizeof(Kb.key), 4);
    }
    fprintf( log , "\n" );
	// BIO_dump_fp the IV indented 4 spaces to the right
    BIO_dump_indent_fp( log , Kb.iv, INITVECTOR_LEN, 4);
    fflush( log ) ;

    //*************************************
    // Receive  & Display   Message 1
    //*************************************
    BANNER( log ) ;
    fprintf( log , "         MSG1 Receive\n");
    BANNER( log ) ;

    char *IDa , *IDb ;
    Nonce_t  Na ;
    
    // Get MSG1 from Amal
    MSG1_receive( log , fd_A2K , &IDa , &IDb , Na ) ;

    fprintf( log , "\nKDC received message 1 from Amal with:\n"
                   "    IDa = '%s'\n"
                   "    IDb = '%s'\n" , IDa , IDb ) ;

    fprintf( log , "    Na ( %lu Bytes ) is:\n" , NONCELEN ) ;
     // BIO_dump_fp the nonce Na
    BIO_dump_fp( log , Na, NONCELEN);

    fflush( log ) ;


    // PA-04 Part Two
    // will go here


    //*************************************   
    // Final Clean-Up
    //*************************************
    
    fprintf( log , "\nThe KDC has terminated normally. Goodbye\n" ) ;
    fclose( log ) ;  
    return 0 ;
}
