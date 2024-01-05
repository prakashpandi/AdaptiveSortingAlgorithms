
#pragma once
#include <iterator>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cassert>

const int MIN_RUN_LENGTH = 12;

namespace ESE
{


    /**
     *
     * @tparam InputIterator
     * @tparam OutputIterator
     * @tparam Less
     * @param first
     * @param beyond
     * @param result
     * @param less
     * @return Iterator for the sorted data
     *
     * Searches the data of the given iterator for both upwards and downwards runs.
     * Then merges runs one by one into a sorted list, that begins empty
     * Very small runs are collected in a vector to be sorted via mergesort in the end and then merged into the sorted list
     * Sorted list is copied into Iterator return Type.
     *
     * No optimisation implemented towards the order of the merged runs
     */
	template<typename InputIterator, typename OutputIterator, typename Less>
	OutputIterator
		sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less);

    /**
    Variant of MergeSort
    */
	template<typename RandomAccessIterator, typename Less>
	void
		sort_a(RandomAccessIterator first, RandomAccessIterator beyond, Less less);

    /**
     *
     * @tparam RandomAccessIterator
     * @tparam Less
     * @param first
     * @param beyond
     * @param less
     *
     * Searches the data of the given iterator for both upwards and downwards runs.
     * Then merges runs one by one into a sorted list, that begins empty
     * Very small runs are collected in a vector to be sorted via mergesort in the end and then merged into the sorted list
     * Sorted list is copied back to original iterator.
     *
     * No optimisation implemented towards the order of the merged runs
     */
    template<typename RandomAccessIterator, typename Less>
    void
        sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less);

    template<typename RandomAccessIterator, typename Less>
    std::tuple<std::vector<int>,std::vector<int>>
        get_run_lengths(RandomAccessIterator first, RandomAccessIterator beyond, Less less);

    template<typename RandomAccessIterator, typename Less>
    bool
    is_sorted(RandomAccessIterator first, RandomAccessIterator beyond, Less less);


	template<typename InputIterator, typename OutputIterator, typename Less>
	OutputIterator
		sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less) {
		if (first == beyond)
			return result;
        typedef typename std::iterator_traits<InputIterator>::value_type vType;
		std::vector<vType> Copy;
		std::copy(first, beyond, std::back_inserter(Copy));
        ESE::sort(Copy.begin(), Copy.end(), less);
		for (int i = 0; i < Copy.size(); ++i) {
			*result = Copy[i];
			++result;
		}
		return result;
	};

	template<typename RandomAccessIterator, typename Less>
	void
		sort_a(RandomAccessIterator first, RandomAccessIterator beyond, Less less) {
		int range = beyond - first;
		if (range < 2)
			return;
        typedef typename std::iterator_traits<RandomAccessIterator>::value_type vType;
		std::vector<vType> CopyFront;
		std::vector<vType> CopyBack;
		auto middle = first + range / 2;
		std::copy(first, middle, std::back_inserter(CopyFront));
		std::copy(middle, beyond, std::back_inserter(CopyBack));

        ESE::sort_a(CopyFront.begin(), CopyFront.end(), less);
        ESE::sort_a(CopyBack.begin(), CopyBack.end(), less);

		//merge both halfs
		int i = 0;
		int frontPos = 0;
		int backPos = 0;
		while (first + i != beyond) {
			if (frontPos == CopyFront.size()) {
				*(first + i) = CopyBack[backPos];
				backPos++;
			}
			else if (backPos == CopyBack.size()) {
				*(first + i) = CopyFront[frontPos];
				frontPos++;
			}
			else if (less(CopyBack[backPos], CopyFront[frontPos])) {
				*(first + i) = CopyBack[backPos];
				backPos++;
			}
			else {
				*(first + i) = CopyFront[frontPos];
				frontPos++;
			}
			++i;
		}
	}


    template<typename RandomAccessIterator, typename Less>
    void
        sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less){
        int index_up = 0;
        int index_down = 0;
        int index_iterator = 0;
        int start_of_current_down_run = 0;
        int start_of_current_up_run = 0;

        typedef typename std::iterator_traits<RandomAccessIterator>::value_type vType;
        std::list<vType> sorted_part;
        std::vector<vType> unsorted;

        std::tuple<std::vector<int>,std::vector<int>> runs = get_run_lengths(first,beyond,less);
        std::vector<int> runs_up = std::get<0>(runs);
        std::vector<int> runs_down = std::get<1>(runs);

        while (first+index_iterator < beyond){
            if(runs_up[index_up] > MIN_RUN_LENGTH){
                if(sorted_part.empty()){
                    for (int k = 0; k<runs_up[index_up];++k){
                        sorted_part.push_back(*(first+index_iterator+k));
                    }
                }else {
                    auto it = sorted_part.begin();
                    for (int k = 0; k < runs_up[index_up];) {
                        if (less(*(first + index_iterator + k),*it)||it==sorted_part.end()) {
                            sorted_part.insert(it, *(first + index_iterator + k));
                            ++k;
                        }else {
                            it = std::next(it);
                        }
                    }
                }
                index_down += runs_up[index_up]-1;
                --runs_down[index_down];
                index_iterator += runs_up[index_up];
                index_up++;
            }else if (runs_down[index_down] > MIN_RUN_LENGTH){
                if(sorted_part.empty()){
                    for (int k = runs_down[index_down]-1; k>=0;--k){
                        sorted_part.push_back(*(first+index_iterator+k));
                    }
                } else {
                    auto it = sorted_part.begin();
                    for (int k = runs_down[index_down] - 1; k >= 0;) {
                        if (less(*(first + index_iterator + k),*it)||it==sorted_part.end()) {
                            sorted_part.insert(it, *(first + index_iterator + k));
                            --k;
                        }else {
                            it = std::next(it);
                        }
                    }
                }
                index_up += runs_down[index_down]-1;
                --runs_up[index_up];
                index_iterator += runs_down[index_down];
                index_down++;
            }else{
                unsorted.push_back(*(first+index_iterator));
                if(--runs_up[index_up]==0){
                    ++index_up;
                }
                if(--runs_down[index_down]==0){
                    ++index_down;
                }
                ++index_iterator;
            }
        }

        ESE::sort_a(unsorted.begin(),unsorted.end(),less);
        auto it = sorted_part.begin();
        for (int k = 0; k<unsorted.size();){
            if(less(unsorted[k],*it)||it==sorted_part.end()){
                sorted_part.insert(it,unsorted[k]);
                ++k;
            }else {
                it = std::next(it);
            }
        }
        int i = 0;
        for (auto elem:sorted_part){
            *(first+i) = elem;
            ++i;
        }
    }

    template<typename RandomAccessIterator, typename Less>
    std::tuple<std::vector<int>,std::vector<int>>
    get_run_lengths(RandomAccessIterator first, RandomAccessIterator beyond, Less less){
        std::vector<int> run_lengths_up;
        std::vector<int> run_lengths_down;
        int cur_run_up = 1;
        int cur_run_down = 1;
        int i = 0;
        while(first + ++i != beyond){
        if (less(*(first+i),*(first+i-1))) { //step down
            run_lengths_up.push_back(cur_run_up);
            cur_run_up = 1;
            ++cur_run_down;
        } else {
            run_lengths_down.push_back(cur_run_down);
            cur_run_down = 1;
            ++ cur_run_up;
        }
        }
        run_lengths_up.push_back(cur_run_up);
        run_lengths_down.push_back(cur_run_down);
        return std::make_tuple(run_lengths_up,run_lengths_down);
    }

    template<typename RandomAccessIterator, typename Less>
    bool
    is_sorted(RandomAccessIterator first, RandomAccessIterator beyond, Less less){
        int i = 0;
        bool ret=true;
        while(first + ++i != beyond) {
            if (less(*(first + i), *(first + i - 1))) { //step down
                std::cout << i << std::endl;
                ret = false;
            }
        }
        return ret;
    }
}
