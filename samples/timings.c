/******************************************************************************
 *
 * Time WIT heur and opt implode
 *
 ******************************************************************************/

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <wit.h>

#ifdef SUN4
#include <sys/time.h>
#include <sys/resource.h>
#endif


#define RS6K

/* RS6K mclock function prototype */
#ifdef RS6K
#ifdef __cplusplus
extern "C"
  {
#endif
   long int mclock(void);
#ifdef __cplusplus
  }
#endif
#endif

/* SUN process time function */
#ifdef SUN4
double sunProcessTime () 
   {
   struct rusage myRusage;
   int who = RUSAGE_SELF;
   int rc  = -1;

   rc = getrusage (who, &myRusage);

   if (rc != 0)
      {
      WitRun::current->message (genericProgramErrorFmsg,
         "WitTimingSection::sunProcessTime");  // May not return.
      return (0.0);
      }
   else
      return (myRusage.ru_utime.tv_sec + myRusage.ru_utime.tv_usec/1e6);
   }

#endif


/* Function used in this application to obtain CPU time in seconds */
double cpuTime () {
#ifdef RS6K
    return mclock () * .01;
#else
#ifdef SUN4
    return sunProcessTime ();

#else
    return ((double) clock ()) / (double) CLOCKS_PER_SEC;
#endif
#endif
}

/******************************************************************************
 * Main Program                                                   
 * The name of the wit data file must be specified on command line.
 ******************************************************************************/

