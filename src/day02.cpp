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

	auto processInput = [](auto val) -> std::vector<int64_t>
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
				   | rv::transform(processInput)
				   | rv::join
				   | r::to<std::vector<int64_t>>();

	int64_t accumulator1 = 0;
	int64_t accumulator2 = 0;
	for (auto val : numbers | rv::pairwise | rv::stride(2))
	{
		auto range = rv::iota(std::get<0>(val), std::get<1>(val) + 1);
		for (auto num : range)
		{
			std::string str = std::to_string(num);

			auto splits = rv::iota(2l, static_cast<int64_t>(str.length()) + 1)
						  | rv::filter([len = str.length()](auto num) -> auto
									   { return (len % num == 0); });
			for (auto split : splits)
			{
				std::string val = str.substr(0, str.length() / split);
				auto pattern
					= rv::repeat(val, split) | rv::join | r::to<std::string>();
				if (pattern == str)
				{
					if (split == 2)
						accumulator1 += num;

					accumulator2 += num;
					break;
				}
			}
		}
	}
	std::cout << '\n';
	std::cout << "part 1: " << accumulator1 << '\n';
	std::cout << "part 2: " << accumulator2 << '\n';
}
