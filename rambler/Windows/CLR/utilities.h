//2015-01-28

#pragma once

#include "rambler/string_types.hpp"

namespace Rambler {

	rambler::String ramblerString(System::String^ dotNetString);
	System::String^ dotNetString(rambler::String ramblerString);

}