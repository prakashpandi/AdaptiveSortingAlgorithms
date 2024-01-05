#pragma once
namespace KLE
{
	/// <summary>
	/// find smallest entry and write it
	/// </summary>
	/// <typeparam name="InputIterator"></typeparam>
	/// <typeparam name="OutputIterator"></typeparam>
	/// <typeparam name="Less"></typeparam>
	/// <param name="first"></param>
	/// <param name="beyond"></param>
	/// <param name="result"></param>
	/// <param name="less"></param>
	/// <returns>last entry of result</returns>
	template<typename InputIterator, typename OutputIterator, typename Less>
	OutputIterator
		sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less) {
		//first mark positions of gaps
		std::list<InputIterator> markings;
		auto i = first;
		auto j = i;
		j++;
		markings.push_back(i);
		while (j != beyond) {
			if (!(less(*i, *j)))
				markings.push_back(j);
			i++;
			j++;
		}
		if (markings.size() == 1) {
			for (auto write = first; write != beyond; write++) {
				*result = *write;
				result++;
			}
		}
		else {
			//iterrate over markings
			auto writer = first;
			while (writer != beyond) {
				auto smallest = markings.end();
				auto reader = markings.begin();
				//read first element
				while (smallest == markings.end()) {
					if (*reader != beyond)
						smallest = reader;
					reader++;
				}
				while (reader != markings.end()) {
					if (*reader != beyond && less(**reader, **smallest))
						smallest = reader;
					reader++;
				}
				*result = **smallest;
				writer++;
				result++;
				auto lockup = *smallest;
				lockup++;
				if (lockup != beyond && less(**smallest, *lockup))
					(*smallest)++; //next element in bereich
				else
					*smallest = beyond; //close part
			}
		}
		return result;
	}

		/// <summary>
		/// bubble sort
		/// </summary>
		/// <typeparam name="RandomAccessIterator"></typeparam>
		/// <typeparam name="Less"></typeparam>
		/// <param name="first"></param>
		/// <param name="beyond"></param>
		/// <param name="less"></param>
		template<typename RandomAccessIterator, typename Less>
	void
		sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less) {
		bool sorted = false;
		while (!sorted) {
			auto j = first;
			sorted = true;
			for (int i=0; j!=beyond; i++) {
				j++;
				if (j!=beyond && less((first[i + 1]), first[i])) {
					std::swap(first[i], first[i + 1]);
					sorted = false;
				}
			}
		}
	}

}