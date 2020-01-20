#include "project_config.h"
#if RUN_AS_UNIT_TESTING() == true

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "unit_tests/object_unit_test.h"

int main(int argc, char* argv[]) 
{
	char* xxx[2];
	xxx[0] = (char*)"xxx";
	xxx[1] =(char*) "-s";
	int result = Catch::Session().run(2, xxx);
	return result;
}


#endif
