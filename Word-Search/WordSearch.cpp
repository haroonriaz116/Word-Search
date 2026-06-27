#include <cctype>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

// Declare variables
string response;
string word;
int gridSize;
vector<vector<char>> grid;
vector<string> WordList;
set<string> guessedWords;
set<string> foundWords;

// Function to display instructions
void printInstructions() {
  cout << "This program will generate a grid of random letters based on the "
          "size you provide."
       << endl;
  cout << "Enter a word you think is present in the grid." << endl;
  cout << "If the word is not found, you will be notified." << endl;
  cout << "If the word has already been guessed before, you will be notified."
       << endl;
  cout << "You can type \033[1;33m'RESTART'\033[0m to start a new game with a "
          "different grid size."
       << endl;
  cout
      << "You can type \033[1;33m'QUIT'\033[0m to quit the game and play again."
      << endl;
  cout << "You can type \033[31m'HELP'\033[0m for further "
          "instructions."
       << endl;
}

// Function to generate a random letter
char generateRandomLetter(const vector<string> &WordList, int position) {
  if ((position + 1) % 3 == 0) {
    // Generate a random vowel
    const string vowels = "AEIOU";
    return vowels[rand() % vowels.length()];
  } else {
    // Generate a random consonant
    int random = rand() % (26 + WordList.size());
    if (random < 26) {
      return 'A' + random;
    } else {
      const string &word = WordList[random - 26];
      return toupper(word[rand() % word.length()]);
    }
  }
}

// Function to generate a random letters grid of given size
void generateRandomGrid(vector<vector<char>> &grid, int size,
                        const vector<string> &WordList) {
  for (int i = 0; i < size; i++) {
    vector<char> row;
    for (int j = 0; j < size; j++) {
      row.push_back(generateRandomLetter(WordList, i * size + j));
    }
    grid.push_back(row);
  }
}

// Function to display the grid
void displayGrid(const vector<vector<char>> &grid) {
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].size(); j++) {
      cout << grid[i][j] << ' ';
    }
    cout << endl;
  }
}

// Function to load the word list from a file
void loadWordList(vector<string> &WordList, const string &filename) {
  ifstream file(filename);
  if (file.is_open()) {
    while (getline(file, word)) {
      WordList.push_back(word);
    }
    file.close();
  } else {
    cout << "\033[31mError! Failed to Open File: \033[0m" << filename << endl;
  }
}

// Function to check if a word is present in the word list
bool isWordInList(const vector<string> &WordList, string &word) {
  // Convert the word to lowercase
  for (int j = 0; j < word.length(); j++) {
    word[j] = tolower(word[j]);
  }

  for (const string &isInList : WordList) {
    if (isInList == word) {
      return true;
    }
  }
  return false;
}

// Function to check if a word is present in the grid
bool isWordInGrid(vector<vector<char>> &grid, const string &word, int row,
                  int col, int index) {
  int gridSize = grid.size();
  if (index == word.length())
    return true;

  if (row >= 0 && row < gridSize && col >= 0 && col < gridSize &&
      grid[row][col] == word[index]) {
    char temp = grid[row][col];
    grid[row][col] = '#';

    bool isInGrid = isWordInGrid(grid, word, row - 1, col, index + 1) ||
                    isWordInGrid(grid, word, row + 1, col, index + 1) ||
                    isWordInGrid(grid, word, row, col - 1, index + 1) ||
                    isWordInGrid(grid, word, row, col + 1, index + 1) ||
                    isWordInGrid(grid, word, row - 1, col - 1, index + 1) ||
                    isWordInGrid(grid, word, row - 1, col + 1, index + 1) ||
                    isWordInGrid(grid, word, row + 1, col - 1, index + 1) ||
                    isWordInGrid(grid, word, row + 1, col + 1, index + 1);

    // Revert the special character back to its original character
    grid[row][col] = temp;

    return isInGrid;
  }
  return false;
}

