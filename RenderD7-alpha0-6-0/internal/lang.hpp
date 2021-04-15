#include "json.hpp"

#include <string>

namespace RenderD7::Lang
{
    std::string getSys();
    std::string get(const std::string &key);
	void load(const std::string &lang);
}