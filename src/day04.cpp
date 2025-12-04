#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

using std::string;

namespace r = std::ranges;
namespace rv = std::ranges::views;

class Grid
{
	public:
	Grid() = default;
	Grid(int width, int height) : width(width), height(height)
	{
		this->data.resize(this->width * this->height);
	};

	auto operator[](int x, int y) -> int&
	{
		sentinal = 0;
		if (x >= 0 && x < this->width && y >= 0 && y < this->height)
			return data[(y * this->width) + x];
		else
			return sentinal;
	};
	auto AddRow(auto row) -> void { this->data.append_range(row); }

	auto begin() { return data.begin(); }
	auto end() { return data.end(); }

	int width{0};
	int height{0};

	private:
	std::vector<int> data;
	int sentinal{0};
};

auto main() -> int
{
	Grid data{};

	std::ifstream input{RESOURCES_PATH "input_day4.txt"};
	if (input.is_open())
	{
		auto zeros = [](auto val) -> char
		{
			return val == '.' ? '0' : val;
		};
		auto ones = [](auto val) -> char
		{
			return val == '@' ? '1' : val;
		};
		string line;
		while (input >> line)
		{
			if (data.width == 0)
				data.width = line.length();
			data.height++;

			auto row = line
					   | rv::transform(zeros)
					   | rv::transform(ones)
					   | rv::transform([](auto val) { return string(1, val); })
					   | rv::transform([](auto val) { return std::stoi(val); })
					   | r::to<std::vector<int>>();
			data.AddRow(row);
		}
	}

	auto accumulator{0};
	auto count{-1};

	while (count != 0)
	{
		Grid sums{data.width, data.height};
		for (int y{0}; y < data.height; y++)
		{
			for (int x{0}; x < data.width; x++)
			{
				if (data[x, y] == 0)
					continue;

				for (int i{-1}; i < 2; i++)
				{
					for (int j{-1}; j < 2; j++)
					{
						sums[x, y] += data[x + i, y + j];
					}
				}
			}
		}
		std::cout << '\n';
		for (int y{0}; y < data.height; y++)
		{
			for (int x{0}; x < data.width; x++)
			{
				int val = sums[x, y];
				if (val == 0)
				{
					std::cout << '.';
					continue;
				}
				if (val < 5 && val > 0)
				{
					data[x, y] = 0;
					std::cout << "\033[1;34;32m";
				}
				else
				{
					std::cout << "\033[1;34;31m";
				}
				std::cout << sums[x, y];
			}
			std::cout << "\033[0m\n";
		}
		auto sum = sums
				   | rv::filter([](auto val) { return val > 0; })
				   | rv::filter([](auto val) { return val < 5; })
				   | r::to<std::vector<int>>();
		count = sum.size();
		std::cout << count << '\n';
		accumulator += count;
	}
	std::cout << "\033[1;34;32m" << accumulator << '\n';
	std::cout << "\033[0m\n";
}
