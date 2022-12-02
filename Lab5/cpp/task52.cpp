#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../../helpers/helpers.h"
using namespace std;

/* program should write the data in another free block if the data is bigger than the size of the block
 create a struct with pointers to the previous and next blocks and data to store
 All the blocks will be in the list. The bitmap array will show the state of the blocks (free or busy)

 Initialise the list of blocks and bitmap in one function.
 Separately initialise the first and the last blocks (the first block will not have previous pointer and the last block will not have the next pointer)

 max size of a block 1 byte. User enters the size of the data that he wants to write in the block
 */

struct Block
{
    Block* m_left;
    Block* m_right;
    int m_size;
    int m_data;


    //Block() : m_left{nullptr}, m_size{10}, m_data{0} {}

    Block(Block* left = nullptr, Block* right = nullptr, int size = 8, int data = 0) : m_left{left}, m_right{right}, m_size{size}, m_data{data} {}

    friend ostream& operator <<(ostream& out, Block& block)
    {
        out << block.m_data;
        return out;
    }
};


//global variables
vector<int> m_arr_bitmap;
vector<Block*> m_arr_blocks;


void printBlocks(const vector<Block*>& arr)
{
    cout << "-------------------------------------------------------------------" << endl;
    printf("%15s %15s %15s %15s", "Block number", "Left address", "Right address","Data");
    cout << endl;
    cout << "-------------------------------------------------------------------" << endl;
//    cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < arr.size(); ++i)
    {
        printf("%15d %15p %15p %15d", i, arr.at(i)->m_left, arr.at(i)->m_right, arr.at(i)->m_data);
        cout << endl;
    }
    cout << endl;
}


//initialising the storage
void initialise()
{
    //adding the first block in the array
    Block* temp_new_block = new Block();
    m_arr_blocks.push_back(temp_new_block);
    m_arr_bitmap.push_back(0);

    cout << "The block space has been successfully initialised" << endl;
}


//m_array_bitmap[i] == 0 => the block has some space in it
//Otherwise, the block is full
void allocate()
{
    //getting the previous block from the array, and creating a new one
    if (m_arr_blocks.empty())
    { initialise(); }
    else
    {
        Block* block_previous = m_arr_blocks.back();
        Block* block_new = new Block();

        block_new->m_left = block_previous;
        block_previous->m_right = block_new;


        //adding a new block and 0 to the arrays
        m_arr_blocks.push_back(block_new);
        m_arr_bitmap.push_back(0);
    }

    cout << "A block has been allocated" << endl;
}


void write(int data)
{
    //allocating memory if there are no blocks, and user tries to write something
    if (m_arr_blocks.empty())
    { allocate(); }

    //getting the last block from the array and its index in the bitmap
    Block* block = m_arr_blocks.back();
    int size_block = block->m_size;
    int size_data_in_block = block->m_data;
    int index_block = m_arr_blocks.size() - 1;


    //checking if there's enough space for the data, and the block is not full
    if (size_data_in_block + data > size_block)
    {
        //adding as much data as I can to the block
        int free_space = size_block - size_data_in_block;
        block->m_data += free_space;


        //changing the state of the block on full in the bitmap
        m_arr_bitmap[index_block] = 1;


        //adding the other part of the data to a new block
        int data_other = data - free_space;
        allocate();
        write(data_other);

        cout << "Some part of the data has been added to this block. And another part to the other block" << endl;
        cout << free_space << " bytes have been added to the block with index: " << index_block << endl;
    }
    else if (size_data_in_block + data == size_block)
    {
        //adding data to the block
        block->m_data += data;

        //changing the state of the block on full in the bitmap
        m_arr_bitmap[index_block] = 1;

        //adding a free block because this block is full
        allocate();

        cout << "The data has been added. And an empty block has been created" << endl;
    }
    else
    {
        block->m_data += data;
        cout << "The data has been added" << endl;
    }
}


void free()
{
    if (m_arr_blocks.empty())
    { cout << "There are no blocks in the array" << endl; }
    else if (m_arr_blocks.size() == 1)
    {
        Block* block = m_arr_blocks.back();

        //deleting the block
        m_arr_blocks.pop_back();
        m_arr_bitmap.pop_back();
        delete block;
        block = nullptr;

        cout << "A block has been freed" << endl;
    }
    else
    {
        //nulling the pointer of the block before the last one
        int index_last_block = m_arr_blocks.size() - 1;
        Block* block_before_last = m_arr_blocks.at(index_last_block - 1);
        block_before_last->m_right = nullptr;

        //deleting the last block
        Block* block = m_arr_blocks.back();
        m_arr_blocks.pop_back();
        m_arr_bitmap.pop_back();
        delete block;
        block = nullptr;

        cout << "A block has been freed" << endl;
    }
}


