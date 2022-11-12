#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
using namespace std;


void redirectStreams()
{
    //creating input and output streams
    string path_in = "C:\\Users\\nikit\\CLionProjects\\Operating Systems\\Lab3\\files\\task3_input.txt";
    string path_out = "C:\\Users\\nikit\\CLionProjects\\Operating Systems\\Lab3\\files\\task3_output.txt";
    ifstream fin(path_in);
    ofstream fout(path_out);


    //saving old input and output buffers
    streambuf* cin_buf = cin.rdbuf();
    streambuf* cout_buf = cout.rdbuf();


    //changing the buffers of the streams to redirect them
    cin.rdbuf(fin.rdbuf());
    cout.rdbuf(fout.rdbuf());


    //testing the changed streams
    string temp;
    while (getline(cin,temp))
    { cout << temp << endl; }


    Sleep(5000);

    //resetting standard streams
    cin.rdbuf(cin_buf);
    cout.rdbuf(cout_buf);


    //testing the reset streams
    cout << "The streams have been reset" << endl;
    cout << "Enter a word" << endl;
    cin >> temp;
    
    cout << endl;
    cout << "Your word is " << temp << endl;
}


int main()
{
    redirectStreams();

    return 0;
}