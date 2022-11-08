#include <iostream>
#include <Windows.h>
#include <thread>
#include <cmath>
#include <conio.h>
using namespace std;


//initialising an event
HANDLE hEvent = CreateEvent(
	NULL,               
	TRUE,               
	FALSE,             
	TEXT("Click")  
);
BOOL res;



//functions drawing shapes
void drawCircle(int radius)
{

	for (int i = 0; i <= 2 * radius; i++)
	{
		for (int j = 0; j <= 2 * radius; j++)
		{
			double distance = sqrt((double)(i - radius)
				* (i - radius) + (j - radius)
				* (j - radius));

			if (distance > radius - 0.5 &&
				distance < radius + 0.5)
				printf("*");
			else
				printf(" ");
		}
		printf("\n");
	}
	cout << endl;
	cout << endl;
}


void drawRectangle(int l, int b)
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


void drawTriangle(int side)
{
	int i, j;
	for (i = 1; i <= side; i++)
	{
		for (j = i; j < side; j++)
			printf(" ");

		for (j = 1; j <= (2 * i - 1); j++)
		{
			if (i == side || j == 1 || j == (2 * i - 1))
				printf("*");
			else
				printf(" ");
		}
		printf("\n");
	}
	cout << endl;
	cout << endl;
}


void fireEvent()
{
	//if a key has been pressed, change the state of the event
	char c = '2';
	while (c=_getch() != ' ')
	{ res = SetEvent(hEvent); }
}


void listenForEvent()
{
	//if there hasn't been an event, wait for it to happen
	while (res == 0)
	{
		cout << "Waiting for the event to happen" << endl;
		this_thread::sleep_for(chrono::seconds(2));
	}

	cout << "The event has been set" << endl;
	cout << endl;
	
	//printing shapes
	drawCircle(3);
	drawRectangle(3, 3);
	drawTriangle(4);

	exit(10);
}


int main()
{
    thread thread3(listenForEvent);
    thread thread4(fireEvent);

    thread3.join();
    thread4.join();

    return 0;
}