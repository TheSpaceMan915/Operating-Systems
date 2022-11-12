#include <iostream>
#include <vector>
#include "../helpers/helpers.h"
using namespace std;


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

    printArr(size,m_arr_data);
    cout << "The block space has been successfully initialised" << endl;
}


//m_arr_bitmap[i] == 1 => the block if free
//Otherwise, the block is busy
//allocating empty blocks
void allocateBlocks(int amount)
{
    //getting the indexes of free blocks
    vector<int>arr_indexes;
    for (int i = 0; i < m_size; i++)
    {
        if (m_arr_bitmap[i] == 1)
        { arr_indexes.push_back(i); }
    }

    //if the number of free blocks >= the amount user asks for
    if (arr_indexes.size() >= amount)
    {
        //allocating blocks
        int temp = 0;
        for (int j = 0; j < amount; j++)
        {
            temp = arr_indexes.back();
            arr_indexes.pop_back();

            m_arr_bitmap[temp] = 0;
        }
    }
    else
    { cout << "There are not enough blocks to allocate" << endl; }
}



int main()
{
    initialiseBlockSpace(10);
    allocateBlocks(3);
    printArr(m_size,m_arr_bitmap);

    cout << "The program has finished" << endl;
    return 0;
}