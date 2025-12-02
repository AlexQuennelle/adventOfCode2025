#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <ranges>
#include <string>
#include <vector>

constexpr int DialSize = 100;

class Dial
{
	public:
	auto GetCountPart1() const -> int { return this->countPart1; }
	auto GetCountPart2() const -> int { return this->countPart2; }

	void operator+=(const int num)
	{
		int loop = std::abs(num) / DialSize;
		if (num < 0)
		{
			int size = dialPos == 0 ? DialSize : dialPos;
			if ((std::abs(num) % DialSize) >= size)
				loop++;
		}
		else
		{
			if (num % DialSize >= (DialSize - dialPos))
				loop++;
		}

		this->dialPos
			= (((this->dialPos + num) % DialSize) + DialSize) % DialSize;

		countPart1 = dialPos == 0 ? countPart1 + 1 : countPart1;
		countPart2 += loop;
	}
	friend auto operator<<(std::ostream& os, const Dial dial) -> std::ostream&
	{
		os << dial.dialPos;
		return os;
	}

	private:
	int dialPos{50};
	int countPart1{0};
	int countPart2{0};
};

auto ReadFile(const std::string& filepath) -> std::vector<std::string>
{
	std::vector<std::string> data(0);

	std::ifstream input{filepath};
	if (input.is_open())
	{
		std::string value;
		while (input >> value)
		{
			data.push_back(value);
		}
	}
	else
	{
		std::cerr << "ERROR: Unable to open file at " << filepath << '\n';
	}
	return data;
}

auto main() -> int
{
	namespace r = std::ranges;
	namespace rv = std::ranges::views;

	auto data = ReadFile(RESOURCES_PATH "input_day1.txt");

	auto splitData = [](auto val) -> std::pair<char, int>
	{
		return {val[0], std::stoi(rv::drop(val, 1) | r::to<std::string>())};
	};
	auto convertData = [](auto pair) -> int
	{
		auto [sign, num] = pair;
		if (sign == 'L')
			num = num * -1;
		return num;
	};
	auto turns = data | rv::transform(splitData) | rv::transform(convertData);

	Dial dial;
	for (const auto turn : turns)
	{
		dial += turn;
	}
	std::cout <<"Part 1: "<< dial.GetCountPart1() << '\n';
	std::cout <<"Part 2: "<< dial.GetCountPart2() << '\n';
}
