#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional>
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
				ranges.emplace_back(range[0], range[1]);
			}
			else
			{
				inventory.push_back(std::stoll(line));
			}
		}
	}
	r::sort(ranges);
	r::sort(inventory);
	std::vector<std::pair<int64_t, int64_t>> freshIDs = ranges;
	for (auto i = freshIDs.begin(); i < freshIDs.end(); i++)
	{
		for (auto j = i + 1; j < freshIDs.end(); j++)
		{
			if (i->second >= j->first - 1)
			{
				i->second = std::max(i->second, j->second);
				freshIDs.erase(j);
				j--;
			}
			else
			{
				break;
			}
		}
	}
	int acc1{0};
	for (auto id : inventory)
	{
		for (auto range : freshIDs)
		{
			if (id >= range.first && id <= range.second)
			{

				acc1++;
				break;
			}
		}
	}
	std::cout << acc1 << '\n';
	auto countRange = [](auto pair) -> int64_t
	{
		return (pair.second + 1) - pair.first;
	};
	int64_t acc2
		= r::fold_left(freshIDs | rv::transform(countRange), 0, std::plus<>());
	std::cout << acc2 << '\n';
}
