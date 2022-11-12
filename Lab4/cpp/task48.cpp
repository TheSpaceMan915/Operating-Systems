//
// Created by nikit on 11/10/2022.
//
#include <iostream>
#include <thread>
#include <mutex>
#include <iterator>
#include <list>
#include <time.h>
using namespace std;


list<int> m_list;
unsigned long long m_list_size;
mutex m_section;        //critical section


void printList()
{
    for (int number : m_list)
    {
        cout << number << '\t';
    }
    cout << endl;
}

/*void heapify(int arr[], int n, int i)
{
    int largest = i; // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n && arr[l] > arr[largest])
        largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest])
        largest = r;

    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}


void heapSort(int arr[], int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i >= 0; i--) {
        // Move current root to end
        swap(arr[0], arr[i]);

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}*/

void sortList()
{
   const int min = 5;
   const int max = 10;
   int rand_number = 0;

    while (true)
    {
        this_thread::sleep_for(chrono::seconds(8));
        cout << endl;
        cout << "Before sorting: ";
        printList();
        //rand_number = (rand() % (max - min + 1) + min);

        m_list.sort();
        cout << "After sorting: ";
        printList();
        cout << endl;
    }

}


//adding and deleting elements in the list
void doAddDell()
{
    int rand_position = 0;
    int rand_number1 = 0;
    int rand_number2 = 0;
    int min = 0;
    unsigned long long max = 0;

    while (true)
    {
        m_section.lock();

        //getting current size of the list
        max = m_list.size() - 1;
        list<int>::iterator it_rand = m_list.begin();

        //generating a random position and moving the it_rand
        rand_position = (rand() % (max - min + 1) + min);
        cout << "rand_position: " << rand_position << endl;
        advance(it_rand, rand_position);

        //generating random values
        max = 100;
        rand_number1 = (rand() % (max - min + 1) + min);
        rand_number2 = (rand() % (max - min + 1) + min);

        cout << "rand_number1: " << rand_number1 << endl;
        cout << "rand_number2: " << rand_number2 << endl;


        //adding two new elements and deleting an element
        m_list.insert(it_rand, rand_number1);
        m_list.insert(it_rand, rand_number2);
        m_list.erase(it_rand);

        //this_thread::sleep_for(chrono::seconds(rand_number1));
        cout << "Random adding and deleting have happened" << endl;
        m_section.unlock();
        this_thread::sleep_for(chrono::seconds(16));
    }
}

int main()
{
    srand(time(NULL));

    //pushing some ints in the list
    m_list.push_back(2);
    m_list.push_back(4);
    m_list.push_back(8);
    m_list.push_back(7);
    m_list.push_back(43);

    thread thread1(doAddDell);
    thread thread2(sortList);

    thread1.join();
    thread2.join();

    return 0;
}