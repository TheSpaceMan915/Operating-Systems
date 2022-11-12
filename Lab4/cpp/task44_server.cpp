#include <iostream>
#include <Windows.h>
#include <fstream>
using namespace std;


HANDLE m_pipe_handle;
const int m_buff_size = 1023;
char m_input_file[m_buff_size];
char  m_file_output[m_buff_size];


void createPipeHandle()
{
	char input_buff[m_buff_size];
	char output_buff[m_buff_size];
	DWORD input_buff_size = sizeof(input_buff);
	DWORD output_buff_size = sizeof(output_buff);


	m_pipe_handle = CreateNamedPipe(
		TEXT("\\\\.\\pipe\\serverpipe"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		output_buff_size,
		input_buff_size,
		0,
		NULL);

	//checking if the pipe has been created
	if (m_pipe_handle == INVALID_HANDLE_VALUE)
	{
		cout << "CreateNamedPipe has failed. The error code: " << GetLastError() << endl;
	}
	else
	{
		cout << "The pipe has been created successfully" << endl;
	}
}


void openConnection()
{
	BOOL flag_connection = ConnectNamedPipe(m_pipe_handle, NULL);


	//cheching that the connection has been successfully established
	if (flag_connection == FALSE)
	{
		cout << "ConnectNamedPipe has failed. The error code: " << GetLastError() << endl;
	}
	else
	{
		cout << "The connection with the pipe has been established" << endl;
	}
}


void readFileNames()
{
	DWORD not_read_bytes;


	BOOL flag_read_input = ReadFile(
		m_pipe_handle,
		m_input_file,
		m_buff_size,
		&not_read_bytes,
		NULL);

	if (flag_read_input == FALSE)
	{
		cout << "ReadFile input has failed. The error code: " << GetLastError() << endl;
	}
	else
	{
		cout << "Reading success!" << endl;
		cout << "The client's data: " << m_input_file << endl;
	}


	BOOL flag_read_output = ReadFile(
		m_pipe_handle,
		m_file_output,
		m_buff_size,
		&not_read_bytes,
		NULL);

	if (flag_read_output == FALSE)
	{
		cout << "ReadFile has failed. The error code: " << GetLastError() << endl;
	}
	else
	{
		cout << "Reading success!" << endl;
		cout << "The client's data: " << m_file_output << endl;
	}
}


void disconnectPipe()
{
	DisconnectNamedPipe(m_pipe_handle);
	CloseHandle(m_pipe_handle);

	cout << "The pipe has been disconnected" << endl;
}


void copyFile()
{
	ifstream fin(m_input_file, ios::binary);
	ofstream fout(m_file_output, ios::binary);


	fout << fin.rdbuf();


	fin.close();
	fout.close();
}


int main()
{
    createPipeHandle();
    openConnection();

    readFileNames();
    disconnectPipe();

    copyFile();
    return 0;
}