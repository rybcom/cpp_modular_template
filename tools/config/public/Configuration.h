#pragma once
#include <string>
#include <memory>
#include "INIReader.h"


#pragma region macros

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



#define ConfigProperty_DefaultValue(type, name,default_value) \
public:											\
	type  get##name() const { return name;}	\
private:										\
	type name{default_value};					\

#define ConfigProperty(type, name) \
public:											\
	type   name() const { return name;}	\
private:										\
	type name{};								\


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

		std::string get_string(const std::string& section, const std::string& name, const std::string& default_value) const;

		long get_int(const std::string& section, const std::string& name, long default_value) const;

		double get_real(const std::string& section, const std::string& name, double default_value) const;

		bool get_bool(const std::string& section, const std::string& name, bool default_value) const;

		bool has_value(const std::string& section, const std::string& name) const;

#pragma endregion

#pragma region members

	protected:

		INIReader  _reader;

		std::string _configFilePath = "not_set";
#pragma endregion

	};
}
