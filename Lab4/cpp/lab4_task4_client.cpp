#include <iostream>
#include <Windows.h>
using namespace std;


HANDLE m_connection_handle;


void openConnection()
{
	m_connection_handle = CreateFile(
		TEXT("\\\\.\\pipe\\serverpipe"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (m_connection_handle == INVALID_HANDLE_VALUE)
	{
		cout << "CreateFile has failed. The error code: " << GetLastError() << endl;
	}
	else
	{
		cout << "The client has successfully connected to the pipe" << endl;
	}
}


void writeToPipe()
{
	const int m_buff_size2 = 1023;
	//defining the names of input and output files and the size of the buffer
	char str_input[m_buff_size2] = R"(C:\Users\nikit\CLionProjects\Operating Systems\Lab4\files\task4_text.txt)";
	char str_output[m_buff_size2] = R"(C:\Users\nikit\CLionProjects\Operating Systems\Lab4\files\task4_text_copy.txt)";
	DWORD not_writen_bytes;


	BOOL flag_write_input = WriteFile(
		m_connection_handle,
		str_input,
		m_buff_size2,
		&not_writen_bytes,
		NULL);


	if (flag_write_input == FALSE)
	{
		cout << "WriteFile input has failed. The error code: " << GetLastError() << endl;
	}
	else
	{
		cout << "The client has sent a message to the pipe" << endl;
	}


	BOOL flag_write_output = WriteFile(
		m_connection_handle,
		str_output,
		m_buff_size2,
		&not_writen_bytes,
		NULL);


	if (flag_write_output == FALSE)
	{
		cout << "WriteFile output has failed. The error code: " << GetLastError() << endl;
	}
	else
	{
		cout << "The client has sent a message to the pipe" << endl;
	}


	//closing the handle
	CloseHandle(m_connection_handle);
}


int main()
{
    openConnection();
    writeToPipe();

    return 0;
}