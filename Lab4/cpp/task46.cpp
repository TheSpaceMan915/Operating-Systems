#include <iostream>
#include <random>
#include <vector>
using namespace std;


//classes
struct Guess
{
    int m_row;
    int m_column;

    Guess() : m_row{0}, m_column{0} {}

    Guess(int x, int y) : m_row{x}, m_column{y} {}
};


//global vars
const int m_size = 6;
char m_arr_gameboard[m_size][m_size];

//random generator
random_device m_rd;
mt19937 m_gen(m_rd());
//min=0 and max=2
uniform_int_distribution<int> m_distr_positions(0, m_size - 1);


void createGameboard()
{
    for (int i = 0; i < m_size; i++)
    {
        for (int j = 0; j < m_size; j++)
        {  m_arr_gameboard[i][j] = '.'; }
    }
}


void placeFirstPlayerShip()
{
    int row = 0;
    int column = 0;

    cout << "Enter row: " << endl;
    cin >> row;
    cout << "Enter column: " << endl;
    cin >> column;

    m_arr_gameboard[row][column] = '1';
}


void placeSecondPlayerShip()
{
    int row = 0;
    int column = 0;

    cout << "Enter row: " << endl;
    cin >> row;
    cout << "Enter column: " << endl;
    cin >> column;

    m_arr_gameboard[row][column] = '2';
}


void printGameboard()
{
    for (int i = 0; i < m_size; i++)
    {
        if ( i == 0)
        {
          cout << "\t0\t 1\t 2\t 3\t 4\t 5" << endl;
        }
        cout << i << '\t';
        for (int j = 0; j < m_size; j++)
        {
            cout << m_arr_gameboard[i][j] << '\t';
        }
        cout << endl;
    }
    cout << endl;
}


void printGuesses(vector<Guess*>& guesses)
{
    cout << "Your guesses:" << endl;
    for (int i = 0; i < guesses.size(); i++)
    { cout << i << ". Row:" << guesses[i]->m_row << " Column: " << guesses[i]->m_column << endl; }
}


void playFirst(vector<Guess*>& player1_guesses)
{
    //if the array of guesses is empty, make a random guess
    if (player1_guesses.empty())
    {
        int row_rand = 0;
        int column_rand = 0;

        do
        {
            row_rand = m_distr_positions(m_gen);
            column_rand = m_distr_positions(m_gen);
        } while ((m_arr_gameboard[row_rand][column_rand] == '1') || (m_arr_gameboard[row_rand][column_rand] == 'x'));

        //if there's an enemy's ship
        if (m_arr_gameboard[row_rand][column_rand] == '2')
        {
            cout << "The second player's ship has been hit" << endl;
            int row_guess = 0;
            int column_guess = 0;


            //below
            row_guess = row_rand + 1;
            column_guess = column_rand;
            if (row_guess < m_size)
            {
                Guess *guess1 = new Guess(row_guess, column_guess);
                player1_guesses.push_back(guess1);
            }


            //above
            row_guess = row_rand - 1;
            column_guess = column_rand;
            if (row_guess >= 0)
            {
                Guess *guess2 = new Guess(row_guess, column_guess);
                player1_guesses.push_back(guess2);
            }


            //on the right
            row_guess = row_rand;
            column_guess = column_rand + 1;
            if (column_guess < m_size)
            {
                Guess *guess3 = new Guess(row_guess, column_guess);
                player1_guesses.push_back(guess3);
            }


            //on the left
            row_guess = row_rand;
            column_guess = column_rand - 1;
            if (column_guess >= 0)
            {
                Guess *guess4 = new Guess(row_guess, column_guess);
                player1_guesses.push_back(guess4);
            }
        }
        else
        { cout << "The first player has missed" << endl; }

        m_arr_gameboard[row_rand][column_rand] = 'x';
    }
    else
    {
        Guess *temp = nullptr;
        int row = 0;
        int column = 0;
        do
        {
            //get a guess from the list
            temp = player1_guesses.back();
            player1_guesses.pop_back();

            row = temp->m_row;
            column = temp->m_column;
        } while ((m_arr_gameboard[row][column] == '1') || (m_arr_gameboard[row][column] == 'x'));


        if (m_arr_gameboard[row][column] == '2')
        { cout << "The second player's ship has been hit" << endl; }
        else
        { cout << "The first player has missed" << endl; }

        m_arr_gameboard[row][column] = 'x';
    }
}


