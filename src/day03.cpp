#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

auto main() -> int
{
	namespace r = std::ranges;
	namespace rv = std::ranges::views;
#ifdef PART1

	int accumulator = 0;
	std::ifstream input{RESOURCES_PATH "input_day3.txt"};
	// std::ifstream input{RESOURCES_PATH "test3.txt"};
	if (input.is_open())
	{
		std::string line;
		while (input >> line)
		{
			auto br = rv::zip(
				line
					| rv::transform([](auto num)
									{ return std::string(1, num); })
					| rv::transform([](auto num) { return std::stoi(num); })
					| r::to<std::vector<int>>(),
				rv::iota(0));
			auto bank = br | r::to<std::vector<std::tuple<int, int>>>();
			auto filterDecreasing = [](auto pair) -> bool
			{
				auto [fst, snd] = pair;
				return std::get<1>(fst) < std::get<1>(snd);
			};
			auto pullOutVals = [](auto pair)
			{
				auto [fst, snd] = pair;
				return std::to_string(std::get<0>(fst))
					   + std::to_string(std::get<0>(snd));
			};
			auto test = rv::cartesian_product(bank, bank)
						| rv::filter(filterDecreasing)
						| rv::transform(pullOutVals)
						| rv::transform([](auto num) { return std::stoi(num); })
						| r::to<std::vector<int>>();
			accumulator += *r::max_element(test, {});
			std::cout << *r::max_element(test, {}) << '\n';
		}
	}
	std::cout << accumulator << '\n';
#endif // PART1
	// std::ifstream input{RESOURCES_PATH "input_day3.txt"};
	std::ifstream input{RESOURCES_PATH "test3.txt"};
	if (input.is_open())
	{
		std::string line;
		while (input >> line)
		{
		}
	}
}
