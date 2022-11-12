#include <iostream>
#include <Windows.h>
#include <thread>
#include <string>
using namespace std;


HANDLE m_pipe_handle3;
const int m_buff_size3 = 1023;
char m_input_file2[m_buff_size3];
char m_output_encrypted[m_buff_size3];
char m_output_decrypted[m_buff_size3];


void createPipeHandle()
{
	char input_buff[m_buff_size3];
	char output_buff[m_buff_size3];
	DWORD input_buff_size = sizeof(input_buff);
	DWORD output_buff_size = sizeof(output_buff);


	m_pipe_handle3 = CreateNamedPipe(
		TEXT("\\\\.\\pipe\\serverpipe"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		output_buff_size,
		input_buff_size,
		0,
		NULL);


	//checking if the pipe has been created
	if (m_pipe_handle3 == INVALID_HANDLE_VALUE)
	{
		cout << "CreateNamedPipe has failed. The error code: " << GetLastError() << endl;
	}
	else
	{
		cout << "The pipe has been created successfully" << endl;
	}
}


void openConnectionForClient()
{
	BOOL flag_connection = ConnectNamedPipe(m_pipe_handle3, NULL);


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


void readFileName()
{
	DWORD not_read_bytes;


	BOOL flag_read_input = ReadFile(
		m_pipe_handle3,
		m_input_file2,
		m_buff_size3,
		&not_read_bytes,
		NULL);

	if (flag_read_input == FALSE)
	{
		cout << "ReadFile input has failed. The error code: " << GetLastError() << endl;
	}
	else
	{
		cout << "Reading success!" << endl;
		cout << "The client's data: " << m_input_file2 << endl;
	}
}


//This function receives text and shift
//It returns the encrypted text
string encryptStr(string text, int s)
{
	string result = "";

	// traverse text
	for (int i = 0; i < text.length(); i++) {
		// apply transformation to each character
		// Encrypt Uppercase letters
		if (isupper(text[i]))
			result += char(int(text[i] + s - 65) % 26 + 65);

		// Encrypt Lowercase letters
		else
			result += char(int(text[i] + s - 97) % 26 + 97);
	}

	// Return the resulting string
	return result;
}


void createResponds()
{
	//reading the file
	string str_respond = m_input_file2;

	//encrypting the text
	string str_encrypted = encryptStr(str_respond, 4);

    //decrypting the text
    string str_decrypted = encryptStr(str_encrypted,-4);

	//putting the encrypted text in the array
	for (int i = 0; i < str_encrypted.length(); i++)
	{
        m_output_encrypted[i] = str_encrypted[i];
	}

    //putting the decrypted text in the array
    for (int i = 0; i < str_encrypted.length(); i++)
    {
        m_output_decrypted[i] = str_decrypted[i];
    }
}


void writeToClient()
{
	//sending the text to the client
	DWORD not_read_bytes;

	BOOL flag_write_encrypted = WriteFile(
            m_pipe_handle3,
            m_output_encrypted,
            m_buff_size3,
            &not_read_bytes,
            NULL);

	if (flag_write_encrypted == FALSE)
	{
		cout << "WriteFile output_encrypted has failed. The error code: " << GetLastError() << endl;
	}
	else
	{
		cout << "The encrypted message has been put in the buffer" << endl;
	}


    BOOL flag_write_decrypted = WriteFile(
            m_pipe_handle3,
            m_output_decrypted,
            m_buff_size3,
            &not_read_bytes,
            NULL);

    if (flag_write_decrypted == FALSE)
    {
        cout << "WriteFile output_decrypted has failed. The error code: " << GetLastError() << endl;
    }
    else
    {
        cout << "The decrypted message has been put in the buffer" << endl;
    }
}


void flushTheBuffer()
{
	BOOL flag_flush_buff = FlushFileBuffers(m_pipe_handle3);

	if (flag_flush_buff == FALSE)
	{
		cout << "FlushFileBuffers has failed. The error code: " << GetLastError() << endl;
	}
	else
	{
		cout << "The server has sent the encrypted message to the client" << endl;
	}
}

void disconnectPipe()
{
	DisconnectNamedPipe(m_pipe_handle3);
	CloseHandle(m_pipe_handle3);

	cout << "The pipe has been disconnected" << endl;
}


int main()
{
    createPipeHandle();
    openConnectionForClient();

    readFileName();
    createResponds();

    writeToClient();
    flushTheBuffer();

    this_thread::sleep_for(chrono::seconds(7));
    disconnectPipe();

    return 0;
}