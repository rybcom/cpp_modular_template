#include "Utilities.h"
#include <stdarg.h>
#include <memory>
#include <fstream>
#include <iostream>

namespace aux
{

#pragma region string operations

	std::string string_format(const std::string fmt_str, ...)
	{
		int final_n, n = ((int)fmt_str.size()) * 2;
		std::unique_ptr<char[]> formatted;
		va_list ap;
		while (1) {
			formatted.reset(new char[n]);
			strcpy(&formatted[0], fmt_str.c_str());
			va_start(ap, fmt_str);
			final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
			va_end(ap);
			if (final_n < 0 || final_n >= n)
				n += abs(final_n - n + 1);
			else
				break;
		}
		return std::string(formatted.get());
	}

	void split(std::string const& text, char delim, std::vector<std::string> & elems)
	{
		std::stringstream ss;
		ss.str(text);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
	}

	std::vector<std::string> split(std::string const& text, char delim)
	{
		std::vector<std::string> elems;
		aux::split(text, delim, elems);
		return elems;
	}

	std::wstring string_to_wstring(std::string const& s)
	{
		std::wstring temp(s.length(), L' ');
		std::copy(s.begin(), s.end(), temp.begin());
		return temp;
	}

	std::string wstring_to_string(std::wstring const& s)
	{
		std::string temp(s.length(), ' ');
		std::copy(s.begin(), s.end(), temp.begin());
		return temp;
	}

	void check_rotation_bounds(Rotation & rotation)
	{
		check_yaw_value(rotation.h);
		check_pitch_value(rotation.p);
	}

#pragma endregion



}