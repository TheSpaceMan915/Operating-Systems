#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <vector>
using namespace std;


void callProcesses(vector<string> &vec_paths, vector<double> &vec_times)
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	LPCSTR temp_path = NULL;
	double temp_time = 0.0;


	for (int i = 0; i < vec_paths.size(); i++)
	{
		//convert the string to c-style string
		temp_path = vec_paths.at(i).c_str();
		temp_time = vec_times.at(i);

		if (CreateProcess(temp_path, NULL, NULL, NULL,
			FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
		{
			CloseHandle(pi.hThread);
			if (WaitForSingleObject(pi.hProcess, temp_time * 1000) >= WAIT_TIMEOUT)
			{ cout << "The process has been terminated later"<< endl; }
			else
			{ cout << "The process has been terminated earlier" << endl; }
			TerminateProcess(pi.hProcess, 0);
			CloseHandle(pi.hProcess);
		}
	}
	
}


void readFile(string file_name)
{
	vector<string> arr_paths;
	vector<double> arr_times;
	string temp = "";
	double temp_time = 0.0;


	//creating file stream
	fstream fin(file_name);
	if (!fin.is_open())
	{ cerr << "The file cannot be opened" << endl; }
	else
	{
		//read the file line by line
		while (getline(fin, temp))
		{
			arr_paths.push_back(temp);

			
			//read and convert the string to double
			getline(fin, temp);
			temp_time = stod(temp);

			arr_times.push_back(temp_time);
		}

		fin.close();
	}

	//starting the function calling processes
	callProcesses(arr_paths, arr_times);
}

int main()
{
    readFile(R"(C:\Users\nikit\CLionProjects\Operating Systems\Lab3\files\task1.txt)");

    return 0;
}