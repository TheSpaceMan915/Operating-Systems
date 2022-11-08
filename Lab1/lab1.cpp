#include <iostream>
#include <vector>
#include <time.h>
using namespace std;

class MemoryBlock
{
private:
    int m_id_process;
    int* m_address;
    int m_size;
    bool m_state;
    //add a string to discribe the process

    static int amount_busy_memory;
    static int amount_free_memory;
    
    static int number_busy_blocks;
    static int number_free_blocks;

public:
    MemoryBlock(int id_process,int* address, int size, bool flag) : m_id_process{id_process}, m_address{address}, m_size{size}, m_state{flag} {}

    void printState() 
    {
        cout << "State: ";
        if (m_state)
        { cout << " busy"; }
        else
        { cout << " free"; }
        
        cout << endl;
    }

    int getSize() { return m_size; }

    int* getAddress() { return m_address; }

    bool getState() { return m_state; }

    int getIdProcess() { return m_id_process; }

    static int getAmountFreeMemory() { return amount_free_memory; }

    static int getAmountBusyMemory() { return amount_busy_memory; }

    static int getNumberFreeBlocks() { return number_free_blocks; }

    static int getNumberBusyBlocks() { return number_busy_blocks; }

    void setAddress(int* address) { m_address = address; }

    void setIdProcess(int id) { m_id_process = id;  }

    void setState(bool state) { m_state = state; }

    void setSize(int size) { m_size = size; }

    static void setAmountFreeMemory(int value) { amount_free_memory = value; }

    static void setAmountBusyMemory(int value) { amount_busy_memory = value; }

    static void setNumberFreeBlocks(int value) { number_free_blocks = value; }

    static void setNumberBusyBlocks(int value) { number_busy_blocks = value; }
};

int MemoryBlock::amount_busy_memory = 0;
int MemoryBlock::amount_free_memory = 40;

int MemoryBlock::number_busy_blocks = 0;
int MemoryBlock::number_free_blocks = 1;


void bubbleSort(vector<MemoryBlock*> &arr)
{
    for (int i = 1; i < arr.size(); ++i)     //цикл устанавливает кол-во итераций
    {
        for (int j = 0; j < arr.size() - i; j++)     //цикл идёт по элем. массива и сравн. сосед. элем.
        {
            if (arr.at(j)->getSize() > arr.at(j + 1)->getSize())
            {
                MemoryBlock* z = arr.at(j + 1);
                arr.at(j + 1) = arr.at(j);
                arr.at(j) = z;
            }
        }
    }
}


void allocate(vector<MemoryBlock*>& list, int amount)
{
    cout << endl;
    //get the counters
    int free_memory = MemoryBlock::getAmountFreeMemory();
    int busy_memory = MemoryBlock::getAmountBusyMemory();

    int free_blocks = MemoryBlock::getNumberFreeBlocks();
    int busy_blocks = MemoryBlock::getNumberBusyBlocks();


    if (free_memory < amount)
    { 
        cerr << "There's only " << free_memory << " byte available memory" << endl;
        return;
    }

    bubbleSort(list);

    for (MemoryBlock* block : list)
    {
        //if the block is free
        if (!block->getState())
        {
            if (block->getSize() == amount)
            {
                block->setState(true);
                block->setIdProcess(rand());
                
                int* start = block->getAddress();
                int* end = start + amount;
                int id_process = block->getIdProcess();
                for (int* ptr = start; ptr != end; ptr++)
                {
                    //change the value of the pointer
                    //write the id of the process in the array
                    *ptr = id_process;
                }


                cout << "The address of the memory block: " << start << endl;


                //change the amount of free and busy memory            
                MemoryBlock::setAmountFreeMemory(free_memory - amount);
                MemoryBlock::setAmountBusyMemory(busy_memory + amount);

                MemoryBlock::setNumberBusyBlocks(++busy_blocks);
                MemoryBlock::setNumberFreeBlocks(--free_blocks);
                break;
            }
            else if (block->getSize() > amount)
            {
                //change the old block
                int old_size = block->getSize();
                int* old_address = block->getAddress();
                int rand_id = rand();
                block->setSize(amount);
                block->setState(true);
                block->setIdProcess(rand_id);

                //add the new process id to the array;
                int* old_block_end = old_address + amount;
                for (int* start = old_address; start != old_block_end; start++)
                { *start = rand_id; }


                cout << "The address of the memory block: " << old_address << endl;


                //create a block containg free memory
                //the beginning of this block is at the address: old_address + amount
                int* new_block_address = old_address + amount;
                int new_block_size = old_size - amount;
                MemoryBlock* block_two = new MemoryBlock(NULL, new_block_address, new_block_size, false);

                int* new_block_end = new_block_address + new_block_size;
                for (int* start = new_block_address; start != new_block_end; start++)
                { *start = NULL; }

                list.push_back(block_two);

                //change the amount of free and busy memory
                MemoryBlock::setAmountFreeMemory(free_memory - amount);
                MemoryBlock::setAmountBusyMemory(busy_memory + amount);

                MemoryBlock::setNumberBusyBlocks(++busy_blocks);
                //MemoryBlock::setNumberFreeBlocks(++free_blocks);
                break;
            }
        }
        
    }
    cout << endl;
}

