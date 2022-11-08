#include <iostream>
#include <thread>
#include <Windows.h>
#include <chrono>
#include <time.h>
#include <cstdlib>
using namespace std;

/// <summary>
/// if semaphone if available, WaitForSingleObject() returns WAIT_OBJECT_0
/// Otherwise, it returs anything but that
/// </summary>


//creating a semaphone
const int m_number_petrol_pumps = 4;
int m_number_free_pumps = 4;
HANDLE m_semaphone = CreateSemaphore(
	NULL, 
	m_number_petrol_pumps,		//initial count
	m_number_petrol_pumps,		//maximum count
	NULL);


void fillCarUp()
{
	while (true)
	{
		//cout << "A car has started filling up" << endl;
		int number = rand() % 5 + 10;
		this_thread::sleep_for(chrono::seconds(number));

		ReleaseSemaphore(
			m_semaphone,
			1,					//increase count by 1
			NULL);
		m_number_free_pumps++;

		cout << "The car has finished filling up" << endl;
	}
	
}


void countCars()
{
	srand(time(NULL));
	int m_counter_cars = 0;

	while (true)
	{ 
		int number = rand() % 5 + 10;
		DWORD res = WaitForSingleObject(m_semaphone, 0L);

		if (res != WAIT_OBJECT_0)
		{
			m_counter_cars++;
			cout << "The car has just driven by!" << endl;
			cout << "The number of cars driven by: " << m_counter_cars << endl;
		}
		else
		{ 
			cout << "The car stopped at the station" << endl; 
			m_number_free_pumps--;
			cout << "The number of free pumps " << m_number_free_pumps << endl;
		}

		this_thread::sleep_for(chrono::seconds(number));
	}
}


int main()
{
    cout << endl;
    thread thread5(countCars);
    thread thread6(fillCarUp);

    thread5.join();
    thread6.join();

    return 0;
}