void playSecond(vector<Guess*>& player2_guesses)
{
    //if the array of guesses is empty, make a random guess
    if (player2_guesses.empty())
    {
        int row_rand = 0;
        int column_rand = 0;

        do
        {
            row_rand = m_distr_positions(m_gen);
            column_rand = m_distr_positions(m_gen);
        } while ((m_arr_gameboard[row_rand][column_rand] == '2') || (m_arr_gameboard[row_rand][column_rand] == 'x'));

        //if there's an enemy's ship
        if (m_arr_gameboard[row_rand][column_rand] == '1')
        {
            cout << "The first player's ship has been hit" << endl;
            int row_guess = 0;
            int column_guess = 0;


            //below
            row_guess = row_rand + 1;
            column_guess = column_rand;
            if (row_guess < m_size)
            {
                Guess *guess1 = new Guess(row_guess, column_guess);
                player2_guesses.push_back(guess1);
            }


            //above
            row_guess = row_rand - 1;
            column_guess = column_rand;
            if (row_guess >= 0)
            {
                Guess *guess2 = new Guess(row_guess, column_guess);
                player2_guesses.push_back(guess2);
            }


            //on the right
            row_guess = row_rand;
            column_guess = column_rand + 1;
            if (column_guess < m_size)
            {
                Guess *guess3 = new Guess(row_guess, column_guess);
                player2_guesses.push_back(guess3);
            }


            //on the left
            row_guess = row_rand;
            column_guess = column_rand - 1;
            if (column_guess >= 0)
            {
                Guess *guess4 = new Guess(row_guess, column_guess);
                player2_guesses.push_back(guess4);
            }
        }
        else
        { cout << "The second player has missed" << endl; }

        m_arr_gameboard[row_rand][column_rand] = 'x';
    }
    else
    {
        Guess *temp = nullptr;
        int row = 0;
        int column = 0;
        do
        {
            //get a guess from the list
            temp = player2_guesses.back();
            player2_guesses.pop_back();

            row = temp->m_row;
            column = temp->m_column;
        } while ((m_arr_gameboard[row][column] == '2') || (m_arr_gameboard[row][column] == 'x'));


        if (m_arr_gameboard[row][column] == '1')
        { cout << "The first player's ship has been hit" << endl; }
        else
        { cout << "The second player has missed" << endl; }

        m_arr_gameboard[row][column] = 'x';
    }
}


void menu(vector<Guess*>& player1_guesses, vector<Guess*>& player2_guesses)
{
    int choice;

    do
    {
        cout << "What do you want to do?" << endl;
        cout << "1)Place a first player's ship" << endl;
        cout << "2)Place a second player's ship" << endl;
        cout << "3)Play the first player's turn" << endl;
        cout << "4)Play the second player's turn" << endl;
        cout << "5)Print the gameboard" << endl;
        cout << "6)Print first player's guesses" << endl;
        cout << "7)Print second player's guesses" << endl;
        cout << "8) Exit" << endl;

        cin >> choice;
        cin.ignore(32767, '\n');
        cout << endl;

        switch (choice)
        {
            case 1:
                placeFirstPlayerShip();
                break;

            case 2:
                placeSecondPlayerShip();
                break;
            case 3:
                playFirst(player1_guesses);
                break;

            case 4:
                playSecond(player2_guesses);
                break;

            case 5:
                printGameboard();
                break;

            case 6:
                printGuesses(player1_guesses);
                break;

           case 7:
                printGuesses(player2_guesses);
                break;
        }
    } while (choice != 8);
}

int main()
{
    vector<Guess*> arr_player1_guesses;
    vector<Guess*> arr_player2_guesses;

    createGameboard();
    menu(arr_player1_guesses, arr_player2_guesses);

    return 0;
}