#include "Image.hpp"

#include <iostream>

int main(int argc, char * argv[])
{
	if (argc != 3)
	{
		std::cout << "Enter input filename and output filename.\n";
		return argc;
	}
	Image(argv[1]).connected_components().print(argv[2]).print();
	return 0;
}
