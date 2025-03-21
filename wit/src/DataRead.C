//==============================================================================
// WIT
//
// Based On:
//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "DataRead.C"
//
// Contains the implementation of class DataReader.
//------------------------------------------------------------------------------

#include <DataRead.h>
#include <Problem.h>
#include <ApiMgr.h>
#include <InputID.h>
#include <Timing.h>
#include <MsgFac.h>
#include <Parser.h>

//------------------------------------------------------------------------------

WitDataReader * WitDataReader::activeInstance_   = NULL;
bool            WitDataReader::errorHasOccurred_ = false;

//------------------------------------------------------------------------------

WitDataReader::WitDataReader (WitProblem * theProblem):

      WitProbAssoc   (theProblem),

      inputFileName_ (),
      inputFile_     (NULL)
   {
   }

//------------------------------------------------------------------------------

WitDataReader::~WitDataReader ()
   {
   }

//------------------------------------------------------------------------------

void WitDataReader::readData (const char * fileName)
   {
   myMsgFac () ("witFileParam", fileName);

   if (errorHasOccurred_)
      myMsgFac () ("postErrReadDataSmsg");

   if (activeInstance_ != NULL)
      {
      errorHasOccurred_ = true;

      myMsgFac () ("overlapReadDataSmsg");
      }

   activeInstance_ = this;

   try
      {
      protectedReadData (fileName);
      }

   catch (...)
      {
      errorHasOccurred_ = true;
      activeInstance_   = NULL;

      throw;
      }

   activeInstance_ = NULL;
   }

//------------------------------------------------------------------------------

bool WitDataReader::active ()
   {
   return (this == activeInstance_);
   }

//------------------------------------------------------------------------------

void WitDataReader::protectedReadData (const char * fileName)
   {
   WitTimer::enterSection ("input");

   inputFileName_ = fileName;

   inputFile_ =
      (inputFileName_ == WitSTDIN)?
         stdin:
         openFile (inputFileName_.myCstring (), "r");

   myApiMgr ()->myInputID ()->setDataFileName   (inputFileName_.myCstring ());
   myApiMgr ()->myInputID ()->setDataFileLineNo (0);

   WitParser::parse (myProblem (), inputFileName_.myCstring (), inputFile_);

   myApiMgr ()->myInputID ()->setDataFileName   (NULL);
   myApiMgr ()->myInputID ()->setDataFileLineNo (0);

   if (inputFileName_ != WitSTDIN)
      if (fclose (inputFile_) != 0)
         myMsgFac () ("fcloseSmsg", inputFileName_);

   inputFileName_ = "";

   inputFile_     = NULL;

   WitTimer::leaveSection ("input");

   myMsgFac ().abortIfErrors ();
   }
