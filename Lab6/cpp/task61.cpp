#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
using namespace std;
namespace fs = filesystem;


// data block object
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

        return out;
    }
};


//one inode describes one file
struct Inode
{
    //the array for 12 blocks
    vector<Block*> m_arr_blocks;

    //indirect blocks
    Block* m_indirect_block;
    Block** m_double_indirect_block;
    Block*** m_triple_indirect_block;

    string m_file_path;


    Inode(string file_path, int position)
    {
        m_file_path = file_path;

        //initialising the indirect blocks
        Block* block_new1 = new Block();
        Block* block_new2 = new Block();

        //double indirect
        m_double_indirect_block = &block_new1;

        //triple indirect
        Block** ptr2_block = &block_new2;
        m_triple_indirect_block = &ptr2_block;
    }
};


vector<Inode*> arr_inodes;
Inode* current_file;
int current_position_in_file;


// working with blocks_________________________________________________________________
void allocate()
{
    Block* block_new = new Block();

    //adding a new block to the array
    current_file->m_arr_blocks.push_back(block_new);
    cout << "A block has been allocated" << endl;
}


void writeDataToBlocks(int data)
{
    //allocating memory if there are no blocks, and user tries to write something
    if (current_file->m_arr_blocks.empty())
    { allocate(); }


    //getting the last block from the array
    Block* block = current_file->m_arr_blocks.back();
    int size_block = block->m_size;
    int size_data_in_block = block->m_data;


    //checking if there's enough space for the data, and the block is not full
    if (size_data_in_block + data > size_block)
    {
        //adding as much data as I can to the block
        int free_space = size_block - size_data_in_block;
        block->m_data += free_space;


        //creating a new block and connecting to the previous one
        allocate();
        Block* block_new =current_file->m_arr_blocks.back();
        block_new->m_left = block;
        block->m_right = block_new;


        //adding the other part of the data to a new block
        int data_other = data - free_space;
        writeDataToBlocks(data_other);

        cout << "Some part of the data has been added to this block. And another part to the other block" << endl;
//        cout << free_space << " bytes have been added to the block with index: " << index_block << endl;
    }
    else if (size_data_in_block + data == size_block)
    {
        //adding data to the block
        block->m_data += data;
        cout << "The data has been added" << endl;
    }
    else
    {
        block->m_data += data;
        cout << "The data has been added" << endl;
    }
}


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


// working with files_________________________________________________________________
void createFile(string path)
{
    cout << "Enter a name of the file" << endl;
    string file_name = "";

    cin >> file_name;
    path += file_name;
    path += ".txt";

    ofstream fin(path);
    if (!fin)
    { cerr << "The file stream has not been opened" << endl; }
    else
    {
        Inode* inode = new Inode(path,0);
        arr_inodes.push_back(inode);

        fin.close();
        cout << "The file has been created" << endl;
    }
}


void openFile()
{
    cout << "Files: " << endl;
    for (int i = 0; i < arr_inodes.size(); ++i)
    { cout <<i << ":\t" << arr_inodes.at(i)->m_file_path << endl; }

    //choosing a file you want to work with
    int choice = INT_MAX;
    while (choice >= arr_inodes.size())
    {
        cout << "Choose a file you would like to work with" << endl;
        cin >> choice;
    }

//    int choice = 0;

    //setting the current file
    current_file = arr_inodes.at(choice);
    cout << "The file has been chosen" << endl;
}


void writeBlocksToFile(int length)
{
    //writing the blocks to a file
    ofstream fout(current_file->m_file_path);
    if (!fout)
    { cerr << "The file stream has not been opened" << endl; }
    else
    {
        int data_block = 0;
        int size_arr_blocks = current_file->m_arr_blocks.size();


        //checking  if there are enough blocks in the file
        int index_end = size_arr_blocks - current_position_in_file;
        if (length <= index_end)
        {
            for (int i = current_position_in_file; length > 0; ++i)
            {
                //getting the data from the block
                data_block = current_file->m_arr_blocks.at(i)->m_data;

                //writing to the file
                fout << data_block << endl;

                length--;
            }

            cout << "The blocks have been written to the file" << endl;
        }
        else
        { cerr << "The are not enough blocks. Change the position in the file" << endl; }

        fout.close();
    }
}


int countLines()
{
    int counter_lines = 0;
    ifstream fin(current_file->m_file_path);


    if (!fin)
    { cerr << "Can't count the lines. The file stream has not been opened" << endl; }
    else
    {
        string str_temp = "";

        //counting how many lines there are in the file
        while (fin)
        {
            getline(fin,str_temp);
            counter_lines++;
        }

        fin.close();
    }

    return counter_lines;
}


void printCurrentPosition()
{ cout << "The current position in the file: " << current_position_in_file << endl; }


void changeCurrentPosition(int position)
{
    int size_arr = current_file->m_arr_blocks.size();


    //checking if the position is valid
    if (position < size_arr)
    {
        current_position_in_file = position;

        cout << "The position has been changed" << endl;
    }
    else
    { cerr << "The position should be less than " << size_arr << endl; }
}


