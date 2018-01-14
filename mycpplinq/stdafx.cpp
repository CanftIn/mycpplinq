// stdafx.cpp : source file that includes just the standard includes
// mycpplinq.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "cpplinq.hpp"
#include <iostream>  
#include <vector> 

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
template<typename TIterator>
struct from_range
{
	using value_type = typename std::iterator_traits<TIterator>::value_type;

	TIterator current, upcoming, end;

	from_range(TIterator begin, TIterator end)
		: current(begin), upcoming(begin), end(end) {}

	template<typename TRangeBuilder>
	decltype(auto) operator >> (TRangeBuilder builder) const
	{
		return builder.build(*this);
	}

	decltype(auto) front() const { return *current; }

	bool next() { return upcoming == end ? false : (current = upcoming++, true); }
};

template<typename TContainer>
auto from(TContainer const& container)
{
	return from_range<decltype(std::begin(container))>(std::begin(container), std::end(container));
}

struct to_vector_builder
{
	template<typename TRange>
	auto build(TRange range)
	{
		std::vector<TRange::value_type> result;
		while (range.next())
		{
			result.push_back(range.front());
		}
		return result;
	}
};

auto to_vector()
{
	return to_vector_builder();
}

int main()
{
	int a[] = { 1, 2, 3, 4 };  
	auto v = from(a) >> to_vector();
	for (int i : v) std::cout << i << std::endl;

}