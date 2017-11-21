#pragma once
class GameStatics
{
public:
	static std::string ReadTextFileIntoString(const char* file);
	static string GetFileExtensionFromName(const char* fileName);
	static std::vector<std::string> SplitStringByDelimiter(std::string str, char delim);

	static time_t GetFileModificationDate(std::string file);
};

