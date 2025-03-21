
// RW2STL - inserted:
#include <string>
#include <iostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
// file: aEngMgr.C
// author:  tom

#include <assert.h>
#include <mcl/src/mcl.h>
#include <scenario/src/machdep.h>
#include <sce/src/scenSceP.h> 
#include <scenario/src/calendar.h>
#include <scenario/src/timeVecs.h>



#include <sce/src/priorMgr.h>
#include <sce/src/dmApData.h>
#include <sce/src/appData.h>
#include <sce/src/ipSce.h>
#include <sce/src/demLet.h>
#include <sce/src/prRule.h>
#include <sce/src/mPlantH.h>
#include <sce/src/fFileMgr.h>
#include <sce/src/sceParam.h>
#include <sce/src/scenSce.h>


// #define PRMNGR_INFO_MODE
// #define PRMNGR_DEBUG_MODE






/********************************************************************************
// high level set Priorities
********************************************************************************/
void
LgFrScePriorityMgr::setPriorities(const std::string & fileName)
{

  LgFrSceSetOfParameters & setOfParameters =  sceScenSP_->setOfParameters();    
  

  std::cout << "Executing the SCE Priority Manager ... \n" ;



#ifdef PRMNGR_DEBUG_MODE
    std::cout << "     ... PriorMgr: reading priority rules file" << std::endl;
#endif
  // read the rules line by line !!!!! implement !!!!!!!!
  // This populates:
  //          nRules_, ruleVec_, and rulePtrVec_
  this->processPriorityManagerRulesFile(fileName);


  // sort the rules ... based on prRule->priority()
  qsort(rulePtrVec_, 
        nRules_, 
	sizeof(LgFrScePriorityRule *), 
        this->compareVoidPrRuleFunc());

  // loop thru demands creating one demLet for each demand/period pair with postive demandVol
  // this step populates each demLet's prRuleValHitVec[]
  this->initializeDemLets();


  // sort the demLets ... compare based on demLet->ruleSignature()
  std::cout << "     ... PriorMgr: sorting ("  << listMax_ << ") demLets" << std::endl;
  qsort(demLetList_, 
        listMax_, 
	sizeof(LgFrSceDemLet*), 
        this->compareVoidDemLetFunc());
  std::cout << "     ... PriorMgr: done sorting demLets" << std::endl;

  this->assignWitPriorityFromDemLets();

#ifdef PRMNGR_DEBUG_MODE
  std::cout << "     ... PriorMgr (DEBUG): printing sorted demLets" << std::endl;
  this->printDemLets(); 
#endif 

  // Smooth out the priorities so that they are non-increasing.   
  // basically this means there should be no "holes"
  // use a Wit Iterator
  this->testForNonIncreasingPriorities();


  // DONE !!
  // now cleanup out the memory ... or delete this altogether in ipSce.C


}

//


int 
comparePrRule(const void *aa, const void *bb)
{

  LgFrScePriorityRule ** aaa = (LgFrScePriorityRule **) aa;
  LgFrScePriorityRule ** bbb = (LgFrScePriorityRule **) bb;

  LgFrScePriorityRule * b =  *bbb;
  LgFrScePriorityRule * a =  *aaa;

  return b->priority() - a->priority();
}



int
compareDemLet(const void * aa, const void * bb)
{     

  LgFrSceDemLet ** aaa = (LgFrSceDemLet **) aa;
  LgFrSceDemLet ** bbb = (LgFrSceDemLet **) bb;

  LgFrSceDemLet * b =  *bbb;
  LgFrSceDemLet * a =  *aaa;

  std::vector<int> * aRuleSig = a->ruleSignature();
  std::vector<int>::iterator aIt;
  std::vector<int> * bRuleSig = b->ruleSignature();
  std::vector<int>::iterator bIt;

  // compare their ruleSignatures
  aIt = aRuleSig->begin();
  bIt = bRuleSig->begin();
  while ( (aIt < aRuleSig->end()) && (bIt < bRuleSig->end()) )  {
    // if the current ruleSig pointers are NOT equal, then return the difference
    if (  *aIt != *bIt )  
      return *bIt - *aIt;
    aIt++;
    bIt++;
  }

  // 
  if ((aIt < aRuleSig->end()) && (bIt >=  bRuleSig->end()) )  
    return 1;
  if ((aIt >= aRuleSig->end()) && (bIt <  bRuleSig->end()) )  
    return -1;

  // if you get here, then we compare demandPd
  return a->demandPd() - b->demandPd();
}


