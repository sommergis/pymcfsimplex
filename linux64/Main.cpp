/*--------------------------------------------------------------------------*/
/*---------------------------- File Main.C ---------------------------------*/
/*--------------------------------------------------------------------------*/
/** @file
 * 
 * Sample Main file to illustrate the use of any solver deriving from
 * MCFClass. By changing just *two lines of code* and little more (see comment
 * PECULIARITY, if exists) the file works with any derived solver.
 *
 * An instance of a Min Cost Flow problem in DIMACS standard format is read
 * from file and solved. In addition, the same problem can be written on a
 * file in MPS format. 
 *
 * \version 4.00
 *
 * \date 30 - 12 - 2009
 *
 * \author Alessandro Bertolini \n
 *         Operations Research Group \n
 *         Dipartimento di Informatica \n
 *         Universita' di Pisa \n
 *
 * \author Antonio Frangioni \n
 *         Operations Research Group \n
 *         Dipartimento di Informatica \n
 *         Universita' di Pisa \n
 */
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*------------------------------ INCLUDES ----------------------------------*/
/*--------------------------------------------------------------------------*/

// Johannes Sommer: include because of method "strlen" in line 175
#include <string.h>

#include <fstream>
#include <sstream>

#include "MCFSimplex.h"
#define MCFSOLVER MCFSimplex

// just change the two lines above and any MCFClass solver can be used (with
// the exception of PECULIARITY)

/*--------------------------------------------------------------------------*/
/*------------------------------- MACROS -----------------------------------*/
/*--------------------------------------------------------------------------*/

#define PRINT_RESULTS 0

/* If PRINT_RESULTS != 0, the optimal flows and potentials are printed
   after that the problem is successfully solved to optimality (so, watch
   out if your instance is very large). */

/*--------------------------------------------------------------------------*/
/*-------------------------------- USING -----------------------------------*/
/*--------------------------------------------------------------------------*/

#if( OPT_USE_NAMESPACES )
 using namespace MCFClass_di_unipi_it;
#endif

/*--------------------------------------------------------------------------*/
/*------------------------------- FUNCTIONS --------------------------------*/
/*--------------------------------------------------------------------------*/

template<class T>
inline T ABS( const T x )
{
 return( x >= T( 0 ) ? x : -x );
 }

/*--------------------------------------------------------------------------*/
// This function reads the first part of a string (before white spaces) and
// copy T value in the variable sthg (of T type)


template<class T>
static inline void str2val( const char* const str , T &sthg )
{
 istringstream( str ) >> sthg;
 }

/*--------------------------------------------------------------------------*/
// This function skips comment line in a input stream, where comment line is 
// marked by an initial '#' character

void SkipComments( ifstream &iParam , string &buf )
{
 do {
  iParam >> ws;
  getline( iParam , buf );
 }
 while( buf[ 0 ] == '#' );
}

/*--------------------------------------------------------------------------*/
// This function tries to read the parameter file; if it finds it, the
// corresponding parameters are set in the MCFClass object

void SetParam( MCFClass *mcf )
{
 ifstream iParam( "config.txt" ); 
 if( ! iParam.is_open() )
  return;

 string buf;
 int num;
 SkipComments( iParam , buf );
 str2val( buf.c_str(), num );        // get number of int parameters

 for( int i = 0 ; i < num ; i++ ) {  // read all int parameters
  int param , val;
  
  SkipComments( iParam , buf );
  str2val( buf.c_str(), param );     // parameter name
  
  SkipComments( iParam , buf );
  str2val( buf.c_str(), val );       // parameter value

  mcf->SetPar( param , val );

  }  // end( for( i ) )

 SkipComments( iParam , buf );
 str2val( buf.c_str() , num );       // get number of double parameters

 for( int i = 0 ; i < num ; i++ ) {  // read all double parameters
  int param;
  double val;
  SkipComments( iParam , buf );
  str2val( buf.c_str(), param );     // parameter name
  
  SkipComments( iParam , buf );
  str2val( buf.c_str() , val );      // parameter value
  
  mcf->SetPar( param , val );

  }  // end( for( i ) )
 }  // end( SetParam )

/*--------------------------------------------------------------------------*/
/*--------------------------------- MAIN -----------------------------------*/
/*--------------------------------------------------------------------------*/

