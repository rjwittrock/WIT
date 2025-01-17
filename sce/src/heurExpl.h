#ifndef LGFRSCEOPTEXPLODER
#define LGFRSCEOPTEXPLODER

#include <wit/src/wit.h>
#include <scenario/src/machdep.h>
#include <sce/src/mPlantH.h>

// LgFrSceOptExploder -- Usage notes
// 
// This object can be used instead of the witMrp() function
// to do mrp explosions.  It's advantage is that it does a
// "full" explode meaning that it explodes a product even if
// product can not feasibly be built because of cycleTimes.
// the exploded requirements get recorded as "pastDue" demand.
// Demand that is past due can be further exploded in the past
// due "bucket".


// AFter invoking the explode() method, one can then query the object
// for output, in a fashion similar to using wit.  The four mrp output
// attributes of WIT can be provided by this object.  Like, WIT, if you
// request, say, the reqVol for a part, use the method
// reqVol(char * part, float ** reqVol);
// The memory management is similar to wit.  You, the CLient, are responsible
// for (portable)free'ing the memory.

// Constructing a LgFrSceOptExploder:
// There is no default constructor.  You must create one with a
// witRun  pointer as parameter.  The witRun MUST be populated.
// The constructor will create a one-period "miniWitRun" as its
// private data.  The miniWitRun is based on the original Witrun,
// which the object keeps a pointer to.  It does do a witCopyWitRun().
// NOTE:  if you change the structure of the witRun then the object will
// be out of synch with the original and you should construct a new one
// based on the latest witRun.  





class LgFrSceOptExploder {
public:
  
  // The "Action" method ... Just do it
  int explode();

  // set supplyVols and demandVols to the current
  // setting in originalWitRun_.  this is needed for
  // FSS users.
  void
  synchronize();


  // determine if this part is something that you deal with
  witBoolean
  exists(const char * part);
  

  // Get the four wit mrp part attributes:
  int mrpProdVol(  const char * part, float ** mrpProdVol);
  int mrpExcessVol(const char * part, float ** mrpExcessVol);
  int reqVol(      const char * part, float ** reqVolPtr);
  int mrpConsVol(  const char * part, float ** mrpConsVolPtr);
  

  // Get two new PastDue attributes:
  // Note!!!  These don't work in this implementation. They return
  // 0.  The logic of this explode engine does not distinguish 
  // between past due demand and period 0 demand.
  int pastDueReqVol(const char * part, float * pastDueReqVolPtr );
  int pastDueMrpConsVol(const char * part, float * pastDueMrpConsVolPtr );
  
  
  // get the reqVol with past due reqVol added to period 0
  int reqVolWithPastDue(const char * part, float ** reqVolPtr );
  
  // get the  mrpConsVol with past due mrpConsVol added to period 0
  int mrpConsVolWithPastDue(const char * part, float ** mrpConsVolPtr );


  // Print out all the attributes (in "del" format)
  void print(char * filename);

  WitRun *
  giveMeYourWitRun();
  

  void 
  setAllSupplyToZero();

  void 
  setAllDemandToZero();
  
  
  // preferred constructor
  LgFrSceOptExploder(WitRun * originalWitRun,
                         LgFrMultiPlantHelper & mpHelper);
  
  
  // destructor
  ~LgFrSceOptExploder();
  
  // Self-test
  static void test();
  
  
private:
  WitRun *  originalWitRun_;
  WitRun *  localWitRun_;
  int       state_;
  
  
  // default constructor -- don't use!
  LgFrSceOptExploder();
  
  // copy constructor  -- don't use!
  LgFrSceOptExploder( const LgFrSceOptExploder& source );
  
  // assignment operator -- don't use!
  LgFrSceOptExploder&
  operator=(const LgFrSceOptExploder& rhs);

  
  // internal method ... populates the witrun copy
  void populateLocalWitRun(LgFrMultiPlantHelper & mpHelper);

  // a lean (and smart) version of witCopyData()
  void
  smartCopyData(WitRun * const lhsWitRun,
                WitRun * const rhsWitRun,
                LgFrMultiPlantHelper & mpHelper);

  // does some model re-structuring so that an implosion
  // engine can do an explosion.
  void
  restructureForImplosionToSolveExplosion();


  // get the state of the object ...
  int state();

};


#endif