// ----------------------------------------------------------------
void
LgFrScePriorityMgr::assignWitPriorityFromDemLets()
{


#ifdef PRMNGR_INFO_MODE
  std::cout << "     ... PriorMgr: assigning WIT  priorities" << std::endl;
#endif
  // now calculate the wit priority values and assign within demLet
  // loop over sorted demLetList
  int witPriorityVal = 0;
  int a=0;
  LgFrSceDemLet * theLastDemLet = demLetList_[a];
  for (a=1; a<listMax_; a++) {
    LgFrSceDemLet * myDemLet = demLetList_[a];
    // compare the bitVec of this demLet with the last to see IF THERE IS A DROP IN PRIORITY
    int compareVal = compareDemLet((void *) &myDemLet, (void *) &theLastDemLet);
    if (0 != compareVal)  {
	witPriorityVal++;
    }
    myDemLet->priority(witPriorityVal);
    theLastDemLet = myDemLet;
  }

  

  // Set the priorities in Wit.
  for (a=0; a<listMax_; a++) {
    LgFrSceDemLet & myDemLet = *(demLetList_[a]);

    std::string witPartName = myDemLet.witPartName();
    std::string witDemandName = myDemLet.witDemandName();
    /*
    // TINP
    if (0 == (myDemLet.demandLevel()).compare("TINP"))   {
      mpHelper_->compressedDemandNameWitPdf(witDemandName, 
					    myDemLet.sourceLoc(),
					    myDemLet.customerLoc(),
					    myDemLet.demandClass(),
					    myDemLet.demandLevel(),
					    myDemLet.partClass());
//      witPartName = mpHelper_->geoPlannerDemandPartName(myDemLet.partName(), witDemandName);
      witPartName = myDemLet.witPartname();
    }

    // INDP
    else    {
      witPartName = myDemLet.witPartname();
//      witPartName = mpHelper_->pdfPartname(myDemLet.partName(), myDemLet.sourceLoc());
      mpHelper_->compressedDemandName(fullWitDemandName, 
				      myDemLet.customerLoc(),
				      myDemLet.demandClass(),
				      myDemLet.demandLevel(),
				      myDemLet.partClass());
    }
      
    */

    int * priority;
    witGetDemandPriority(theWitRun_, witPartName.c_str(), witDemandName.c_str(), &priority);
    
    priority[myDemLet.demandPd()] = myDemLet.priority();
    witSetDemandPriority(theWitRun_, witPartName.c_str(), witDemandName.c_str(), priority);
    witFree(priority);

    // need to reverse the order of  priority values between SCE and WIT
    int scePriorityVal = witPriorityVal - myDemLet.priority();
    myDemLet.priority(scePriorityVal);
  }
#ifdef PRMNGR_INFO_MODE
  std::cout << "     ... PriorMgr: done assigning WIT  priorities" << std::endl;  
#endif
}
// ----------------------------------------------------------------


// ----------------------------------------------------------------

LgFrSceDemLetCompareVoidFnPtr
LgFrScePriorityMgr::compareVoidDemLetFunc()
{
  return compareDemLet;
}

LgFrSceDemLetCompareVoidFnPtr
LgFrScePriorityMgr::compareVoidPrRuleFunc()
{
  return comparePrRule;
}


