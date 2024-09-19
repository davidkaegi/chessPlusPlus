#ifndef PAWN_H
#define PAWN_H
#include <cctype>
#include <iostream>
#include <string>
#include "pieces.h"
class Pawn : public virtual Piece {
public:
  Pawn(char let, int num, bool col) : Piece(let, num, col) {}
  
virtual char getType() { return 'P'; }

  bool isEmpty() { return false; }

  string allMoves() {
    return this->diagMoves()+this->orthMoves();
    // if on 1 or 8, promotion is handled in the board class
  }

  string diagMoves() {
    string output;
    if (letter != 'a') {
      string take;
      take += 'P';
      take += (char)((int)letter - 1);
      if (colour == 0)//white
        take += to_string(number + 1);//row number goes up
      else//black
        take += to_string(number - 1);//row number goes down
      output += take + '*' + take + '!';//'*' is normal capture, '!' is en passant
      //both moves are checked for legality separately
    }
    if (letter != 'h') {
      string take;
      take+='P';
      take += (char)((int)letter + 1);
      if (colour == 0)
        take += to_string(number + 1);
      else
        take += to_string(number - 1);
      output += take + '*' + take + '!';
    }
    return output;
  }

  string orthMoves() {
    string output;
    output+= 'P';
    if (colour == false) {//white
      output += letter;
      output+= to_string(number + 1);//row increases by 1
    }
    else {//black
      output += letter;
      output+= to_string(number - 1);//row decreases by 1
    }
    if (hasMoved == false) {
      //two space pawn move
      output+='~';//this tells the legalize function to cancel the second move if the first is illegal
      output+='P';
      if (colour == false)
        output += letter + to_string(number + 2);
      else
        output += letter + to_string(number - 2);
    }
    output += '|';//after this point, stop cancelling moves in the legalize function
    return output;
  }
};
#endif