#ifndef YAL_HPP
#define YAL_HPP

#include <vector>
#include <algorithm>

namespace YAL {

    // iterative way
    template <class RandomAccessIterator, class Less>
    void build_heap(RandomAccessIterator first, RandomAccessIterator beyond, Less less) {

        int size = std::distance(first, beyond);
        auto it = std::next(first);

        for(int i = 1; i < size; i++) { // i = index of it

            if(less(*std::next(first, (i - 1) / 2), *it)) {

                int j = i;

                while(less(*std::next(first, (j - 1) / 2), *std::next(first, j))) {
                    std::iter_swap(std::next(first, j), std::next(first, (j - 1) / 2));
                    j = (j - 1) / 2;
                }
            }
            it++;
        }
    }
    template <class RandomAccessIterator, class Less>
    void sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less ) {

        build_heap(first, beyond, less);

        for(auto it = beyond - 1; it >= first; it--) {

            std::iter_swap(first, it);

            RandomAccessIterator j = first, lchild;

            do 
            {
                auto diff = std::distance(first, j);
                lchild = std::next(first, 2*diff + 1);

                if(less(*lchild, *std::next(lchild)) && std::distance(lchild, it - 1) > 0) {
                    std::advance(lchild, 1);
                }

                if(less(*j, *lchild) && std::distance(lchild, it) > 0) {
                    std::iter_swap(j, lchild);
                }

                j = lchild;
                
            } while (std::distance(lchild, it) > 0);
        }
    }

    template <class InputIterator, class OutputIterator, class Less>
    OutputIterator sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less) {

        int n = std::distance(first, beyond);
        std::vector<typename std::iterator_traits<InputIterator>::value_type> vec(first, beyond);
        build_heap(vec.begin(), vec.end(), less);
        // std::cout << "za" << std::endl;
        //for(auto& a: vec) {
        //    std::cout << a << std::endl;
        //}
        std::advance(result, n - 1);
        for(int i = n - 1; i > 0; i--) {

            std::iter_swap(vec.begin(), std::next(vec.begin(), i));
            *(result) = *std::next(vec.begin(), i);
            result--;

            int j = 0; 
            int index = 0;
            while (index < i) {

                index = 2 * j + 1;

                if(less(*std::next(vec.begin(), index), *std::next(vec.begin(), index + 1)) && index < (i - 1)) {
                    index++;
                }
                if(less(*std::next(vec.begin(), j), *std::next(vec.begin(), index)) && index < i) {
                    std::iter_swap(std::next(vec.begin(), j), std::next(vec.begin(), index));
                }
                
                j = index;
            }
        }
        result++;
        return result;
    }
}

#endif
