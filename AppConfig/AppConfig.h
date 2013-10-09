#pragma once
#ifdef APPCONFIG_EXPORTS
#define APPCONFIG_API __declspec(dllexport)
#else
#define APPCONFIG_API __declspec(dllimport)
#endif

#include <string>
#include <map>
#include <vector>
#include <sstream>
/*!
"\nThe CommandLineParser class is designed for command line arguments parsing\n"
"Keys map: \n"
"Before you start to work with CommandLineParser you have to create a map for keys.\n"
"    It will look like this\n"
"    const char* keys =\n"
"    {\n"
"        {    s|  string|  123asd |string parameter}\n"
"        {    d|  digit |  100    |digit parameter }\n"
"        {    c|noCamera|false    |without camera  }\n"
"        {    1|        |some text|help            }\n"
"        {    2|        |333      |another help    }\n"
"    };\n"
"Usage syntax: \n"
"    \"{\" - start of parameter string.\n"
"    \"}\" - end of parameter string\n"
"    \"|\" - separator between short name, full name, default value and help\n"
"Supported syntax: \n"
"    --key1=arg1  <If a key with '--' must has an argument\n"
"                  you have to assign it through '=' sign.> \n"
"<If the key with '--' doesn't have any argument, it means that it is a bool key>\n"
"    -key2=arg2   <If a key with '-' must has an argument \n"
"                  you have to assign it through '=' sign.> \n"
"If the key with '-' doesn't have any argument, it means that it is a bool key\n"
"    key3                 <This key can't has any parameter> \n"
"Usage: \n"
"      Imagine that the input parameters are next:\n"
"                -s=string_value --digit=250 --noCamera lena.jpg 10000\n"
"    CommandLineParser parser(argc, argv, keys) - create a parser object\n"
"    parser.get<string>(\"s\" or \"string\") will return you first parameter value\n"
"    parser.get<string>(\"s\", false or \"string\", false) will return you first parameter value\n"
"                                                                without spaces in end and begin\n"
"    parser.get<int>(\"d\" or \"digit\") will return you second parameter value.\n"
"                    It also works with 'unsigned int', 'double', and 'float' types>\n"
"    parser.get<bool>(\"c\" or \"noCamera\") will return you true .\n"
"                                If you enter this key in commandline>\n"
"                                It return you false otherwise.\n"
"    parser.get<string>(\"1\") will return you the first argument without parameter (lena.jpg) \n"
"    parser.get<int>(\"2\") will return you the second argument without parameter (10000)\n"
"                          It also works with 'unsigned int', 'double', and 'float' types \n"
*/
/************************************************************************/
/* 优先级说明：加入同一个参数在多处定义，那么使用哪个值作为程序运行所需要的值
* 如果采用gui进行配置，获得的参数值第一
* 命令行参数第二，
* 配置文件参数第三，
* key_map默认参数第四
*/
/************************************************************************/

class APPCONFIG_API AppConfig
{
public:
	//! the default constructor
	AppConfig(int argc, const char* const argv[], const char* key_map);
	AppConfig(int argc, const wchar_t* const argv[], const char* key_map);
	~AppConfig();
	//! get parameter, you can choose: delete spaces in end and begin or not
	template<typename _Tp>
	_Tp get(const std::string& name, bool space_delete=true)
	{
		if (!has(name))
		{
			return _Tp();
		}
		std::string str = getString(name);
		return analyzeValue<_Tp>(str, space_delete);
	}

	//! print short name, full name, current value and help for all params
	void printParams();

	void readParamsFromJson(const std::string& path, bool overwrite = true);
	void saveParamsToJson(const std::string& path, bool overwrite = true);

protected:
	int argc_char;
	char **argv_char;
	std::map<std::string, std::vector<std::string> > build_in_data;
	std::map<std::string, std::vector<std::string> > data;
	std::string getString(const std::string& name);
	std::string getBuildInString(const std::string& keys);

	bool has(const std::string& keys);

	template<typename _Tp>
	_Tp analyzeValue(const std::string& str, bool space_delete=false);

	template<typename _Tp>
	static _Tp getData(const std::string& str)
	{
		_Tp res = _Tp();
		std::stringstream s1(str);
		s1 >> res;
		return res;
	}

	template<typename _Tp>
	_Tp fromStringNumber(const std::string& str);//the default conversion function for numbers

	void appconfig(int argc, const char* const argv[], const char* key_map);
	void parseKeys(std::map<std::string, std::vector<std::string>>& data_map, const char* key_map);
	void parseParams(int argc, const char* const argv[], std::map<std::string, std::vector<std::string>>& data_map);
};

template<> APPCONFIG_API
	bool AppConfig::get<bool>(const std::string& name, bool space_delete);

template<> APPCONFIG_API
	std::string AppConfig::analyzeValue<std::string>(const std::string& str, bool space_delete);

template<> APPCONFIG_API
	int AppConfig::analyzeValue<int>(const std::string& str, bool space_delete);

template<> APPCONFIG_API
	unsigned int AppConfig::analyzeValue<unsigned int>(const std::string& str, bool space_delete);

template<> APPCONFIG_API
	unsigned __int64 AppConfig::analyzeValue<unsigned __int64>(const std::string& str, bool space_delete);

template<> APPCONFIG_API
	float AppConfig::analyzeValue<float>(const std::string& str, bool space_delete);

template<> APPCONFIG_API
	double AppConfig::analyzeValue<double>(const std::string& str, bool space_delete);