// SCE 6.1 ... use isXXXAll booleans to speed up 
// ----------------------------------------------------------------
void 
LgFrScePriorityMgr::calculateRuleSignature(LgFrSceDemLet * myDemLet)
{

  // populate this sucker ...
  std::vector<int> * ruleSig = myDemLet->ruleSignature();

  // loop through the sorted rules and pattern match !!!
  std::string attribute;
  int r=0;
  for (r=0; r<nRules_; r++) {
    LgFrScePriorityRule * curRule = rulePtrVec_[r];

    if (! curRule->isPartAll())  {
      attribute = curRule->partName();
      if (attribute.compare(myDemLet->partName()))
	continue;
    }
    if (! curRule->isSourceLocAll())  {
      attribute = curRule->sourceLoc();
      if (attribute.compare(myDemLet->sourceLoc()))
	continue;
    }
    if (! curRule->isCustomerLocAll())  {
      attribute = curRule->customerLoc();
      if ( attribute.compare(myDemLet->customerLoc()))
	continue;
    }
    if (! curRule->isDemandClassAll())  {
      attribute = curRule->demandClass();
      if (attribute.compare(myDemLet->demandClass()))
	continue;
    }
    if (! curRule->isDemandLevelAll())  {
      attribute = curRule->demandLevel();
      if (attribute.compare(myDemLet->demandLevel()))
	continue;
    }
    if (! curRule->isPartClassAll())  {
      attribute = curRule->partClass();
      if (attribute.compare(myDemLet->partClass()))
	continue;
    }
    if (curRule->startPeriod() > myDemLet->demandPd())
      continue;
    if (curRule->endPeriod() < myDemLet->demandPd())
      continue;

    // ok, we have a hit !!
    // check if this hits a new priority value 
    int priority = curRule->priority();
    if ( (!ruleSig->empty()) &&  (priority == ruleSig->back()) ) 
      continue;

    // finally, we have a new priority vale to add to the ruleSignature !
    ruleSig->push_back(curRule->priority());

  }

}



// this is the old way
#if 0
// ----------------------------------------------------------------
void 
LgFrScePriorityMgr::calculateRuleSignature(LgFrSceDemLet * myDemLet)
{

  // populate this sucker ...
  std::vector<int> * ruleSig = myDemLet->ruleSignature();

  // loop through the sorted rules and pattern match !!!
  int r=0;
  for (r=0; r<nRules_; r++) {
    LgFrScePriorityRule * curRule = rulePtrVec_[r];
    std::string attribute;
    attribute = curRule->partName();
    if ( (attribute.compare("ALL"))    && (attribute.compare(myDemLet->partName())))
      continue;
    attribute = curRule->sourceLoc();
    if ( (attribute.compare("ALL"))   && (attribute.compare(myDemLet->sourceLoc())))
      continue;
    attribute = curRule->customerLoc();
    if ( (attribute.compare("ALL")) && (attribute.compare(myDemLet->customerLoc())))
      continue;
    attribute = curRule->demandClass();
    if ( (attribute.compare("ALL")) && (attribute.compare(myDemLet->demandClass())))
      continue;
    attribute = curRule->demandLevel();
    if ( (attribute.compare("ALL")) && (attribute.compare(myDemLet->demandLevel())))
      continue;
    attribute = curRule->partClass();
    if ( (attribute.compare("ALL"))   && (attribute.compare(myDemLet->partClass())))
      continue;
    if (curRule->startPeriod() > myDemLet->demandPd())
      continue;
    if (curRule->endPeriod() < myDemLet->demandPd())
      continue;

    // ok, we have a hit !!
    // check if this hits a new priority value 
    int priority = curRule->priority();
    if ( (!ruleSig->empty()) &&  (priority == ruleSig->back()) ) 
      continue;

    // finally, we have a new priority vale to add to the ruleSignature !
    ruleSig->push_back(curRule->priority());

  }

}
#endif




// ----------------------------------------------------------------

