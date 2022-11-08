#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Page
{
private:
	int m_value;
	int m_counter_modifications;
	int m_counter_requests;
	bool m_flag_in_memory;		//the page is in memory - true, otherwise false

public:

	Page() : m_value{ 0 }, m_counter_modifications{ 0 }, m_counter_requests{ 0 }, m_flag_in_memory{ false } {}

	Page(int value, int counter_modifications, int counter_requests, bool flag_in_memory) : m_value{ value }, m_counter_modifications{ counter_modifications }, m_counter_requests{ counter_requests }, m_flag_in_memory{ flag_in_memory} {}


	int getValue() { return m_value;  }

	int getCounterModifications() { return m_counter_modifications; }

	int getCounterRequests() { return m_counter_requests; }

	bool getFlag() { return m_flag_in_memory; }


	void setCounterModifications(int value) { m_counter_modifications = value; }

	void setCounterRequests(int value) { m_counter_requests = value; }

	void setValue(int value) { m_value = value; }

	void setFlag(bool value) { m_flag_in_memory = value; }

	string checkState()
	{
		string res = "";
		if (m_flag_in_memory)
		{ res = "In the memory"; }
		else
		{ res = "On the disk"; }

		return res;
	}
};


//global variables
const int m_size_memory = 4;
const int m_size_disk = 6;
const int m_max_memory_index = m_size_disk + m_size_memory;


vector<Page*> m_arr_memory;
vector<Page*> m_arr_disk;

//thread thread_timer(sort,NULL);
//thread will sort elements in the list. It will be the timer
//sort elements using modification counter


void printArray(vector<Page*>& arr)
{
	for (Page* page : arr)
	{
		cout << page->getValue() << " ";
	}
	cout << endl;
	cout << endl;
}

void printMemory()
{
	cout << "Memory: ";
	printArray(m_arr_memory);

	cout << "Disk: ";
	printArray(m_arr_disk);
}

void printPagesInfo()
{
	cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;
	printf("%30s %30s %30s %30s", "Page number", "Amount of modifications", "Amount of requests", "State");
	cout << endl;
	cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;

	Page* temp = NULL;
	int amount_modifications = 0;
	int amount_requests = 0;
	string state = "";

	int j = 0;
	for (j = 0; j < m_arr_memory.size(); j++)
	{
		temp = m_arr_memory.at(j);
		amount_modifications = temp->getCounterModifications();
		amount_requests = temp->getCounterRequests();
		
		state = temp->checkState();
			
		printf("%30d %30d %30d %30s", j, amount_modifications, amount_requests, state.c_str());
		cout << endl;
	}

	for (int k = 0; k < m_arr_disk.size(); k++)
	{
		temp = m_arr_disk.at(k);
		amount_modifications = temp->getCounterModifications();
		amount_requests = temp->getCounterRequests();

		state = temp->checkState();

		printf("%30d %30d %30d %30s", k + j, amount_modifications, amount_requests, state.c_str());
		cout << endl;
	}
	cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;
}

void createListPages()
{
	Page* temp = NULL;
	//initialise the memory array
	for (int i = 0; i < m_size_memory; i++)
	{
		temp = new Page(0, 0, 0, true);
		m_arr_memory.push_back(temp);
	}
	
	//initialise the disk array
	for (int j = 0; j < m_size_disk; j++)
	{
		temp = new Page(0, 0, 0, false);
		m_arr_disk.push_back(temp);
	}
}


Page* findNotRecentlyUsedPage()
{
	Page* res = NULL;
	int amount_modifications = 0;
	int amount_requests = 0;
	int min_sum = INT_MAX;
	
	
	int sum = 0;
	Page* temp = NULL;
	for (int i = 0; i < m_size_memory; i++)
	{
		temp = m_arr_memory.at(i);
		amount_modifications = m_arr_memory.at(i)->getCounterModifications();
		amount_requests = m_arr_memory.at(i)->getCounterRequests();
	
		sum = amount_modifications + amount_requests;

		if (sum < min_sum)
		{
			min_sum = sum;
			res = temp;
		}
	}

	return res;
}


