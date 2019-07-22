/*----------------------------------------------------------------------------*/
/* Program apiAll                                                             */
/*                                                                            */
/* This is a WIT application program that invokes each of WIT's API functions */
/* at least once. Its purpose is for testing WIT, especially WIT's API.       */
/*----------------------------------------------------------------------------*/

#include <cassert>
#include <string>
#include "wit.h"


bool isObjectImportant(std::string item) {
  bool retVal=false;
  if (item.find("12R6332")!=std::string::npos) 
    retVal=true;
  return retVal;
}

#
/*----------------------------------------------------------------------------*/
/* Main Program                                                               */
/*----------------------------------------------------------------------------*/

int main ()
{
  int   t;

  WitRun *  theWitRun;
  witNewRun(&theWitRun);
  witInitialize( theWitRun );

  witReadData(theWitRun,"../../data/06May18base422_eso2.06Jun05_version/implode0.wit");

  witAttr objItrState;
  char *  partName;
  char *  opnName;
  char *  demandName;
  int     bomEntIdx;
  int     subIdx;
  int     bopEntIdx;

  witGetObjItrState (theWitRun, & objItrState);

  assert (objItrState == WitINACTIVE);

  witResetObjItr (theWitRun);

  while (WitTRUE)
  {
    witAdvanceObjItr  (theWitRun);

    witGetObjItrState (theWitRun, & objItrState);

    if      (objItrState == WitAT_PART)
    {
      witGetObjItrPart (theWitRun, & partName);
      if( !isObjectImportant(partName) )
        witSetPartSelForDel(theWitRun,partName,WitTRUE);
      witFree (partName);
    }
    else if (objItrState == WitAT_DEMAND)
    {
      witGetObjItrDemand (theWitRun, & partName, & demandName);
      if( !isObjectImportant(partName) )
        witSetDemandSelForDel(theWitRun,partName,demandName,WitTRUE);
      witFree (demandName);
      witFree(partName);
    }
    else if (objItrState == WitAT_OPERATION)
    {
      witGetObjItrOperation (theWitRun, & opnName);
      if( !isObjectImportant(opnName) )
        witSetOperationSelForDel(theWitRun,opnName,WitTRUE);
      witFree (opnName);
    }
    else if (objItrState == WitAT_BOM_ENTRY)
    {
      witGetObjItrBomEntry (theWitRun, & opnName, & bomEntIdx);
      witFree (opnName);
    }
    else if (objItrState == WitAT_SUB_ENTRY)
    {
      witGetObjItrSubsBomEntry (theWitRun, & opnName, & bomEntIdx, & subIdx);
      witFree (opnName);
    }
    else if (objItrState == WitAT_BOP_ENTRY)
    {
      witGetObjItrBopEntry (theWitRun, & opnName, & bopEntIdx);
      witFree (opnName);
    }
    else if (objItrState == WitINACTIVE)
      break;
    else
      assert (WitFALSE);
  }
  witPurgeData(theWitRun);
  witSetMesgFileAccessMode(theWitRun,WitFALSE,"w");
  witWriteData(theWitRun,"../../data/06May18base422_eso2.06Jun05_version/implode0Pruned.wit");
  return 0;
}