int main( int argc , char **argv )
{
 // reading command line parameters - - - - - - - - - - - - - - - - - - - - -

 if( argc < 2 )
 {
  cerr << "Usage: MCFSolve <input file> [<output MPS file>]" << endl;

  return( -1 );
  }

 // opening input stream- - - - - - - - - - - - - - - - - - - - - - - - - - -

 ifstream iFile( argv[ 1 ] );
 if( ! iFile )
 {
  cerr << "ERROR: opening input file " << argv[ 1 ] << endl;
  return( -1 );
  }

 try {
  // construct the solver - - - - - - - - - - - - - - - - - - - - - - - - - -

  MCFSOLVER *mcf = new MCFSOLVER();

  mcf->SetMCFTime();  // do timing

  // load the network - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  char c = argv[1][strlen( argv[1]) - 4 ];
  // PECULIARITY: MCFSimplex expects quadratic case too, so there are two
  // different ways to call LoadDMX.
  bool quad = ( c == 'q' );
  mcf->LoadDMX( iFile, quad );

  // set "reasonable" values for the epsilons, if any - - - - - - - - - - - -

  MCFClass::FNumber eF = 1;
   for( register MCFClass::Index i = mcf->MCFm() ; i-- ; )
    eF = max( eF , ABS( mcf->MCFUCap( i ) ) );

   for( register MCFClass::Index i = mcf->MCFn() ; i-- ; )
    eF = max( eF , ABS( mcf->MCFDfct( i ) ) );   

   MCFClass::CNumber eC = 1;
   for( register MCFClass::Index i = mcf->MCFm() ; i-- ; )
    eC = max( eC , ABS( mcf->MCFCost( i ) ) );
   // PECULIARITY: MCFSimplex expects quadratic case too, so there are two
   // different ways to call SetPar for parameters kEpsFlw and kEpsCst
   if (quad) // if "quadratic" network (.qdmx) is loaded
   {
		for( register MCFClass::Index i = mcf->MCFm() ; i-- ; )
		 eC = max( eC , ABS( mcf->MCFQCoef( i ) ) );
		mcf->SetPar( MCFSOLVER::kEpsFlw, // the epsilon for flows
			         (double) numeric_limits<MCFClass::FNumber>::epsilon() * eF * mcf->MCFm() / 10);  
		mcf->SetPar( MCFSOLVER::kEpsCst, // the epsilon for costs
			         (double) numeric_limits<MCFClass::CNumber>::epsilon() * eC * mcf->MCFm() / 10);  
   }
   else // if "linear" network (.dmx) is loaded
   {
	    mcf->SetPar( MCFSOLVER::kEpsFlw, // the epsilon for flows
			         (double) numeric_limits<MCFClass::FNumber>::epsilon() * eF * mcf->MCFm() * 10);  
		mcf->SetPar( MCFSOLVER::kEpsCst, // the epsilon for costs
			         (double) numeric_limits<MCFClass::CNumber>::epsilon() * eC * mcf->MCFm() * 10);  
   }

  // set other parameters from configuration file (if any)- - - - - - - - - -

   SetParam( mcf );

  // solver call- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

   mcf->SolveMCF();

   MCFSOLVER::CRow cr = new MCFSOLVER::CNumber[3];
   MCFSOLVER::Index_Set nms = new MCFSOLVER::Index[3];

  // output results - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  switch( mcf->MCFGetStatus() ) {
   case( MCFClass::kOK ):
	cout << "Optimal Objective Function value = " << mcf->MCFGetFO() << endl;
    double tu , ts;
    mcf->TimeMCF( tu , ts );
    cout << "Solution time (s): user " << tu << ", system " << ts << endl;
    #if( PRINT_RESULTS )
    {
     if( ( numeric_limits<MCFClass::CNumber>::is_integer == 0 ) ||
		 ( numeric_limits<MCFClass::FNumber>::is_integer == 0 ) )
     {
		cout.setf( ios::scientific, ios::floatfield );
		cout.precision( 12 );
	 }

     MCFClass::FRow x = new MCFClass::FNumber[ mcf->MCFm() ];
     mcf->MCFGetX( x );
     for( MCFClass::Index i = 0 ; i < mcf->MCFm() ; i++ )
		cout << "x[" << i << "] = "  << x[ i ] << endl;
	 delete[] x;
     MCFClass::CRow pi = new MCFClass::CNumber[ mcf->MCFn() ];
     mcf->MCFGetPi( pi );
     for( MCFClass::Index i = 0 ; i < mcf->MCFn() ; i++ )
      cout << "pi[" << i << "] = "  << pi[ i ] << endl;
     delete[] pi;
     }
    #endif

    // check solution
    mcf->CheckPSol();
    mcf->CheckDSol();

    break;
   case( MCFClass::kUnfeasible ):
    cout << "MCF problem unfeasible." << endl;
    break;
   case( MCFClass::kUnbounded ):
    cout << "MCF problem unbounded." << endl;
    break;
   default:
    cout << "Error in the MCF solver." << endl;
   }

  // output the problem in MPS format - - - - - - - - - - - - - - - - - - - -

  if( argc > 2 )
  {
   ofstream oFile( argv[ 2 ] );

   mcf->WriteMCF( oFile , MCFClass::kMPS );
   }

  // destroy the object - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete( mcf );
  }
 // manage exceptions - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 catch( exception &e ) {
  cerr << e.what() << endl;
  return( 1 );
  }
 catch(...) {
  cerr << "Error: unknown exception thrown" << endl;
  return( 1 );
  }

 // terminate - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 return( 0 );

 }  // end( main )

/*--------------------------------------------------------------------------*/
/*------------------------- End File Main.C --------------------------------*/
/*--------------------------------------------------------------------------*/