void 
LgFrScePriorityMgr::processPriorityManagerRulesFile(
						    const std::string & fileName)
{
 
 std::string  partName;
 std::string  sourceLoc;
 std::string  customerLoc;
 std::string  demandClass;
 std::string  demandLevel;
 std::string  partClass;
 std::string  effect_start;
 std::string  effect_end;
 int          priority;
 int          startPeriod;
 int          endPeriod;

 
 std::ifstream inFile;
 long lineNo = 0;
 std::string dataLine;
 int t = 0; 
 
 MclFacility * theErrFacil = sceScenSP_->getErrFacility();
 LgFrSceSetOfParameters & setOfParameters =  sceScenSP_->setOfParameters();   


 int thePeriod = 0;
 
 std::ofstream outFile;
 LgFrSceScrubber * myScrubber = sceScenSP_->scrubber();
 
 this->openInputFileAndScrubOutputFile(fileName, "Priority Manager Rules ",
				       inFile, outFile, myScrubber, theErrFacil);

 
 std::cout << "Processing " << fileName << ":\t" << std::flush;
 long recCount = 0;
 long badCount = 0;
 long commentCount = 0;



 // TEMP STUPIDITY ... FIX ... use a better container!!
 LgFrScePriorityRule  ** tempRulePtrVec = new LgFrScePriorityRule * [100000];
 
 int ruleCounter = 0;
 
 while(inFile.peek() != EOF) {
   int result = 0;
 
   // Read next line of file
   std::getline( inFile, dataLine );
   lineNo ++;

   dataLine = sce::strip( dataLine );
   
   //  skip blanks or comments '*'
   if ((dataLine[0] == '*') || dataLine.empty()) {
     commentCount++;
     continue;
   }
   
   // construct a tokenizer
   SCETokenizer next(dataLine);
   
   if ( flatFileMgr_->containsIllegalCharacters( dataLine )) {
     (*theErrFacil)("IllegalCharacter",MclArgList() 
		    << fileName 
		    << (int)lineNo 
		    << dataLine);
   }
  

   partName     = flatFileMgr_->nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
   if (result == -1) {
     badCount++;
     continue;
   }

   sourceLoc    = flatFileMgr_->nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
   if (result == -1) {
     badCount++;
     continue;
   }
   customerLoc  = flatFileMgr_->nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
   if (result == -1) {
     badCount++;
     continue;
   }
   demandClass  = flatFileMgr_->nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
   if (result == -1) {
     badCount++;
     continue;
   }
   demandLevel  = flatFileMgr_->nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
   if (result == -1) {
     badCount++;
     continue;
   }
   partClass    = flatFileMgr_->nextQuotedString(next, result, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE);
   if (result == -1) {
     badCount++;
     continue;
   }

   result = flatFileMgr_->nextStartEndDates(startPeriod, endPeriod, next, fileName, lineNo, dataLine, *theCal_);
   if (result == 1) {
     badCount++;
     continue;
   }

   int nPeriods;
   witGetNPeriods(theWitRun_, &nPeriods);
   if ((startPeriod == nPeriods) || (endPeriod < 0)) {
     (*theErrFacil)("OutOfRange",MclArgList() 
		    << "Effectivity dates" 
		    << fileName 
		    << (int) lineNo 
		    << dataLine 
		    << "Rule ignored.");
     badCount++;
     continue;
   }

   flatFileMgr_->nextInt(priority, next, fileName, dataLine, lineNo, OPTIONAL_WITH_MESSAGE, -INT_MAX, INT_MAX);


   bool isPartAll=0;
   bool isSourceLocAll=0;
   bool isCustomerLocAll=0;
   bool isDemandClassAll=0;
   bool isDemandLevelAll=0;
   bool isPartClassAll=0;

   if (! partName.compare("ALL"))  
     isPartAll = 1;
   if (! sourceLoc.compare("ALL"))  
     isSourceLocAll = 1;
   if (! customerLoc.compare("ALL"))  
     isCustomerLocAll = 1;
   if (! demandClass.compare("ALL"))  
     isDemandClassAll = 1;
   if (! demandLevel.compare("ALL"))  
     isDemandLevelAll = 1;
   if (! partClass.compare("ALL"))  
     isPartClassAll = 1;
   

   // FINISH_ME: do we need to store the effect_start and effect_end as std::string's??   
   //            don't think we use it anywhere, but will we in the future?
   //            For now, it is being stored in the PriorityRule as a default string, if we don't
   //            need it then we should eliminate it from the class
   LgFrScePriorityRule * myNewRule = new LgFrScePriorityRule(partName,
							     sourceLoc,
							     customerLoc,
							     demandClass,
							     demandLevel,
							     partClass,
							     effect_start,
							     effect_end,
							     priority,
							     startPeriod,
							     endPeriod,
							     isPartAll,
							     isSourceLocAll,
							     isCustomerLocAll,
							     isDemandClassAll,
							     isDemandLevelAll,
							     isPartClassAll);
   
   // test the rule to see if it's OK !! FINISH_ME
   //   if (! myNewRule->isGood())  {
   //     badCount++;
   //     continue;
   //   }
   
   // ok, it's good
   tempRulePtrVec[ruleCounter] = myNewRule;
   ruleCounter++;

   recCount++;
   if (myScrubber -> scrubMode() && outFile.good())
     {
       outFile << dataLine << std::endl;
     }
   
 }
 
 nRules_ = ruleCounter;

 // now save the final list of rules in a right-sized vector 
 rulePtrVec_ = new LgFrScePriorityRule * [nRules_];

 int r=0;
 for (r=0; r<nRules_; r++) 
   rulePtrVec_[r] = tempRulePtrVec[r];

 delete [] tempRulePtrVec;


 std::cout << recCount << " records processed." << std::endl;
 inFile.close();


 flatFileMgr_->sceIOLogManager().printInputLog(fileName,lineNo,recCount,badCount,commentCount,recCount);
}
// ----------------------------------------------------------------

