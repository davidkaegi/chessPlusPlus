#ifndef ENGINE_H
#define ENGINE_H
#include "board.h"
#include <unordered_map>
using namespace std;

int x=0;

class Engine : public Board {
public:
  Engine(bool type = 0) : Board(type) {
    maxDepth = 5;
    depth = 0;
    //maybe change these values for fairy
    m['P']=0.9;
    m['N']=3;
    m['B']=3.01;
    m['R']=5;
    m['Q']=9;
    m[' ']=0;
  }

  void unCastle(char direction) {    // for the engine
    delete square[(int)turn * 7][4]; // delete the empty space on the king
                                     // square
    if (direction == '<') {
      delete square[(int)turn * 7][0]; // delete empty on the rook square
      // shuffles around piece pointers to reflect board state
      square[(int)turn * 7][4] = square[(int)turn * 7][2];
      // if white's turn, then 0*7=0, if black's, then 1*7=7
      square[(int)turn * 7][0] = square[(int)turn * 7][3];
      square[(int)turn * 7][3] = new Piece();
      square[(int)turn * 7][2] = new Piece();
      square[(int)turn * 7][4]->move(toAN(
          4, (int)turn * 7)); // tells piece objects their new position (king)
      square[(int)turn * 7][0]->move(toAN(0, (int)turn * 7)); //^(rook)
      square[(int)turn * 7][4]->setHasMoved(false);
      square[(int)turn * 7][0]->setHasMoved(false);
    } else if (direction == '>') {
      delete square[(int)turn * 7]
                   [7]; // delete the empty space on the rook square
      square[(int)turn * 7][4] = square[(int)turn * 7][6];
      square[(int)turn * 7][7] = square[(int)turn * 7][5];
      square[(int)turn * 7][6] = new Piece();
      square[(int)turn * 7][5] = new Piece();
      square[(int)turn * 7][4]->move(toAN(4, (int)turn * 7));
      square[(int)turn * 7][7]->move(toAN(7, (int)turn * 7));
      square[(int)turn * 7][4]->setHasMoved(false);
      square[(int)turn * 7][7]->setHasMoved(false);
    } else
      cout << "CASTLE_ERROR\n"; // should never occur
  }

  void setMaxDepth(int max) { maxDepth = max; }

  float evaluate() {
    float sum = 0;
    int row,col;
    for (row = 0; row < 8; row++) {
      for (col = 0; col < 8; col++) {
        if (square[row][col]->isEmpty() == false) {
          char type = square[row][col]->getType();
          bool colour = square[row][col]->getColour();
          float change = 0; // values are mostly the pieces' material values
          if (type == 'P') { // incentivize pawn pushing by valuing more if
                               // they're closer to promotion
            if (colour == false)
              change = 0.9 + (float)row * 0.1;
            else
              change = 1.6 - (float)row / 10.0;
          } 
          else {
            if (type == 'B') {
              change = 3.01; // bishops are slightly better than knights
              if (square[row][col]->getHasMoved()==true) change+=0.4;
              //this gives incentive to develop pieces
            }
            else if (type == 'N') {
              change = 3;
              if (square[row][col]->getHasMoved()==true) change+=0.4;
            }
            else if (type == 'R')
              change = 5;
            else if (type == 'Q')
              change = 9;
          }
          if (colour == false)
            sum += change;
          else
            sum -= change;
        }
      }
    }
    // this section makes the bot value control over the centre
    for (int row = 3; row <= 4; row++) {
      for (int col = 3; col <= 4; col++) {
        if (square[row][col]->isEmpty() == false) {
          if (square[row][col]->getColour() == false)
            sum += 0.4;
          else
            sum -= 0.4;
        }
      }
    }
    return sum;
  }

