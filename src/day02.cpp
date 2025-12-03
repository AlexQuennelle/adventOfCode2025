#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

auto main() -> int
{
	namespace r = std::ranges;
	namespace rv = std::ranges::views;
	std::string data;
	std::ifstream input{RESOURCES_PATH "input_day2.txt"};
	if (input.is_open())
	{
		std::string line;
		while (std::getline(input, line))
		{
			data += line;
		}
	}
	// data = "11-22,95-115"
	// 	   ",998-1012,1188511880-1188511890,222220-222224,1698522-"
	// 	   "1698528,446443-446449,38593856-38593862,565653-565659, "
	// 	   "824824821-824824827,2121212118-2121212124";

	auto test3 = [](auto val) -> std::vector<int64_t>
	{
		auto stringify = [](auto val) -> std::string
		{
			return r::to<std::string>(val);
		};
		auto range
			= val
			  | rv::split('-')
			  | rv::transform(stringify)
			  | rv::transform([](const auto& str) { return std::stoll(str); })
			  | r::to<std::vector<int64_t>>();
		return range;
	};
	auto numbers = data
				   | rv::split(',')
				   | rv::transform(test3)
				   | rv::join
				   | r::to<std::vector<int64_t>>();

	int64_t accumulator = 0;
	for (auto val : numbers | rv::pairwise | rv::stride(2))
	{
		auto range = rv::iota(std::get<0>(val), std::get<1>(val) + 1);
		for (auto num : range)
		{
			std::string str = std::to_string(num);
			if (str.length() % 2 != 0)
				continue;

			auto midpoint = str.length() / 2;
			if (str.substr(0, midpoint) == str.substr(midpoint))
			{
				accumulator += num;
				std::cout << str << '\n';
			}
		}
	}
	std::cout << '\n';
	std::cout << "part 1: " << accumulator << '\n';
}
