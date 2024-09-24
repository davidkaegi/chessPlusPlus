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
  int depth=5;
  float cutoff=1.5;
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
  Engine board(type);
  if (fairy=='Y') {
    output20x('-');
    board.setMaxDepth(2);//fairy chess has more possible moves to consider, so the engine runs slower
  }
  board.setMaxDepth(5);// default is 5
  char colour='W';
  if (bot=='Y') {
    cout<<"Which colour would you like to play as (W/B): ";
    cin>>colour;
    output20x('-');
  }
  board.savePosition();
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
    if (move[0]=='/') {//commands
      if (move=="/resign") {
        if (board.getTurn()==false) cout<<"Black ";
        else cout<<"White ";
        cout<<"wins by resignation\n";
        return 0;//quits the game
      }
      else if (move=="/undo") {
        int n;
        cin>>n;
        board.undo(n);
      }
      else if (move=="/on") bot='Y';//turns on the engine
      else if (move=="/off") bot='N';//turns off the engine
      else if (move=="/onemove") {
        x=0;
        board.engineMove();
        if (x<10000) {
          depth+=1;
          board.setMaxDepth(depth);
          cutoff+=0.2;
          board.setCutoff(cutoff);
        }
        else if (x>200000) {
          depth-=1;
          board.setMaxDepth(depth);
          cutoff-=0.1;
          board.setCutoff(cutoff);
        }
      }
      else if (move=="/setdepth") {
        cin>>depth;
        board.setMaxDepth(depth);
      }
      else if (move=="/setcutoff") {
        float n;
        cin>>n;
        board.setCutoff(n);
      }
      else if (move=="/printall") {
        for (int i=0;i<8;i++) {
          for (int j=0;j<8;j++) {
            cout<<i<<' '<<j<<' '
            <<board.getSquare(i,j)->getPosition()<<endl;
          }
        }
      }
      else cout<<"Please refer to instructions for a list of commands\n";
    }
    else if (board.move(move)==true) {
      if (board.endGame()==false) {
        if (bot=='Y') {
          x=0;
          board.engineMove();
          if (x<15000) {
            depth+=1;
            board.setMaxDepth(depth);
            cutoff+=0.2;
            board.setCutoff(cutoff);
          }
          else if (x>205000) {
            depth-=1;
            board.setMaxDepth(depth);
            cutoff-=0.1;
            board.setCutoff(cutoff);
          }
        }
      }
    }
  }
  if (board.insuffMatl()==true) cout<<"Draw by insufficient material\n";
  else if (board.getFiftyMove()==0) cout<<"Draw by fifty move rule\n";
  else if (board.threefold()==true) cout<<"Draw by threefold repetition\n";
  else if (board.isCheck()==false) cout<<"Draw by stalemate\n";
  else {
    if (board.getTurn()==false) cout<<"Black";
    else cout<<"White";
    cout<<" wins by checkmate\n";
  }
  board.changeTurn();
  cout<<board;
  return 0;  
}