  float engineMove(string seq="") { // type double, not void because depth!=0 iterations return the best evaluation
  if (depth==maxDepth) return runningEval;
  if (depth==0) runningEval=evaluate();
  if (depth>=5) {
    if (seq.substr((depth-1)*4,8)==seq.substr((depth-5)*4,8)) {
      //this scenario is probably repeating moves
      return 0;
    }
  }
  pastEval[depth]=runningEval;
  x++;
  if (depth>=2) {
    //if the position is worse than it was after black/white's last turn, the program stops evaluating the line
    if (turn==false) {
      if (pastEval[depth]>pastEval[depth-2]+cutoff) {
        return pastEval[depth];
      }
    }
    else {
      if (pastEval[depth]<pastEval[depth-2]-cutoff) {
        return pastEval[depth];
      }
    }
  }
    int row,col,i,l;
    string move;
    int rowTgt, colTgt;
    Piece *taken,*passant;
    bool oldHasMoved;
    float bestEval,result;
    char oldLastMove;
    char newLastMove='x';
    bool ep=false;//is the chosen move en passant
    int rowStart=0, rowEnd=0,colStart=0, colEnd=0;//where the "best" move starts and ends
    if (turn==false) {
      bestEval=-101;
    }
    else {
      bestEval=101;
    }
    //lots of varibles declared here as to now have to redeclare them over and over
    for (row=0;row<8;row++) {
      for (col=0;col<8;col++) {
        Piece *here = square[row][col];//the piece we are trying to move
        if (here->isEmpty()==false) {
          if (here->getColour()==turn) {
            //now we are only looking at pieces that can move this turn
            move=here->allMoves();
            move=legalize(move,turn);
            //list of all legal moves this turn
            l=move.length();
            for (i=0;i<l;i+=4) {
              rowTgt=(int)(move[i+2])-49;
              colTgt=(int)(move[i+1])-97;
              oldLastMove=lastMove;
              if (move[i+3]=='O') {
                castle(move[i + 2]);
                if (move[i + 2] == '>') {
                  if (turn==false) runningEval += 0.8-depth/16; // I think shortcastle is easier to play than longcastle
                  else runningEval-=0.9-depth/16;
                }
                else {
                  if (turn==false) runningEval += 0.7-depth/16; // the engine has limitations, so short is incentivized more
                  else runningEval-=0.7-depth/16;
                }
                depth++;
                lastMove = 'x';
                if (depth>=maxDepth-1)
                  result=runningEval;
                else
                  result = engineMove(seq+move.substr(i,4));
                depth--;
                turn = flip(turn);
                // castling is usually a good idea, so I gave the engine an
                // incentive to do so
                unCastle(move[i + 2]);
              }
              else {
                //setting the new board state after the moves
                //also ensure that the board can be reset
                oldHasMoved=here->getHasMoved();
                lastMove='x';
                taken=square[rowTgt][colTgt];
                if (taken->getType()=='K') {
                  //the engine does not actually check if the king is in check when moving
                  //those moves will be caught here and provide incentive to not do so
                  if (turn == false) return 100-depth;
                  else return -100+depth;
                }
                square[rowTgt][colTgt]=here;
                if (here->getType()=='B'||here->getType()=='N') {
                  if (here->getHasMoved()==false) {
                    if (turn==false) runningEval+=0.4-depth/64;
                    else runningEval-=0.4-depth/64;
                    //this gives an incentive to develop pieces
                  }
                }
                if (here->getType()=='K') {
                  if (here->getHasMoved()==false) {
                    if (turn==false) runningEval-=0.5;
                    else runningEval+=0.5;
                    //this gives incentive not to needlessly move the king
                    //as to keep the ability to castle
                  }
                }
                here->move(move.substr(i+1,2));
                square[row][col]=new Piece;
                if (move[i]=='P') {
                  //incentive to move pawns
                  if (turn==false) runningEval+=0.1;
                  else runningEval-=0.1;
                  if (move[i+3]=='!') {
                    passant=square[row][colTgt];
                    square[row][colTgt]=new Piece;
                  }
                  else if (move[i+3]=='|'&&oldHasMoved==false) {
                    if (turn==false) runningEval+=0.1;
                    else runningEval-=0.1;
                    lastMove=move[i+1];
                  }
                  else if (rowTgt==7||rowTgt==0) {
                    //promotion; the fairy acts as a normal queen when fairy chess is disabled
                    //the engine will always choose to promote to queen
                    square[rowTgt][colTgt]=new Fairy((char)(colTgt + 97), rowTgt + 1, turn, 'Q', qComb);
                    if (turn==false) runningEval+=7.4;
                    else runningEval-=7.4;
                  }
                }
                //moves on to the next layer
                if (turn==false) runningEval+=m[taken->getType()];
                else runningEval-=m[taken->getType()];
                if (rowTgt==3||rowTgt==4) {
                  if (colTgt==3||colTgt==4) {
                    if (turn==false) runningEval+=0.4-depth/64;
                    else runningEval-=0.4-depth/64;
                    if (taken->isEmpty()==false) {
                      if (turn==false) runningEval+=0.4-depth/64;
                      else runningEval-=0.4-depth/64;
                    }
                    //incentive to control the centre
                  }
                }
                depth++;
                turn=flip(turn);
                //the engine stops searching one move early to
                //avoid one side from thinking it gets a free capture at
                //max depth
                if (depth>=maxDepth-1&&taken->isEmpty()==true)
                  result=runningEval;
                else
                  result=engineMove(seq+move.substr(i,4));
                turn=flip(turn);
                depth--;
                runningEval=pastEval[depth];
                //returns to this layer
                if (move[i]=='P') {
                  if (move[i+3]=='!') {
                    square[row][colTgt]=passant;
                  }
                  else if (rowTgt==7||rowTgt==0) {
                    delete square[rowTgt][colTgt];//no need to store a queen that no longer exists
                  }
                }
                here->move(row,col);
                square[row][col]=here;
                square[rowTgt][colTgt]=taken;
                here->setHasMoved(oldHasMoved);
              }
              if (turn==false) {//white wants to maximize
                if (result>bestEval) {
                  if (depth>=2) {
                    if (result>pastEval[depth-1]+cutoff) {
                      return result;
                    }
                    //if the move the other colour is refutable, why bother check other moves
                  }
                  //steps to remember which move
                  bestEval=result;
                  rowStart=row;
                  colStart=col;
                  rowEnd=rowTgt;
                  colEnd=colTgt;
                  newLastMove=lastMove;
                  if (move[i+3]=='!') {
                    ep=true;
                  }
                  else ep=false;
                }
              }
              else {//black wants to minimize
                if (result<bestEval) {
                  if (depth>=2) {
                    if (result<pastEval[depth-1]-cutoff) {
                      return result;
                    }
                    //if the move the other colour made is refutable, why bother with checking other moves
                  }
                  bestEval=result;
                  //steps to remember which move
                  bestEval=result;
                  rowStart=row;
                  colStart=col;
                  rowEnd=rowTgt;
                  colEnd=colTgt;
                  newLastMove=lastMove;
                  if (move[i+3]=='!') {
                    ep=true;
                  }
                  else ep=false;
                }
              }
              lastMove=oldLastMove;
            }
          }
        }
      }
    }
    if (depth==0) {
      lastMove=newLastMove;
      cout << "Engine played ";
      if ((char)(colEnd+97)=='O') {
        if ((char)(rowEnd+49)=='<') cout<<"Long Castle\n";
        else cout<<"Short Castle\n";
        castle((char)(rowEnd+49));
        turn=flip(turn);
        fiftyMove--;
      }
      else {
        cout << "Engine played " << square[rowStart][colStart]->getType()
             << (char)(colStart + 97) << rowStart + 1 << " to "
             << (char)(colEnd + 97) << rowEnd + 1 << endl;
        if (square[rowStart][colStart]->getType() == 'P') {
          fiftyMove=50;
          if (rowEnd == 0 || rowEnd == 7) {
            delete square[rowStart][colEnd]; // byebye pawn
            square[rowStart][colStart] =
                new Fairy((char)(colEnd + 97), rowEnd + 1, turn, 'Q', qComb);
          }
        }
        else if (square[rowEnd][colEnd]->isEmpty()==false) {
          fiftyMove=50;
        }
        else fiftyMove--;
        delete square[rowEnd][colEnd];
        square[rowEnd][colEnd]=square[rowStart][colStart];
        square[rowStart][colStart]=new Piece;
        square[rowEnd][colEnd]->move(rowEnd,colEnd);
        if (ep == true) {
          delete square[rowStart][colEnd]; // byebye pawn
          square[rowStart][colEnd] = new Piece;
        }
        turn=flip(turn);
      }
      savePosition();
    }
    if (turn==false) {
      if (bestEval<-50) {
        if (isCheck()==false) {
          return 0;//the position is stalemate
        }
      }
    }
    else {
      if (bestEval>50) {
        if (isCheck()==false) {
          return 0;//the position is stalemate
        }
      }
    }
    return bestEval;
  }

