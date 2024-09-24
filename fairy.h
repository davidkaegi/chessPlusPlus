#ifndef FAIRY_H
#define FAIRY_H
#include "pieces.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"
#include "knight.h"
#include "king.h"
#include "pawn.h"
#include <iostream>
#include <string>

class Fairy:public Piece
{
public:

Fairy(char let=' ',int num=0,bool col=0,char tp1=' ',char tp2=' ') : Piece (let,num,col) {
  type=tp1;
  if (tp1 == 'R')//depending on type, various piece objects are created
    sub1=new Rook(letter,number,colour);
  else if (tp1 == 'B')
    sub1=new Bishop(letter,number,colour);
  else if (tp1 == 'N')
    sub1=new Knight(letter,number,colour);
  else if (tp1 == 'Q')
    sub1=new Queen(letter,number,colour);
  else if (tp1=='K') {
    sub1=new King(letter,number,colour);
    sub1->setHasMoved(true);//fairy pieces can't castle
  }
  else
    sub1=new Piece();//if type is junk, an empty piece woth no moves is created
  if (tp2 == 'R')
    sub2=new Rook(letter,number,colour);
  else if (tp2 == 'B')
    sub2=new Bishop(letter,number,colour);
  else if (tp2 == 'N')
    sub2=new Knight(letter,number,colour);
  else if (tp2 == 'Q')
    sub2=new Queen(letter,number,colour);
  else if (tp2=='K') {
    sub2=new King(letter,number,colour);
    sub2->setHasMoved(true);//fairy pieces can't castle
  }
  else
    sub2=new Piece();
}

char getType() {return type;}
bool isEmpty() { return false; }

void setHasMoved(bool in) {
  sub1->setHasMoved(in);
  sub2->setHasMoved(in);
}

string allMoves() {
  //fairy pieces combine moves by adding allMoves() of two piece objects
  sub1->move(letter,number);//update the sub pieces' positions before moving
  sub2->move(letter,number);
  string out=sub1->allMoves()+sub2->allMoves();
  return out;
}

protected:
char type;
Piece *sub1,*sub2;
};

#endif