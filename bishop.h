#ifndef BISHOP_H
#define BISHOP_H
#include "pieces.h"
#include <cctype>
#include <iostream>
#include <string>
class Bishop : public virtual Piece {
public:
  Bishop(char let, int num, bool col) : Piece(let, num, col) {}

  virtual char getType() { return 'B'; }

  virtual bool isEmpty() { return false; }

  virtual string allMoves() {
    string output;
    // checks all four diagonals seperately
    for (int col = (int)letter - 1, row = number - 1; col >= 97 && row >= 1;
         col--, row--) {
      // down left
      // makes notation in the form Be3>Bf2>Bg<
      output += 'B';
      output += (char)col; //(char)col will be a lowercase letter a through h
                           //representing the file
      output +=
          to_string(row); // row is the rank the is on as a number 1 through 8
      if (col == 97 || row == 1) // the last square the will be on the board
        output += '<'; // it is the end of the diagonal, the moves after it are
                       // not cancelled
      else
        output += '>'; // if the square is not empty, further moves in the
                       // diagonal will be cancelled
    }
    for (int col = (int)letter - 1, row = number + 1; col >= 97 && row <= 8;
         col--, row++) {
      // up left
      output += 'B';
      output += (char)col;
      output += to_string(row);
      if (col == 97 || row == 8)
        output += '<';
      else
        output += '>';
    }
    for (int col = (int)letter + 1, row = number - 1; col <= 104 && row >= 1;
         col++, row--) {
      // down right
      output += 'B';
      output += (char)col;
      output += to_string(row);
      if (col == 104 || row == 1)
        output += '<';
      else
        output += '>';
    }
    for (int col = (int)letter + 1, row = number + 1; col <= 104 && row <= 8;
         col++, row++) {
      // up right
      output += 'B';
      output += (char)col;
      output += to_string(row);
      if (col == 104 || row == 8)
        output += '<';
      else
        output += '>';
    }
    return output;
  }
};
#endif
