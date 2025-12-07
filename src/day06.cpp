#include <algorithm>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <string>

using std::string;
using std::vector;

using fun = std::function<int64_t(int64_t, int64_t)>;

struct op
{
	fun func;
	int init;
};

namespace r = std::ranges;
namespace rv = std::ranges::views;

auto main() -> int
{
	vector<vector<int64_t>> lines;
	vector<op> ops;
	// std::ifstream input{RESOURCES_PATH "test6.txt"};
	std::ifstream input{RESOURCES_PATH "input_day6.txt"};
	if (input.is_open())
	{
		string line;
		while (std::getline(input, line))
		{
			if (line[0] == '*' || line[0] == '+')
			{
				auto convert = [](auto val) -> op
				{
					if (val == '*')
						return {.func = std::multiplies<>(), .init = 1};
					else
						return {.func = std::plus<>(), .init = 0};
				};
				ops = line
					  | rv::filter([](auto val) -> bool { return val != ' '; })
					  | rv::transform(convert)
					  | r::to<vector<op>>();
			}
			else
			{
				auto chunkFun = [](auto a, auto b)
				{
					return std::isalnum(a) && std::isalnum(b);
				};
				auto cleanArr = [](auto val) -> string
				{
					return val
						   | rv::filter([](auto a) { return a != ' '; })
						   | r::to<string>();
				};
				auto toNum = [](auto val) -> int64_t
				{
					return std::stoll(val);
				};
				auto nums = line
							| rv::chunk_by(chunkFun)
							| rv::transform(cleanArr)
							| rv::filter([](auto val) { return val != ""; })
							| rv::transform(toNum)
							| r::to<vector<int64_t>>();
				lines.push_back(nums);
			}
		}
	}
	uint64_t width = lines[0].size();
	uint64_t height = lines.size();
	vector<vector<int64_t>> cols(width, vector<int64_t>(height));
	for (uint64_t i{0}; i < width; i++)
	{
		for (uint64_t j{0}; j < height; j++)
		{
			cols[i][j] = lines[j][i];
		}
	}

	auto func = [](auto pair) -> int64_t
	{
		auto [nums, op] = pair;
		for (auto num : nums)
		{
			std::cout << num << ' ';
		}
		std::cout << "= ";
		std::cout << r::fold_left(nums, op.init, op.func) << '\n';
		return r::fold_left(nums, op.init, op.func);
	};
	auto test = r::fold_left(rv::zip(cols, ops) | rv::transform(func), 0,
							 std::plus<>());
	std::cout << test << '\n';
}
