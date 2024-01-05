#ifndef SORTINGALGORITHM_V1_MWE_HPP
#define SORTINGALGORITHM_V1_MWE_HPP

#include <vector>

//Algorithm inspired by: https://en.wikipedia.org/wiki/Merge_sort#Bottom-up_implementation

namespace MWE {
    template<typename InputIterator, typename OutputIterator, typename Less>
    OutputIterator sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less);

    template<typename RandomAccessIterator, typename Less>
    void sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less);

    template<typename RandomAccessIterator1, typename RandomAccessIterator2, typename Less>
    void bottomUpMerge(RandomAccessIterator1 iter1, RandomAccessIterator2 iter2, Less less, int iLeft, int iRight, int iEnd);

    template<typename InputIterator, typename OutputIterator, typename Less>
    OutputIterator MWE::sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less) {
        typedef typename std::iterator_traits<InputIterator>::value_type vType;
        std::vector<vType> v1(first, beyond);
        MWE::sort(v1.begin(), v1.end(), less);

        for (vType& i : v1) {
            *result++ = i;
        }

        return result;
    }

    template<typename RandomAccessIterator, typename Less>
    void MWE::sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less) {
        typedef typename std::iterator_traits<RandomAccessIterator>::value_type vType;
        std::vector<vType> vec2(first, beyond);

        int length = beyond - first;
        for (int width = 1; width < length; width = 2 * width) {
            for (int i = 0; i < length; i += 2 * width) {
                MWE::bottomUpMerge(first, vec2.begin(), less, i, std::min(i + width, length), std::min(i + 2 * width, length));
            }

            std::copy(vec2.begin(), vec2.end(), first);
        }
    }

    template<typename RandomAccessIterator1, typename RandomAccessIterator2, typename Less>
    void MWE::bottomUpMerge(RandomAccessIterator1 iter1, RandomAccessIterator2 iter2, Less less, int iLeft, int iRight, int iEnd) {
        int i = iLeft;
        int j = iRight;

        for (int k = iLeft; k < iEnd; k++) {
            if (i < iRight && (j >= iEnd || less(iter1[i], iter1[j]))) {
                iter2[k] = iter1[i];
                i++;
            }
            else {
                iter2[k] = iter1[j];
                j++;
            }
        }
    }

} // MWE

#endif //SORTINGALGORITHM_V1_MWE_HPP
