#ifndef KING_H
#define KING_H
#include "pieces.h"
#include <cctype>
#include <iostream>
#include <string>
class King : public virtual Piece {
public:
  King(char let, int num, bool col) : Piece(let, num, col) {}

  virtual char getType() { return 'K'; }

  bool isEmpty() { return false; }

  string allMoves() {
    string output;
    int iLet = (int)letter;                          // int letter
    for (int colOff = -1; colOff <= 1; colOff++) {   // column offset
      for (int rowOff = -1; rowOff <= 1; rowOff++) { // row offset
        if (onBoard(iLet + colOff, number + rowOff) == true) {
          if (colOff != 0 || rowOff != 0) { // the king is already there
            // makes Notation like Ke2,
            output += 'K';
            output += (char)(iLet + colOff); // letter a-h
            output += to_string(number + rowOff);
            output += ',';
          }
        }
      }
    }
    if (hasMoved == false)
      output += "KO-O"; // internal castle notation
    return output;
  }
};
#endif