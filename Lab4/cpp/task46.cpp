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

/*class Player
{
private:
    vector<Guess*> m_arr_guesses;

public:
    Player() : m_arr_guesses() {}

    vector<Guess*>& getArrGuesses() { return m_arr_guesses; }
};*/


//global vars
const int m_size = 3;
char m_arr_chars[m_size][m_size] =
{
        {'2', '.', '.'},
        {'.' ,'1','1'},
        {'2','.','.'},
};

//random generator
random_device m_rd;
mt19937 m_gen(m_rd());
//min=0 and max=2
uniform_int_distribution<int> m_distr_positions(0, m_size - 1);


void printArray()
{
    for (int i = 0; i < m_size; i++)
    {
        for (int j = 0; j < m_size; j++)
        { cout << m_arr_chars[i][j] << '\t'; }
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


void playFirstTurn(vector<Guess*>& player1_guesses)
{
    int row = 0;
    int column = 0;

    //if the array of guesses is empty, make a random guess
    if (player1_guesses.empty())
    {
        do
        {
            row = m_distr_positions(m_gen);
            column = m_distr_positions(m_gen);
        } while ((m_arr_chars[row][column] == '1') || (m_arr_chars[row][column] == 'x'));


        //if there's an enemy's ship
        if (m_arr_chars[row][column] == '2')
        {
            cout << "The second player's ship has been hit" << endl;

            //creating guesses for the first player
            int row_guess = 0;
            int column_guess = 0;
            if (row == 0)
            {
                if (column == 0)
                {
                    Guess* guess1 = new Guess(row + 1, column);
                    Guess* guess2 = new Guess(row, column + 1);

                    //adding guesses to the list
                    player1_guesses.push_back(guess1);
                    player1_guesses.push_back(guess2);
                }
                else if (column == 1)
                {
                    Guess* guess3 = new Guess(row, column - 1);
                    Guess* guess4 = new Guess(row, column + 1);
                    Guess* guess5 = new Guess(row + 1, column);

                    player1_guesses.push_back(guess3);
                    player1_guesses.push_back(guess4);
                    player1_guesses.push_back(guess5);
                }
                else
                {
                    Guess* guess6 = new Guess(row, column - 1);
                    Guess* guess7 = new Guess(row + 1, column);

                    player1_guesses.push_back(guess6);
                    player1_guesses.push_back(guess7);
                }
            }
            else if (row == 1)
            {
                if (column == 0)
                {
                    Guess* guess8 = new Guess(row + 1, column);
                    Guess* guess9 = new Guess(row, column + 1);
                    Guess* guess10 = new Guess(row - 1, column);

                    //adding guesses to the list
                    player1_guesses.push_back(guess8);
                    player1_guesses.push_back(guess9);
                    player1_guesses.push_back(guess10);
                }
                else if (column == 1)
                {
                    Guess* guess11 = new Guess(row + 1, column);
                    Guess* guess12 = new Guess(row, column + 1);
                    Guess* guess13 = new Guess(row - 1, column);
                    Guess* guess14 = new Guess(row, column - 1);

                    player1_guesses.push_back(guess11);
                    player1_guesses.push_back(guess12);
                    player1_guesses.push_back(guess13);
                    player1_guesses.push_back(guess14);
                }
                else
                {
                    Guess* guess15 = new Guess(row - 1, column);
                    Guess* guess16 = new Guess(row + 1, column);

                    player1_guesses.push_back(guess15);
                    player1_guesses.push_back(guess16);
                }
            }
            else
            {
                if (column == 0)
                {
                    Guess* guess17 = new Guess(row, column + 1);
                    Guess* guess18 = new Guess(row - 1, column);

                    player1_guesses.push_back(guess17);
                    player1_guesses.push_back(guess18);
                }
                else if (column == 1)
                {
                    Guess* guess19 = new Guess(row, column - 1);
                    Guess* guess20 = new Guess(row, column + 1);
                    Guess* guess21 = new Guess(row - 1, column);

                    player1_guesses.push_back(guess19);
                    player1_guesses.push_back(guess20);
                    player1_guesses.push_back(guess21);
                }
                else
                {
                    Guess* guess22 = new Guess(row, column - 1);
                    Guess* guess23 = new Guess(row - 1, column);

                    player1_guesses.push_back(guess22);
                    player1_guesses.push_back(guess23);
                }
            }
        }
        else
        { cout << "The first player has missed" << endl; }
    }
    else
    {
        Guess* temp = nullptr;
        do
        {
            //get a guess from the list
            temp = player1_guesses.back();
            player1_guesses.pop_back();

            row = temp->m_row;
            column = temp->m_column;
        } while ((m_arr_chars[row][column] == '1') || (m_arr_chars[row][column] == 'x'));


        if (m_arr_chars[row][column] == '2')
        { cout << "The second player's ship has been hit" << endl; }
        else
        { cout << "The first player has missed" << endl; }
    }

    m_arr_chars[row][column] = 'x';
}


void playSecondTurn(vector<Guess*>& player2_guesses)
{
    int row = 0;
    int column = 0;

    //if the array of guesses is empty, make a random guess
    if (player2_guesses.empty())
    {
        do
        {
            row = m_distr_positions(m_gen);
            column = m_distr_positions(m_gen);
        } while ((m_arr_chars[row][column] == '2') || (m_arr_chars[row][column] == 'x'));


        //if there's an enemy's ship
        if (m_arr_chars[row][column] == '1')
        {
            cout << "The first player's ship has been hit" << endl;

            //creating guesses for the first player
            int row_guess = 0;
            int column_guess = 0;
            if (row == 0)
            {
                if (column == 0)
                {
                    Guess* guess1 = new Guess(row + 1, column);
                    Guess* guess2 = new Guess(row, column + 1);

                    //adding guesses to the list
                    player2_guesses.push_back(guess1);
                    player2_guesses.push_back(guess2);
                }
                else if (column == 1)
                {
                    Guess* guess3 = new Guess(row, column - 1);
                    Guess* guess4 = new Guess(row, column + 1);
                    Guess* guess5 = new Guess(row + 1, column);

                    player2_guesses.push_back(guess3);
                    player2_guesses.push_back(guess4);
                    player2_guesses.push_back(guess5);
                }
                else
                {
                    Guess* guess6 = new Guess(row, column - 1);
                    Guess* guess7 = new Guess(row + 1, column);

                    player2_guesses.push_back(guess6);
                    player2_guesses.push_back(guess7);
                }
            }
            else if (row == 1)
            {
                if (column == 0)
                {
                    Guess* guess8 = new Guess(row + 1, column);
                    Guess* guess9 = new Guess(row, column + 1);
                    Guess* guess10 = new Guess(row - 1, column);

                    //adding guesses to the list
                    player2_guesses.push_back(guess8);
                    player2_guesses.push_back(guess9);
                    player2_guesses.push_back(guess10);
                }
                else if (column == 1)
                {
                    Guess* guess11 = new Guess(row + 1, column);
                    Guess* guess12 = new Guess(row, column + 1);
                    Guess* guess13 = new Guess(row - 1, column);
                    Guess* guess14 = new Guess(row, column - 1);

                    player2_guesses.push_back(guess11);
                    player2_guesses.push_back(guess12);
                    player2_guesses.push_back(guess13);
                    player2_guesses.push_back(guess14);
                }
                else
                {
                    Guess* guess15 = new Guess(row - 1, column);
                    Guess* guess16 = new Guess(row + 1, column);

                    player2_guesses.push_back(guess15);
                    player2_guesses.push_back(guess16);
                }
            }
            else
            {
                if (column == 0)
                {
                    Guess* guess17 = new Guess(row, column + 1);
                    Guess* guess18 = new Guess(row - 1, column);

                    player2_guesses.push_back(guess17);
                    player2_guesses.push_back(guess18);
                }
                else if (column == 1)
                {
                    Guess* guess19 = new Guess(row, column - 1);
                    Guess* guess20 = new Guess(row, column + 1);
                    Guess* guess21 = new Guess(row - 1, column);

                    player2_guesses.push_back(guess19);
                    player2_guesses.push_back(guess20);
                    player2_guesses.push_back(guess21);
                }
                else
                {
                    Guess* guess22 = new Guess(row, column - 1);
                    Guess* guess23 = new Guess(row - 1, column);

                    player2_guesses.push_back(guess22);
                    player2_guesses.push_back(guess23);
                }
            }
        }
        else
        { cout << "The second player has missed" << endl; }
    }
    else
    {
        Guess* temp = nullptr;
        do
        {
            //get a guess from the list
            temp = player2_guesses.back();
            player2_guesses.pop_back();

            row = temp->m_row;
            column = temp->m_column;
        } while ((m_arr_chars[row][column] == '2') || (m_arr_chars[row][column] == 'x'));


        if (m_arr_chars[row][column] == '1')
        { cout << "The first player's ship has been hit" << endl; }
        else
        { cout << "The second player has missed" << endl; }
    }

    m_arr_chars[row][column] = 'x';
}

void menu(vector<Guess*>& player1_guesses, vector<Guess*>& player2_guesses)
{
    int choice;

    do
    {
        cout << "What do you want to do?" << endl;
        cout << "1)Play the first player's turn" << endl;
        cout << "2)Play the second player's turn" << endl;
        cout << "3)Print the gameboard" << endl;
        cout << "4)Print first player's guesses" << endl;
        cout << "5)Print second player's guesses" << endl;
        cout << "6) Exit" << endl;

        cin >> choice;
        cin.ignore(32767, '\n');
        cout << endl;

        switch (choice)
        {
            case 1:
                playFirstTurn(player1_guesses);
                break;

            case 2:
                playSecondTurn(player2_guesses);
                break;

            case 3:
                printArray();
                break;

            case 4:
                printGuesses(player1_guesses);
                break;

           case 5:
                printGuesses(player2_guesses);
                break;
        }


    } while (choice != 6);
}

int main()
{
    vector<Guess*> arr_player1_guesses;
    vector<Guess*> arr_player2_guesses;

    menu(arr_player1_guesses, arr_player2_guesses);

    return 0;
}