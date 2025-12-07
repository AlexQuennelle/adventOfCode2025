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

class Column
{
	public:
	auto Transpose() -> Column
	{
		vector<string> newData(this->width, string(height, ' '));
		for (uint64_t i{0}; i < this->width; i++)
		{
			for (uint64_t j{0}; j < this->height; j++)
			{
				newData[i][j] = data[j][i];
			}
		}
		return {.data = newData, .width = this->height, .height = this->width};
	}
	auto ToNums() -> vector<int64_t>
	{
		auto clean = [](string val) -> string
		{
			return val
				   | rv::filter([](auto ch) { return ch != ' '; })
				   | r::to<string>();
		};
		return this->data
			   | rv::transform(clean)
			   | rv::transform([](const auto& val) { return std::stoll(val); })
			   | r::to<vector<int64_t>>();
	}

	vector<string> data;
	uint64_t width{0};
	uint64_t height{0};
};

auto GetColWidths(std::ifstream& input) -> vector<int64_t>
{
	vector<int> nums;
	input.seekg(-1, input.end);
	if (input.peek() == '\n')
	{
		input.seekg(-1, input.cur);
		int64_t i = input.tellg();
		int col = 0;
		for (i; i > 0; i--)
		{
			int curr = input.peek();
			if (curr == '\n')
				break;
			else if (curr != ' ')
			{
				nums.push_back(col);
				col = 0;
			}
			else
			{
				col++;
			}
			input.seekg(i, input.beg);
		}
	}
	input.seekg(0, input.beg);
	return rv::reverse(nums) | r::to<vector<int64_t>>();
}

auto main() -> int
{
	vector<vector<int64_t>> lines;
	vector<op> ops;
	vector<int64_t> colWidths;
	vector<Column> columns;

	std::ifstream input{RESOURCES_PATH "input_day6.txt"};
	if (input.is_open())
	{
		colWidths = GetColWidths(input);
		columns = vector<Column>(colWidths.size());
		for (uint64_t i{0}; i < columns.size(); i++)
		{
			columns[i].width = static_cast<uint64_t>(colWidths[i]);
		}
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
				auto lineM = line;
				for (auto& column : columns)
				{
					column.data.push_back(lineM.substr(0, column.width));
					lineM.erase(0, column.width + 1);
					column.height++;
				}
			}
		}
	}
	auto foldFunc = [](auto pair) -> int64_t
	{
		auto [nums, op] = pair;
		return r::fold_left(nums, op.init, op.func);
	};
	auto part1Cols
		= rv::zip(columns
					  | rv::transform([](auto col) { return col.ToNums(); }),
				  ops)
		  | rv::transform(foldFunc);
	auto part2Cols
		= rv::zip(columns
					  | rv::transform([](auto col) { return col.Transpose(); })
					  | rv::transform([](auto col) { return col.ToNums(); }),
				  ops)
		  | rv::transform(foldFunc);
	auto output1 = r::fold_left(part1Cols, 0, std::plus<>());
	auto output2 = r::fold_left(part2Cols, 0, std::plus<>());
	std::cout << "Part 1: " << output1 << '\n';
	std::cout << "Part 2: " << output2 << '\n';
}