void writeValue(int index)
{
	if (index >= m_max_memory_index)
	{ 
		cerr << "There's no memory cell with this index" << endl;
		return;
	}


	int input;
	cout << "Enter your number" << endl;
	cin >> input;
	cin.ignore(32767, '\n');

	if (index < m_size_memory)
	{
		Page* page = m_arr_memory.at(index);


		if (!page->getFlag())
		{
			page->setFlag(true);
			cout << "The page with the index " << index << " was loaded from the disk" << endl;
		}
		
		//increment the counter of the page and set the value in it
		int counter_modifications = page->getCounterModifications();
		page->setCounterModifications(++counter_modifications);
		page->setValue(input);
	}
	else
	{
		//find the page that I need to free
		Page* page_freed = findNotRecentlyUsedPage();
		page_freed->setFlag(false);


		//find the page on the disk
		int index_disk = index - m_size_memory;
		Page* page_on_disk = m_arr_disk.at(index_disk);


		if (!page_on_disk->getFlag())
		{
			page_on_disk->setFlag(true);
			cout << "The page with the index " << index << " was loaded from the disk" << endl;
		}

		//change the counters of the disk page and set the value in it
		int counter_modifications = page_on_disk->getCounterModifications();
		page_on_disk->setCounterModifications(++counter_modifications);
		page_on_disk->setValue(input);
	}
}

void readValue(int index)
{
	if (index >= m_max_memory_index)
	{
		cerr << "There's no memory cell with this index" << endl;
		return;
	}

	//check if
	int res = 0;
	if (index < m_size_memory)
	{
		Page* page = m_arr_memory.at(index);

		if (!page->getFlag())
		{
			page->setFlag(true);
			cout << "The page with the index " << index << " was loaded from the disk" << endl;
		}

		//check the increment
		int counter_requests = page->getCounterRequests();
		page->setCounterRequests(++counter_requests);
		res = page->getValue();
	}
	else
	{
		int index_disk = index - m_size_memory;
		Page* page_on_disk = m_arr_disk.at(index_disk);

		if (!page_on_disk->getFlag())
		{
			page_on_disk->setFlag(true);
			cout << "The page with the index " << index << " was loaded from the disk" << endl;
		}

		int counter_requests = page_on_disk->getCounterRequests();
		page_on_disk->setCounterRequests(++counter_requests);
		
		res = page_on_disk->getValue();
	}

	cout << res << endl;
}

void menu()
{
	int choice, index, value_entered;

	do
	{
		cout << "What do you wanna do?" << endl;
		cout << "1) Read a value from a page" << endl;
		cout << "2) Put a value in a page" << endl;
		cout << "3) Print memory" << endl;
		cout << "4) Print memory pages info" << endl;
		cout << "5) Clear the concole" << endl;
		cout << "6) Exit" << endl;

		cin >> choice;
		cin.ignore(32767, '\n');
		cout << endl;

		switch (choice)
		{
		case 1:
			cout << "Enter the index of the page you'd like to read data from" << endl;
			cin >> index;
			cin.ignore(32767, '\n');

			readValue(index);
			cout << endl;
			break;

		case 2:
			cout << "Enter the index of the page you'd like to put data in" << endl;
			cin >> index;
			cin.ignore(32767, '\n');
			
			writeValue(index);
			cout << endl;
			break;

		case 3:
			printMemory();
			break;

		case 4:
			printPagesInfo();
			break;

		case 5:
			system("cls");
			break;
		}


	} while (choice != 6);
}


int main()
{
    createListPages();
    menu();

    /*
    writeValue(7);
    writeValue(7);

    readValue(7);
    readValue(7);

    write(8);
    */
}