  void setCutoff(float n) {cutoff=n;}

  void savePosition() {
    prev50move[l]=fiftyMove;
    lMove[l]=lastMove;
    pastDepth[l]=maxDepth;
    pastCutoff[l]=cutoff;
  for (int row=0;row<8;row++) {
    for (int col=0;col<8;col++) {
      pieces[l][row][col]=square[row][col]->getType();
      colours[l][row][col]=square[row][col]->getColour();
      moved[l][row][col]=square[row][col]->getHasMoved();
    }
  }
  l++;
}

void undo(int n) {//goes back n board states, cannot go back more than 99
  if (n>=l||n>=100) {
    cout<<"ERROR\n";
    return;
  }
  l-=n;
  int i=(l-1)%100;
  if (n%2==1) {
    changeTurn();
  }
  fiftyMove=prev50move[i];
  lastMove=lMove[i];
  maxDepth=pastDepth[i];
  cutoff=pastCutoff[i];
  for (int row=0;row<8;row++) {
    for (int col=0;col<8;col++) {
      delete square[row][col];
      char type=pieces[i][row][col];
      if (type!=' ') {
        if (type=='P') {
          square[row][col]=new Pawn((char)(col+97),row+1,colours[i][row][col]);
        }
        else if (isFairy==true) {
          char comb=' ';
          if (type=='N') comb=nComb;
          else if (type=='B') comb=bComb;
          else if (type=='R') comb=rComb;
          else if (type=='Q') comb=qComb;
          square[row][col]= new Fairy((char)(col+97),row+1,colours[i][row][col],type,comb);
          //the fairy piece will act the same as a normal piece when fairy is disabled
        }
        else {
          if (type=='N') 
            square[row][col]=new Knight((char)(col+97),row+1,colours[i][row][col]);
          else if (type=='B') 
            square[row][col]=new Bishop((char)(col+97),row+1,colours[i][row][col]);
          else if (type=='R') 
            square[row][col]=new Rook((char)(col+97),row+1,colours[i][row][col]);
          else if (type=='Q') 
            square[row][col]=new Queen((char)(col+97),row+1,colours[i][row][col]);
          else if (type=='K') 
            square[row][col]=new King((char)(col+97),row+1,colours[i][row][col]);
        }
        square[row][col]->setHasMoved(moved[i][row][col]);
      }
      else square[row][col]= new Piece;
    }
  }
}

protected:
  int depth;
  int maxDepth;
  float pastEval[10];
  float runningEval;
  unordered_map<char,float> m;
  float cutoff=1.5;
  bool refutable=false;
  int prev50move[100];
};

#endif