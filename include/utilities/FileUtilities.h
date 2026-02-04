//
// Created by Nutellis on 29-Jan-26.
//

#ifndef ALKYONERENDERENGINE_FILEUTILITIES_H
#define ALKYONERENDERENGINE_FILEUTILITIES_H

#include <string>
#include <iostream>
#include <fstream>

namespace FileUtilities
{
	inline std::string ReadFilePath(const std::string& filepath) {

		std::ifstream file(filepath);
		std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

		return data;
	}


	inline std::string Normalise(const std::string& file_name)
	{
		std::string nname;
		nname.reserve(file_name.size());
		for (const char c : file_name)
		{
			if (c == '\\')
			{
				if (nname.back() != '/')
				{
					nname += '/';
				}
			}
			else
			{
				nname += c;
			}
		}
		return nname;
	}

	inline std::string FileStem(const std::string& file_name)
	{
		size_t slash = file_name.find_last_of("\\/");
		size_t dot = file_name.find_last_of(".");
		if (slash != std::string::npos)
		{
			return file_name.substr(slash + 1, dot - slash - 1);
		}
		else
		{
			return file_name.substr(0, dot);
		}
	}

	inline std::string FileExtension(const std::string& file_name)
	{
		size_t separator = file_name.find_last_of(".");
		if (separator == std::string::npos)
		{
			return "";
		}
		else
		{
			return file_name.substr(separator);
		}
	}

	inline std::string ChangeExtension(const std::string& file_name, const std::string& ext)
	{
		size_t separator = file_name.find_last_of(".");
		if (separator == std::string::npos)
		{
			return file_name + ext;
		}
		else
		{
			return file_name.substr(0, separator) + ext;
		}
	}

	inline std::string ParentPath(const std::string& file_name)
	{
		size_t separator = file_name.find_last_of("\\/");
		if (separator != std::string::npos)
		{
			return file_name.substr(0, separator + 1);
		}
		else
		{
			return "./";
		}
	}

	inline const char* SplitPath(const char* filepath)
	{
		std::string temp;
		char* tempchar;

		int i = 0, pos = 0, j = 0, count = 0, size = 0;;
		while (filepath[i] != '\0' && filepath[i] != '.')
		{
			if (filepath[i] == '/')
			{
				pos = i;
				count = 0;
			}
			if (filepath[i + 1] == '.')
			{
				size = count;
			}
			count++;
			i++;
		}
		temp.resize(size);

		i = pos + 1;
		while (filepath[i] != '.')
		{
			temp[j] = filepath[i];
			j++;
			i++;
		}
		temp += ".asset";
		tempchar = new char[temp.size()];
		strcpy(tempchar, temp.c_str());
		return (const char*)tempchar;
	}
};

#endif //ALKYONERENDERENGINE_FILEUTILITIES_H