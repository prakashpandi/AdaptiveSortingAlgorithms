#ifndef ASort_h
#define ASort_h

#include <vector>

#ifdef DEBUG
#include <iostream>
#include <boost/timer/timer.hpp>
#endif

#define INSERTION_THRESHOLD 50

namespace CKA
{
    // it is adaptive merge sort with sortness checking before and insertion sort
    template <class RandomAccessIterator, class Less>
    void sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less)
    {
#ifdef DEBUG
        boost::timer::auto_cpu_timer timer;
#endif
        bool is_sorted = true;
        bool is_sorted_in_reverse = true;

        for (RandomAccessIterator i = first; i != beyond - 1; ++i)
        { // checks if the array already sorted in right way
            if (is_sorted && !less(*i, *(i + 1)))
            {
                is_sorted = false;
                continue;
            }
            if (is_sorted_in_reverse && less(*i, *(i + 1)))
            {
                is_sorted_in_reverse = false;
                continue;
                ;
            }
            if (!is_sorted && !is_sorted_in_reverse)
                break;
        }

        if (is_sorted)
        {
#ifdef DEBUG
            std::cout << "is_sorted return" << std::endl;
#endif
            return;
        }

        if (is_sorted_in_reverse)
        {
            std::reverse(first, beyond);
#ifdef DEBUG
            std::cout << "is_sorted_in_reverse return" << std::endl;
#endif
            return;
        }

        int size = std::distance(first, beyond);
        // if size is too smol do insertion sort
        if (size < INSERTION_THRESHOLD)
        {
            for (RandomAccessIterator i = first + 1; i != beyond; ++i)
            {
                typename std::iterator_traits<RandomAccessIterator>::value_type key = *i;
                RandomAccessIterator j = i - 1;
                while (j >= first && less(key, *j))
                {
                    *(j + 1) = *j;
                    --j;
                }

                *(j + 1) = key;
            }
#ifdef DEBUG
            std::cout << "insertion sort return" << std::endl;
#endif
            return;
        }

        RandomAccessIterator middle = first + size / 2;
        CKA::sort(first, middle, less);
        CKA::sort(middle, beyond, less);

        // merge()
        std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type> arr;
        arr.reserve(size);
        RandomAccessIterator l{first}, r{middle};

        while (l != middle && r != beyond)
        {
            if (!less(*r, *l))
            {
                arr.push_back(*(l++));
                continue;
            }
            arr.push_back(*(r++));
        }

        while (l != middle)
            arr.push_back(*(l++));

        while (r != beyond)
            arr.push_back(*(r++));

        auto i_arr = arr.begin();
        for (auto i = first; i != beyond; ++i)
            std::iter_swap(i, i_arr++);

#ifdef DEBUG
        std::cout << "end of merge return" << std::endl;
#endif
    }

    template <class InputIterator, class OutputIterator, class Less>
    OutputIterator sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less)
    {
        std::vector<typename std::iterator_traits<InputIterator>::value_type> random_arr;
        random_arr.reserve(std::distance(first, beyond));
        std::copy(first, beyond, std::back_inserter(random_arr));
        CKA::sort(random_arr.begin(), random_arr.end(), less);
        return std::copy(random_arr.begin(), random_arr.end(), result);
    }
}

#endif
