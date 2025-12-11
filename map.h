#include <fstream>
#include <string>
#include <iostream>
using namespace std; 

void readMap(const string& filename)
{
	ifstream inFile(filename);
	if (!inFile.is_open())
	{
		cerr << "error:failed to open the file" << filename << "." << endl;  // cerr 是即时输出，即使程序马上崩溃，错误信息也已经显示在终端，帮你快速定位崩溃原因（比如 “文件打开失败”）。
		return;
	}
	// 逐行读取数据
	string line;
	int a, b;

}



#pragma once
