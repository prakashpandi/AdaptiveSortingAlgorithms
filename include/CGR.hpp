#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>

namespace CGR 
{
    template<typename RandomAccessIterator, typename Less>
    inline void
    insertionSort(RandomAccessIterator first, RandomAccessIterator last, Less less)
    {   
        // early return if range to sort is 1
        if (first == last) return;

        // first element is trivialy sorted
        auto temp = first;
        temp++;

        // insert next element to sorted elements
        while (temp != last)
        {
            // element to insert
            auto element = std::move(*temp);

            // find location inside sorted elements
            auto pos = temp;
            while (pos != first)
            {
                // get left neighbor element
                auto left = pos;
                left--;

                // check if it is not smaller
                if (!less(element, *left))
                {
                    break;
                }

                // shift left neighbor one position to the right
                *pos = std::move(*left);
                pos = std::move(left);
            }

            // found final position for element
            if (pos != temp)
            {
                *pos = std::move(element);
            }
            temp++;
        }
    }

    // seems that this is slower :(
    // maybe problems with function calls 
    // could implement own binary search implementation 
    // but probably the swap are the dominating factor
    template<typename RandomAccessIterator, typename Less>
    inline void
    binaryInsertionSort(RandomAccessIterator first, RandomAccessIterator last, Less less)
    {   
        for (auto it = first; it != last; it++)
        {
            // use std::upper_bound to find insert position
            // for RandomAccessIterator the number of comparison 
            // are logarithmic so we should have fewer comparisons
            auto temp = std::upper_bound(first, it, *it, less);
    
            // use std::rotate to shift all values greater than
            // temp to right to make room for new element
            std::rotate(temp, it, it + 1); 
        }
    }   

    template<typename RandomAccessIterator, typename Less>
    inline void
    shellSort(RandomAccessIterator first, RandomAccessIterator last, Less less)
    {
        // calculate length
        int length = std::distance(first, last);

        // initialize vector for gap sequence
        std::vector<int> gaps{};
        
        // gap sequence from stackoverflow
        /* int i = 0;
        int h = 0;
        while (true)
        {
            i++;
            h = std::ceil(std::pow(2.48, i-1));
            if (h < length) 
            {
                gaps.push_back(h);
            } else {
                break;
            }
        } */

        // Tokuda sequence A361507 oeis.org
        int i = 0;
        int h = 1;
        while (true)
        {
            if (h < length) 
            {
                gaps.push_back(h);
            } else {
                break;
            }
            h = std::floor(2.25*h) + 1;
            i++;
        }

        // go over gaps sequence in reverse
        for (auto it = gaps.rbegin(); it != gaps.rend(); it++)
        {
            int gap = *it;
            // insertion sort with gapped size
            for (int i = *it; i < length; i++)
            {
                auto temp = std::move(*(first + i));
                int j = 0;
                for (j = i; j >= gap && less(temp, (first[j - gap])); j -= gap)
                {
                    //first[j] = first[j - gap];
                    std::iter_swap((first + j), (first + j - gap));
                }
                *(first + j) = std::move(temp);
            }
        }       
    }

    // simple heap sort just using functions from STL
    template<typename RandomAccessIterator, typename Less>
    inline void
    heapSortSTL(RandomAccessIterator first, RandomAccessIterator last, Less less)
    {   
        std::make_heap(first, last, less);
        std::sort_heap(first, last, less);
    }

    // helper function to calculate median of three for intro sort
    template<typename RandomAccessIterator, typename Less>
    inline RandomAccessIterator 
    medianOfThree(RandomAccessIterator first, RandomAccessIterator mid, RandomAccessIterator last, Less less)
    {
        if (less(*first, *mid) && less(*last, *first)){return first;}
        else if (less(*mid, *first) && less(*first, *last)){return first;}
        else if (less(*last, *mid) && less(*first, *last)){return last;}
        else if (less(*mid, *last) && less(*last, *first)){return last;}
        else {return mid;}
    }

    // implementation of intro sort called by wrapper function
    // and pivot element as median of three
    template<typename RandomAccessIterator, typename Less>
    inline void
    _introSortMedianPivot(RandomAccessIterator first, RandomAccessIterator last, Less less, int rec_depth)
    {   
        // switch to insertion sort if range is small
        int size = std::distance(first, last);
        if (size <= 32)
        {
            insertionSort(first, last, less);
            return;
        }

        // if recursion depth reaches zero switch to heap sort
        if (rec_depth == 0)
        {
            heapSortSTL(first, last, less);
            return;
        }

        // pivot as median of three
        auto pivot = last;
        --pivot;
        auto mid = first;
        std::advance(mid, size/2);
        mid = medianOfThree(first, mid, pivot, less);
        std::iter_swap(mid, pivot);

        // swap misplaced elements
        auto left = first;
        auto right = pivot;
        while (left != right)
        {
            // search for misplaced elements
            while (!less(*pivot, *left) && left != right)
            {
                left++;
            }
            while (less(*pivot, *right) && left != right)
            {
                right--;
            }
         
            // swap two misplaced elements
            if (left != right)
            {
                std::iter_swap(left, right);
            }
        }

        // move pivot to final position
        if (pivot != left && less(*pivot, *left))
        {
            std::iter_swap(pivot, left);
        }

        //divide range
        _introSortMedianPivot(first, left, less, rec_depth - 1);
        _introSortMedianPivot(left + 1, last, less, rec_depth - 1);
    }

