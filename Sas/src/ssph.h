#pragma once

#include <memory>
#include <utility>
#include <algorithm>
#include<iostream>
#include<sstream>
#include <functional>
#include<string>
#include<string_view>
#include<vector>
#include<array>
#include <map>
#include <set>

#include <unordered_map>
#include <unordered_set>


#include "Sas/Core/Log.h"
#include "Sas/Debug/Instrumentor.h"


#ifdef SS_PLATFORM_WINDOWS 
	#include <Windows.h>
#endif