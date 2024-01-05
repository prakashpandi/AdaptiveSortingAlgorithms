namespace TMA
{
	template<typename RandomAccessIterator, typename Less>
	void sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less) {
		std::vector<unsigned short int> sorted_parts;
		std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type> first_container(first, beyond);
		unsigned short int container_length = std::distance(first, beyond);
		unsigned short int counter = 0;

		//go through all elements, to sort them into sorted parts
		while (counter < container_length - 1)
		{
			//ascending parts --> copy as they are
			if (!less(first_container[counter + 1], first_container[counter]))
			{
				sorted_parts.push_back(counter);
				while (!less(first_container[counter + 1], first_container[counter]))
				{
					counter++;
					if (counter == container_length - 1)
					{
						break;
					}
				}
				counter++;
			}
			//descending parts --> reverse and copy
			else
			{
				unsigned short int first_element = counter;
				sorted_parts.push_back(counter);
				while (!less(first_container[counter], first_container[counter + 1]))
				{
					counter++;
					if (counter == container_length - 1)
					{
						break;
					}
				}
				counter++;
				std::reverse(first_container.begin() + first_element, first_container.begin() + counter);
			}
		}
		//in case the last element does not belong to the last sorted part, add it as a further part
		if (counter == container_length - 1)
		{
			sorted_parts.push_back(counter);
		}

		//check if already sorted (only one sorted part exists)
		if (sorted_parts.size() == 1)
		{
			std::copy(first_container.begin(), first_container.end(), first);
			return;
		}

		std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type> second_container(container_length);
		bool writing_to_first = false;
		unsigned short int counter_first_sequence;
		unsigned short int end_first_sequence;
		unsigned short int counter_second_sequence;
		unsigned short int end_second_sequence;

		while (sorted_parts.size() > 1)
		{
			unsigned short int length = sorted_parts.size();

			if (writing_to_first == false)
			{
				counter = 0;
				for (unsigned short int i = 0; i < length - 1; i += 2)
				{
					counter_first_sequence = sorted_parts[i];
					end_first_sequence = sorted_parts[i + 1];
					counter_second_sequence = sorted_parts[i + 1];
					end_second_sequence;

					if (i == length - 2)
					{
						end_second_sequence = container_length;
					}
					else
					{
						end_second_sequence = sorted_parts[i + 2];
					}

					while ((counter_first_sequence < end_first_sequence) && (counter_second_sequence < end_second_sequence))
					{
						if (less(first_container[counter_first_sequence], first_container[counter_second_sequence]))
						{
							second_container[counter++] = first_container[counter_first_sequence++];
						}
						else
						{
							second_container[counter++] = first_container[counter_second_sequence++];
						}
					}
					while (counter_first_sequence < end_first_sequence)
					{
						second_container[counter++] = first_container[counter_first_sequence++];
					}
					while (counter_second_sequence < end_second_sequence)
					{
						second_container[counter++] = first_container[counter_second_sequence++];
					}
				}
				if (length % 2 == 1) {
					end_second_sequence = container_length;
					while (counter_second_sequence < end_second_sequence)
					{
						second_container[counter++] = first_container[counter_second_sequence++];
					}
				}
				for (int i = 0; i < length / 2; i++)
				{
					sorted_parts.erase(sorted_parts.begin() + i + 1);
				}
				writing_to_first = true;
			}
			else
			{
				counter = 0;
				for (unsigned short int i = 0; i < length - 1; i += 2)
				{
					counter_first_sequence = sorted_parts[i];
					end_first_sequence = sorted_parts[i + 1];
					counter_second_sequence = sorted_parts[i + 1];
					end_second_sequence;

					if (i == length - 2)
					{
						end_second_sequence = container_length;
					}
					else
					{
						end_second_sequence = sorted_parts[i + 2];
					}

					while ((counter_first_sequence < end_first_sequence) && (counter_second_sequence < end_second_sequence))
					{
						if (less(second_container[counter_first_sequence], second_container[counter_second_sequence]))
						{
							first_container[counter++] = second_container[counter_first_sequence++];
						}
						else
						{
							first_container[counter++] = second_container[counter_second_sequence++];
						}
					}
					while (counter_first_sequence < end_first_sequence)
					{
						first_container[counter++] = second_container[counter_first_sequence++];
					}
					while (counter_second_sequence < end_second_sequence)
					{
						first_container[counter++] = second_container[counter_second_sequence++];
					}
				}
				if (length % 2 == 1) {
					counter_second_sequence = sorted_parts[length - 1];
					while (counter_second_sequence < container_length)
					{
						first_container[counter++] = second_container[counter_second_sequence++];
					}
				}
				for (int i = 0; i < length / 2; i++)
				{
					sorted_parts.erase(sorted_parts.begin() + i + 1);
				}
				writing_to_first = false;
			}
		}

		if (writing_to_first == true)
		{
			std::copy(second_container.begin(), second_container.end(), first);
		}
		else
		{
			std::copy(first_container.begin(), first_container.end(), first);
		}
		return;
	}

	template<typename InputIterator, typename OutputIterator, typename Less>
	OutputIterator sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less) {
		std::vector<unsigned short int> sorted_parts;
		std::vector<typename std::iterator_traits<InputIterator>::value_type> first_container(first, beyond);
		unsigned short int container_length = std::distance(first, beyond);
		unsigned short int counter = 0;

		//go through all elements, to sort them into sorted parts
		while (counter < container_length - 1)
		{
			//ascending parts --> copy as they are
			if (!less(first_container[counter + 1], first_container[counter]))
			{
				sorted_parts.push_back(counter);
				while (!less(first_container[counter + 1], first_container[counter]))
				{
					counter++;
					if (counter == container_length - 1)
					{
						break;
					}
				}
				counter++;
			}
			//descending parts --> reverse and copy
			else
			{
				unsigned short int first_element = counter;
				sorted_parts.push_back(counter);
				while (!less(first_container[counter], first_container[counter + 1]))
				{
					counter++;
					if (counter == container_length - 1)
					{
						break;
					}
				}
				counter++;
				std::reverse(first_container.begin() + first_element, first_container.begin() + counter);
			}
		}
		//in case the last element does not belong to the last sorted part, add it as a further part
		if (counter == container_length - 1)
		{
			sorted_parts.push_back(counter);
		}

		//check if already sorted (only one sorted part exists)
		if (sorted_parts.size() == 1)
		{
			std::copy(first_container.begin(), first_container.end(), result);
			return result;
		}

		std::vector<typename std::iterator_traits<InputIterator>::value_type> second_container(container_length);
		bool writing_to_first = false;
		unsigned short int counter_first_sequence;
		unsigned short int end_first_sequence;
		unsigned short int counter_second_sequence;
		unsigned short int end_second_sequence;

		while (sorted_parts.size() > 1)
		{
			unsigned short int length = sorted_parts.size();

			if (writing_to_first == false)
			{
				counter = 0;
				for (unsigned short int i = 0; i < length - 1; i += 2)
				{
					counter_first_sequence = sorted_parts[i];
					end_first_sequence = sorted_parts[i + 1];
					counter_second_sequence = sorted_parts[i + 1];
					end_second_sequence;

					if (i == length - 2)
					{
						end_second_sequence = container_length;
					}
					else
					{
						end_second_sequence = sorted_parts[i + 2];
					}

					while ((counter_first_sequence < end_first_sequence) && (counter_second_sequence < end_second_sequence))
					{
						if (less(first_container[counter_first_sequence], first_container[counter_second_sequence]))
						{
							second_container[counter++] = first_container[counter_first_sequence++];
						}
						else
						{
							second_container[counter++] = first_container[counter_second_sequence++];
						}
					}
					while (counter_first_sequence < end_first_sequence)
					{
						second_container[counter++] = first_container[counter_first_sequence++];
					}
					while (counter_second_sequence < end_second_sequence)
					{
						second_container[counter++] = first_container[counter_second_sequence++];
					}
				}
				if (length % 2 == 1) {
					end_second_sequence = container_length;
					while (counter_second_sequence < end_second_sequence)
					{
						second_container[counter++] = first_container[counter_second_sequence++];
					}
				}
				for (int i = 0; i < length / 2; i++)
				{
					sorted_parts.erase(sorted_parts.begin() + i + 1);
				}
				writing_to_first = true;
			}
			else
			{
				counter = 0;
				for (unsigned short int i = 0; i < length - 1; i += 2)
				{
					counter_first_sequence = sorted_parts[i];
					end_first_sequence = sorted_parts[i + 1];
					counter_second_sequence = sorted_parts[i + 1];
					end_second_sequence;

					if (i == length - 2)
					{
						end_second_sequence = container_length;
					}
					else
					{
						end_second_sequence = sorted_parts[i + 2];
					}

					while ((counter_first_sequence < end_first_sequence) && (counter_second_sequence < end_second_sequence))
					{
						if (less(second_container[counter_first_sequence], second_container[counter_second_sequence]))
						{
							first_container[counter++] = second_container[counter_first_sequence++];
						}
						else
						{
							first_container[counter++] = second_container[counter_second_sequence++];
						}
					}
					while (counter_first_sequence < end_first_sequence)
					{
						first_container[counter++] = second_container[counter_first_sequence++];
					}
					while (counter_second_sequence < end_second_sequence)
					{
						first_container[counter++] = second_container[counter_second_sequence++];
					}
				}
				if (length % 2 == 1) {
					counter_second_sequence = sorted_parts[length - 1];
					while (counter_second_sequence < container_length)
					{
						first_container[counter++] = second_container[counter_second_sequence++];
					}
				}
				for (int i = 0; i < length / 2; i++)
				{
					sorted_parts.erase(sorted_parts.begin() + i + 1);
				}
				writing_to_first = false;
			}
		}

		if (writing_to_first == true)
		{
			std::copy(second_container.begin(), second_container.end(), result);
		}
		else
		{
			std::copy(first_container.begin(), first_container.end(), result);
		}
		return result;
	}
}
