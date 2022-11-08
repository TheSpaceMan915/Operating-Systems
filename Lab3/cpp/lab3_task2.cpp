#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
using namespace std;


void copyFile(string path)
{
	//copy the .exe file
	string output_path ="C:\\Users\\nikit\\source\\repos\\OS Lab3\\OS Lab3\\fileCopy.exe";
	ifstream fin(path, ios::binary);
	ofstream fout(output_path, ios::binary);

	fout << fin.rdbuf();
	
	fin.close();
	fout.close();

	//delete the copy
	//0 - file has been deleted
	//not 0 - something went wrong
	int res = remove(output_path.c_str());
	
	if (res == 0)
	{ cout << "The file has been deleted" << endl; }
	else
	{ cerr << "Something went wrong" << endl; }

	Sleep(5000);
	cout << res << endl;
}

void runFile(string path)
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	LPCSTR str = path.c_str();

	while (!CreateProcess(str, NULL, NULL, NULL,FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
	{ 
		//wait while there is no file
		Sleep(5000); 
	}

	CloseHandle(pi.hThread);
	if (WaitForSingleObject(pi.hProcess, 5 * 1000) == WAIT_TIMEOUT)
	{
		TerminateProcess(pi.hProcess, 0);
	}
	CloseHandle(pi.hProcess);
}


int main()
{
    string str_path = "C:\\Users\\nikit\\AppData\\Roaming\\Zoom\\bin\\Zoom.exe";
    runFile(str_path);
    copyFile(str_path);

    return 0;
}