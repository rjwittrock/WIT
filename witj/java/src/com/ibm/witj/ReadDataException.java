
//==============================================================================
// WIT-J Java Source File ReadDataException.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class ReadDataException
//
// One kind of unchecked Exception thrown by WIT-J.
//
// When WIT-J throws a ReadDataException, this indicates that:
//
// * WIT has issued a severe error message while attempting to read a data file
//   for the readData method of class Problem.
// * As a result of the throw, WIT-J is now in a non-functional state.
//   No further calls to WIT-J methods should be made for the rest of the
//   program process.
//
// Class Hierarchy:
//
// RuntimeException (java.lang.RuntimeException)
//    WitjException
//       TerminalException
//          ReadDataException
//==============================================================================

public final class ReadDataException extends TerminalException
{
//==============================================================================
// Private Constructor
//
// To be called from C++.
//==============================================================================

   private ReadDataException (String message)
      {
      super (
         "   WIT has issued a severe error message while attempting to read\n"
       + "   a data file for the readData method of class Problem.\n"
       +    "\n"
       + "   This indicates an error in the data file.\n");
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// serialVersionUID
//
// This field is required in order to avoid a compiler warning whenever an
// instance is constructed, because this class inherits from class Throwable,
// which implements the interface Serializable.
//------------------------------------------------------------------------------

   private static final long serialVersionUID = 1L;

//==============================================================================
}
