#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <random>
#include <iosfwd>
#include "DataStructures.h"
#include <optional>


#define DECLARE_STRONG_TYPE(nametype, type) \
struct nametype \
{ \
	type as_##type; \
}

namespace aux
{



#pragma region runtime

	template <class T, class... Args>
	std::unique_ptr <T> make_unique_ptr(Args&& ... args)
	{
		return std::unique_ptr <T>(new T(std::forward <Args>(args)...));
	}

	template<class T, class C >
	T* cast_ptr_to(C* data)
	{
		return dynamic_cast<T*>(data);
	}

	template<class T, class C >
	bool can_cast_to(C* data)
	{
		C* dynamic_cast_result = aux::cast_ptr_to<T>(data);
		if (dynamic_cast_result)
		{
			return true;
		}
		return false;
	}

	template <typename M>
	std::vector<typename M::mapped_type> map_value_to_vector(const  M& m)
	{
		std::vector<typename M::mapped_type> v;
		for (typename M::const_iterator it = m.begin(); it != m.end(); ++it) {
			v.push_back(it->second);
		}

		return v;
	}

	template <typename M>
	std::vector<typename M::key_type> map_key_to_vector(const  M& m)
	{
		std::vector<typename M::key_type> v;
		for (typename M::const_iterator it = m.begin(); it != m.end(); ++it) {
			v.push_back(it->first);
		}

		return v;
	}

	template<typename Container, typename Pred, typename Operation>
	void for_each(Container c, Operation op)
	{
		auto begin = c.begin();
		auto end = c.end();

		for (; begin != end; begin++) {
			op(*begin);
		}
	}

	template<typename Container, typename Pred, typename Operation>
	void for_each_where(Container c, Pred p, Operation op)
	{
		auto begin = c.begin();
		auto end = c.end();

		for (; begin != end; begin++) {
			if (p(*begin)) {
				op(*begin);
			}
		}
	}

	template<typename T, template <typename, typename...> class Container, typename Operation, typename...Args>
	void for_each(Container<std::unique_ptr<T>, Args...> const & c, Operation op)
	{
		for (auto & refUniquePtr : c)
		{
			op(*refUniquePtr);
		}
	}

	template<typename T, template <typename, typename...> class Container, typename Pred, typename Operation, typename...Args>
	void for_each_where(Container<std::unique_ptr<T>, Args...> const & c, Pred p, Operation op)
	{
		for (auto & refUniquePtr : c)
		{
			T & refValue = *refUniquePtr;
			if (p(refValue))
			{
				op(refValue);
			}
		}
	}

#pragma endregion

#pragma region string operations

	template <typename ...Args>
	std::string string_compose(Args&& ...args)
	{
		std::ostringstream stream;
		(stream << ... << std::forward<Args>(args));

		return stream.str();
	}

	std::string to_lower_case(std::string const & text);

	std::string string_format(const std::string fmt_str, ...);

	void split(std::string const& text, char delim, std::vector<std::string> & elems);

	std::vector<std::string> split(std::string const& text, char delim);

	std::wstring string_to_wstring(std::string const& s);

	std::string wstring_to_string(std::wstring const& s);

	void shell_execute(std::string const& file, bool show_window = false);

	void shell_execute(std::string const& file, std::string const & params, bool show_window = false);

	std::string get_config_file(std::string const & file);

	std::optional<std::string> get_command_parameter(std::size_t param_index);


#pragma endregion


#pragma region coord system

	//void geodetic_to_geocentric_coords(double lat, double lon, double alt, OUT double& x, OUT double& y, OUT double& z);

	//void geocentric_to_geodetic_coords(double x, double y, double  z, OUT double& lat, OUT double& lon, OUT double& alt);

#pragma endregion


#pragma region maths

	const double DEGTORAD = 0.017453292519943295769236907684886f;
	const double RADTODEG = 57.295779513082320876798154814105f;

	template <typename T>
	inline T deg_to_rad(T & value)
	{
		return (T)(value * DEGTORAD);
	}

	template <typename T>
	inline T rad_to_deg(T & value)
	{
		return (T)(value * RADTODEG);
	}

	template <typename T>
	inline T shiftTo_0_360(T const & value)
	{

		T result = value;

		while (result > 360.0)
		{
			result = (T)(result - 360.0);
		}

		while (result < 0)
		{
			result = (T)(result + 360.0);
		}
		return result;
	}

	template <typename T>
	inline T shiftTo_180_180(T const & value)
	{
		T result = value;

		while (result > 180.0)
		{
			result = (T)(result - 360.0);
		}

		while (result < -180.0)
		{
			result = (T)(result + 360.0);
		}

		return result;
	}

	template <typename T>
	inline T shiftTo_90_90(T const & value)
	{
		T result = value;

		while (result > 90.0)
		{
			result = (T)(result - 180.0);
		}

		while (result < -90.0)
		{
			result = (T)(result + 180.0);
		}

		return result;
	}

	template <typename T>
	inline Vector3<T> get_random_vector3(T from, T to)
	{
		Vector3<T> vec;
		vec.x = aux::get_random_value_from_range<T>(from, to);
		vec.y = aux::get_random_value_from_range<T>(from, to);
		vec.z = aux::get_random_value_from_range<T>(from, to);
		return vec;
	}

	template <typename T>
	inline Vector4<T> get_random_vector4(T from, T to)
	{
		Vector4<T> vec;
		vec.x = aux::get_random_value_from_range<T>(from, to);
		vec.y = aux::get_random_value_from_range<T>(from, to);
		vec.z = aux::get_random_value_from_range<T>(from, to);
		vec.w = aux::get_random_value_from_range<T>(from, to);
		return vec;
	}

	Color get_random_color();

	int get_int_from_color(aux::Color const & color);

	template <typename T>
	inline T get_random_value_from_range(T from, T to)
	{
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_real_distribution<> distr(from, to);

		return static_cast<T>(distr(eng));
	}


	template <typename T>
	inline T clamp_value(T const& value, T const& min, T const& max)
	{
		if (value < min)
		{
			return min;
		}
		if (value > max)
		{
			return max;
		}

		return value;
	}

	template <typename T> 
	inline int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	template<typename T>
	inline bool floating_are_same(T a, T b, T epsilon = (T)0.00001)
	{
		return fabs(a - b) < epsilon;
	}

	template<typename T>
	inline bool floating_are_not_same(T a, T b, T epsilon = (T)0.00001)
	{
		return fabs(a - b) > epsilon;
	}

	template <class T>
	inline bool try_parse(std::string input, T & var)
	{
		static const std::string ws(" \t\f\v\n\r");
		size_t pos = input.find_last_not_of(ws);
		if (pos != std::string::npos)
			input.erase(pos + 1);
		else input.clear();
		std::stringstream buffer(input);
		return buffer >> var && buffer.eof();
	}



#pragma endregion


}

