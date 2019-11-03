//------------------------------------
//  vexed.cpp
//     Move pieces to be adjacent.  Once adjacent pieces match, they are cleared off the board.  Once the entire
//     board is clear, play advances to the next level.
//     
//  Prog 4: Vexed
//  Author: Karan Singh Kochar
//  Using Codio, for UIC CS 141, Fall 2019
// ------------------------------------

#include <iostream>   // For cin, cout
#include <fstream>    // For file input
#include <vector>     // to use vector
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
const char players[] = {'&', '@', '+', '%', '^', '#', '=', '*'};  //char represented as a player in our game

//------------------------------------------------------------------------------------------------------------------
// Class used to store a game board
class Board
{
public:
    // Constructor declarations
    Board(); // default Constructor
    Board( int boardNumber, int allBoardArr[], int parValue); // overloaded constructor

    // Get and Set member functions, corresponding to the private data members
    int getPar(){  // getter for Par value for board.
        return parValue;
    }

    // Utility member function declarations.
    void DisplayBoard();
    int  checkArray(int rowInput, int columnInput, char direction); // checks the array for the move direction and moves if valid move
    void doSomething();
    void gravityEffect();
    bool gravityRequired();
    int  magicIndex(char c);
    void Vanish(int sudoIndex);
    bool completionCheck();

private:
    // The private data members go here
    int boardNumber; //Board level
    char boardArr[8][10]; // 2D Board Array
    int parValue; // Par value for each board.
    vector <char> charsInBoard;

    //private functions go here.
    char ChangeNumToChar(int val);


};  // end class Board

// Default constructor deceleration for Board ---------------------------------------------------------------------
Board::Board(){
    boardNumber = -1;
    parValue= -1;
    // setting all elements of board to -1
    for(int i = 0; i < BoardRows; i++){
        for(int j = 0; j < BoardColumns ; j++){
            boardArr[i][j] = -1;
        }
    }
} //Board::Board() ends here.

// Board(game level, 1D board, par value) -------------------------------------------------------------------------
// Overloaded Constructor for Board class
Board::Board(int boardNumber, int allBoardArr[], int parValue){
    this->boardNumber = boardNumber;
    this->parValue = parValue;
    charsInBoard.clear(); // starting with a clean vector;
    //deciphering elements from a 1d array to a 2d array.
    for(int i = 0; i < BoardRows; i++){
        for (int j = 0; j < BoardColumns; j++){
            char c = ChangeNumToChar(allBoardArr[i*BoardColumns + j]); // Converting int to designated char;
            this->boardArr[i][j] = c; // Mapping 1d array to the 2d board array
        }
    }

} // Board::Board(int boardNumber... ends here.

// checkArray(current row index, current column input, move direction) ----------------------------------------------
int Board::checkArray(int rowInput, int columnInput, char direction){
    char temp = boardArr[rowInput][columnInput];
    if( temp == '.' || temp == ' '){
        cout << "Attempting to move an invalid character.  Please retry." << endl;
        return -1; //since error
    }
    else {
        if( boardArr[rowInput][columnInput+1] == ' ' && direction == 'R'){
            boardArr[rowInput][columnInput+1] = boardArr[rowInput][columnInput];
            boardArr[rowInput][columnInput] = ' ';
            this-> DisplayBoard();
            this->doSomething();
        }
        else if( boardArr[rowInput][columnInput-1] == ' ' && direction == 'L' ){
            boardArr[rowInput][columnInput-1] = boardArr[rowInput][columnInput];
            boardArr[rowInput][columnInput] = ' ';
            this-> DisplayBoard();
            this->doSomething();
        }
        else{
            cout << "Attempting to move into a non-empty space.  Please retry." << endl;
            return -1; // since error
        }

    }
    return 0; // valid input hence 0.

} // Board::checkArray ends here.

// doSomething()  ---------------------------------------------------------------------------------------------------
void Board::doSomething(){ // just a function to bring all the following functions together.

    gravityEffect(); // if any elements are floating then use gravity to bring them down.

    for (int i = 0; i < sizeof(players); ++i) { // iterating through all the game pieces and checking if any could be eliminated.
        int sudoIndex = this->magicIndex(players[i]); //returns sudo index. (i am not considering the case where there are multiple instances of max values)
        if(sudoIndex != 0) {
            this->Vanish(sudoIndex); // get rid of the peaces that has to go.
            this-> DisplayBoard(); //display board.
            doSomething();
        }
    }
}// doSomething() ends here.


