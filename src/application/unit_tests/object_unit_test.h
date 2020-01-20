#include "project_config.h"
#include "catch.hpp"

TEST_CASE("nieco")
{
	SECTION("any_of")
	{
		int a = 2;
		REQUIRE(a == 2);
	}

	SECTION("all_of")
	{
		REQUIRE(true == true);
	}
}
