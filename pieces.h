#ifndef PIECES_H
#define PIECES_H
#include <cctype>
#include <iostream>
#include <string>

using namespace std;
class Piece {
public:

bool onBoard(int let,int num) {//is the position on the board
  if (let>=97&&let<=104) {
    if (num>=1&&num<=8) {
      return true;
    }
  }
  return false;
}

  Piece(char let=' ', int num=0, bool col=false) {
    letter = let;
    number = num;
    colour = col;
    hasMoved = false;
  }

  virtual ~Piece() {}//nothing to delete here :(
  //no destructors in the subclasses, they also have nothing to delete

  void setPos(int let, int num) {//set the internal position of the piece without flipping hasMoved
    letter=(char)(let+97);
    number=num+1;
  }

  void move(char let, int num) {//move the internal position of the piece and flip hasMoved
    letter = let;
    number = num;
    hasMoved = true;
  }
//function overloading two moves
  void move(string in) {
    letter=in[0];
    number=(int)(in[1])-48;
    hasMoved=true;
  }

  bool getHasMoved() { return hasMoved; }

  void setHasMoved(bool in) {hasMoved=in;}

  virtual bool isEmpty() { return true; }

  virtual char getType() {
    return ' ';
  }

  bool getColour() {return colour;}

  virtual string allMoves() {
    return "";
  }

  string getPosition() {//return the internal position of the piece in form like "a3"
    string out;
    out+=letter;
    out+=to_string(number);
    return out;
  }

protected:
  char letter;//coloumn / file of the file
  int number;//row / rank of the piece
  bool colour; // 0 - white, 1 - black
  bool hasMoved;
};

#endif