// gravityRequired() --------------------------------------------------------------------------------------------------
// checks if any character is floating in the air. i.e. has an empty space below the character
bool Board::gravityRequired(){
    for (int i = 0; i < BoardRows; ++i) {
        for (int j = 0; j < BoardColumns; ++j) {
            char element = boardArr[i][j];
            if( element != '.' && element != ' '){
                if(boardArr[i+1][j] == ' '){
                    return true;
                }
            }
        } //for int j....
    } // for int i...
    return false;
}// gravityRequired() ends here.

// gravityEffect() -----------------------------------------------------------------------------------------------------
// brings all the elements floating in the air down, i.e. makes sure that no element has space beneath.
void Board::gravityEffect(){

    int flag = 1; //need to use a flg so that i can get out of for loops back into the while loop
    while(gravityRequired()){

        for (int i = BoardRows-1; i >= 0; --i) {
            if(flag){ // required to exit out of the for loop to prevent multiple elements to fall down.
                for (int j = BoardColumns-1; j >= 0; --j) { // scans from the bottom to pull bottom most element down 1st
                    char element = boardArr[i][j];
                    if( element != '.' && element != ' '){ // making sure it a player char
                        if(boardArr[i+1][j] == ' '){
                            // changing player position
                            boardArr[i+1][j] = element;
                            boardArr[i][j] = ' ';
                            this-> DisplayBoard();
                            flag = 0;
                            break;
                        }
                    }
                }// int for j...
            } // if(flag)...
            else{
                flag = 1;
                break;
            }

        } // for int i...
    } // while(gravityExists)...

}// gravityEffect() ends here.

// magicIndex(player char) ----------------------------------------------------------------------------------------------------
// function takes player char and check its similar neighbour's ocurance with respect of its position through the board
// it returns integer with row and column encoded
int Board::magicIndex(char c){
    int temp[BoardRows][BoardColumns]; // just a temporary 2d array
    for (int i = 0; i < BoardRows; i++){ // initializing temp 2d array
        for (int j = 0; j < BoardColumns; j++) {
            temp[i][j] = 0;
        }
    }

    for (int i = 0; i < BoardRows; ++i) { // iterating through boardArr to add weights to character occurrences.
        for (int j = 0; j < BoardColumns; ++j) {
            if(boardArr[i][j] == c){
                int count = 0;
                if(boardArr[i+1][j] == c){ count++; }
                if(boardArr[i-1][j] == c){ count++; }
                if(boardArr[i][j+1] == c){ count++; }
                if(boardArr[i][j-1] == c){ count++; }
                temp[i][j] = count;
            }
        }
    }
    // self explanatory integers
    int highestIndexValue = 0;
    int rowIndex= 0, columnIndex= 0;

    for (int i = 0; i < BoardRows ; ++i) { // finding highest weights in the board
        for (int j = 0; j <  BoardColumns; ++j) {
            if(temp[i][j] > highestIndexValue){
                highestIndexValue = temp[i][j];
                rowIndex = i;
                columnIndex = j;
            }

        }

    }
    return 10*rowIndex+columnIndex; // encoded row and column

}
// vanish (index to delete)------------------------------------------------------------------------------------
void Board::Vanish(int sudoIndex){
    int i = sudoIndex/ 10;
    int j = sudoIndex% 10;
    char element = boardArr[i][j];

    if(boardArr[i][j+1] == element || boardArr[i][j-1] == element || boardArr[i+1][j] == element){
        if(boardArr[i][j+1] == element){ // checking right
            boardArr[i][j+1] = ' ';
        }
        if(boardArr[i][j-1] == element){ // checking left
            boardArr[i][j-1] = ' ';
        }
        if(boardArr[i+1][j] == element){ // checking down
            boardArr[i+1][j] = ' ';
        }
        boardArr[i][j] = ' '; // setting that element
    }
}//vanish ends here.

// CompletionCheck() ------------------------------------------------------------------------------------------
bool Board::completionCheck(){
    for (int i = 0; i < BoardRows; ++i) {
        for (int j = 0; j < BoardColumns; ++j) {
            if(boardArr[i][j] != ' ' && boardArr[i][j] != '.'){
                return false;
            }
        }
    }
    return true;
}//completionCheck ends here.

