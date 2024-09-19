#ifndef QUEEN_H
#define QUEEN_H
#include <cctype>
#include <iostream>
#include <string>
#include "pieces.h"
#include "bishop.h"
#include "rook.h"
class Queen : public virtual Rook, virtual Bishop {//double inheritance!!!
public:
  Queen(char let, int num, bool col)
      : Piece(let, num, col), Rook(let, num, col), Bishop(let, num, col) {}

  virtual char getType() { return 'Q'; }

  string allMoves() { return Rook::allMoves() + Bishop::allMoves(); }

  bool isEmpty() { return false; }
};

#endif