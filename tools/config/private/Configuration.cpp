
#include "Configuration.h"
#include <iostream>

namespace config
{

	std::string Configuration::get_string(const std::string& section, const std::string& name, const std::string& default_value) const
	{
		return _reader.GetString(section, name, default_value);
	}

	long Configuration::get_int(const std::string& section, const std::string& name, long default_value) const
	{
		return _reader.GetInteger(section, name, default_value);
	}

	double Configuration::get_real(const std::string& section, const std::string& name, double default_value) const
	{
		return _reader.GetReal(section, name, default_value);
	}

	bool Configuration::get_bool(const std::string& section, const std::string& name, bool default_value) const
	{
		return _reader.GetBoolean(section, name, default_value);
	}

	bool Configuration::has_value(const std::string& section, const std::string& name) const
	{
		return _reader.HasValue(section, name);
	}

	
}
