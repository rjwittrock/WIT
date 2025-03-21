/******************************************************************************
 *
 * Sample WIT API Program   
 *
 * This program determines the number of additional demand streams which can
 * be satisfied when the supply of critical parts is increased by a
 * given percentage.
 *
 * The steps are:
 *    - Read a WIT data file
 *    - Run the optimized implosion
 *    - Count number of demand streams which were not met
 *    - Get list of critical parts
 *    - Increase supply of critical parts
 *    - Rerun the optimized implosion
 *    - Count number of demand streams which were not met
 *    - Compare number of demands which were not met before and after
 *      the critical part supply was increased
 *
 ******************************************************************************/

#include <stdlib.h>
#include <wit.h>

#define OPT_IMPLODE

#define PERCENT_INCREASE 20.

static
int numUnmetDemands(
    WitRun * const theWitRun,              /* The WIT Environment            */
    const int nPeriods,                    /* Number of periods              */
    const int nParts,                      /* Number of parts                */
    char **  partList );                   /* List of part names             */

/******************************************************************************
 * Main Program                                                   
 * The name of the wit data file must be specified on command line.
 ******************************************************************************/

void main (int argc, char * argv[]) {

   int nPeriods;                         /* Number of periods in model   */
   int nParts;                           /* Number of parts in model     */
   char ** partList;                     /* List of parts in model       */
   char ** critPartList;                 /* List of critical parts       */
   int  *  critPartPeriod;               /* Period of part shortage      */
   int     critPartListLen;              /* Length of critPartList       */
   int unmetDemand1, unmetDemand2;       /* Count of unsatisfied demands */
#ifdef OPT_IMPLODE
   float objFuncValue1, boundsObj1,      /* Objective function values    */
         objFuncValue2, boundsObj2;
#endif
   char * solnFileName;                  /* Temporary solution file name */
   int i;                                /* Loop index                   */
   WitRun *  theWitRun;                  /* Current Wit Run              */

   /*
    * Make sure a wit.data file was specified on command line.
    */
   if ( argc < 2 ) {
      printf( "usage: %s wit_data_file_name\n",argv[0]);
      exit(1);
   }

   /*
    * Establish environment for WIT to run.
    */
   witNewRun(&theWitRun);

   /*
    * Send WIT messages to file wit.out, and write over an existing file.
    */
   witSetMesgFileAccessMode( theWitRun, WitTRUE, "w" );
   witSetMesgFileName( theWitRun, WitTRUE, "wit.out" );

   /* 
    * Initialize WIT
    */
   witInitialize( theWitRun );

   /*
    * Turn on 3.4 compatibility mode.
    */
   witSetWit34Compatible( theWitRun, WitTRUE );

   /* 
    * Read WIT data file specified on command line.
    */
   witReadData( theWitRun, argv[1] );

   /*
    * Set WIT to compute critical part list
    */
   witSetComputeCriticalList( theWitRun, WitTRUE );

   /*
    * Invoke optimized implosion.
    */
#ifdef OPT_IMPLODE
   witOptImplode( theWitRun );
#else
   witHeurImplode( theWitRun );
#endif

   /*
    * Get number of periods and list of part names.
    */
   witGetNPeriods( theWitRun, &nPeriods );
   witGetParts( theWitRun, &nParts, &partList );

   /*
    * Count number of unmet demand streams and get objective
    * function values
    */
   unmetDemand1 = numUnmetDemands( theWitRun, nPeriods, nParts, partList );
#ifdef OPT_IMPLODE
   witGetObjValues( theWitRun, &objFuncValue1, &boundsObj1 );
#endif

   /*
    * Create a list of critical parts for each period.
    */
   witGetCriticalList( theWitRun, 
      &critPartListLen, &critPartList, &critPartPeriod );

   /*
    * Save the implosion solution to use later as a starting point for
    * second implosion.  This is done to improve the performance of the
    * second implosion.
    */
   solnFileName = tmpnam( NULL );
   witWriteProdSched( theWitRun, solnFileName, WitBSV );
   witSetMesgFileAccessMode( theWitRun, WitFALSE, "a" );
   witWriteShipSched( theWitRun, solnFileName, WitBSV );

   /*
    * Increase supply of critical parts
    */
   for ( i=0; i<critPartListLen; i++ ) {

      float * supply;

      /*
       * Get original supply of critical part.
       */
      witGetPartSupplyVol( theWitRun, critPartList[i], &supply );

      /*
       * Increase available supply
       */
      supply[ critPartPeriod[i] ] *= (1. + PERCENT_INCREASE/100.);
      witSetPartSupplyVol( theWitRun, critPartList[i], supply ); 

      /*
       * free supply storage.
       */
      free( supply );

   } /* for ( i=0; i<critPartListLen[t]; i++ ) */

   /*
    * Re-implode with increased supply.
    */
#ifdef OPT_IMPLODE
   witOptImplode( theWitRun );
#else
   witHeurImplode( theWitRun );
#endif

   /*
    * Count number of unmet demand streams
    */
   unmetDemand2 = numUnmetDemands( theWitRun, nPeriods, nParts, partList );
#ifdef OPT_IMPLODE
   witGetObjValues( theWitRun, &objFuncValue2, &boundsObj2 );
#endif

   /* 
    * Write result.
    */
   printf( "Before increasing supply:\n" );
   printf( "  Number of unmet demand streams : %d\n", unmetDemand1  );
#ifdef OPT_IMPLODE
   printf( "  Objective function value       : %f\n", objFuncValue1 );
#endif
   printf( "After increasing supply:\n" );
   printf( "  Number of unmet demand streams : %d\n", unmetDemand2  );
#ifdef OPT_IMPLODE
   printf( "  Objective function value       : %f\n", objFuncValue2 );
#endif

   /*
    * Free partList storage
    */
   for ( i=0; i<nParts; i++ ) free( partList[i] );
   free( partList );

   /* 
    * Free critical part list storage
    */
   for ( i=0; i<critPartListLen; i++ ) free( critPartList[i] );
   free( critPartList );
   free( critPartPeriod );
   
   /* Delete solution file */
   remove( solnFileName );

   /* Free storage associated with the WIT environment */
   witDeleteRun( theWitRun );

   exit (0);

}  /* main */

