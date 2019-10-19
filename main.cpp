//------------------------------------
//  vexed.cpp
//     Move pieces to be adjacent.  Once adjacent pieces match, they are cleared off the board.  Once the entire
//     board is clear, play advances to the next level.
//     
//  Prog 4: Vexed
//  Author: Dale Reed
//  Using Codio, for UIC CS 141, Fall 2019
//
//  Running the program looks like:
/*
		Welcome to Vexed!
		The objective is to place identical pieces next to each other, so that they vanish,
		clearing the board completely within the indicated number of moves. On each move
		enter the row, column, and direction (L or R) to move, in either upper or lower
		case. You may also enter 'x' to exit the program, or 'r' to reset the current level.


			Board 0 par 4
			  0 1 2 3 4 5 6 7 8 9
			-----------------------
		  A | . . . . . . . . . . | A
		  B | . . . . . . . . . . | B
		  C | . . . @ &     . . . | C
		  D | . . . . .     . . . | D
		  E | . . .         . . . | E
		  F | . . . &     @ . . . | F
		  G | . . . . & @ . . . . | G
		  H | . . . . . . . . . . | H
			-----------------------
			  0 1 2 3 4 5 6 7 8 9
		1. Your move: c4r

			Board 0 par 4
			  0 1 2 3 4 5 6 7 8 9
			-----------------------
		  A | . . . . . . . . . . | A
		  B | . . . . . . . . . . | B
		  C | . . . @   &   . . . | C
		  D | . . . . .     . . . | D
		  E | . . .         . . . | E
		  F | . . . &     @ . . . | F
		  G | . . . . & @ . . . . | G
		  H | . . . . . . . . . . | H
			-----------------------
			  0 1 2 3 4 5 6 7 8 9

			Board 0 par 4
			  0 1 2 3 4 5 6 7 8 9
			-----------------------
		  A | . . . . . . . . . . | A
		  B | . . . . . . . . . . | B
		  C | . . . @       . . . | C
		  D | . . . . . &   . . . | D
		  E | . . .         . . . | E
		  F | . . . &     @ . . . | F
		  G | . . . . & @ . . . . | G
		  H | . . . . . . . . . . | H
			-----------------------
			  0 1 2 3 4 5 6 7 8 9

			Board 0 par 4
			  0 1 2 3 4 5 6 7 8 9
			-----------------------
		  A | . . . . . . . . . . | A
		  B | . . . . . . . . . . | B
		  C | . . . @       . . . | C
		  D | . . . . .     . . . | D
		  E | . . .     &   . . . | E
		  F | . . . &     @ . . . | F
		  G | . . . . & @ . . . . | G
		  H | . . . . . . . . . . | H
			-----------------------
			  0 1 2 3 4 5 6 7 8 9

			Board 0 par 4
			  0 1 2 3 4 5 6 7 8 9
			-----------------------
		  A | . . . . . . . . . . | A
		  B | . . . . . . . . . . | B
		  C | . . . @       . . . | C
		  D | . . . . .     . . . | D
		  E | . . .         . . . | E
		  F | . . . &   & @ . . . | F
		  G | . . . . & @ . . . . | G
		  H | . . . . . . . . . . | H
			-----------------------
			  0 1 2 3 4 5 6 7 8 9
		2. Your move:

 */
#include <iostream>   // For cin, cout
#include <fstream>    // For file input
#include <chrono>     // Used in pausing for some milliseconds using sleep_for(...)
#include <thread>     // Used in pausing for some milliseconds using sleep_for(...)
using namespace std;

// Global constants
const int MaxNumberOfBoards = 118;
const int BoardRows = 8;
const int BoardColumns = 10;
const int NumberOfPresetBoardParValues = 60;    // After the first 60 par values, the default par value is 15
const char FileName[] = "boards.txt";
const int SleepAmount = 150;                    // Length of time to pause, in milliseconds


//------------------------------------------------------------------------------------------------------------------
// Class used to store a game board
class Board
{
    public:
        // Constructor declarations  

    
        // Get and Set member functions, corresponding to the private data members

    
        // Utility member function declarations.  


    private:
        // The private data members go here
        // ...
        // 
};  // end class Board



//------------------------------------------------------------------------------------------------------------------
// Class used to read in and store all game boards
class AllBoards
{
    public:
        //------------------------------------------------------------------------------------------------------------------
        // Constructor that reads in data from the data file.
        AllBoards();    // Declaration.  See the definition outside the class, below.
    
        // Get and Set member functions.
    
        //------------------------------------------------------------------------------------------------------------------
        // Find and return a particular board.
        Board getBoard( int boardIndex)
        {
            // Validate board index number
            if( boardIndex < 0 || boardIndex > MaxNumberOfBoards) {
                cout << "In getBoard() inside AllBoards class, boardIndex "
                     << boardIndex << " is out of range. Exiting..." << endl;
                exit( -1);
            }
            
            // Construct a Board from one of all the boards
            Board newBoard( boardIndex, allBoards[ boardIndex], levelParValues[ boardIndex]);
            return newBoard;
        }
    
