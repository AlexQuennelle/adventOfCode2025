#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>

using std::string;
using std::string_view;

auto GetDigits(const string_view str, const uint64_t length) -> string
{
	namespace r = std::ranges;
	namespace rv = std::ranges::views;

	if (length <= 0)
		return "";
	if (str.length() <= length)
		return string(str);

	auto view = str.substr(0, str.length() - (length - 1));
	auto zipped = rv::zip_transform([](auto a, auto b) -> auto
									{ return std::pair<char, uint64_t>(a, b); },
									view, rv::iota(0));
	auto [digit, index]
		= *r::max_element(zipped, {}, &std::pair<char, uint64_t>::first);
	return string(1, digit).append(
		GetDigits(str.substr(index + 1ul), length - 1ul));
}

auto main() -> int
{
	uint64_t part1{0};
	uint64_t part2{0};
	std::ifstream input{RESOURCES_PATH "input_day3.txt"};
	if (input.is_open())
	{
		std::string line;
		while (input >> line)
		{
			string str1 = GetDigits(line, 2);
			part1 += std::stoull(str1);
			string str2 = GetDigits(line, 12);
			part2 += std::stoull(str2);
		}
	}
	std::cout << "Part 1: " << part1 << '\n';
	std::cout << "Part 2: " << part2 << '\n';
}