// Function to reset the game and prompt for grid size
void resetGame(int &gridSize, vector<vector<char>> &grid,
               set<string> &guessedWords, set<string> &foundWords) {
  while (true) {
    cout << "Enter the size of the grid: ";
    cin >> response;

    if (response == "QUIT" || response == "Quit" || response == "quit") {
      cout << "\033[1;33mGoodbye!\033[0m" << endl;
      exit(0);
    } else if (response == "RESTART" || response == "Restart" ||
               response == "restart") {
      continue;
    } else if (response == "HELP" || response == "Help" || response == "help") {
      printInstructions();
      continue;
    } else {
      // Try to convert response to an integer
      try {
        gridSize = stoi(response);

        if (gridSize < 5) {
          cout << "\033[31mInvalid Input: Grid Size Must be Greater than "
                  "4!\033[0m"
               << endl;
        } else {
          break;
        }
      } catch (invalid_argument &) {
        cout << "\033[31mInvalid Input: Please enter a valid number!\033[0m"
             << endl;
      }
    }
  }

  grid.clear();
  generateRandomGrid(grid, gridSize, WordList);
  displayGrid(grid);
  guessedWords.clear();
  foundWords.clear();
}

int main() {
  // Seed the random number generator
  srand(time(0));
  cout << "\033[1;36mWelcome to the Word Search Game!\033[0m" << endl;
  printInstructions();

  // Load the word list
  loadWordList(WordList, "WordList.txt");
  resetGame(gridSize, grid, guessedWords, foundWords);

  while (true) {
    cout << "Enter a word to guess: ";
    cin >> word;

    if (word == "HELP" || word == "Help" || word == "help") {
      printInstructions();
      continue;
    }

    // Check if the word has already been guessed
    if (guessedWords.find(word) != guessedWords.end()) {
      cout << "\033[31mIncorrect Input: Word Already Guessed!\033[0m" << endl;
      continue;
    }

    bool isInList = isWordInList(WordList, word);

    // Convert the word to uppercase
    for (int j = 0; j < word.length(); j++) {
      word[j] = toupper(word[j]);
    }

    // Search for the uppercase word in the grid
    bool isInGrid = false;
    for (int row = 0; row < gridSize; row++) {
      for (int col = 0; col < gridSize; col++) {
        if (isWordInGrid(grid, word, row, col, 0)) {
          isInGrid = true;
          break;
        }
      }
      if (isInGrid) {
        break;
      }
    }

    // Update the guessed words set
    guessedWords.insert(word);

    // Display the result
    if (isInGrid && isInList) {
      foundWords.insert(word);
      cout << "\033[32mWord Found!\033[0m" << endl;
      // Display the list of found words
      cout << "\033[32mFound Words:\033[0m ";
      for (const string &foundWord : foundWords) {
        cout << foundWord << ", ";
      }
      cout << endl;
    } else if (word == "QUIT" || word == "Quit" || word == "quit" ||
               word == "RESTART" || word == "Restart" || word == "restart") {
      // Skip displaying the "Incorrect Input: Word not Found!" message for
      // "restart" and "quit"
    } else if (isInGrid && !isInList) {
      cout << "\033[31mIncorrect Input: Word not Found in List!\033[0m" << endl;
    } else {
      cout << "\033[31mIncorrect Input: Word not Found in Grid!\033[0m" << endl;
    }

    if (word == "QUIT" || word == "Quit" || word == "quit") {
      cout << "Would you like to play again? "
              "(\033[32mY\033[0m/\033[31mN\033[0m) ";
      cin >> response;

      // Check for invalid input
      while (response != "Y" && response != "y" && response != "N" &&
             response != "n") {
        cout << "\033[31mInvalid Input!\033[0m" << endl;
        cout << "Do you want to play again? "
                "(\033[32mY\033[0m/\033[31mN\033[0m) ";
        cin >> response;
      }

      // Check for response
      if (response == "Y" || response == "y") {
        resetGame(gridSize, grid, guessedWords, foundWords);
        continue;
      } else if (response == "N" || response == "n") {
        cout << "\033[1;33mGoodbye!\033[0m";
        break;
      }
    } else if (word == "RESTART" || word == "Restart" || word == "restart") {
      resetGame(gridSize, grid, guessedWords, foundWords);
      continue;
    }
  }
  return 0;
}