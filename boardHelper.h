//a handful of functions used in the Board class
#ifndef BOARDHELPER_H
#define BOARDHELPER_H
#include <iostream>
#include <string>
using namespace std;
int letInt(char letter) { // convert letter to board array index
  return (int)letter - 97;
}
int numInt(char number) { // character number to board array index
  return (int)number - 49;
} // char number digit to int number -1

bool flip(bool in) {
  if (in == true)
    return false;
  return true;
}

bool isNumber(char in) {
  if ((unsigned int)in - 49 <= 7)
    return true; // all chars lower than 49 will wrap to 4 billion
  return false;
}

bool isLetter(char in) {
  if ((unsigned int)in - 97 <= 7)
    return true;
  return false; // too low = wrap
}

string toAN(int letter, int number) { // to alphanumeric
  string output;
  output = (char)(letter + 97);
  output += (char)(number + 49);
  return output;
}
bool isMajor(char in) {
  if (in == 'R')
    return true;
  if (in == 'B')
    return true;
  if (in == 'N')
    return true;
  if (in == 'Q')
    return true;
  if (in == 'K')
    return true;
  return false;
}

string outputSquare(char symbol) {//for outputting the board
  string out;
  //weird \033 stuff is for colouring purposes
  if (symbol == '#') {//white square
    out+= "\033[47m\033[30m";
    out += ' ';
  }
  else if (symbol == ' ') {//black square
    out += "\033[40m\033[37m";
    out += ' ';//black square
  }
  else if (isMajor(toupper(symbol))==true||toupper(symbol)=='P')//pieces don't change colouring
    out += symbol;
  else {
    out += "\033[0m";
    out += symbol;//resets colouring once on the border of the board
  }
  return out;
}
#endif