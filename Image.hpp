#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <type_traits>

class Image
{
	typedef std::vector<std::vector<int>>::size_type size_type;
public:
	Image(const char *);
	~Image() = default;
	Image(Image &&);
	Image & operator=(Image &&);
	Image connected_components() const;
	const Image & print() const;
	const Image & print(const char *) const;
	size_type rows() const;
	size_type columns() const;
private:
	Image() = default;
	Image make_frame() const;
	void zero_edges();
	int first_pass();
	void second_pass();
	void manage_equivalent_labels();
	int third_pass();
	std::vector<std::vector<int>> _image;
	std::vector<int> _equivalent;
	static constexpr int _w = 3;
};

#endif
