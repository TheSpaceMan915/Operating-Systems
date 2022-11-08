#include <iostream>
#include <Windows.h>
#include <thread>
using namespace std;


HANDLE m_connection_handle2;
const int m_buff_size4 = 1023;


void openConnectionWithServer()
{
	m_connection_handle2 = CreateFile(
		TEXT("\\\\.\\pipe\\serverpipe"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (m_connection_handle2 == INVALID_HANDLE_VALUE)
	{
		cout << "CreateFile has failed. The error code: " << GetLastError() << endl;
	}
	else
	{
		cout << "The client has successfully connected to the pipe" << endl;
	}
}


void writeToServer()
{
	//defining the name of the input file and the size of the buffer
	char str_input[m_buff_size4] = "ThisShouldBeEncrypted";
	DWORD not_writen_bytes;


	BOOL flag_write_input = WriteFile(
		m_connection_handle2,
		str_input,
		m_buff_size4,
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
}


void readFromServer()
{
	char read_buf[m_buff_size4];
	DWORD not_writen_bytes;

	BOOL flag_read_from_pipe = ReadFile(
		m_connection_handle2,
		read_buf,
		m_buff_size4,
		&not_writen_bytes,
		NULL);


	if (flag_read_from_pipe == FALSE)
	{
		cout << "The ReadFile has failed. The error code: " << GetLastError() << endl;
	}
	else
	{
		cout << "Reading success!" << endl;
		cout << "The data from the server: " << read_buf << endl;
	}
}



int main()
{
    openConnectionWithServer();
    writeToServer();

    this_thread::sleep_for(chrono::seconds(3));
    readFromServer();

    //closing the handle
    CloseHandle(m_connection_handle2);
    return 0;
}