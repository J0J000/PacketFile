#include "StdAfx.h"
#include "SearchFile.h"
#include "Pack.h"


int main()
{
	char flag[10];
	while (1)
	{
		printf("请选择\n1——打包 2——解包 3——退出  ");
		scanf("%s", flag);
		switch (flag[0]) {
		case '1':
		{
			Pack pf;
			vector<string> vec_file;
			vector<string>::iterator f;
			char fileSrcPath[MAX_FILE_NUM][MAX_PATH_LEN];
			char fileDesPath[MAX_PATH_LEN] = "";
			int fNum = 0;

			while (1) {
				printf("\n请添加需要打包的文件路径（结束添加请输入‘#’）:");
				scanf("%s", fileSrcPath[fNum]);
				if (!strcmp(fileSrcPath[fNum], "#"))
					break;
				vec_file.clear();
				Search::getInstance()->GetFileList(fileSrcPath[fNum], vec_file);
				//添加要打包的文件
				for (f = vec_file.begin(); f < vec_file.end(); f++) {
					pf.AddFile((*f).c_str(), fileSrcPath[fNum]);
				}
				fNum++;
			}
			cout << endl<<"共" << fNum << "个文件，打包到:";
			scanf("%s", fileDesPath);
			pf.SetPackedFile(fileDesPath);//设置输出文件
			pf.PackFile();//执行打包
			break;
		}
		case '2':
		{
			char fileSrcPath[MAX_PATH_LEN] = "";
			char fileDesPath[MAX_PATH_LEN] = "";
			printf("\n请输入需要解包的文件路径:");
			scanf("%s", fileSrcPath);
			printf("解包到:");
			scanf("%s", fileDesPath);

			Pack upf;
			upf.UnpackFile(fileSrcPath, fileDesPath);//执行解包
			break;
		}
		case '3': 
		{
			cout<<"Exit..."<<endl; 
			exit(0);
		}
		default:
			cout << flag[0] << "命令错误" << endl;
		}
	}

	cin.clear();
	cin.get();

	system("pause");
	return 0;
}