        //------------------------------------------------------------------------------------------------------------------
        // Retrieve the par value
        int getParValue( int boardIndex) {
            int theParValue = 15;   // default
            if( boardIndex < NumberOfPresetBoardParValues) {
                theParValue = levelParValues[ boardIndex];
            }
            return theParValue;
        } // end getParValue()
    
    private:
        int par = 0;             // The number of moves it should take to solve this level
        int currentLevel = -1;   // Which board we're on
        int allBoards[ 118][ BoardRows * BoardColumns];   // Array to store all 118 8x10 boards
    
        // Par values for levels 0 through 59.  Default is 15 after that.
        const int levelParValues[ NumberOfPresetBoardParValues] =
            // 0   1   2   3   4   5   6   7   8   9
            { 4,  3, 14,  5,  6,  4,  4, 14,  5, 25,  //  0 -  9
             12, 17, 17,  8,  4, 12,  9,  8, 12,  8,  // 10 - 19
             10, 10, 16, 13, 20, 14, 10,  9, 12, 14,  // 20 - 29
             15, 13, 20,  8, 15, 10, 10, 11,  7, 23,  // 30 - 39
              8, 11, 16, 14, 12, 13, 13,  3, 35, 18,  // 40 - 49
             26, 10, 13, 18, 26, 12, 15,  5, 22, 15}; // 50 - 59
}; //end class allBoards


//------------------------------------------------------------------------------------------------------------------
// AllBoards constructor
AllBoards::AllBoards()
 {
    // Read in all the boards from the data file
    ifstream inputFileStream;  // declare the input file stream

    // Open input file and verify file open worked.
    inputFileStream.open( FileName);
    if( !inputFileStream.is_open()) {
        cout << "Could not find input file. " << FileName << ".  Exiting..." << endl;
        exit( -1);
    }

    // Read the five lines of comments at the top of the datafile.  Datafile structure is:
    //    // Originally from Vexed v2.0 - globals.c "Global variable declarations"
    //    // Copyright (C) 1999 James McCombe (cybertube@earthling.net)
    //    // September 1,2001 - Version 2.0 changes by Mark Ingebretson (ingebret@yahoo.com) and others.
    //    // Oct 13, 2019  Format changed by Dale Reed (reed @ uic.edu)
    //    // Covered by the GNU General Public License https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
    //    // Level 000
    //    9 9 9 9 9 9 9 9 9 9
    //    9 9 9 9 9 9 9 9 9 9
    //    9 9 9 2 1 0 0 9 9 9
    //    9 9 9 9 9 0 0 9 9 9
    //    9 9 9 0 0 0 0 9 9 9
    //    9 9 9 1 0 0 2 9 9 9
    //    9 9 9 9 1 2 9 9 9 9
    //    9 9 9 9 9 9 9 9 9 9
    //    // Level 001
    //    9 9 9 9 9 9 9 9 9 9
    //    9 9 9 9 9 9 9 9 9 9
    // ...
    
    // Read and discard the five lines of comments at the beginning of the file.
    string inputLine;
    for( int i=0; i<5; i++) {
        getline(inputFileStream, inputLine);
    }
    
    // Now read each of the 118 (MaxNumberOfBoards) boards.  Each one starts with a comment that has the board number.
    for( int boardIndex = 0; boardIndex < MaxNumberOfBoards; boardIndex++) {
        // Read the comment with the board number
        getline(inputFileStream, inputLine);
        
        // For each board now read the BoardRows * BoardColumns number of board values, as integers
        for( int row=0; row<BoardRows; row++) {
            for( int col=0; col<BoardColumns; col++) {
                inputFileStream >> allBoards[ boardIndex][ row * BoardColumns + col];
            }
            // Get rid of end-of-line character at the end of each board, so we can get a clean start on reading the
            //   first line of the next board, on the line with the comment and board number.
            getline(inputFileStream, inputLine);
        }
    }//end for( int boardIndex=0...

    // Close the input file.
    inputFileStream.close();
}//end AllBoards() constructor


//------------------------------------------------------------------------------------------------------------------
void displayDirections()
{
    cout << "Welcome to Vexed!  \n"
         << "The objective is to place identical pieces next to each other, so that they vanish,   \n"
         << "clearing the board completely within the indicated number of moves. On each move      \n"
         << "enter the row, column, and direction (L or R) to move, in either upper or lower       \n"
         << "case. You may also enter 'x' to exit the program, or 'r' to reset the current level. \n"
         << endl;
}


//------------------------------------------------------------------------------------------------------------------
// Driver for the program, using the classes and functions declared above
int main()
{
    AllBoards allTheBoards;     // Reads in and stores all the boards from a data file
    int currentBoardIndex = 0;  // Starting board index
    Board theBoard;             // The board instance, that is set to hold the values for each level
    int score = 0;              // Score accumulates par points for each level finished
    
    displayDirections();
    
	theBoard = allTheBoards.getBoard( currentBoardIndex);
	theBoard.displayBoard();
        
    return 0;
}
