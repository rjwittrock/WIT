//------------------------------------------------------------------------------
// WIT-J C++ Header File StringJOR.h.
//
// Contains the declaration of class StringJOR.
//------------------------------------------------------------------------------

#ifndef StringJOR_h
#define StringJOR_h

#include <JniClient.h>

//------------------------------------------------------------------------------
// Class StringJOR
//
// "String Java Object Representation"
// Represents a Java String.
//
// An instance of this class stores a local reference to the represented Java
// String. Thus an instance of this class should always be destroyed by the time
// the native method in which it was created returns.
//
// Class Hierarchy:
//
// JniClient
//    StringJOR
//------------------------------------------------------------------------------

namespace WitJ
{
class StringJOR: public JniClient
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit StringJOR (JNIEnv *);
      //
      // Constructor.
      // The represented Java String will be the empty string.

   ~StringJOR ();
      //
      // Destructor.

   void makeJavaString (const char * theCString);
      //
      // Creates a new Java String that's a copy of theCString.
      // The new Java String becomes the represented Java String.
      // This function may only be called once per StringJOR instance.

   inline _jstring * getJString ()
      {
      return myJString_;
      }

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   StringJOR              (const StringJOR &);
   StringJOR & operator = (const StringJOR &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   _jstring * myJString_;
      //          
      // A local reference to the represented Java String.
   };
};
//
// End of namespace WitJ.

#endif
