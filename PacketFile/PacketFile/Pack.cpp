#include "Pack.h"

Pack::Pack()
{
	memset(&Fhead, 0x0, sizeof(Fhead));
	memset(PackedPath, 0x0, sizeof(PackedPath));
}

//添加文件
void Pack::AddFile(const char * filePath, const char* oriPath)
{
	if (Fhead.Fnum >= MAX_FILE_NUM - 1)
	{
		cout << "文件个数上限：" << MAX_FILE_NUM << endl;
		return;
	}
	strcpy(Fhead.Fname[Fhead.Fnum], filePath);
	cout << "[添加文件]-"<<Fhead.Fnum+1<<" " << filePath << endl;
	/**************/
	string tmp = oriPath;
	tmp = tmp.erase(tmp.find_last_of("\\"));
	strcpy(Fhead.oriPath[Fhead.Fnum], tmp.c_str());
	//cout << "[test]   原始路径：" << tmp << endl;
	/**************/
	Fhead.Fnum++;
}

//初始化打包文件
void Pack::SetPackedFile(char * desFile)
{
	memset(PackedPath, 0x0, sizeof(PackedPath));
	strcpy(PackedPath, desFile);
}

//获取文件大小
//pf：以二进制方式打开的文件指针
long Pack::GetFileSize(FILE *pf)
{
	fseek(pf, 0,/*SEEK_END*/ 2); //指针移到文件尾
	return ftell(pf);
}


//打包文件
void Pack::PackFile()
{
	if (Fhead.Fnum < 1)
	{
		cout << "没有添加文件" << endl;
		return;
	}
	if (strlen(PackedPath) < 1)
	{
		cout << "没有指定打包文件输出位置" << endl;
		return;
	}

	FILE *desFile = NULL;
	FILE *CurrentFile = NULL;

	//获取文件大小
	for (int i = 0; i < Fhead.Fnum; i++)
	{
		if ((CurrentFile = fopen(Fhead.Fname[i], "rb")) == NULL)
		{
			cout << "文件" << Fhead.Fname[i] << "打开失败 [" << strerror(errno) << "]" << endl;
			return;
		}
		Fhead.Size[i] = GetFileSize(CurrentFile);
		fclose(CurrentFile);
	}
	//检查目标文件夹是否存在
	CheckPath(PackedPath);
	//写文件
	if ((desFile = fopen(PackedPath, "wb")) == NULL)
	{
		cout << "打包文件创建失败 [" << strerror(errno) << "]" << endl;
		return;
	}
	fwrite(&Fhead, sizeof(Fhead), 1, desFile);
	for (int i = 0; i < Fhead.Fnum; i++)
	{
		if ((CurrentFile = fopen(Fhead.Fname[i], "rb")) == NULL)
		{
			cout << "文件" << Fhead.Fname[i] << "打开失败 [" << strerror(errno) << "]" << endl;
			fclose(CurrentFile);
			fclose(desFile);
			return;
		}
		unsigned char *tmp = new unsigned char[Fhead.Size[i]];
		fread(tmp, Fhead.Size[i], 1, CurrentFile);
		if (ferror(CurrentFile))
		{
			cout << "文件" << Fhead.Fname[i] << "打开失败 [" << strerror(errno) << "]" << endl;
			fclose(CurrentFile);
			fclose(desFile);
			return;
		}
		fwrite(tmp, Fhead.Size[i], 1, desFile);
		if (ferror(desFile))
		{
			cout << "文件" << PackedPath << "写入失败 [" << strerror(errno) << "]" << endl;
			fclose(CurrentFile);
			fclose(desFile);
			return;
		}
		delete[] tmp;
		fclose(CurrentFile);
	}
	fclose(desFile);
	cout << "# 打包完成 #" << endl << endl;
}