void main (int argc, char * argv[]) {

   double startBaseTimeHeur, readBaseTimeHeur, implodeBaseTimeHeur,
          endBaseTimeHeur;
   double startBaseTimeOpt, readBaseTimeOpt, implodeBaseTimeOpt,
          endBaseTimeOpt,
          implode2StartBaseTimeOpt, implode2EndBaseTimeOpt;
   float objValueHeur, boundsValueHeur;
   float objValueOpt,  boundsValueOpt; 
   WitRun * theWitRun;

   /*
    * Make sure a wit.data file was specified on command line.
    */
   if ( argc < 2 ) {
      printf( "usage: %s wit_data_file_name\n",argv[0]);
      exit(1);
   }
   printf("\n\n******\nWit data file name: %s \n", argv[1] );

   /* get data in cache */
   witNewRun(&theWitRun);
   witSetMesgTimesPrint( theWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, 0 );
   witInitialize( theWitRun );
   witReadData( theWitRun, argv[1] );
   
   /* Get problem size metrics */
   {
     int i,j;
     int nParts, nMat=0, nCap=0, nDemands=0;
     char ** partList;

     witGetParts( theWitRun, &nParts, &partList );
     for ( i=0; i<nParts; i++ ) {
       witAttr partCat;
       witGetPartCategory( theWitRun, partList[i], &partCat );
       if( partCat == WitMATERIAL ) nMat++;
       else                         nCap++;
     }
     printf(" nParts=%d\n",nMat );
     printf(" nCapacities=%d\n",nCap );

     for ( i=0; i<nParts; i++ ) {
       int nPartDemands;
       char ** demandList;
       witGetPartDemands( theWitRun, partList[i], &nPartDemands, &demandList );
       nDemands += nPartDemands;
       for ( j=0; j<nPartDemands; j++ ) {
         free( demandList[i] );
       }
       free( demandList );
     }
     printf(" nDemands=%d\n",nDemands );
     
     
     /* Free partlist */
     for ( i=0; i<nParts; i++ ) {
       free( partList[i] );
     }
     free( partList );
     
   }

   /* operations, BOM/BOP arcs, subst. arcs, periods */
   
   {
     char ** operationList;
     int listLen, i,j;
     int nBom, nBop, nSubs, nTotalBom=0, nTotalBop=0, nTotalSubs = 0;
     int nPeriods;

     witGetOperations (theWitRun, &listLen, &operationList);
     printf(" nOperations =%d\n",listLen );
     for (i=0; i < listLen; i++) {
       witGetOperationNBomEntries (theWitRun, operationList[i], &nBom);
       nTotalBom += nBom;
       witGetOperationNBopEntries (theWitRun, operationList[i], &nBop);
       nTotalBop += nBop;

       for (j=0; j<nBom; j++) {
         witGetBomEntryNSubsBomEntries (theWitRun, operationList[i], j, &nSubs);
         nTotalSubs += nSubs;
       }
     }

     printf(" nBom Arcs =%d\n",nTotalBom );
     printf(" nBop Arcs =%d\n",nTotalBop );
     printf(" nSubsArcs =%d\n",nTotalSubs );
     for (i=0; i<listLen; i++)
       free (operationList[i]);
     free (operationList);

     witGetNPeriods (theWitRun, &nPeriods);
     printf(" nPeriods =%d\n",nPeriods );
   }

   /*   witSetMesgTimesPrint (theWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, UCHAR_MAX);
   witSetMesgTimesPrint (theWitRun, WitTRUE, 167, UCHAR_MAX);
   witSetMesgTimesPrint (theWitRun, WitTRUE, 180, 10); 
   witDisplayData (theWitRun, WitSTDOUT);
   witSetMesgTimesPrint (theWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, 0); */
   
   /* 
    * Run heuristic implosion and evaluate objective function value
    * for comparison with optImplode obj. funct value.
    */
   witSetAutoPriority( theWitRun, WitTRUE );
   witHeurImplode( theWitRun );
   witEvalObjectives( theWitRun );
   witGetObjValues( theWitRun, &objValueHeur, &boundsValueHeur );

   witDeleteRun(theWitRun);

   /*
    * Time heuristic
    */
   startBaseTimeHeur = cpuTime();
   witNewRun(&theWitRun);
   witSetMesgTimesPrint( theWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, 0 );

   witInitialize( theWitRun );
   witReadData( theWitRun, argv[1] );
   readBaseTimeHeur = cpuTime(); 
   printf(" Heuristic read time %f\n", readBaseTimeHeur-startBaseTimeHeur);

   witSetAutoPriority( theWitRun, WitTRUE );
   witHeurImplode( theWitRun );
   implodeBaseTimeHeur = cpuTime(); 
   printf(" Heuristic implode time %f\n", implodeBaseTimeHeur-readBaseTimeHeur);

   witWriteExecSched( theWitRun, "/dev/null", WitBSV );
   witWriteShipSched( theWitRun, "/dev/null", WitBSV );

   witDeleteRun( theWitRun );

   endBaseTimeHeur = cpuTime(); 
   printf(" Heuristic total time %f\n", endBaseTimeHeur-startBaseTimeHeur);

   /*
    * Time implosion
    */
   startBaseTimeOpt = cpuTime();
   witNewRun(&theWitRun);
   witSetMesgTimesPrint( theWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, 0 );

   witInitialize( theWitRun );
   witReadData( theWitRun, argv[1] );
   readBaseTimeOpt = cpuTime(); 
   printf("\n");
   printf(" Optimize read time %f\n", readBaseTimeOpt-startBaseTimeOpt);

   witSetAccAfterOptImp( theWitRun, WitTRUE );
   witOptImplode( theWitRun );
   implodeBaseTimeOpt = cpuTime(); 
   printf(" Optimize implode time %f\n", implodeBaseTimeOpt-readBaseTimeOpt);

   witWriteExecSched( theWitRun, "/dev/null", WitBSV );
   witWriteShipSched( theWitRun, "/dev/null", WitBSV );

   /* Get Optimal objective function value */
   witGetObjValues( theWitRun, &objValueOpt, &boundsValueOpt );

   /* change a partSupply and re-implode */
   {
   int nParts,i;
   char ** partList;    
   float * supplyVol;
   witGetParts( theWitRun, &nParts, &partList );
   witGetPartSupplyVol( theWitRun, partList[0], &supplyVol );
   supplyVol[0] = 2. * (supplyVol[0] + 1.);
   witSetPartSupplyVol( theWitRun, partList[0], supplyVol );
   free(supplyVol);
   for ( i=0; i<nParts; i++ ) free( partList[i] );
   free( partList );
   }

   implode2StartBaseTimeOpt = cpuTime(); 
   witOptImplode( theWitRun );
   implode2EndBaseTimeOpt = cpuTime(); 
   printf(" Optimize accelerated implode time %f\n", 
            implode2EndBaseTimeOpt-implode2StartBaseTimeOpt);
   endBaseTimeOpt = cpuTime(); 
   printf(" Optimize total time %f\n", endBaseTimeOpt-startBaseTimeOpt);

   printf("\n");
   printf(" Heuristic optimal value: %f\n", objValueHeur );
   printf(" LP        optimal value: %f\n", objValueOpt );
   printf(" Heuristic deviation from optimal: %f%%\n", 
          100.*fabs(objValueHeur-objValueOpt)/objValueOpt );

   witDeleteRun( theWitRun );
   
   exit (0);

}  /* main */
