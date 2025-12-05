#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace r = std::ranges;
namespace rv = std::ranges::views;

using std::string;

auto main() -> int
{
	// std::vector<std::pair<int64_t, int64_t>> ranges;
	std::unordered_set<int64_t> freshID;
	// std::map<int64_t, bool> inventory;
	int64_t acc{0};
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
				for (auto num : rv::iota(range[0],range[1]+1))
				{
					freshID.insert(num);
				}
				// ranges.emplace_back(range[0], range[1] + 1);
				// freshID.insert_range(rv::iota(range[0], range[1] + 1));
			}
			else
			{
				if (freshID.contains(std::stoll(line)))
					acc++;
				// inventory[std::stoll(line)] = false;
				// std::cout << line << '\n';
			}
		}
	}
	std::cout << '\n' << acc << '\n';
	// for (auto pair : ranges)
	// {
	// 	auto [start, end] = pair;
	// 	for (auto num : rv::iota(start, end))
	// 	{
	// 		inventory
	// 	}
	// }
}
