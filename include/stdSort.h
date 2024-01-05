// ======================================================================
//
// Copyright (c) 2015 OVGU Magdeburg
//
// This software and related files are part of an Algorithm Engineering
// implementation project on adaptive sorting of polygon vertices,
// conducted at computer science department (FIN) at OVGU Magdeburg, 
// Germany. All rights reserved.
//
// ======================================================================

#ifndef STDSORT_H
#define STDSORT_H

template<class InputIterator, class OutputIterator, class Less>
OutputIterator 
via_std_sort (InputIterator first, 
              InputIterator beyond, 
              OutputIterator result, 
              Less less)
{ typedef typename std::iterator_traits< InputIterator>::value_type T; 
  std::vector<T>  V( first, beyond);
  std::sort( V.begin(), V.end(), less );
  result = std::copy( V.begin(), V.end(), result );
  return result; 
}

template<class InputIterator, class OutputIterator, class Less>
OutputIterator 
via_std_heapsort (InputIterator first, 
                  InputIterator beyond, 
                  OutputIterator result, 
                  Less less)
{ typedef typename std::iterator_traits< InputIterator>::value_type T; 
  std::vector<T>  V( first, beyond);
  std::make_heap( V.begin(), V.end(), less );
  std::sort_heap( V.begin(), V.end(), less );
  result = std::copy( V.begin(), V.end(), result );
  return result; 
}

template<class RandomAccessIterator, class Less>
void
via_std_heapsort (RandomAccessIterator first, 
                  RandomAccessIterator beyond, 
                  Less less)
{ std::make_heap( first, beyond, less );
  std::sort_heap( first, beyond, less );
  return;
}

template<class InputIterator, class OutputIterator, class Less>
OutputIterator 
via_stable_sort (InputIterator first, 
                 InputIterator beyond, 
                 OutputIterator result, 
                 Less less)
{ typedef typename std::iterator_traits< InputIterator>::value_type T; 
  std::vector<T>  V;
  std::copy( first, beyond, std::back_inserter(V));
  std::stable_sort( V.begin(), V.end(), less );
  result = std::copy( V.begin(), V.end(), result );
  return result; 
}

template<class RandomAccessIterator, class Less>
void
via_stable_sort ( RandomAccessIterator first, 
                  RandomAccessIterator beyond, 
                  Less less)
{ std::stable_sort( first, beyond, less);
  return; 
}

#endif // STDSORT_H
