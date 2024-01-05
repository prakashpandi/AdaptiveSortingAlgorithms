// HVA.hpp

#ifndef HVA_HPP
#define HVA_HPP

#include <algorithm>

namespace HVA {

    // Function template for non-random access iterators
    template<typename InputIterator, typename OutputIterator, typename Less>
    OutputIterator sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less) {
        std::vector<typename std::iterator_traits<InputIterator>::value_type> temp(first, beyond);
        std::sort(temp.begin(), temp.end(), less);
        return std::copy(temp.begin(), temp.end(), result);
    }

    // Function template for random access iterators
    template<typename RandomAccessIterator, typename Less>
    void sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less) {
        std::sort(first, beyond, less);
    }

} // namespace HVA

#endif // HVA_HPP
