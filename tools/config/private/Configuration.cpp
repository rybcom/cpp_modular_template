
#include "Configuration.h"
#include <iostream>
#include <sstream>
#include <regex>

namespace config
{

	namespace
	{

		inline void split(std::string const& text, char delim, std::vector<std::string> & elems)
		{
			std::stringstream ss;
			ss.str(text);
			std::string item;
			while (std::getline(ss, item, delim)) {
				elems.push_back(item);
			}
		}

		inline std::vector<std::string> split(std::string const& text, char delim)
		{
			std::vector<std::string> elems;
			split(text, delim, elems);
			return elems;
		}

		inline std::string remove_brackets(std::string const & text)
		{
			return std::regex_replace(text, std::regex(R"([{}()\[\]])"), "");
		}

	}

	std::string Configuration::get_string(const std::string& section, const std::string& name, const std::string& default_value) const
	{
		return _reader.GetString(section, name, default_value);
	}

	int Configuration::get_int(const std::string& section, const std::string& name, long default_value) const
	{
		return static_cast<int>(_reader.GetInteger(section, name, default_value));
	}

	unsigned int Configuration::get_unsigned_int(const std::string& section, const std::string& name, unsigned int default_value) const
	{
		return static_cast<unsigned int>(_reader.GetInteger(section, name, default_value));
	}

	double Configuration::get_real(const std::string& section, const std::string& name, double default_value) const
	{
		return _reader.GetReal(section, name, default_value);
	}

	float Configuration::get_float(const std::string& section, const std::string& name, float default_value) const
	{
		return static_cast<float>(_reader.GetReal(section, name, default_value));
	}


	template<class T, size_t Size>
	std::array<T, Size> Configuration::get_array(const std::string& section, const std::string& name, std::array<T, Size> default_value /*= {}*/) const
	{

		using TextElements = std::vector<std::string>;

		TextElements elements = split(remove_brackets(get_string(section, name)), ',');

		if (elements.empty())
		{
			return default_value;
		}

		std::array<T, Size> result;

		if (Size != elements.size())
		{
			return default_value;
		}

		for (size_t i = 0; i < Size; ++i)
		{
			try
			{
				result[i] = static_cast<T>(std::stof(elements[i]));
			}
			catch (...)
			{
				return default_value;
			}
		}

		return result;

	}

	template<class T>
	std::vector<T> Configuration::get_vector(const std::string& section, const std::string& name, std::vector<T> default_value /*= {}*/) const
	{
		using TextElements = std::vector<std::string>;

		TextElements elements = split(remove_brackets(get_string(section, name)), ',');

		if (elements.empty())
		{
			return default_value;
		}

		std::vector<T> result;

		for (auto const & element : elements)
		{
			try
			{
				result.push_back(static_cast<T>(std::stof(element)));
			}
			catch (...)
			{
				return default_value;
			}
		}

		return result;
	}

	template<>
	std::vector<std::string> Configuration::get_vector(const std::string& section, const std::string& name, std::vector<std::string> default_value /*= {}*/) const
	{
		using TextElements = std::vector<std::string>;

		TextElements elements = split(remove_brackets(get_string(section, name)), ',');

		if (elements.empty())
		{
			return default_value;
		}
		return elements;
	}

	bool Configuration::get_bool(const std::string& section, const std::string& name, bool default_value) const
	{
		return _reader.GetBoolean(section, name, default_value);
	}

	bool Configuration::has_value(const std::string& section, const std::string& name) const
	{
		return _reader.HasValue(section, name);
	}

	std::vector<std::string> Configuration::get_section_value_names(const std::string& section) const
	{
		return _reader.GetSectionValuesNames(section);
	}

	template std::array<double, 6> Configuration::get_array<double, 6>(const std::string&, const std::string&, std::array<double, 6>) const;
	template std::array<double, 4> Configuration::get_array<double, 4>(const std::string&, const std::string&, std::array<double, 4>) const;
	template std::array<double, 3> Configuration::get_array<double, 3>(const std::string&, const std::string&, std::array<double, 3>) const;
	template std::array<float, 3> Configuration::get_array<float, 3>(const std::string&, const std::string&, std::array<float, 3>) const;
	template std::array<int, 2> Configuration::get_array<int, 2>(const std::string&, const std::string&, std::array<int, 2>) const;
	template std::array<double, 2> Configuration::get_array<double, 2>(const std::string&, const std::string&, std::array<double, 2>) const;
	template std::vector<uint32_t> Configuration::get_vector<uint32_t>(const std::string&, const std::string&, std::vector<uint32_t>) const;

	template std::vector<uint8_t> Configuration::get_vector<uint8_t>(const std::string&, const std::string&, std::vector<uint8_t>) const;
	

}
