#ifndef ROOK_H
#define ROOK_H
#include <cctype>
#include <iostream>
#include <string>
#include "pieces.h"
class Rook : public virtual Piece {
public:
  Rook(char let, int num, bool col) : Piece(let, num, col) {}

  virtual char getType() { return 'R'; }

  virtual bool isEmpty() { return false; }

  virtual string allMoves() {
    string output;
    for (int col = (int)letter - 1; col >= 97; col--) {//cycles the alphabet down to a
      //leftward moves
      output += 'R';
      output += (char)col;//letter a-h
      output += to_string(number);//number unchanged
      if (col == 97)
        output += '<';
      else
        output += '>';
    }
    for (int col = (int)letter + 1; col <= 104; col++) {//cycles the alphabet up to h
      //rightward moves
      output += 'R';
      output += (char)col;
      output += to_string(number);
      if (col == 104)
        output += '<';
      else
        output += '>';
    }
    for (int row = number - 1; row >= 1; row--) {//cycles numbers down to 1
      //downward
      output += 'R';
      output += letter;//letter unchanged
      output += to_string(row);
      if (row == 1)
        output += '<';
      else
        output += '>';
    }
    for (int row = number + 1; row <= 8; row++) {//cycles number up to 8
      //upward
      output += 'R';
      output += letter;
      output += to_string(row);
      if (row == 8)
        output += '<';
      else
        output += '>';
    }
    return output;
  }
};
#endif