    // implementation of intro sort called by wrapper function
    // and pivot element the element in the middle of the range 
    template<typename RandomAccessIterator, typename Less>
    inline void
    _introSortMidPivot(RandomAccessIterator first, RandomAccessIterator last, Less less, int rec_depth)
    {   
        // switch to insertion sort if range is small
        int size = std::distance(first, last);
        if (size <= 32)
        {
            insertionSort(first, last, less);
            return;
        }

        // if recursion depth reaches zero switch to heap sort
        if (rec_depth == 0)
        {
            heapSortSTL(first, last, less);
            return;
        }

        // original pivot choice 
        // pivot initialized as last valid element 
        auto pivot = last;
        --pivot;
        
        // choose middle element as pivot
        // should yield good results for nearly sorted ranges
        auto mid = first;
        std::advance(mid, size/2);
        std::iter_swap(mid, pivot);

        // swap misplaced elements
        auto left = first;
        auto right = pivot;
        while (left != right)
        {
            // search for misplaced elements
            while (!less(*pivot, *left) && left != right)
            {
                left++;
            }
            while (!less(*right, *pivot) && left != right)
            {
                right--;
            }
            // swap two misplaced elements
            if (left != right)
            {
                std::iter_swap(left, right);
            }
        }

        // move pivot to final position
        if (pivot != left && less(*pivot, *left))
        {
            std::iter_swap(pivot, left);
        }

        //divide range
        _introSortMidPivot(first, left, less, rec_depth - 1);
        _introSortMidPivot(left + 1, last, less, rec_depth - 1);
    }

    // wrapper function which calculates the maximale allowed recursion depth 
    // and then call intro sort
    template<typename RandomAccessIterator, typename Less>
    inline void
    introSort(RandomAccessIterator first, RandomAccessIterator last, Less less)
    {
        // calculate max recursion depth before switch to heap sort
        int size = std::distance(first, last);
        int rec_depth = 2 * std::log2(size);

        _introSortMidPivot(first, last, less, rec_depth);
        //_introSortMedianPivot(first, last, less, rec_depth);
    }

    template<typename RandomAccessIterator, typename Less>
    inline void
    mergeSortSTL(RandomAccessIterator first, RandomAccessIterator last, Less less, int size = 0)
    {   
        // calculate size of range
        // only necessary once 
        if ( size == 0 && first != last)
        {
            size = std::distance(first, last);
        }

        // original merge sort with divide to single elements
        /*
        if (size <= 1)
        {
            return;
        }
        */

        // merge sort with switching to insertion sort for small ranges
        //*
        if (size <= 32)
        {
            insertionSort(first, last, less);
            // binaryInsertionSort(first, last, less);
            return;
        }
        //*/

        // divide elements in two halfs left and right
        auto left_elem  = size / 2;
        auto right_elem = size - left_elem; 
        auto mid = first; 
        std::advance(mid, left_elem);

        // recursivly call of merge sort with inplace merge afterwards
        mergeSortSTL(first, mid, less);
        mergeSortSTL(mid, last, less);
        std::inplace_merge(first, mid, last, less);
    }
   
    // split sort achieves better adoptability by using longest increasung sub
    // sequence, uses a in-place variation of split sort
    // https://codereview.stackexchange.com/questions/212693/splitsort-an-adaptive-algorithm-to-handle-collections-with-few-inversions
    template<typename RandomAccessIterator, typename Less>
    inline void
    splitSort(RandomAccessIterator first, RandomAccessIterator last, Less less)
    {   
        if (std::distance(first, last) < 2)
        {
           return;
        } 

        auto mid = first;
        for (auto it = std::next(first); it != last; it++)
        {
            if (less(*it, *mid))
            {
                if (mid != first)
                {
                    mid--;
                }
            }
            else
            {
                mid++;
                std::iter_swap(mid, it);
            }
        }
        
        // std::sort is faster than own implementation of merge sort
        // std::sort(mid, last, less);
        mergeSortSTL(mid, last, less);
        // introSort(mid, last, less);
        std::inplace_merge(first, mid, last, less);
    }

    template<typename InputIterator, typename OutputIterator, typename Less>
    OutputIterator
    sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less)
    {
        // typedef for vector type
        typedef typename std::iterator_traits<InputIterator>::value_type v_type;

        // copy range to vector 
        std::vector<v_type> vec{};
        vec.assign(first, beyond);

        // call sorting function 
        // insertionSort(vec.begin(), vec.end(), less);
        // shellSort(vec.begin(), vec.end(), less);
        // heapSortSTL(vec.begin(), vec.end(), less);
        // introSort(vec.begin(), vec.end(), less);
        // mergeSortSTL(vec.begin(), vec.end(), less);
        splitSort(vec.begin(), vec.end(), less);

        return std::copy(vec.begin(), vec.end(), result);
    }

    template<typename RandomAccessIterator, typename Less>
    void
    sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less)
    {
        // call sort function
        // mergeSortSTL(first, beyond, less);
        // shellSort(first, beyond, less);
        introSort(first, beyond, less);
        // splitSort(first, beyond, less);
    }
}
