/* Ultimate MadLibs

   Author: Adonis Pugh
 ----------------------------
   This program implements a digital version of the classic game "Mad Libs."
   The game is run by prompting users to enter a Mad Lib file, locating
   placeholders, collecting the proper user input values for each file, and
   replacing the placeholders with the appropriate user choice. The user can
   play the game as many times as they want, so long as all the Mad Lib files
   are valid. At the end, the number of stories created, the total number of
   placeholders replaced, and the average number of placeholders replaced is
   printed to the screen. */

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;

/***********************************
 *      PROTOYPE FUNCTIONS         *
 **********************************/

void intro();
void promptFile(string& filename);
void collectChoices(string& filename, string& allChoices, int& numStories);
void displayStory(string& filename, string& allChoices, int& numReplaced, bool& playAgain);
bool isVowel(char& letter);


/***********************************
 *          FUNCTIONS              *
 **********************************/

int main() {
    string allChoices;
    string filename;
    int numReplaced = 0;
    int numStories = 0;
    bool playAgain = true;
    intro();
    while(playAgain) {      //repeated until player ends game
        promptFile(filename);
        collectChoices(filename, allChoices, numStories);
        displayStory(filename, allChoices, numReplaced, playAgain);
    }
    double avgReplaced = static_cast<double>(numReplaced)/numStories;
    cout << "\n" << "Mad Lib stories you created: " << numStories << endl;
    cout << "Total placeholders replaced: " << numReplaced << " (" << fixed << setprecision(1)
         << avgReplaced << " per story)" << endl;
    return 0;
}

/* The opening titles are printed on the screen. */
void intro() {
    cout << "Welcome to CS 106B Mad Libs!" << endl;
    cout << "I will ask you to provide various words" << endl;
    cout << "and phrases to fill in a story." << endl;
    cout << "At the end, I will display your story to you." << endl;
    cout << endl;
}

/* This boolean checks if a letter is a vowel. */
bool isVowel(char& letter) {
    return static_cast<char>(tolower(letter)) == 'a' || letter == 'e' || letter == 'i' || letter == 'o'|| letter == 'u';
}

/* The user is prompted to enter a file name. */
void promptFile(string& filename) {
    filename = promptUserForFile("Mad Lib input file? ",
                                 "Unable to open that file.  Try again.");
    cout << endl;
}

/* The input file is read line by line, the placeholders are located, and the user is
 * prompted to enter a value for each placeholder. All the user values are recorded. */
void collectChoices(string& filename, string& allChoices, int& numStories) {
    ifstream input;
    input.open(filename);
    string line = "";
    allChoices = "";
    while(getline(input, line)) {
        int open = line.find('<');
        int close = line.find('>');
        while(open != string::npos && close != string::npos) {
            string placeHolder = "";
            for(int i = open + 1; i < close; i++) {
                placeHolder += line[i];
            }
            string modifier = "a";
            if(isVowel(placeHolder[0])) {
                modifier = "an";
            }
            string choice = getLine("Please type " + modifier + " " + placeHolder + ":");
            allChoices += choice + "\n";
            open = line.find('<', close + 1);
            close = line.find('>', close + 1);
        }
    }
    input.close();
    numStories++;
    cout << endl;
}

/* After all the user input values have been collected, the file is read again. The placeholders are
 * located and replaced with the input values in the correct order the user entered. Each line of the
 * story is printed to the screen once the placeholder has been replaced with the choice of the user. */
void displayStory(string& filename, string& allChoices, int& numReplaced, bool& playAgain) {
    ifstream input;
    input.open(filename);
    string line = "";
    int replaced = 0;
    cout << "Your Mad Lib story:" << endl;
    while(getline(input, line)) {
        int open = line.find('<');
        int close = line.find('>');
        while(open != string::npos && close != string::npos) {
            for(int i = open; i <= close; i++) {
                line.erase(open, 1);
            }
            string choice = allChoices.substr(0, allChoices.find("\n"));
            line.insert(open, choice);
            replaced++;
            numReplaced++;
            allChoices.erase(0, allChoices.find("\n") + 1);
            open = line.find('<', open);
            close = line.find('>', open);
        }
        cout << line << endl;
    }
    input.close();
    cout << "\n" << replaced << " placeholder(s) replaced." << endl;
    playAgain = getYesOrNo("Do another Mad Lib (Y/N)? ");
}