void
LgFrScePriorityMgr::printDemLets()
{
  int d=0;
  for (d=0; d<listMax_; d++) {
    LgFrSceDemLet * myDemLet = demLetList_[d];
    myDemLet->print();
  }
  
}





// ******************************************************************************
// initializeDemLets()
// ------------------------------
// DemandDemLets ... 
// * Loop Through all demand/period pairs 
//     - Create an demLet for each positive demandVol
//     * initialize each demLet's rule hit vector
//       - for each demlet, for each rule, Set the prRuleValHitVec
// - QSORT the demLetList
// - Note: the calling program is responsible for deleting any demand app data 
//         ptrs that are "new-ed" in here.
// - FINISH_ME: Put demVol in appData
// ********************************************************************************
void
LgFrScePriorityMgr::initializeDemLets()
{
  int nPeriods;

  float * demandVol;
  
#ifdef PRMNGR_INFO_MODE
  std::cout << "     ... PriorMgr: initializing DemLets" << std::endl;
  std::cout << "            ... Counting number of demLets required --->   listMax_ = ";
#endif
    
  witGetNPeriods(theWitRun_, &nPeriods);
  int nParts;
  char  **partList;
  witGetParts(theWitRun_, &nParts, &partList);


  // for performance testing, demVolThreshold should be .00001 
  float demVolThreshold = -0.0001;
  aPointer_ = 0;
  listMax_ = 0;

  // Let listMax_ = num demLets required == number of non-zero demandVols
  int i = 0; 
  for(i=0; i<nParts; i++){

    char * witPartName = partList[i];
    if (mpHelper_->isPartSpecialFeature(theWitRun_, witPartName))   
      continue;

    int nDemands;
    char ** dList;
    witGetPartDemands(theWitRun_, witPartName, &nDemands, &dList);
    int j = 0; 
    for(j=0; j<nDemands; j++){
      char * witDemandName = dList[j];
      float * demandVol;
      witGetDemandDemandVol( theWitRun_, witPartName, witDemandName,
                                 &demandVol);
      
      int t = 0; 
      for (t=0; t<nPeriods; t++)   {
        if (demandVol[t] > demVolThreshold)
	  listMax_++;
      }
      witFree(demandVol);
      witFree(dList[j]);
    }
    witFree(dList);
  }


#ifdef PRMNGR_INFO_MODE
  std::cout << listMax_ << std::endl;

  std::cout << "            ... Begin initializing the DemLets ... ";
#endif



  std::string plannerPart;
  std::string geo;

  // Now create the vector of demLet Ptrs
  vecDemLetList_ = new LgFrSceDemLet [listMax_];
  // and the vector of demLets
  demLetList_ =     new LgFrSceDemLet * [listMax_];
 
  // now load each demLet
  for( i=0; i<nParts; i++){
    char * witPartName = partList[i];

    if (mpHelper_->isPartSpecialFeature(theWitRun_, witPartName))   
      continue;

    int nDemands;
    char ** dList;
    witGetPartDemands(theWitRun_, witPartName, &nDemands, &dList);
    int j = 0; 
    for(j=0; j<nDemands; j++){
      char * witDemandName = dList[j];
      //      std::string demandName(dList[j]);
      witGetDemandDemandVol( theWitRun_, witPartName, witDemandName, &demandVol);

      // we create one demlet for each demand/period pair with positive demandVol
      witGetDemandDemandVol(theWitRun_, witPartName, witDemandName, &demandVol);
      int t = 0; 
      for (t=0; t<nPeriods; t++) {
	if (demandVol[t] > demVolThreshold) {

          LgFrSceDemLet & myDemLet = vecDemLetList_[aPointer_];
          demLetList_[aPointer_] = &myDemLet;

	  std::string sourceLoc;
	  std::string customerLoc;
	  std::string demandClass;
	  std::string demandLevel;
	  std::string partClass;


	  // TINP
	  if (mpHelper_->isPartSpecialGeoPlannerDemandPart(theWitRun_, witPartName, plannerPart, geo)) {
	    mpHelper_->uncompressedDemandNamesWithPdf(geo,
						      sourceLoc,
						      customerLoc,
						      demandClass,
						      demandLevel,
						      partClass);
	    myDemLet.partName(plannerPart);
	    myDemLet.sourceLoc(sourceLoc);	    
	  }

	  // INDP
	  else {
	    mpHelper_->uncompressedDemandNames(witDemandName,
					       customerLoc,
					       demandClass,
					       demandLevel,
					       partClass);


	    std::string partName(mpHelper_->partname(witPartName));
	    myDemLet.partName(partName);
	    std::string theSourceLoc(mpHelper_->pdf(witPartName));
	    myDemLet.sourceLoc(theSourceLoc);
	  }

	  std::string witPartNameString(witPartName);
	  std::string witDemandNameString(witDemandName);
	  myDemLet.witPartName(witPartNameString);
	  myDemLet.witDemandName(witDemandNameString);
	  myDemLet.customerLoc(customerLoc);
	  myDemLet.demandClass(demandClass);
	  myDemLet.demandLevel(demandLevel);
	  myDemLet.partClass(partClass);
          myDemLet.demandVol(demandVol[t]);
          myDemLet.demandPd(t);

	  std::vector<int> * ruleSig = myDemLet.ruleSignature();
	  calculateRuleSignature(&myDemLet);
#ifdef PRMNGR_DEBUG_MODE
	  myDemLet.print();
#endif

          aPointer_++;
        }
      }

      witFree(dList[j]);
      witFree(demandVol);

    }
    witFree(dList);
    witFree(partList[i]);
  }
  witFree(partList);
  
  assert(aPointer_ == listMax_);
  aPointer_ = 0;
#ifdef PRMNGR_INFO_MODE
  std::cout << "     Successfull." << std::endl;
#endif
}



