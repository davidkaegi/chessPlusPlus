#ifndef KNIGHT_H
#define KNIGHT_H
#include "pieces.h"
#include <cctype>
#include <iostream>
#include <string>
class Knight : public virtual Piece {
public:
  Knight(char let, int num, bool col) : Piece(let, num, col) {}

  virtual char getType() { return 'N'; }

  bool isEmpty() { return false; }

  string allMoves() {
    string output;
    int iLet = (int)letter;//letter as an integer
    int change[8][2] = {{1, 2},  {2, 1},  {-1, 2},  {-2, 1},
                        {1, -2}, {2, -1}, {-1, -2}, {-2, -1}};
    //all pairs of changes to the knight's position, (letter, number)
    for (int i = 0; i < 8; i++) {
      if (onBoard(iLet + change[i][0], number + change[i][1]) == true) {
        //makes notation like Nf3,
        output += 'N';
        output += (char)(iLet + change[i][0]);
        output += to_string(number + change[i][1]);
        output += ',';
      }
    }
    return output;
  }
};
#endif