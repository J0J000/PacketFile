#pragma once
#include "StdAfx.h"

struct Record
{
	char Fname[MAX_FILE_NUM][MAX_PATH_LEN];//文件名
	unsigned int Size[MAX_FILE_NUM];//文件大小
	unsigned int Fnum;//文件个数
	char oriPath[MAX_FILE_NUM][MAX_PATH_LEN];//记录每个文件的原始路径
};

class Pack
{
	Record Fhead;//文件头
	char PackedPath[MAX_PATH_LEN]; //生成的打包文件的路径

public:
	Pack();
	~Pack() {}
	void AddFile(const char * filePath, const char* oriPath); //添加文件
	void SetPackedFile(char * desFile);//初始化打包文件
	long GetFileSize(FILE *pf); //获取文件大小 pf：以二进制方式打开的文件指针
	void PackFile();//打包文件
	void UnpackFile(char *packedPathName, char *unpackedPathName);//解包

private:
	void PrintInfo();//输出pack内的文件信息
	void CheckPath(string desPath);//检查并创建文件夹
};


