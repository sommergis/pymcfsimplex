%define MODULEDOCSTRING
"
****************************************************************************
* pyMCFSimplex                                                             *
****************************************************************************

pyMCFSimplex is a Python-Wrapper for the C/C++ MCFSimplex Solver Class from 
the University of Pisa developed and maintained by A. Bertolini and 
A. Frangioni.
MCFSimplex is a Class that solves big sized Minimum Cost Flow Problems
very fast.

pyMCFSimple is brought to you by
 Johannes Sommer, 2013
 G#.Blog: www.sommer-forst/blog"
%enddef

%module(docstring=MODULEDOCSTRING) pyMCFSimplex

%{
/* for arrays */
#define SWIG_FILE_WITH_INIT
/* Includes for SWIG */
#include "OPTUtils.h"
#include "MCFClass.h"
#include "MCFSimplex.h"
%}
/* for arrays */
%include "numpy.i"
%init %{
import_array();
%}

/* Exceptions */
%include exception.i
%exception {
    try {
        $action
    } catch(std::exception& e) {
        // const_cast for Perl
        SWIG_exception(SWIG_RuntimeError, const_cast<char *>(e.what()));
    } catch(...) {
        SWIG_exception(SWIG_RuntimeError, "Unknown error");
    }
}

namespace std {
	class exception { 
		public:
			exception();
			exception(const exception& rhs);
			virtual ~exception();
			virtual const char *what(void);
	};
}

/* python helper for version */
%pythoncode %{
	__version__ = "0.9.1"
	def version():
		d = {}
		d["MCFSimplex"] = "1.0"
		d["pyMCFSimplex"] = __version__
		return d
%}

/* c helper methods for double arrays */
%inline %{
double *new_darray(int size) {
	return (double *) malloc(size*sizeof(double));
}
double darray_get(double *a, int index) {
	return a[index];
}
void darray_set(double *a, int index, double value) {
	a[index] = value;
}
/* c helper methods for unsigned int arrays */
unsigned int *new_uiarray(int size) {
	return (unsigned int *) malloc(size*sizeof(unsigned int));
}
unsigned int uiarray_get(unsigned int *a, int index) {
	return a[index];
}
void uiarray_set(unsigned int *a, int index, unsigned int value) {
	a[index] = value;
}
%}
%rename(delete_darray) free(void *);
%rename(delete_uiarray) free(void *);

/* python helper methods for arrays */
%pythoncode %{
	# Turn a Python list into a C double array
	def CreateDoubleArrayFromList(l):
		d = new_darray(len(l))
		for i in range(0,len(l)):
			darray_set(d,i,l[i])
		return d
	# Turn a Python list into a C unsigned int array
	def CreateUIntArrayFromList(l):
		ui = new_uiarray(len(l))
		for i in range(0,len(l)):
			uiarray_set(ui,i,l[i])
		return ui
	# Print out some elements of a double array
	def PrintElements(a, first, last):
		for i in range(first,last):
			print darray_get(a,i)
			
	# Print out some elements of an uint array
	def PrintElements(a, first, last):
		for i in range(first,last):
			print uiarray_get(a,i)
%}

%include std_wiostream.i
%include std_map.i
%include std_string.i
%apply const std::string & {std::string &};
%apply const std::string & {std::string *};

%include typemaps.i
%apply const double & { double & val};
%apply const int & { int & val};

/* Ignores abstract methods of MCFClass, that are not used by MCFSimplex */
%ignore MCFGetX();
%ignore MCFGetPi();
%ignore MCFGetRC();
%ignore MCFCosts();
%ignore MCFQCoef();
%ignore MCFUCaps();
%ignore MCFDfcts();
%ignore MCFSNdes();
%ignore MCFENdes();
%ignore MCFGetState();
%ignore MCFPutState( MCFStatePtr S );

/* Ignore Method WriteMCF() because of C outstream */
%ignore WriteMCF(ostream &oStrm , int frmt = 0 );

/* Ingore Method with C-Stream input in Wrapper */
%ignore LoadDMX(istream &DMXs , bool IsQuad = false );

/* The following will be available in Python through SWIG*/
%include "MCFClass.h"
%include "MCFSimplex.h"
