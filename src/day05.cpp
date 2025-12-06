#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace r = std::ranges;
namespace rv = std::ranges::views;

using std::string;

auto main() -> int
{
	std::vector<std::pair<int64_t, int64_t>> ranges;
	std::vector<int64_t> inventory;

	std::ifstream input{RESOURCES_PATH "input_day5.txt"};
	// std::ifstream input{RESOURCES_PATH "test5.txt"};
	if (input.is_open())
	{
		bool swapProcessing{false};
		string line;
		while (std::getline(input, line))
		{
			if (line == "")
			{
				swapProcessing = true;
				continue;
			}

			if (!swapProcessing)
			{
				auto chunkFun = [](auto a, auto b)
				{
					return std::isalnum(a) && std::isalnum(b);
				};
				auto range
					= line
					  | rv::chunk_by(chunkFun)
					  | rv::transform([](auto val)
									  { return val | r::to<string>(); })
					  | rv::filter([](auto val) { return val != "-"; })
					  | rv::transform([](auto val) { return std::stoll(val); })
					  | r::to<std::vector<int64_t>>();
				std::cout << range[0] << ' ' << range[1] << '\n';
				ranges.emplace_back(range[0], range[1]);
			}
			else
			{
				inventory.push_back(std::stoll(line));
				std::cout << line << '\n';
			}
		}
	}
	r::sort(ranges);
	r::sort(inventory);
	for (auto pair : ranges)
	{
		std::cout << pair.first << "<->" << pair.second << '\n';
	}
	std::cout << ranges.size() << '\n';
	auto rangeOverlap = [](auto a, auto b) -> bool
	{
		return a.second >= b.first;
	};
	auto mergeRanges = [](auto val) -> std::pair<int64_t, int64_t>
	{
		return {
			val.begin()->first,
			std::max(val.begin()->second, val.back().second),
		};
	};
	auto freshIDs = ranges
				| rv::chunk_by(rangeOverlap)
				| rv::transform(mergeRanges)
				| r::to<std::vector<std::pair<int64_t, int64_t>>>();
	for (auto pair : freshIDs)
	{
		std::cout << pair.first << "<->" << pair.second << '\n';
	}
	std::cout << freshIDs.size() << '\n';
	std::cout << '\n';
	int acc1{0};
	for (auto id : inventory)
	{
		std::cout << id;
		for (auto range : freshIDs)
		{
			if (id >= range.first && id <= range.second)
			{

				std::cout << ' ' << range.first << "<->" << range.second << "✓";
				acc1++;
				std::cout << acc1;
				break;
			}
		}
		std::cout << '\n';
	}
	std::cout << acc1 << '\n';
}
