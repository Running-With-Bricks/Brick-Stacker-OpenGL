#include "pch.hpp"
#include "std_split_str.hpp"

namespace std
{
	void split_str(const std::string& str, const char delim, std::vector<std::string>& out)
	{
		// create a stream from the string  
		std::stringstream s(str);

		std::string s2;
		while (std::getline(s, s2, delim))
		{
			out.push_back(s2); // store the string in s2  
		}
	}
}