// ---------------------
// Smooth the priorities so that they are Increasing in WIT
//  --------------------
void
LgFrScePriorityMgr::testForNonIncreasingPriorities()
{
  int   * priority;
  int nPeriods;
  char  *partName, *demandName;
    
  witGetNPeriods(theWitRun_, &nPeriods);
  int nParts;
  char  **partList;

  // this is kludgy !! need to coordinate with ipSCE to see how it is set there
  LgFrSceSetOfParameters & setOfParameters =  sceScenSP_->setOfParameters();    
  int defaultIndependentDemandPriority = setOfParameters.getInt("defaultIndependentDemandPriority");
  int defaultPriority = 1000000000 - defaultIndependentDemandPriority;

#ifdef PRMNGR_INFO_MODE
  std::cout << "     ... testing priorities for non-increasing values" << std::endl;
#endif

  witGetParts(theWitRun_, &nParts, &partList);


  // setting this to 1 up front means the warning message will never be told (don't need this warning)
  // FINISH_ME: we may want to warn if a demLet hits ZERO rules !?!?
  int hasIncreasingPriorityMessageBeenToldYet = 1;
  int i = 0; 
  for(i=0; i<nParts; i++){
    partName = partList[i];
    int nDemands;
    char ** dList;
    witGetPartDemands(theWitRun_, partList[i], &nDemands, &dList);
    int j = 0; 
    for(j=0; j<nDemands; j++){
      demandName = dList[j];
      witGetDemandPriority( theWitRun_, partName, demandName, &priority);
#ifdef PRMNGR_DEBUG_MODE
      this->dumpaVec("OriginalPriorityVec:", priority);
#endif


      bool didWeHitDemLetYet = false;
      bool hasPriorityChanged = false;
      int  prevPrio = -INT_MAX;


      // walk forward through the timeVec, starting at pd 0
      int t = 0; 
      for (t=0; t<nPeriods; t++)   {
	// first thing we check is to find the first period with a priority
	// when we find this, we walk backwards and set all the earlier priorities to this first one
	if ( (! didWeHitDemLetYet) && (priority[t] < 1000000000) ) {
	  didWeHitDemLetYet = true;
	  // now lets go backwards set earlier periods to priority[firstPeriodWithDemLet]
	  int tt;
	  for (tt=t; tt>0; tt--) {
	    priority[tt-1] = priority[tt];
	    hasPriorityChanged = true;
	  }
	  prevPrio = priority[t];
	}

	else {
	  if (priority[t] == 1000000000)  {
	    priority[t] = prevPrio;
	    hasPriorityChanged = true;
	  }
	  
	  else if (priority[t] <  prevPrio) {
	    priority[t] = prevPrio;
	    hasPriorityChanged = true;
	  }
	  
	  prevPrio = priority[t];
	}
      }
      if (hasPriorityChanged)
	witSetDemandPriority( theWitRun_, partName, demandName, priority);

#ifdef PRMNGR_DEBUG_MODE
      this->dumpaVec("SmoothedPriorityVec:", priority);
#endif

      witFree(dList[j]);
      witFree(priority);
    }
    witFree(dList);
    witFree(partList[i]);
  }
  witFree(partList);
#ifdef PRMNGR_INFO_MODE
  std::cout << "     ... done testing priorities for non-increasing values" << std::endl;
#endif
}



