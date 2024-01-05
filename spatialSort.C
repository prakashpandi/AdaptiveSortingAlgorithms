// ======================================================================
//
// Copyright (c) OVGU Magdeburg
//
// This software and related files are part of an Algorithm Engineering
// implementation project on adaptive sorting of polygon vertices,
// conducted at computer science department (FIN) at OVGU Magdeburg,
// Germany. All rights reserved.
//
// ======================================================================

#include <iostream>
#include <Point_2.h>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <utility>
#include <iomanip>
#include <vector>
#include <list>
#include <cassert>
#include <functional>

#include <Boost_based_timer.h>
#include <competitors.h>


typedef AE::Point_2<double>   Point_2;
typedef AE::Less_2<double>    Less_2;
typedef AE::Timer             Timer;

int n;
std::string str;

template <class Less>
void
report_and_reset(Timer& uhr, Less& less, std::string algo, int iterations)
{
  double factor =  131072.0 / iterations;
  std::cout << algo << ";";
  std::cout << str << ";";
  std::cout << n << ";";
  std::cout << std::fixed << std::setprecision(2) << uhr.time()*factor << ";";
  std::cout << std::endl;
  uhr.reset(); 
}


int
main(int argc, char** argv)
{
  Timer  uhr;

  std::vector<Point_2>  P;
  std::vector<Point_2>  S;

  int N; 
  std::ifstream F(argv[1]);
  str = argv[1];
  F >> n;
  double x, y;
  for (int i=1; i<=n; ++i)
  { F >> x; F >> y;
    P.push_back( Point_2(x,y));
  }
  F.close();
  std::copy(P.begin(), P.end(), std::back_inserter(S) );

  Less_2  xy_less;
  int correction = std::log2(n);
  N =  (16777216  >> correction);

  via_std_sort( P.begin(), P.end(), S.begin(), xy_less);

//----------------------------------------------------------------------------
  uhr.start();
  for (int i=0; i<N; ++i) 
    via_std_sort( P.begin(), P.end(), S.begin(), xy_less);
  uhr.stop();
  report_and_reset(uhr, xy_less, "std", N);
// ----------------------------------------------------------------------------
  uhr.start();
  for (int i=0; i<N; ++i) 
    via_stable_sort( P.begin(), P.end(), S.begin(), xy_less);
  uhr.stop();
  report_and_reset(uhr, xy_less, "sms", N);
//----------------------------------------------------------------------------
  uhr.start();
  for (int i=0; i<N; ++i) 
    CKA::sort( P.begin(), P.end(), S.begin(), xy_less);
  uhr.stop();
  report_and_reset(uhr, xy_less, "CKA", N);
  assert( std::is_sorted( S.begin(), S.end(), xy_less) );
//----------------------------------------------------------------------------
  uhr.start();
  for (int i=0; i<N; ++i) 
    CGR::sort( P.begin(), P.end(), S.begin(), xy_less);
  uhr.stop();
  report_and_reset(uhr, xy_less, "CGR", N);
  assert( std::is_sorted( S.begin(), S.end(), xy_less) );
//----------------------------------------------------------------------------
/*
  uhr.start();
  for (int i=0; i<N; ++i) 
    ESE::sort( P.begin(), P.end(), S.begin(), xy_less);
  uhr.stop();
  report_and_reset(uhr, xy_less, "ESE", N);
  assert( std::is_sorted( S.begin(), S.end(), xy_less) );
*/
//----------------------------------------------------------------------------
/*
  uhr.start();
  for (int i=0; i<N; ++i) 
    KLE::sort( P.begin(), P.end(), S.begin(), xy_less);
  uhr.stop();
  report_and_reset(uhr, xy_less, "KLE", N);
  assert( std::is_sorted( S.begin(), S.end(), xy_less) );
*/
//----------------------------------------------------------------------------
  uhr.start();
  for (int i=0; i<N; ++i) 
    MWE::sort( P.begin(), P.end(), S.begin(), xy_less);
  uhr.stop();
  report_and_reset(uhr, xy_less, "MWE", N);
  assert( std::is_sorted( S.begin(), S.end(), xy_less) );
//----------------------------------------------------------------------------
  uhr.start();
  for (int i=0; i<N; ++i) 
    NRA::sort( P.begin(), P.end(), S.begin(), xy_less);
  uhr.stop();
  report_and_reset(uhr, xy_less, "NRA", N);
  assert( std::is_sorted( S.begin(), S.end(), xy_less) );
//----------------------------------------------------------------------------
  uhr.start();
  for (int i=0; i<N; ++i) 
    PPA::sort( P.begin(), P.end(), S.begin(), xy_less);
  uhr.stop();
  report_and_reset(uhr, xy_less, "PPA", N);
  assert( std::is_sorted( S.begin(), S.end(), xy_less) );
//----------------------------------------------------------------------------
  uhr.start();
  for (int i=0; i<N; ++i) 
    TMA::sort( P.begin(), P.end(), S.begin(), xy_less);
  uhr.stop();
  report_and_reset(uhr, xy_less, "TMA", N);
  assert( std::is_sorted( S.begin(), S.end(), xy_less) );
//----------------------------------------------------------------------------
/*
  uhr.start();
  for (int i=0; i<N; ++i) 
    YAL::sort( P.begin(), P.end(), S.begin(), xy_less);
  uhr.stop();
  report_and_reset(uhr, xy_less, "YAL", N);
  assert( std::is_sorted( S.begin(), S.end(), xy_less) );
*/
//----------------------------------------------------------------------------

  return EXIT_SUCCESS;
}

