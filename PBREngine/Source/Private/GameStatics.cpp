#include "EnginePCH.h"
#include "Engine.h"
#include <sstream>


std::string GameStatics::ReadTextFileIntoString(const char* file)
{
	std::ifstream t(file);
	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),	std::istreambuf_iterator<char>());

	return str;
}

string GameStatics::GetFileExtensionFromName(const char* fileName)
{
	std::string fn = fileName;
	return fn.substr(fn.find_last_of("."));
}

// You could also take an existing vector as a parameter.
std::vector<std::string> GameStatics::SplitStringByDelimiter(std::string str, char delimiter)
{
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}

time_t GameStatics::GetFileModificationDate(std::string file)
{
	time_t modifiedTime = time_t();

	struct stat result;
	if (stat(file.c_str(), &result) == 0)
	{
		modifiedTime = result.st_mtime;
	}

	return modifiedTime;
}