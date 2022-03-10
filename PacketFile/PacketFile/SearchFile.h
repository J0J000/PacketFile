#pragma once
#include "StdAfx.h"

class Search
{
public:
	Search() {}
	~Search() {}
	static Search *getInstance();//创建实例
	void GetFileList(char *dir, vector<string> &filelist); //遍历文件夹下的文件名列表(包括嵌套文件夹)

	//void find(char *lpPath);
	//void _find(string path);
	//void ReadFile(const char* fileName);
	//void WriteFile(string fileName);
};
