#include <iostream>
#include <Windows.h>
using namespace std;


//global variables
HANDLE m_pipe_handle;
const int m_buff_size = 1023;
char m_input_message[m_buff_size];
char  m_output_message[m_buff_size];
HANDLE m_semaphone = CreateSemaphore(
        NULL,
        1,		//initial count
        1,		//maximum count
        NULL);
DWORD m_resource_flag;


void printRectangle(int l, int b)
{

    int i, j;
    for (i = 1; i <= l; i++)
    {
        for (j = 1; j <= b; j++)
            if (i == 1 || i == l || j == 1 || j == b)
                printf("*");
            else
                printf(" ");
        printf("\n");
    }
    cout << endl;
    cout << endl;

}


void drawRectangle()
{
    cout << "drawRectangle has started" << endl;

    //asking the semaphone for resources
    m_resource_flag = WaitForSingleObject(m_semaphone,0L);

    if (m_resource_flag == WAIT_OBJECT_0)
    {
        cout << "Resources have been given" << endl;
        printRectangle(5,5);

        ReleaseSemaphore(m_semaphone,1,NULL);
        cout << "Resources have been released" << endl;
    }
    else
    { cout << "There are no resources available" << endl; }
}


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


void readMessage()
{
    DWORD not_read_bytes;


    BOOL flag_read_input = ReadFile(
            m_pipe_handle,
            m_input_message,
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
        cout << "The client's message: " << m_input_message << endl;
    }
}


void disconnectPipe()
{
    DisconnectNamedPipe(m_pipe_handle);
    CloseHandle(m_pipe_handle);

    cout << "The pipe has been disconnected" << endl;
}


int main()
{
    createPipeHandle();
    openConnection();

    //readMessage();
    drawRectangle();
    disconnectPipe();

    return 0;
}