// --------------------------------------------------------------------- 
void
LgFrScePriorityMgr::dumpaVec(std::string aMessage, int * theVec)
{

  int nPeriods;
  witGetNPeriods(theWitRun_, &nPeriods);

  std::cout << aMessage << ": [";
  int t = 0; 
  for (t=0; t<nPeriods-1; t++)
    std::cout << theVec[t] << ",";
  std::cout << theVec[t] << "]" << std::endl;
}
// --------------------------------------------------------------------- 
  // set flag to print trace
void   
LgFrScePriorityMgr::trace(int traceFlag)
{
    traceFlag_ = traceFlag;
  }
// --------------------------------------------------------------------- 
// set filename for trace
void   
LgFrScePriorityMgr::traceFileName(std::string & traceFile)
{
    traceFileName_ = traceFile;
  }

// --------------------------------------------------------------------- 
void 
LgFrScePriorityMgr::openInputFileAndScrubOutputFile(
						    const std::string & fileName,
						    std::string fileDescription,
						    std::ifstream & inFile,
						    std::ofstream & outFile,
						    LgFrSceScrubber * myScrubber,
						    MclFacility * theErrFacil)
{
  
  if (myScrubber -> scrubMode())     {
    if (sce::find(fileName, "/") != std::string::npos) {
      (*theErrFacil)("QualifiedInputFileName",MclArgList() 
		     << fileName);
    }
    
    std::string file = myScrubber -> scrubDir() + fileName;
    outFile.open(file.c_str());
    if (!outFile.good())
      {
	(*theErrFacil)("ScrubOutPutFileNameBad",MclArgList() 
		     << file);
      }
  }

  inFile.open(fileName.c_str() );//,ios::nocreate);
  if (! inFile)  {
    (*theErrFacil)("SevereCannotOpenFile",MclArgList() 
		   << fileDescription 
		   << fileName 
		   << "");
  }

}





// default constructor
LgFrScePriorityMgr::LgFrScePriorityMgr()
:   
nRules_(0),
  nUniquePrValues_(0),
  rulePtrVec_(),
  vecDemLetList_(),
  demLetList_(),
  theCal_(),
  sceScenSP_(),
  theWitRun_(),
  mpHelper_(),
  flatFileMgr_(),
  traceFlag_(0),
  traceFileName_(""),
  listMax_(0),
  aPointer_(0),
  inFile_(),
  outFile_()
{
    // all the work is done in initializer
}


  // general constructor
LgFrScePriorityMgr::LgFrScePriorityMgr( LgFrScenarioForSceSmartPointer sceScenSP,
					WitRun                     * const theWitRun,
					LgFrMultiPlantHelper       * mpHelper,
					LgFrSceFlatFileMgr         * flatFileMgr,
					LgFrCalendar               * theCal)  
:
nRules_(0),
  nUniquePrValues_(0),
  rulePtrVec_(),
  vecDemLetList_(),
  demLetList_(),
  theCal_(theCal),
  sceScenSP_(sceScenSP),
  theWitRun_(theWitRun),
  mpHelper_(mpHelper),
  flatFileMgr_(flatFileMgr),
  traceFlag_(0),
  traceFileName_(""),
  listMax_(0),
  aPointer_(0),
  inFile_(),
  outFile_()
{
}



// destructor
LgFrScePriorityMgr::~LgFrScePriorityMgr()
{
  int r;
  for (r=0; r<nRules_; r++)  {
    delete rulePtrVec_[r];
  }
  delete [] rulePtrVec_;



  delete [] vecDemLetList_;
  delete [] demLetList_;




}













#ifdef NDEBUG
#undef NDEBUG
#endif



// self-test : can I self test ??
void
LgFrScePriorityMgr::test()
{
  // do-nothing test  
}

 