//解包
void Pack::UnpackFile(char *packedPathName, char *unpackedPathName)
{
	FILE *Packed = NULL;
	FILE *Current = NULL;

	//读文件
	if ((Packed = fopen(packedPathName, "rb")) == NULL) {
		cout << "文件" << packedPathName << "打开失败 [" << strerror(errno) << "]" << endl;
		return;
	}
	memset(&Fhead, 0x0, sizeof(Fhead));
	fread(&Fhead, sizeof(Fhead), 1, Packed);
	PrintInfo();//打印文件信息
	//cout << "# 开始解包 #" << endl;
	//cout << "[test] 原路径:" << packedPathName << endl;

	//解包文件到目标路径
	for (int i = 0; i < Fhead.Fnum; i++)
	{
		unsigned char *tmp = new unsigned char[Fhead.Size[i]];
		fread(tmp, Fhead.Size[i], 1, Packed);

		//获取文件名（不包括路径）
		//char tmpFileName[MAX_PATH_LEN];

		//string s1;
		//s1.assign(Fhead.Fname[i], strlen(Fhead.Fname[i]));
		//char s2[MAX_PATH_LEN];
		//strcpy(s2, getName(s1).c_str());
		//string str.assign(packedPathName,0,length(packedPathName)-length(s2));

		////string str = "C:\\Users\\15228\\Desktop\\software\\test_folder\\";
		//string aaa;
		//aaa.assign(Fhead.Fname[i], strlen(Fhead.Fname[i]));
		//const char *chaaaaa = aaa.replace(0, str.length(), "\\").c_str();
		//char tmpChar[MAX_PATH_LEN];
		//strcpy(tmpChar, chaaaaa);
		//memset(tmpFileName, 0x0, sizeof(tmpFileName));

		//cout << "-----------Unpack Processing-----------" << endl;
		string aaa;
		aaa.assign(Fhead.Fname[i], strlen(Fhead.Fname[i]));
		//cout << "[test] aaa:" << aaa << endl;
		/*char tmpFileName[MAX_PATH_LEN];
		int l = aaa.length(), j;
		for (j = l - 1; j >= 0; j--)
			if (aaa[j] == '\\') break;
		strcpy(tmpFileName,(aaa.substr(j + 1, l - j - 1)).c_str());
		cout << "[test] tmpFileName:" << tmpFileName << endl;*/

		/**************/
		char tmpFileName[MAX_PATH_LEN];
		string sss = Fhead.oriPath[i];
		string s = (aaa).erase(0, sss.length()+1);//保留文件名
		strcpy(tmpFileName, s.c_str());
		//cout << "[test] tmpFileName:" << tmpFileName << endl; 
		/**************/

		//获取目标路径
		/*char tmpPathName[MAX_PATH_LEN];
		memset(tmpPathName, 0x0, sizeof(tmpPathName));
		strcpy(tmpPathName, packedPathName);
		cout << "tmpPathName 1:" << tmpPathName << endl;
		char* tmpC = tmpPathName + strlen(tmpPathName);
		while ('\\' != *tmpC)
			tmpC--;
		tmpC++;
		*tmpC = '\0';
		cout << "tmpPathName 2:" << tmpPathName << endl;
		strcat(tmpPathName, tmpFileName);
		cout << "tmpPathName 3:" << tmpPathName << endl;*/
		char tmpPathName[MAX_PATH_LEN];
		memset(tmpPathName, 0x0, sizeof(unpackedPathName));
		strcpy(tmpPathName, unpackedPathName);
		if (tmpPathName[strlen(tmpPathName) - 1] != '\\')
			strcat(tmpPathName,"\\");

		//cout << "[test] tmpPathName: " << tmpPathName << endl;
		strcat(tmpPathName, tmpFileName);
		//cout << "[test]            : " << tmpPathName << endl;

		Current = fopen(tmpPathName, "wb");
		if (Current == NULL)
		{
			CheckPath(tmpPathName);
			Current = fopen(tmpPathName, "wb");
		}
		fwrite(tmp, Fhead.Size[i], 1, Current);
		fclose(Current);
		delete[] tmp;
	}
	fclose(Packed);
	cout << "# 解包完成 #" << endl<<endl;
}


//输出pack内的文件信息
void Pack::PrintInfo()
{
	cout <<endl<< "----------Information List----------" << endl;
	cout << "Tol:" << Fhead.Fnum << endl;
	for (int i = 0; i < Fhead.Fnum; i++)
		cout << Fhead.Fname[i] << "\t\t\t\t" << Fhead.Size[i] << "B" << endl;
	cout << "------------------------------------" << endl<<endl;
}

//检查并创建文件夹
void Pack::CheckPath(string desPath)
{
	//Log &log = Log::getLog("main", "CheckPath");
	int loc = desPath.length();
	int f_loc = desPath.find("\\", 0);
	string subDIR;
	do
	{
		loc = desPath.find("\\", f_loc + 2);
		if (loc != -1)
		{
			subDIR = desPath.substr(0, loc);
			if (_mkdir(subDIR.c_str()) == 0)
				cout << "[创建目录] " << subDIR.c_str() << endl;
		}
		f_loc = loc;
	} while (f_loc != -1);
}
