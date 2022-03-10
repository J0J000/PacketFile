#include "SearchFile.h"

static Search* instance;

Search* Search::getInstance()
{
	if (instance == NULL)
		instance = new Search();
	return instance;
}

//遍历文件夹下的文件名列表(包括嵌套文件夹)
void Search::GetFileList(char *dir, vector<string> &filelist)
{
	//cout << "-----------File Searching-----------" << endl;
	struct stat s;

	if (stat(dir, &s) == 0) {
		if (s.st_mode & S_IFDIR) {
			//std::cout << "[test] it's a directory" << std::endl;
		}
		else if (s.st_mode & S_IFREG) {
			//std::cout << "[test] it's a file" << std::endl;
			filelist.push_back(dir);
			//cout << dir << endl;
			return;
		}
		else {
			std::cout << "Neither file nor directory" << std::endl;
			return;
		}
	}
	else {
		std::cout << "Error, doesn't exist" << std::endl;
		return;
	}

	HANDLE Fhead;
	WIN32_FIND_DATA fileData;
	//WIN32_FIND_DATAA fileData;
	char tmp1[MAX_PATH_LEN];
	string data;
	char Fname[MAX_PATH_LEN],tmp[MAX_PATH_LEN];
	strcpy(Fname, dir);

	//处理路径名
	if (Fname[strlen(Fname) - 1] != '\\')
		strcat(Fname, "\\");
	strcpy(tmp, Fname);
	//cout << "[test] tmp:" << tmp << endl;
	strcat(Fname, "*"); //不加*会出错

	wchar_t tmp2[MAX_PATH_LEN];//用于处理为unicode
	mbstowcs(tmp2, (const char*)Fname, MAX_PATH_LEN - 1);

	//Fhead = FindFirstFileA(Fname, &fileData);
	Fhead = FindFirstFile(tmp2, &fileData);
	FindNextFile(Fhead, &fileData);
	while (FindNextFile(Fhead, &fileData))
	{
		wcstombs(tmp1, (const wchar_t*)fileData.cFileName, MAX_PATH_LEN);
		//若为目录
		if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//与运算判断DIR
		{
			if (fileData.cFileName[0] != '.')
			{
				char szFile[MAX_PATH_LEN];
				strcpy(szFile, tmp);
				//strcat(szFile, fileData.cFileName);
				strcat(szFile, tmp1);
				//cout << "[test] szFile:" << szFile << endl;
				GetFileList(szFile, filelist);
			}
		}
		//若为普通文件
		else
		{
			data = (string)tmp;
			//data += fileData.cFileName;
			data += tmp1;
			/*if (data.find(".h",0)!=string::npos)
				filelist.push_back(data);
			else continue;*/
			filelist.push_back(data);
		}
		//cout<<data<<endl;
	}
	return;
}
