#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "../../helpers/helpers.h"
using namespace std;


struct Block
{
    Block* m_left;
    Block* m_right;
    int m_size;
    int m_data;


    Block(Block* left = nullptr, Block* right = nullptr, int size = 8, int data_this_block = 0) : m_left{left}, m_right{right}, m_size{size}, m_data{data_this_block} {}

    friend ostream& operator <<(ostream& out, Block& block)
    {
        out << block.m_data;
//        out << block.m_data_next_block;
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
    for (int i = 0; i < arr.size(); ++i)
    {
        printf("%15d %15p %15p %15d", i, arr.at(i)->m_left, arr.at(i)->m_right, arr.at(i)->m_data);
        cout << endl;
    }
    cout << endl;
}


//m_array_bitmap[i] == 0 => the block has some space in it
//Otherwise, the block is full
void allocate()
{
    Block* block_new = new Block();

    //adding a new block and 0 to the arrays
    m_arr_blocks.push_back(block_new);
    m_arr_bitmap.push_back(0);

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


        //creating a new block and connecting to the previous one
        allocate();
        Block* block_new = m_arr_blocks.back();
        block_new->m_left = block;
        block->m_right = block_new;


        //adding the other part of the data to a new block
        int data_other = data - free_space;
        write(data_other);

        cout << "Some part of the data has been added to this block. And another part to the other block" << endl;
//        cout << free_space << " bytes have been added to the block with index: " << index_block << endl;
    }
    else if (size_data_in_block + data == size_block)
    {
        //adding data to the block
        block->m_data += data;

        //changing the state of the block on full in the bitmap
        m_arr_bitmap[index_block] = 1;

        cout << "The data has been added" << endl;
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
        Block* block_last = m_arr_blocks.back();

        //deleting the block_last
        m_arr_blocks.pop_back();
        m_arr_bitmap.pop_back();
        delete block_last;
        block_last = nullptr;

        cout << "A block has been freed" << endl;
    }
    else
    {
        //recursively deleting connected blocks
        Block* block_last = m_arr_blocks.back();
        if (block_last->m_left == nullptr)
        {
            m_arr_blocks.pop_back();
            m_arr_bitmap.pop_back();
            delete block_last;
            block_last = nullptr;
        }
        else
        {
            m_arr_blocks.pop_back();
            m_arr_bitmap.pop_back();
            delete block_last;
            block_last = nullptr;
            free();
        }

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
    string file_blocks_path = R"(..\Lab5\files\blocks_transaction.txt)";
    string file_bitmap_path = R"(..\Lab5\files\bitmap_transaction.txt)";


    //saving the blocks in one file
    ofstream fout(file_blocks_path);
    if (!fout)
    { cerr << "The file stream has not been opened" << endl; }
    else
    {
        int sum_data = 0;
        for (Block* block : m_arr_blocks)
        {
            sum_data += block->m_data;
            if (block->m_right == nullptr)
            {
                fout << sum_data << endl;
                sum_data = 0;
            }
        }

        fout.close();
    }

    //saving the bitmap in the other file
    ofstream fout2(file_bitmap_path);
    if (!fout2)
    { cerr << "The file stream has not been opened" << endl; }
    else
    {
        //saving the bitmap
        for (int i : m_arr_bitmap)
        { fout2 << i << endl; }
    }
    fout2.close();

    cout << "A transaction has just been committed" << endl;
}


void rollbackTransaction()
{
    string file_blocks_path = R"(..\Lab5\files\blocks_transaction.txt)";


    //restoring the blocks
    ifstream fin(file_blocks_path);
    if (!fin.is_open()) { cerr << "The file stream has not been opened" << endl; }
    else {
        //deleting the data from the arrays
        m_arr_bitmap.clear();
        m_arr_blocks.clear();

        //initialising new arrays
        allocate();

        //reading data from the file
        int data_temp;
        string str_temp = "";

        while (getline(fin,str_temp))
        {
            data_temp = stoi(str_temp);
            write(data_temp);
        }

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
    allocate();
    menu();

    //printArr(m_size_array,m_array_bitmap);
    cout << "The program has finished" << endl;
    return 0;
}