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
	Image(const char * filename)
	{
		std::ifstream ifs{filename};
		int r, c;
		ifs >> r >> c;
		_image.resize(r);
		for (auto & row : _image)
			row.resize(c);
		for (auto & row : _image)
			for (auto & column : row)
				ifs >> column;
	}

	~Image() = default;

	Image(Image && source)
		: _image{std::forward<std::vector<std::vector<int>>>(source._image)}
		, _equivalent{std::forward<std::vector<int>>(source._equivalent)}
	{}

	Image & operator=(Image && source)
	{
		_image = std::forward<std::vector<std::vector<int>>>(source._image);
		_equivalent = std::forward<std::vector<int>>(source._equivalent);
		return *this;
	}

	Image connected_components() const
	{
		Image frame = make_frame();
		int labels = frame.first_pass();
		frame._equivalent.resize(labels+1);
		for (size_type i = 0; i < frame._equivalent.size(); ++i)
			frame._equivalent[i] = i;
		frame.second_pass();
		frame.manage_equivalent_labels();
		labels = frame.third_pass();
		std::cout << "Connected Components: " << labels << '\n';
		return frame;
	}

	void print() const
	{
		constexpr int w = 4;
		for (auto & row : _image)
		{
			for (auto & column : row)
			{
				std::cout << std::setw(w);
				if (column != 0)
					std::cout << column;
				else
					std::cout << ' ';
			}
			std::cout << '\n';
		}
	}

	void print(const char * filename) const
	{
		constexpr int w = 4;
		std::ofstream ofs(filename);
		for (auto & row : _image)
		{
			for (auto & column : row)
			{
				ofs << std::setw(w);
				if (column != 0)
					ofs << column;
				else
					ofs << ' ';
			}
			ofs << '\n';
		}
	}

	size_type rows() const
	{
		return _image.size();
	}

	size_type columns() const
	{
		return _image.begin()->size();
	}
private:
	Image() = default;

	Image make_frame() const
	{
		Image frame;
		frame._image.resize(rows()+2);
		for (auto & row : frame._image)
			row.resize(columns()+2);
		for (size_type r = 0; r < rows(); ++r)
			for (size_type c = 0; c < columns(); ++c)
				frame._image[r+1][c+1] = _image[r][c];
		frame.zero_edges();
		return frame;
	}

	void zero_edges()
	{
		std::for_each(
			_image.begin()->begin(),
			_image.begin()->end(),
			[](auto & column){column = 0;});

		std::for_each(
			_image.rbegin()->begin(),
			_image.rbegin()->end(),
			[](auto & column){column = 0;});

		std::for_each(
			_image.begin(),
			_image.end(),
			[](auto & row){*row.begin() = 0; *row.rbegin() = 0;});
	}

	int first_pass()
	{
		int count = 0;
		for (size_type r = 1; r < rows() - 1; ++r) {
		for (size_type c = 1; c < columns() - 1; ++c) {
		if (_image[r][c] != 0)
		{
			const int up = _image[r-1][c];
			const int left = _image[r][c-1];
			if ((up == 0) && (left == 0))
				_image[r][c] = ++count;
			else if ((up == 0) || (left == 0))
				_image[r][c] = std::max(up,left);
			else
				_image[r][c] = std::min(up,left);
		}}}
		return count;
	}

	void second_pass()
	{
		for (size_type r = rows() - 2; r > 0; --r) {
		for (size_type c = columns() - 2; c > 0; --c) {
		if (_image[r][c] != 0)
		{
			int & down = _image[r+1][c];
			int & right = _image[r][c+1];
			if ((down == 0) && (right == 0))
				continue;
			else if ((down == 0) || (right == 0))
				_image[r][c] = std::max(down,right);
			else
			{
				_equivalent[std::max(down,right)] = std::min(down,right);
				_image[r][c] = std::min(down,right);
				down = _image[r][c];
				right = _image[r][c];
			}
		}}}
	}

	void manage_equivalent_labels()
	{
		int unique = 0;
		for (size_type i = 1; i < _equivalent.size(); ++i)
		{
			if (_equivalent[i] == i)
				_equivalent[i] = ++unique;
			else
				_equivalent[i] = _equivalent[_equivalent[i]];
		}
	}

	int third_pass()
	{
		int components = 0;
		for (int r = 0; r < rows() - 1; ++r) {
		for (int c = 0; c < columns() - 1; ++c) {
		if (_image[r][c] != 0)
		{
			_image[r][c] = _equivalent[_image[r][c]];
			if (_image[r][c] > components)
				components = _image[r][c];
		}}}
		return components;
	}

	std::vector<std::vector<int>> _image;
	std::vector<int> _equivalent;
};

#endif
