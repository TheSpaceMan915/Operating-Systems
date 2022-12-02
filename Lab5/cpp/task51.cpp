#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../../helpers/helpers.h"
using namespace std;

//program should write the data in another free block if the data is bigger than the size of the block

//global variables
int m_size;
int* m_arr_data;
int* m_arr_bitmap;


//initialising the storage
void initialiseBlockSpace(int size)
{
    m_size = size;
    m_arr_data = new int[size];
    m_arr_bitmap = new int[size];

    for (int i = 0; i < size; i++)
    {
        m_arr_data[i] = 0;
        m_arr_bitmap[i] = 1;
    }

    cout << "The block space has been successfully initialised" << endl;
}


//m_array_bitmap[i] == 1 => the block is free
//Otherwise, the block is busy
//allocating empty blocks
void allocateBlocks()
{
    int amount = 0;

    cout << "How many blocks would you like to allocate?" << endl;
    cin >> amount;

    //getting the indexes of free blocks
    vector<int> arr_block_indexes_free;
    vector<int> arr_block_indexes_allocated;
    for (int i = 0; i < m_size; i++)
    {
        if (m_arr_bitmap[i] == 1)
        { arr_block_indexes_free.push_back(i); }
    }


    //if the number of free blocks >= the amount user asks for
    if (arr_block_indexes_free.size() >= amount)
    {
        //allocating blocks
        int temp = 0;
        for (int j = 0; j < amount; j++)
        {
            temp = arr_block_indexes_free.back();
            arr_block_indexes_free.pop_back();

            arr_block_indexes_allocated.push_back(temp);
            m_arr_bitmap[temp] = 0;
        }

        //printing the indexes of allocated blocks
        cout << "The indexes of allocated blocks: ";
        printVector(arr_block_indexes_allocated);
    }
    else
    { cout << "There are not enough blocks to allocate" << endl; }
}


void freeBlocks()
{
    //getting the amount of blocks to be freed
    cout << "How many blocks do you want to free?" << endl;
    int amount = 0;
    cin >> amount;

    //getting the indexes of blocks to be freed
    cout << "Enter the indexes of blocks" << endl;
    int* arr_indexes = new int[amount];
    int index = 0;

    for (int i = 0; i < amount; ++i)
    {
        cin >> index;
        arr_indexes[i] = index;
    }

    //freeing blocks and deleting data in the arr_data
    for (int i = 0; i < amount; ++i)
    {
        index = arr_indexes[i];
        m_arr_bitmap[index] = 1;
        m_arr_data[index] = 0;
    }

    cout << "The blocks have been freed" << endl;
}


void writeValues()
{
    int amount = 0;
    int index = 0;
    int value = 0;
    int* arr_indexes = new int[amount];

    //getting the amount of blocks to write data in
    cout << "How many blocks do you want to write data in?" << endl;
    cin >> amount;

    cout << "Enter the indexes of blocks" << endl;
    for (int i = 0; i < amount; ++i)
    {
        cin >> index;

        //checking if the block has been allocated
        //if it has, accept the index. Otherwise, skip it
        if (m_arr_bitmap[index] == 0)
        { arr_indexes[i] = index; }
        else
        {
            cout << "The block with index " << index << "is not allocated" << endl;
            cout << "You ought to allocate the block first" << endl;
            return;
        }
    }

    cout << "Enter the values" << endl;
    for (int i = 0; i < amount; ++i)
    {
        cin >> value;
        index = arr_indexes[i];
        m_arr_data[index] = value;
    }
}


void readValues()
{
    int amount = 0;
    int index = 0;
    int* arr_indexes = new int[amount];

    //getting the amount of blocks to read data from
    cout << "How many blocks do you want to read data from?" << endl;
    cin >> amount;

    cout << "Enter the indexes of blocks" << endl;
    for (int i = 0; i < amount; ++i)
    {
        cin >> index;
        arr_indexes[i] = index;
    }

    cout << "Here are the values:" << endl;
    for (int i = 0; i < amount; ++i)
    {
        index = arr_indexes[i];
        cout << m_arr_data[index] << ' ';
    }
    cout << endl;
}


