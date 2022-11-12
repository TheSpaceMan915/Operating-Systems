#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <mutex>
using namespace std;


//critical section
mutex m_section;


void readFile(string path)
{
	//locking cin and cout
	m_section.lock();

	ifstream fin(path);
	if (fin.is_open())
	{
		string temp;
		while (getline(fin, temp))
		{ cout << temp << endl; }
	}
	else
	{ cerr << "Something went wrong" << endl; }

	fin.close();

	//unlocking cin and cout
	m_section.unlock();
}


int main()
{
    string path_file1 = R"(C:\Users\nikit\CLionProjects\Operating Systems\Lab4\files\task1_process1.txt)";
    string path_file2 = R"(C:\Users\nikit\CLionProjects\Operating Systems\Lab4\files\task1_process2.txt)";

    thread thread1(readFile,path_file1);
    thread thread2(readFile, path_file2);

    thread1.join();
    thread2.join();

    return 0;
}