void read(int index_block)
{
    if (index_block >= 0)
    {
        if (index_block < m_arr_blocks.size())
        { cout << "Read data: " << *m_arr_blocks.at(index_block) << endl; }
        else
        { cout << "Enter an index less than or equal to " << m_arr_blocks.size() << endl; }
    }
    else
    { cout << "Enter an index greater than 0" << endl; }
}


void printInfo()
{
    //counting busy blocks
    int counter_busy_blocks = 0;
    for (int i = 0; i < m_arr_bitmap.size(); ++i)
    {
        if (m_arr_bitmap.at(i) == 1)
        { counter_busy_blocks++; }
    }

    cout << "The size of a data block: " << sizeof(m_arr_blocks.at(0)) << endl;
    cout << "The size of the bitmap: " << sizeof(m_arr_bitmap[0]) * m_arr_bitmap.size() << endl;
    cout << "The amount of blocks: " << m_arr_blocks.size() << endl;
    cout << "The amount of busy blocks: " << counter_busy_blocks << endl;
}


void commitTransaction()
{
    string file_blocks_path = R"(C:\Users\nikit\CLionProjects\Operating Systems\Lab5\files\blocks_transaction.txt)";
    //    \\file.txt
    string file_bitmap_path = R"(C:\Users\nikit\CLionProjects\Operating Systems\Lab5\files\bitmap_transaction.txt)";


    //saving the blocks in one file
    ofstream fout(file_blocks_path);
    if (!fout)
    { cerr << "The file stream has not been opened" << endl; }
    else
    {
        for (Block* block : m_arr_blocks)
        {
            fout << block->m_data << endl;
/*            fout << block->m_right << endl;
            fout << block->m_left << endl;
            fout << block->m_size << endl;
            fout << endl;*/
        }
    }
    fout.close();

    //saving the bitmap in the other file
    ofstream fout2(file_bitmap_path);
    if (!fout2)
    { cerr << "The file stream has not been opened" << endl; }
    else
    {
        //saving the bitmap
        for (int i : m_arr_bitmap)
        { fout2 << i << endl; }
//        fout2 << endl;
    }
    fout2.close();

    cout << "A transaction has just been committed" << endl;
}


void rollbackTransaction()
{
    string file_blocks_path = R"(C:\Users\nikit\CLionProjects\Operating Systems\Lab5\files\blocks_transaction.txt)";


    //restoring the blocks
    ifstream fin(file_blocks_path);
    if (!fin.is_open()) { cerr << "The file stream has not been opened" << endl; }
    else {
        //deleting the data from the arrays
        m_arr_bitmap.clear();
        m_arr_blocks.clear();

        //initialising new arrays
        initialise();

        //reading data from the file
        int data_temp;
        string str_temp = "";

        while (getline(fin,str_temp))
        {
            data_temp = stoi(str_temp);
            write(data_temp);
        }
/*        stringstream splitter(temp2);
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
        cout << "1)Allocate a block" << endl;
        cout << "2)Free a block" << endl;
        cout << "3)Write bytes in the block" << endl;
        cout << "4)Read bytes from the block" << endl;
        cout << "5)Print the bitmap" << endl;
        cout << "6)Print the blocks" << endl;
        cout << "7)Print general info" << endl;
        cout << "8)Commit a transaction" << endl;
        cout << "9)Rollback a transaction" << endl;
        cout << "10)Exit" << endl;

        cin >> choice;
        cin.ignore(32767, '\n');
        cout << endl;

        switch (choice)
        {
            case 1:
                allocate();
                break;

            case 2:
                free();
                break;
            case 3:
                //entering the amount of bytes to write
                cout << "Enter the amount of bytes that you'd like to write" << endl;
                int data_to_write;
                cin >> data_to_write;

                write(data_to_write);
                break;

            case 4:
                cout << "Enter the index of the block you'd like to read bytes from" << endl;
                int index;
                cin >> index;

                read(index);
                break;

            case 5:
                printVector(m_arr_bitmap);
                break;

            case 6:
                printBlocks(m_arr_blocks);
                break;

            case 7:
                printInfo();
                break;

            case 8:
                commitTransaction();
                break;

            case 9:
                rollbackTransaction();
                break;
        }
    } while (choice != 10);
}


int main()
{
    initialise();
    menu();

    //printArr(m_size_array,m_array_bitmap);
    cout << "The program has finished" << endl;
    return 0;
}