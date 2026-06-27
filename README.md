# Word Search

A console-based word search game that generates a randomized letter grid and challenges players to find as many hidden dictionary words within it as possible.

## How to play

* The player first chooses a grid size of five or larger, which determines the dimensions of the randomly generated letter grid.
* Letters are seeded both from the alphabet and from words pulled out of a 10,000-word dictionary file, with every third letter biased toward a vowel, so that real words are more likely to appear embedded in the grid.
* The player enters guesses one at a time, and each guess is checked both against the dictionary and against the grid itself, searching in all eight directions recursively from every starting cell.
* A guess only counts as a correct find when it is both a real dictionary word and physically present in the grid, and previously guessed words are tracked so they cannot be guessed again.
* Typing RESTART generates a new grid at a freshly chosen size, typing QUIT ends the current round and asks whether to play again, and typing HELP reprints the instructions at any time.
* A running list of all successfully found words is displayed after each correct guess.

## Tech stack

The game is written in standard C++ and depends only on the C++ standard library, including fstream, sstream, set, and vector, with no external libraries, build tools, or frameworks required.

## Project structure

```
word-search/
├── WordSearch.cpp
└── WordList.txt
```

## Running it locally

Clone the repository and compile the single source file with any standard C++ compiler, making sure WordList.txt remains in the same directory as the executable:

```
git clone https://github.com/your-username/word-search.git
cd word-search
g++ -o wordsearch WordSearch.cpp
./wordsearch
```

## Design notes

Terminal output relies on ANSI escape codes to color-code prompts, warnings, and success messages, keeping feedback like invalid input or a found word visually distinct from regular game text. Word validation against the grid uses backtracking recursion, temporarily marking visited cells so that a letter cannot be reused within the same word search.

## Contact

Haroon Riaz — haroonriaz116@gmail.com
