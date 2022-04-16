#include "lang.hpp"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <3ds.h>
static nlohmann::json appJson;

std::string RenderD7::Lang::getSys()
{
   
	u8 language = 1;
	CFGU_GetSystemLanguage(&language);

	switch(language) {
		case 0:
			return "jp"; // Japanese
			break;

		case 1:
			return "en"; // English
			break;

		case 2:
			return "fr"; // French
			break;

		case 3:
			return "de"; // German
			break;

		case 4:
			return "it"; // Italian
			break;

		case 5:
			return "es"; // Spanish
			break;

		case 6:
			return "zh-CN"; // Chinese (Simplified)
			break;

		// case 7:
		// 	return "ko"; // Korean
		// 	break;

		// case 8:
		// 	return "nl"; // Dutch
		// 	break;

		case 9:
			return "pt"; // Portuguese
			break;

		case 10:
			return "ru"; // Russian
			break;

		case 11:
			return "zh-TW"; // Chinese (Traditional)
			break;

		default:
			return "en"; // Fall back to English if missing
			break;
	}

}
std::string RenderD7::Lang::get(const std::string &key) {
	if (!appJson.contains(key)) return key;

	return appJson.at(key).get_ref<const std::string&>();
}

void RenderD7::Lang::load(const std::string &lang) {
	FILE *values;

	if (access(("romfs:/lang/" + lang + "/app.json").c_str(), F_OK) == 0) {
		values = fopen(("romfs:/lang/" + lang + "/app.json").c_str(), "rt");
		if (values) {
			appJson = nlohmann::json::parse(values, nullptr, false);
			fclose(values);
		}
		if (appJson.is_discarded())
			appJson = { };
		return;

	} else {
		values = fopen("romfs:/lang/en/app.json", "rt");
		if (values) {
			appJson = nlohmann::json::parse(values, nullptr, false);
			fclose(values);
		}
		if (appJson.is_discarded())
			appJson = { };
		return;
	}
}