// printing the board() ------------------------------------------------------------------------------------------
void Board::DisplayBoard(){
    char rowChar = 'A';

    cout << "    Board " << boardNumber << " par " << parValue << endl;
    cout << "      0 1 2 3 4 5 6 7 8 9  " << endl;
    cout << "    -----------------------" << endl;

    for(int i = 0; i < BoardRows; i++) { // just a simple itteratrion
        cout << "  "<< rowChar << " | ";
        for (int j = 0; j < BoardColumns; j++) {
            cout << boardArr[i][j] << " ";
        }
        cout << " | "<< rowChar << endl;
        rowChar ++;
    }
    cout << "    -----------------------" << endl;
    cout << "      0 1 2 3 4 5 6 7 8 9  " << endl;
    this_thread::sleep_for(chrono::milliseconds(SleepAmount)); // delay while displaying

} //Board::DisplayBoard() ends here.

// To convert integer value to the desired char--------------------------------------------------------------------
// This function is private function of class Board.
char Board::ChangeNumToChar(int val){
    switch(val){
        case 0: return ' ';
        case 1: return '&';
        case 2: return '@';
        case 3: return '+';
        case 4: return '%';
        case 5: return '^';
        case 6: return '#';
        case 7: return '=';
        case 8: return '*';
        case 9: return '.';
    }
}// Board::changeNumToChar() ends here.

//------------------------------------------------------------------------------------------------------------------
// Class used to read in and store all game boards
class AllBoards
{
public:
    //-----------------------------------------------------------------------------------------------------------
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


int makeMove(Board &theBoard, int rowInput, int columnInput, char direction){
    //checking move validity
    if(direction == 'R' || direction == 'L'){
        return theBoard.checkArray(rowInput, columnInput, direction); // will return -1 if error was thrown.
    }
    else{
        cout << "Invalid move direction.  Please retry." << endl;
        return -1; // since wrong input.
    }


}


int main()
{
    AllBoards allTheBoards;     // Reads in and stores all the boards from a data file
    int currentBoardIndex = 0;  // Starting board index
    Board theBoard;             // The board instance, that is set to hold the values for each level
    int score = 0;              // Score accumulates par points for each level finished
    int stepCount =1;
    displayDirections();

    char input1;
    char input3;

    int startInputIndex = 0;

    theBoard = allTheBoards.getBoard( currentBoardIndex); //returns a board instance with current board loaded
    theBoard.DisplayBoard();

    while(true){

        cout << stepCount << ". Your move: ";

        cin >> input1;
        input1 = toupper(input1);

        if( input1 == 'X'){ // exiting the game.

            break;
        }
        else if(input1 == 'R'){ // resitting the level
            stepCount = 1;
            theBoard = allTheBoards.getBoard( currentBoardIndex); //returns a board instance with current board loaded
            theBoard.DisplayBoard();
        }
        else if(input1 == 'S'){
            cin >> currentBoardIndex ;
            //currentBoardIndex = columnInput;
            theBoard = allTheBoards.getBoard( currentBoardIndex); //returns a board instance with current board loaded
            theBoard.DisplayBoard();
        }
        else{
            int columnInput;
            cin >> columnInput;

            if(input1 >= 'A' && input1 <= 'H' ){ // row input is in range.


                int rowInput = input1 - 'A';

                if(columnInput >= 0 && columnInput <=9 ){ // column input is in range
                    cin >> input3;
                    input3 = toupper(input3);
                    int response = makeMove( theBoard, rowInput, columnInput , input3);
                    if(response != -1){ // if the move was valid and changes were made to theBoard.
                        stepCount++;
                    }

                }
                else{
                    cout <<  "Move value is out of bounds.  Please retry."<< endl;
                }
            }
            else{
                cout <<  "Move value is out of bounds.  Please retry."<< endl;
            }

        }

        if(theBoard.completionCheck()){

            if(--stepCount != theBoard.getPar()){ // --stepCount: as it incremented everytime there is no error, since this is an error, had to decrement it.
                cout << "Sorry, you took "<< stepCount << " moves and you must finish within " << theBoard.getPar()<<" moves before moving on." << endl;
                stepCount = 1;
//                 theBoard = allTheBoards.getBoard( currentBoardIndex); //returns a board instance with current board loaded
//                 theBoard.DisplayBoard();
            }
            else{
                score = score + theBoard.getPar();
                cout<< "Congratulations!  On to the next level."<< endl;
                cout<< "Score: " << score << endl << endl;
                currentBoardIndex++;
                stepCount = 1;
            }

            theBoard = allTheBoards.getBoard( currentBoardIndex); //loading the next level.
            theBoard.DisplayBoard();
        }
    }

    return 0;
}