void printInfo()
{
    //counting empty blocks
    int counter_empty_blocks = 0;
    for (int i = 0; i < m_size; ++i)
    {
        if (m_arr_data[i] == 0)
        { counter_empty_blocks++; }
    }

    cout << "The size of a data block: " << sizeof(m_arr_data[0]) << endl;
    cout << "The size of the bitmap: " << sizeof(m_arr_bitmap[0]) * m_size << endl;
    cout << "The amount of blocks: " << m_size << endl;
    cout << "The amount of empty blocks: " << counter_empty_blocks << endl;
}


void commitTransaction()
{
    string file_path = R"(C:\Users\nikit\CLionProjects\Operating Systems\Lab5\files\transaction.txt)";

    ofstream fout(file_path);
    if (!fout)
    { cerr << "The file stream has not been opened" << endl; }
    else
    {
        //saving the data array
        fout << "m_arr_data:" << endl;

        for (int i = 0; i < m_size; ++i)
        {
            fout << m_arr_data[i] << ' ';
        }
        fout << endl;


        //saving the bitmap
        fout << "m_arr_bitmap:" << endl;

        for (int i = 0; i < m_size; ++i)
        {
            fout << m_arr_bitmap[i] << ' ';
        }
        fout << endl;

        cout << "A transaction has just been committed" << endl;
    }
}


void rollbackTransaction()
{
    string file_path = R"(C:\Users\nikit\CLionProjects\Operating Systems\Lab5\files\transaction.txt)";

    ifstream fin(file_path);
    if (!fin.is_open())
    { cerr << "The file stream has not been opened" << endl; }
    else
    {
        string temp1 = "";
        string temp2 = "";

        //reading the header
        getline(fin,temp2);

        //reading the arr_data
        getline(fin,temp2);

/*        int number = 0;
        char c = ' ';
        int j = 0;
        for (int i = 0; i < temp2.size(); ++i)
        {
            //taking a char from the string
            c = temp2[i];
            if (c != ' ')
            {
                //casting the char to an int
                number = c - '0';

                //placing the int to the m_arr_data
                m_arr_data[j++] = number;
            }
        }*/
        stringstream splitter(temp2);
        for (int i = 0; i < m_size; ++i)
        {
            //getting a number from the string
            splitter >> temp1;

            //putting the number in the array
            m_arr_data[i] = stoi(temp1);
        }

        //reading the header
        getline(fin,temp2);

        //reading the arr_bitmap
        getline(fin,temp2);

        for (int i = 0; i < m_size; ++i)
        {
            //getting a number from the string
            splitter >> temp1;

            //putting the number in the array
            m_arr_bitmap[i] = stoi(temp1);
        }
/*        int number2 = 0;
        char ch = ' ';
        int k = 0;
        for (int i = 0; i < temp2.size(); ++i)
        {
            //taking a char from the string
            ch = temp2[i];
            if (ch != ' ')
            {
                //casting the char to an int
                number2 = ch - '0';

                //placing the int to the m_arr_bitmap
                m_arr_bitmap[k++] = number2;
            }
        }*/
        cout << "A rollback has been performed successfully" << endl;
    }
}

void menu()
{
    int choice;

    do
    {
        cout << "What do you want to do?" << endl;
        cout << "1)Print bitmap" << endl;
        cout << "2)Print data array" << endl;
        cout << "3)Allocate blocks" << endl;
        cout << "4)Free blocks" << endl;
        cout << "5)Write values in blocks" << endl;
        cout << "6)Read values from blocks" << endl;
        cout << "7)Print info" << endl;
        cout << "8)Commit a transaction" << endl;
        cout << "9)Rollback a transaction" << endl;
        cout << "10)Exit" << endl;

        cin >> choice;
        cin.ignore(32767, '\n');
        cout << endl;

        switch (choice)
        {
            case 1:
                printArr(m_size,m_arr_bitmap);
                break;

            case 2:
                printArr(m_size,m_arr_data);
                break;
            case 3:
                allocateBlocks();
                break;

            case 4:
                freeBlocks();
                break;

            case 5:
                writeValues();
                break;

            case 6:
                readValues();
                break;

            case 7:
                printInfo();
                break;

            case 8:
                commitTransaction();
                break;

            case 9:
                rollbackTransaction();
        }
    } while (choice != 10);
}


int main()
{
    initialiseBlockSpace(10);
    menu();

    //printArr(m_size_array,m_array_bitmap);
    cout << "The program has finished" << endl;
    return 0;
}