void freeMemory(vector<MemoryBlock*>& list, int* address)
{
    //find the block to be freed
    MemoryBlock* block_freed = NULL;
    int block_freed_ind = 0;

    for (int i = 0; i < list.size(); i++)
    {
        if (list.at(i)->getAddress() == address)
        {
            block_freed = list.at(i);
            block_freed_ind = i;
            break;
        }
    }


    //free the space
    int* address_block = block_freed->getAddress();
    int* address_block2 = block_freed->getAddress();
    int size = block_freed->getSize();
    int* end_address = address_block + size;


    for (int* start = address_block; start != end_address; start++)
    { *start = NULL; }


    cout << "The memory block having the size " << size << " has been freed" << endl;

    int free_memory = MemoryBlock::getAmountFreeMemory();
    int busy_memory = MemoryBlock::getAmountBusyMemory();

    int free_blocks = MemoryBlock::getNumberFreeBlocks();
    int busy_blocks = MemoryBlock::getNumberBusyBlocks();

    MemoryBlock::setAmountBusyMemory(busy_memory - size);
    MemoryBlock::setAmountFreeMemory(free_memory + size);

    MemoryBlock::setNumberBusyBlocks(--busy_blocks);


    //try to find the block to the left
    while (*(address_block - 1) == NULL)
    { address_block--; }

    MemoryBlock* left_block = NULL;
    int block_left_ind = 0;
    int size_left_block = 0;
    if (address_block != block_freed->getAddress())
    {
        for (int i = 0; i < list.size(); i++)
        {
            //combine the block to the left and the freed block
            if (list.at(i)->getAddress() == address_block)
            {
                left_block = list.at(i);
                size_left_block = list.at(i)->getSize();
                list.at(i)->setSize(size_left_block + size);
                block_left_ind = i;
                
                list.erase(list.begin() + block_freed_ind);
                delete block_freed;
                block_freed = NULL;
                break;
            }
        }
    }
    
    // and to the right of the freed block
    int* address_right_block = address_block2 + size;
    int id_right_block = *address_right_block;

    if (id_right_block == NULL)
    {
        for (MemoryBlock* block : list)
        {
            //combine the block to the right and the freed block
            if (block->getAddress() == address_right_block)
            {
                if (left_block != NULL)
                {
                    int size_right_block = block->getSize();
                    block->setSize(size_left_block + size_right_block);
                    block->setAddress(left_block->getAddress());

                    list.erase(list.begin() + block_left_ind);
                    delete left_block;
                    left_block = NULL;

                    list.erase(list.begin() + block_freed_ind);
                    delete block_freed;
                    block_freed = NULL;
                    return;
                }
                else
                {
                    int size_right_block = block->getSize();
                    block->setSize(size_right_block + size);
                    block->setAddress(block_freed->getAddress());

                    list.erase(list.begin() + block_freed_ind);
                    delete block_freed;
                    block_freed = NULL;
                    return;
                }
                
            }
        }
    }
    
    if (id_right_block != NULL)
    {
        if(left_block == NULL)
        { 
            block_freed->setIdProcess(NULL);
            block_freed->setState(false);
            MemoryBlock::setNumberFreeBlocks(++free_blocks);
        }
    }

    cout << endl;
}


void printInfo(const vector<MemoryBlock*>& arr)
{
    cout << endl;

    for (MemoryBlock* block : arr)
    {
         cout << "Address: " << block->getAddress() << endl;
         block->printState();
         cout << "Size: " << block->getSize() << endl;
         cout << endl;
    }
 

    cout << "The total number of busy blocks: " << MemoryBlock::getNumberBusyBlocks() << endl;
    cout << "The total amount of busy memory: " << MemoryBlock::getAmountBusyMemory() << endl;
    cout << "The total number of free blocks: " << MemoryBlock::getNumberFreeBlocks() << endl;
    cout << "The total amount of free memory: " << MemoryBlock::getAmountFreeMemory() << endl;
    cout << endl;
}

void printArray(int* arr, int size_arr)
{
    for (int i = 0; i < size_arr; i++)
    {
        cout << arr[i] << '\t';
    }

    cout << endl;
}


void menu(vector<MemoryBlock*>& myList,int* array,int size_arr)
{
    int choice, amount_memory, size_block;

    do
    {
        cout << "What do you want to do?" << endl;
        cout << "1) Allocate memory" << endl;
        cout << "2) Free memory" << endl;
        cout << "3) Print memory info" << endl;
        cout << "4) Clear the concole" << endl;
        cout << "5) Print the array" << endl;
        cout << "6) Exit" << endl;

        cin >> choice;
        cin.ignore(32767, '\n');
        cout << endl;

        switch (choice)
        {
        case 1:
            cout << "Enter the amount of memory you'd like to allocate" << endl;
            cin >> amount_memory;
            cin.ignore(32767, '\n');
            
            allocate(myList, amount_memory);            
            break;

        case 2:
            cout << "Enter the size of the memory block you'd like to free" << endl;
            cin >> size_block;
            cin.ignore(32767, '\n');
            cout << endl;

            for (MemoryBlock* block : myList)
            {
                if (block->getSize() == size_block)
                {
                    freeMemory(myList, block->getAddress());                   
                    break;
                }
            }
            break;

        case 3:
            printInfo(myList);
            break;

        case 4:
            system("cls");
            break;

        case 5:
            printArray(array, size_arr);
            break;
        }
       

    } while (choice != 6);
}


int main()
{
    srand((unsigned)time(NULL));
    const int size = 40;
    int* arr_bytes = new int[size];

    //create a list of blocks and add an initial block to the list
    vector<MemoryBlock*> list_blocks;
    MemoryBlock* initial_block = new MemoryBlock(0, arr_bytes, size, false);
    list_blocks.push_back(initial_block);

    menu(list_blocks,arr_bytes,size);

    /*
    //Testing
    allocate(list_blocks, 20);
    allocate(list_blocks, 10);
    freeMemory(list_blocks, list_blocks.at(0)->getAddress());
    freeMemory(list_blocks, list_blocks.at(1)->getAddress());
    */

    return 0;
}