void readBlocksFromFile(int length)
{
    //reading the blocks from a file
    ifstream fin(current_file->m_file_path);
    if (!fin)
    { cerr << "The file stream has not been opened" << endl; }
    else
    {
        string str_temp = "";
        int number_lines = countLines();


        //checking  if there are enough blocks in the file
        int index_end = number_lines - current_position_in_file;
        if (length <= index_end)
        {
            //skipping the lines before the position in the file
            int number_lines_to_skip = current_position_in_file;
            while (number_lines_to_skip > 0)
            {
                getline(fin,str_temp);
                number_lines_to_skip--;
            }


            //reading the necessary amount of lines
            cout << "Your blocks: " << endl;
            while (length > 0)
            {
                getline(fin,str_temp);
                cout << str_temp << endl;
                length--;
            }
        }
        else
        { cerr << "The are not enough blocks. Change the position in the file" << endl; }
    }

    fin.close();
}


void deleteFile()
{
    cout << "Files: " << endl;
    for (int i = 0; i < arr_inodes.size(); ++i)
    { cout <<i << ":\t" << arr_inodes.at(i)->m_file_path << endl; }

    //choosing a file you want to delete
    int choice = INT_MAX;
    while (choice >= arr_inodes.size())
    {
        cout << "Choose a file you want to delete" << endl;
        cin >> choice;
    }


    //deleting the file
    string file_path = arr_inodes.at(choice)->m_file_path;
    if (remove(file_path.c_str()) != 0)
    { cerr << "deleteFile has failed" << endl; }
    else
    { cout << "The file has been successfully deleted" << endl; }

    //deleting the inode
    arr_inodes.erase(arr_inodes.begin() + choice);
}


//searching for a file
void searchForFile(const fs::path& directory, const fs::path& file_name)
{
    auto d = fs::recursive_directory_iterator(directory);

    auto found = find_if(d, end(d), [&file_name](const auto& dir_entry)
    {
        return dir_entry.path().filename() == file_name;
    });


    if (found != end(d))
    {
        // we have found what we were looking for
        cout << "The file " << file_name << " has been found" << endl;
    }
    else
    { cout << "There is no such file in the directory" << endl; }
}


//creating a directory
void createDirectory()
{
    cout << "Enter a path to the directory" << endl;
    string path_directory = "";
    getline(cin,path_directory);

    bool res = fs::create_directory(path_directory);
    if (res)
    { cout << "A new directory has been created" << endl; }
    else
    {cerr << "createDirectory has failed" << endl; }
}


//deleting a directory
void deleteDirectory()
{
    cout << "Enter a path to the directory" << endl;
    string path_directory = "";
    getline(cin,path_directory);

    bool res = fs::remove_all(path_directory);
    if (res)
    { cout << "The directory has been deleted" << endl; }
    else
    {cerr << "deleteDirectory has failed" << endl; }
}


void menu()
{
    int value = 0;
    string path_directory = R"(..\Lab6\files\)";
    do
    {
        cout << endl;
        cout << "What would you like to do?" << endl;
        cout << "1)Create a file" << endl;
        cout << "2)Open a file" << endl;
        cout << "3)Allocate a block" << endl;
        cout << "4)Write data to blocks" << endl;
        cout << "5)Write blocks to a file" << endl;
        cout << "6)Read blocks from a file" << endl;
        cout << "7)Print blocks" << endl;
        cout << "8)Print the current position in the file" << endl;
        cout << "9)Change the current position in the file" << endl;
        cout << "10)Delete the file" << endl;
        cout << "11)Search for a file in a directory" << endl;
        cout << "12)Create a directory" << endl;
        cout << "13)Delete a directory" << endl;
        cout << "14)Exit" << endl;

        cin >> value;
        cin.ignore(32767, '\n');
        cout << endl;

        switch (value)
        {
            case 1:
            {
                createFile(path_directory);
                break;
            }

            case 2:
            {
                openFile();
                break;
            }

            case 3:
            {
                allocate();
                break;
            }

            case 4:
            {
                cout << "Enter how many bytes you want to write" << endl;
                int data;
                cin >> data;
                writeDataToBlocks(data);
                break;
            }

            case 5:
            {
                {
                    int amount = INT_MAX;
                    while (amount > current_file->m_arr_blocks.size())
                    {
                        cout << "Enter the amount of blocks you'd like to write" << endl;
                        cin >> amount;
                    }

                    writeBlocksToFile(amount);
                    break;
                }
            }

            case 6:
            {
                cout << "Enter the amount of blocks you'd like to read" << endl;
                int amount;
                cin >> amount;

                readBlocksFromFile(amount);
                break;
            }

            case 7:
            {
                printBlocks(current_file->m_arr_blocks);
                break;
            }

            case 8:
            {
                printCurrentPosition();
                break;
            }

            case 9:
            {
                cout << "Enter a new position in the file" << endl;
                int number;
                cin >> number;

                changeCurrentPosition(number);
                break;
            }

            case 10:
            {
                deleteFile();
                break;
            }

            case 11:
            {
                cout << "Enter a directory" << endl;
                string str_path_directory = "";
                getline(cin,str_path_directory);

                cout << "Enter a file" << endl;
                string str_path_file = "";
                getline(cin,str_path_file);

                searchForFile(str_path_directory,str_path_file) ;
                break;
            }

            case 12:
            {
                createDirectory();
                break;
            }

            case 13:
            {
                deleteDirectory();
                break;
            }
        }
    } while (value != 14);
}


int main()
{
/*//    test set up
    string path_directory = R"(..\Lab6\files\)";
    createFile(path_directory);
    openFile();
    allocate();
    writeDataToBlocks(17);
    writeDataToBlocks(3);*/


    menu();

    cout << "Finish checking" << endl;
}