#ifndef LGFR_SCHEDULE_SENSITIVE_PAIR_COMPARE_H
#define LGFR_SCHEDULE_SENSITIVE_PAIR_COMPARE_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/pairComp.h>
#include <scenario/src/idnumber.h>
#include <scenario/src/sortSchP.h>

// This is a Class for implementing 
// Comparison Strategies used for sorting
// LgFrSortingSchedule.

// This class is used when sorting ItemTimeVecPairs on values found
// in a seperate schedule.

// Forward references
class LgFrSortingScheduleFloat;
class LgFrSortingScheduleFloatSmartPointerToConst;
class LgFrSortingScheduleFloatSmartPointer;
class LgFrTimeVecCompareFloat;
class LgFrSortingPartScheduleFloat;
class LgFrSortingPartScheduleFloatSmartPointerToConst;
class LgFrSortingPartScheduleFloatSmartPointer;

class LgFrScheduleSensitivePairCompareFloat : 
   public LgFrPairCompareFloat    {
public:

  // Compare two ItemTimeVecPairFloat's
  // return 0 if equal
  //        <0 if first less than second
  //        >0 if first greater than second
  virtual
    int
      itemTimeVecPairCompareFloat(
         const LgFrItemTimeVecPairFloat &,
         const LgFrItemTimeVecPairFloat &)
           const;
           
  // Set Schedule containing values to be sorted on
  // A pointer to this schedule is kept by this object.
  // The life of the schedule must be greater than the live of this object.
  virtual
     void
        schedule(
           LgFrSortingScheduleFloatSmartPointer );

  // Get Schedule containing values to be sorted on
  virtual
     LgFrSortingScheduleFloatSmartPointerToConst
        schedule()
           const;

  // Set timeVec pair compare object.
  // This object contains the method used for comparing timeVecs
  // in schedule.
  //
  // Initial value is LgFrTimeVecCompareFloat
  virtual
     void
        timeVecCompareFloat(
           const LgFrTimeVecCompareFloat & );

  // Get timeVec pair compair object.
  virtual
     const LgFrTimeVecCompareFloat *
       timeVecCompareFloat()
          const;

  // Make clone copy of this object
  virtual
     LgFrPairCompareFloat *
        clone() 
           const;

  // Make clone copy of this object with new Scenario
  virtual
     LgFrPairCompareFloat *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrScheduleSensitivePairCompareFloat &
    operator=(
	      const LgFrScheduleSensitivePairCompareFloat& rhs);

  // Copy constructor
  LgFrScheduleSensitivePairCompareFloat (
     const LgFrScheduleSensitivePairCompareFloat &);

  // Copy constructor with new Scenario
  LgFrScheduleSensitivePairCompareFloat (
     const LgFrScheduleSensitivePairCompareFloat &,
     LgFrScenario &);

  // Constructor
  LgFrScheduleSensitivePairCompareFloat ();

  // Destructor
  ~LgFrScheduleSensitivePairCompareFloat ();

  // test this class
  static void test();

  // test LgFrScheduleSensitivePairCompareFloat methods.
  // Expects testScenario and testScenario2 to contain data from
  // /u/implode/wit/data/brenda/denise/wit.data
  static
  void
  contextTest
    (
     LgFrScenario & testScenario,
     LgFrLastProblemSolved & lps,		// testScenario's
     LgFrSortingPartScheduleFloatSmartPointer criticalSchedule, // testScenario's,
                                                      // these values will be
                                                      // used for sorting
                                                      // a supply and production
                                                      // schedule
     LgFrSortingPartScheduleFloatSmartPointer supplySchedule,   // testScenario's,
                                                      // this schedule will be
                                                      // sorted by
                                                      // critical value
     LgFrSortingPartScheduleFloatSmartPointer productionSchedule, // testScenario's,
                                                      // this schedule will be
                                                      // sorted by max
                                                      // critical value
     LgFrScenario & testScenario2,		// used to test clone(scenario)
     						// method
     LgFrLastProblemSolved & lps2);		// testScenario2's

private:

  LgFrSortingScheduleFloatSmartPointer   schedulePtr_;
  LgFrTimeVecCompareFloat  *   timeVecCompareFloatPtr_;
  const isAReturnType          id_;

  // Copy constructor used by clone
  LgFrScheduleSensitivePairCompareFloat (
     const LgFrScheduleSensitivePairCompareFloat*);
};

#endif
