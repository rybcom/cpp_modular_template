#include "Utilities.h"
#include <stdarg.h>
#include <memory>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <Windows.h>
#include <filesystem>

namespace aux
{

#pragma region string operations

	std::optional<std::string> get_command_parameter(std::size_t param_index)
	{
		auto params = aux::split(GetCommandLineA(), ' ');

		if (params.size() > (param_index +1))
		{
			return params[(param_index + 1)];
		}

		return	{};
	}


	std::string get_config_file(std::string const & file)
	{
		if (auto ig_param = get_command_parameter(0))
		{
			std::string path = R"(..\init\)" + ig_param.value() + R"(\)" + file;
		
			if (std::filesystem::exists(path))
			{
				return path;
			}
		}

		return  R"(..\init\)" + file;
	}

	std::string to_lower_case(std::string const  & text)
	{
		auto newText = text;
		std::transform(newText.begin(), newText.end(), newText.begin(),
			[](unsigned char c) { return std::tolower(c); });

		return newText;
	}

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

	void shell_execute(std::string const& file, bool show_window)
	{
		ShellExecute(0, 0, aux::string_to_wstring(file).c_str(), 0, 0, show_window ? SW_SHOW : SW_HIDE);
	}

	void shell_execute(std::string const& file, std::string const & params, bool show_window)
	{
		ShellExecute(0, 0, aux::string_to_wstring(file).c_str(), aux::string_to_wstring(params).c_str(), 0, show_window ? SW_SHOW : SW_HIDE);
	}

	int get_int_from_color(Color const & color)
	{
		int red = int(color.x * 255) << 24;
		int green = int(color.y * 255) << 16;
		int blue = int(color.z * 255) << 8;
		int alpha = int(color.w) * 255;

		return red | green | blue | alpha;
	}

#pragma endregion





}