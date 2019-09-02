#include "hzpch.h"
#include "StringUtils.h"
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

namespace Hazel {

	// trim from start (in place)
	void StringUtils::LTrim(std::string& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
		}));
	}

	// trim from end (in place)
	void StringUtils::RTrim(std::string& s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}

	// trim from both ends (in place)
	void StringUtils::Trim(std::string& s) {
		LTrim(s);
		RTrim(s);
	}

}
