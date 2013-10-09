// AppConfigTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include "..\AppConfig\AppConfig.h"

int _tmain(int argc, _TCHAR* argv[])
{
	const char* key_map =
	{
		"{ s | string   | 123asd    | string parameter}"
		"{ d | digit    | 100       | digit parameter }"
		"{ c | noCamera | false     | without camera  }"
		"{ 1 |          | some text | help            }"
		"{ 2 |          | 333       | another help    }"
	};
	AppConfig *config = new AppConfig(argc, argv, key_map);
	std::string file = config->get<std::string>("build_in_load_json");
	std::string file2 = config->get<std::string>("soo");
	config->printParams();
	config->saveParamsToJson(file);
	return 0;
}