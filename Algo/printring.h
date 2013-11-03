#pragma once
#include<iostream>
using namespace std;

namespace RING{
#define SIMONN 6
#define MROW SIMONN
#define MCOL SIMONN

  void Print(int Array[MROW][MCOL])
  {
    int i, j;
    for (i = 0; i < MROW; ++i)
    {
      for (j = 0; j < MCOL; ++j)
      {
        cout << i << "," << j << ":" << Array[i][j] << " ";
      }
      cout << endl;
    }
  }
  void PrintColLeft(int &col, int &row, int &flagx, int &flagy, int Array[MROW][MCOL])
  {
    int num = 0;
    int i = row;
    for (; num < flagx; ++i, ++num)
    {
      cerr << Array[col][i] << " ";
    }
    row = i - 1;
    col += 1;
    flagx--;
  }
  void PrintRowDown(int &col, int &row, int &flagx, int &flagy, int Array[MROW][MCOL])
  {
    int num = 0;
    int i = col;
    for (; num < flagy; ++i, ++num)
    {
      cout << Array[i][row] << " ";
    }
    col = i - 1;
    row -= 1;
    flagy--;
  }
  void PrintColRight(int &col, int &row, int &flagx, int &flagy, int Array[MROW][MCOL])
  {
    int num = 0;
    int i = row;
    for (; num < flagx; --i, ++num)
    {
      cerr << Array[col][i] << " ";
    }
    row = i + 1;
    col -= 1;
    flagx--;
  }
  void PrintRowUp(int &col, int &row, int &flagx, int &flagy, int Array[MROW][MCOL])
  {
    int num = 0;
    int i = col;
    for (; num < flagy; --i, ++num)
    {
      cerr << Array[i][row] << " ";
    }
    col = i + 1;
    row += 1;
    flagy--;
  }

  void PrintRing()
  {
    int Array[MROW][MCOL];
    int k = 1;
    int i, j;
    int FlagX = MROW;
    int FlagY = MCOL - 1;
    for (i = 0; i < MROW; ++i)
    {
      for (j = 0; j < MCOL; ++j)
      {
        Array[i][j] = k;
        k++;
      }
    }
    int row_ = 0;
    int col_ = 0;
    do
    {
      PrintColLeft(row_, col_, FlagX, FlagY, Array);
      PrintRowDown(row_, col_, FlagX, FlagY, Array);
      PrintColRight(row_, col_, FlagX, FlagY, Array);
      PrintRowUp(row_, col_, FlagX, FlagY, Array);
    } while (FlagX > 0 && FlagY + 1 > 0);

    cout << endl;
    Print(Array);
  }

#define PII pair<int,int>
  void setmatrix(int **pm,
    pair<int, int> s, // start
    pair<int, int> e  // end
    )
  {
    static int count = 0;
    if (s == e){
      pm[s.first][s.second] = count++;
      return;
    }
    //l->r
    for (int j = s.second; j <= e.second; ++j){
      pm[s.first][j] = count++;
    }
    //downward
    for (int i = s.first + 1; i <= e.first; ++i){
      pm[i][e.second] = count++;
    }
    //r->l
    for (int j = e.second - 1; j >= s.second; --j){
      pm[e.first][j] = count++;
    }
    //upward
    for (int i = e.first - 1; i >= s.first + 1; --i){
      pm[i][s.second] = count++;
    }
  }
  void printmatrix(int **pm, int n){
    for (int i = 0; i < n; ++i)
    {
      for (int j = 0; j < n; ++j){
        cout << pm[i][j] << "\t";
      }
      cout << endl;
    }
  }

  void printM2(int **pm,
    pair<int, int> s, // start
    pair<int, int> e  // end
    )
  {
    if (s == e){
      cout << pm[s.first][s.second];
      return;
    }
    //l->r
    for (int j = s.second; j <= e.second; ++j){
      cout << pm[s.first][j];
    }
    //downward
    for (int i = s.first + 1; i <= e.first; ++i){
      cout << pm[i][e.second];
    }
    //r->l
    for (int j = e.second - 1; j >= s.second; --j){
      cout << pm[e.first][j];
    }
    //upward
    for (int i = e.first - 1; i >= s.first + 1; --i){
      cout << pm[i][s.second];
    }
  }

  int** allocmem(int n){
    int **pm = new int*[n];
    for (int i = 0; i < n; ++i){
      pm[i] = new int[n];
    }
    return pm;
  }
  void deallocmem(int **pm, int n){
    for (int i = 0; i < n; ++i){
      delete[] pm[i];
    }
    delete[] pm;
  }
  // find diagonal points and print
  void printM(int **pm, int n//size
    ){
    for (int i = 0; i < n; ++i){
      PII s(i, i);
      int tmp = n - 1 - i;
      if (i>tmp){ break; }
      PII e(tmp, tmp);
      printM2(pm, s, e);
    }
  }

  void printMatrixHelix(int n){
    int** pm = allocmem(n);
    for (int i = 0; i < n; ++i){
      //  for (int i = 0; i<n - 1 - i; ++i){
      PII s(i, i);
      int tmp = n - 1 - i;
      if (i>tmp){ break; }
      PII e(tmp, tmp);
      setmatrix(pm, s, e);
    }
    printmatrix(pm, n);
    printM(pm, n);
    deallocmem(pm, n);
  }

  


}//namespace