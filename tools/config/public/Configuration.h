#pragma once
#include <string>
#include <memory>
#include <array>
#include "INIReader.h"


#pragma region macros



#define CONFIGURATION_CLASS(ConfigClass) \
struct ConfigClass : public config::Configuration

#define CONFIGURATION_INIT(ConfigType,InitFile) \
												\
	public:										\
												\
		ConfigType() : Configuration(InitFile)	\
		{										\
			_configFilePath = InitFile;			\
			initProperties();					\
		}										\
												\
	private:									\
												\
		void initProperties()					\


// deprecated >> use as const inline variable for your configuration object
#define ConfigProperty(type, name,default_value) \
public:											\
	type  get##name() const { return name;}	\
private:										\
	type name{default_value}					\

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

		explicit Configuration(std::string const & configPath) 
			:
			_configFilePath{configPath},
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

		bool get_bool(const std::string& section, const std::string& name, bool default_value = {}) const;

		bool has_value(const std::string& section, const std::string& name) const;

#pragma endregion

#pragma region members

	protected:

		INIReader  _reader;

		std::string _configFilePath = "not_set";
#pragma endregion

	};
}
