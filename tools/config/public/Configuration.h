#pragma once
#include <string>
#include <memory>
#include <array>
#include <vector>
#include <map>
#include "INIReader.h"


#pragma region macros

#define CONFIG_SET_SECTION(section) \
static char* current_config_section_name =section

#define CONFIG_LOAD_BOOl(name) \
name = get_bool(current_config_section_name, #name, name)

#define CONFIG_LOAD_FLOAT(name) \
name = get_float(current_config_section_name, #name, name)

#define CONFIG_LOAD_DOUBLE(name) \
name = get_real(current_config_section_name, #name, name)

#define CONFIG_LOAD_INT(name) \
name = get_int(current_config_section_name, #name, name)

#define CONFIG_LOAD_STRING(name) \
name = get_string(current_config_section_name, #name, name)

#define CONFIG_LOAD_GEOPOSITION(name) \
name = data::to_geoposition(get_array<double, 3>(current_config_section_name, #name))

#define CONFIG_LOAD_COLOR(name) \
name = data::to_color(get_array<double, 4>(current_config_section_name, #name)) 

#define CONFIG_LOAD_BOOl_FROM(name,config_name) \
name = get_bool(current_config_section_name, config_name, name)

#define CONFIG_LOAD_FLOAT_FROM(name,config_name) \
name = get_float(current_config_section_name, config_name, name)

#define CONFIG_LOAD_DOUBLE_FROM(name,config_name) \
name = get_real(current_config_section_name, config_name, name)

#define CONFIG_LOAD_INT_FROM(name,config_name) \
name = get_int(current_config_section_name, config_name, name)


#pragma endregion

namespace config
{

	class Configuration
	{
#pragma region api

	public:

		std::string  getConfigFilePath() const
		{
			return _configFilePath;
		};

		explicit Configuration(std::string const& configPath)
			:
			_configFilePath{ configPath },
			_reader{ configPath }
		{
		}

#pragma  endregion


#pragma region reading config file 

	protected:

		std::string get_string(const std::string& section, const std::string& name, const std::string& default_value = {}) const;

		int get_int(const std::string& section, const std::string& name, long default_value = {}) const;

		unsigned int get_unsigned_int(const std::string& section, const std::string& name, unsigned int default_value = {}) const;

		double get_real(const std::string& section, const std::string& name, double default_value = {}) const;

		float get_float(const std::string& section, const std::string& name, float default_value = {}) const;

		template<class T, size_t Size>
		std::array<T, Size>  get_array(const std::string& section, const std::string& name, std::array<T, Size>  default_value = {}) const;

		template<class T>
		std::vector<T> get_vector(const std::string& section, const std::string& name, std::vector<T>  default_value = {}) const;

		bool get_bool(const std::string& section, const std::string& name, bool default_value = {}) const;

		bool has_value(const std::string& section, const std::string& name) const;

		std::vector<std::string> get_section_value_names(const std::string& section) const;

#pragma endregion

#pragma region members

	protected:

		INIReader  _reader;

		std::string _configFilePath = "not_set";
#pragma endregion

	};
}
