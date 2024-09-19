#include <iostream>
#include "board.h"
#include "engine.h"
using namespace std;

template <class T>

void output20x(T a) {
  for (int i=0;i<20;i++) {
    cout<<a;
  }
  cout<<endl;
}

int main() {
  char fairy,bot;
  bool type=0;//is it fairy?
  cout<<"Welcome to Chess\n";
  output20x('-');
  cout<<"Would you like to play Fairy Chess? (Y/N): ";
  cin>>fairy;
  output20x('-');
  cout<<"Would you like to play against the engine? (Y/N): ";
  cin>>bot;
  output20x('-');
  if (fairy=='Y') type=1;//any character other than'Y' will disable fairy chess, this is intentional
  //therefore 2 ifs with the same condition are needed
  if (bot=='Y') {//any character other than'Y' will disable the engine, this is intentional
    Engine board(type);
    if (fairy=='Y') {
      output20x('-');
      board.setMaxDepth(2);//fairy chess has more possible moves to consider, so the engine runs slower
    }
    //board.setMaxDepth(1); default is 3
    char colour;
    cout<<"Which colour would you like to play as (W/B): ";
    cin>>colour;
    output20x('-');
    if (colour=='B') {//you will always play as white unless specifically B is entered,this is intentional
      board.engineMove();
    }
    string move;
    while (board.endGame()==false) {
      if (board.getTurn()==false) cout<<"White";
      else cout<<"Black";
      cout<<" to move\n";
      cout<<board;
      cout<<"Enter move: ";
      cin>>move;
      if (move=="exit") return 0;
      if (board.move(move)==true) {
        if (board.endGame()==false) {
          board.engineMove();
        }
      }
    }
    if (board.insuffMatl()==true) cout<<"Draw by insufficient material\n";
    else if (board.isCheck()==false) cout<<"Draw by stalemate\n";
    else {
      if (board.getTurn()==false) cout<<"Black";
      else cout<<"White";
      cout<<" wins by checkmate\n";
    }
    board.changeTurn();
    cout<<board;
  }
  else {
    Board board(type);
    if (fairy=='Y') output20x('-');
    string move;
    while (board.endGame()==false) {
      if (board.getTurn()==false) cout<<"White";
      else cout<<"Black";
      cout<<" to move\n";
      cout<<board;
      cout<<"Enter move: ";
      cin>>move;
      if (move=="exit") return 0;
      board.move(move);
    }
    if (board.insuffMatl()==true) cout<<"Draw by insufficient material\n";
    else if (board.isCheck()==false) cout<<"Draw by stalemate\n";
    else {
      if (board.getTurn()==false) cout<<"Black";
      else cout<<"White";
      cout<<" wins by checkmate\n";
    }
    board.changeTurn();
    cout<<board;
  }
  return 0;  
}