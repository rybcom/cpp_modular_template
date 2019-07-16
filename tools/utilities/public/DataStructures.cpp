#include "DataStructures.h"
#include "Utilities.h"

namespace aux
{


	std::string Transform::toString() const
	{
		return aux::string_compose("Position: ", position.toString(), ", Rotation :", rotation.toString());
	}

	std::string Rotation::toString() const
	{
		return aux::string_compose(" { h : ", h, ", p :", p, ", r :", r, " }");
	}
	
	//template<>
	//std::string Vector3<double>::toString()
	//{
	//	return aux::string_compose(" { x : ", x, ", y :", y, ", z :", z, " }");
	//}
}