/******************************************************************************
 *
 * Count number of demand streams which are not satisfied.
 *
 *****************************************************************************/
 
int numUnmetDemands(
    WitRun * const theWitRun,          /* WIT environment                */
    const int nPeriods,                /* Number of periods              */
    const int nParts,                  /* Number of parts                */
    char ** partList )                 /* List of part names             */
{

   int i;                                     /* Loop index               */
   int unmetDemands=0;                        /* Count of unmet demands   */

   /*
    * Loop once for each part.
    */
   for ( i=0; i<nParts; i++ ) {

      int nDemands;                         /* Number of demands on part */
      char ** demandList;                   /* List of demands on part   */
      int j,t;                              /* Loop indices              */

      /*
       * Get list of demands defined for part.
       */
      witGetPartDemands( theWitRun, partList[i], &nDemands, &demandList );

      /*
       * Loop once for each demand
       */
      for ( j=0; j<nDemands; j++ ) {

         float * shipq;
         float * demandq;
         witBoolean met;
      
         /* 
          * Get demand and shipment quantity for part 
          */
         witGetDemandDemandVol( theWitRun, 
            partList[i], demandList[j], &demandq );
         witGetDemandShipVol( theWitRun, partList[i], demandList[j], &shipq );

         /*
          * Check to see if demand for any demand stream could not
          * be met. Increment count if demand can not be met.
          */
         met = WitTRUE;
         for ( t=0; t<nPeriods; t++ ) 
            if ( demandq[t] > shipq[t] ) { 
               met = WitFALSE;
               break;
            }
         if ( !met ) unmetDemands++;

         /*
          * Free demand and shipment quantity storage.
          */
         free( demandq );
         free( shipq );

      } /* for ( j=0; j<nDemands; j++ ) */

      /*
       * Free demandList storage
       */
      for ( j=0; j<nDemands; j++ ) free( demandList[j] );
      free( demandList );

   }  /* for ( i=0; i<nParts; i++ ) */

   return unmetDemands;
}
