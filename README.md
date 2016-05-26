# StratifyLib
Stratify Application Library for Data Management and Hardware Abstraction

## Naming Conventions

Stratify Lib uses the following naming conventions

### Classes and Namespaces

Classes and Namespaces use upper CamelCase with a strong preference for a single word.  The exception is pure virtual classes have "Object" appended to the name.

    class PureObject {
    public:
      virtual int method() = 0;
    }

### File and Directory Names

File names follow the class names with hpp appended to header files and cpp appended to C++ source files.  Namespaces have their own directories which contain the header files of all the classes in the namespace.  The namespaces include a header file that includes the classes uses the namespace.  See this example:

    /*! \brief Standard Calculation and Software Routines
    *
    */
    namespace Calc {}
    #include "Calc/Base64.hpp"
    #include "Calc/Ema.hpp"
    #include "Calc/Lookup.hpp"
    #include "Calc/Pid.hpp"
    #include "Calc/Rle.hpp"
    
    using namespace Calc;

In an application, #include <stfy/Calc.hpp> will allow you to declare Pid objects without using Calc::Pid.  If you don't want to use the namespace, just #include <stfy/Calc/Ema.hpp> instead.  All library headers (including those in Stratify Lib) should use this approach and should never include a namespace header in any header file.

### Methods and Functions

Methods and functions follow traditional C and C++ naming styles with lowercase and underscores to separate words.  Methods and functions should start with a verb except when directly accessing a variable.

    Class Object {
    public:
      int amount(){ return m_amount; } //this just returns the amount
      int set_amount(int v){ m_amount = v; } //this sets the value of amount
      int calc_amount(); //this is an action that does something 
      int get_amount(); //since this starts with a verb it needs to do something--like load amount from a file
    private:
      int m_amount; //member variables have an m_ prefix
    }

The above code uses set_* and get_* but not perhaps in the traditional way.  If get_ is used, it implies the value is not immediately available and must be loaded from somewhere.  The convention used by the method amount() (no action word) is used if the value is immediately ready or ready with a trivial calculation such as

    area(){ return m_width*m_height; }


### Variables and Member Variables

Variables are named the same way functions are except they don't start with an action verb.  Member variables have m_ prefixed to the description variable name.  See the amount() and m_amount example above.

### Type Definitions

To maintain compatibility and interoperability with C (i.e. Stratify OS), type definitions are declared in .h files and follow C naming conventions.  Whenever typedef is used, _t should be appended to the type name like size_t or pthread_t.

### Enums and Macros

Both enums and constants are written in all caps with words separated by underscores. Enums are preferred to macros when defining constants because this allows them to auto-populate when using Eclipse (or another C++ IDE).  Here is an example from the Dev class:

    enum {
		  RDONLY /*! Open as read-only */ = LINK_O_RDONLY,
		  READONLY /*! Open as read-only */ = LINK_O_RDONLY,
		  WRONLY /*! Open as write-only */ = LINK_O_WRONLY,
		  WRITEONLY /*! Open as write-only */ = LINK_O_WRONLY,
		  CREATE /*! Create when opening (files) */ = LINK_O_CREAT,
		  CREAT /*! Create when opening (files) */ = LINK_O_CREAT,
		  TRUNCATE /*! Truncate when opening (files) */ = LINK_O_TRUNC,
		  TRUNC /*! Truncate when opening (files) */ = LINK_O_TRUNC,
		  APPEND /*! Append when opening (files)*/ = LINK_O_APPEND,
		  EXCLUSIVE /*! Create exclusively (files) */ = LINK_O_EXCL,
		  EXCL /*! Create exclusively (files) */ = LINK_O_EXCL,
		  READWRITE /*! Open as read-write */ = LINK_O_RDWR,
		  RDWR /*! Open as read-write */ = LINK_O_RDWR,
		  NONBLOCK /*! Open as non-blocking */ = LINK_O_NONBLOCK,
		  NDELAY /*! Open as non-blocking */ = LINK_O_NONBLOCK,
		  ACCMODE /*! Access mode mask */ = LINK_O_ACCMODE
	};

### Standard Abbreviations and a few Quirks

When dealing with pixels or single bits (like a high/low signal on a single pin), these abbreviations are used:

- tst_: test the value
- inv_: invert the value
- set_: set the value to 1
- clr_: clear the value to 0

When not dealing with a single bit, clear_ or invert_ is used (i.e. clr_pixel() and bitmap.clear()).

For fonts and bitmaps, the methods w() and h() are used to access width and height respectively.






