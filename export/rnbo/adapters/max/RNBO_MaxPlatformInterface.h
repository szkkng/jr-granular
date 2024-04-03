#ifndef RNBO_MAX_PLATFORMINTERFACE
#define RNBO_MAX_PLATFORMINTERFACE

#include "platforms/stdlib/RNBO_PlatformInterfaceStdLib.h"
#include <c74_min.h>

class MaxPlatformInterface : public RNBO::PlatformInterfaceStdLib {
	public:
		void printMessage(const char* message) override
		{
			c74::max::object_post(nullptr, message);
		}

		void printErrorMessage(const char* message) override
		{
			c74::max::object_error(nullptr, message);
		}
};

#endif
