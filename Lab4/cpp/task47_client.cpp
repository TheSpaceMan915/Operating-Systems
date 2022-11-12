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


int main()
{
    openConnection();

    //closing the handle
    CloseHandle(m_connection_handle);
    return 0;
}