#pragma once

#include <iostream>
#include <vector>

namespace   Zion
{
	class   InsertionSort
	{
	public:
		template <class T>
		static  void    sortHighToLow(std::vector<T>& list)
		{
			for (int i = 1; i < list.size(); i++)
			{
				T item = list[i];
				if (item.getDistance() > list[i - 1].getDistance())
					sortUpHighToLow(list, i);
			}
		}

		template <class T>
		static  void    sortUpHighToLow(std::vector<T>& list, int i)
		{
			T item = list[i];
			int attemptPos = i - 1;
			while (attemptPos != 0 && list[attemptPos - 1].getDistance() < item.getDistance())
				attemptPos--;
			list.erase(list.begin() + i);
			list.insert(list.begin() + attemptPos, item);
		}
	};
}