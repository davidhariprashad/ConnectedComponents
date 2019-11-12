#include <iostream>
#include <fstream>
#include <random>

constexpr double rate = 1.0/2.0;

int main()
{
	int r;
	std::cout << "Rows: ";
	std::cin >> r;

	int c;
	std::cout << "Columns: ";
	std::cin >> c;

	std::string filename;
	std::cout << "Filename: ";
	std::cin >> filename;

	std::ofstream ofs(filename.c_str());
	ofs << r << ' ' << c << '\n';

	std::default_random_engine engine{std::random_device{}()};
	std::uniform_real_distribution<double> distribution(0.0,1.0);

	for (int i = 0; i < r; ++i)
	{
		for (int j = 0; j < c; ++j)
			if (distribution(engine) <= rate)
				ofs << '1' << ' ';
			else
				ofs << '0' << ' ';
		ofs << '\n';
	}

	return 0;
}
