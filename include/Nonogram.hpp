#pragma once

#include <vector> 
#include <iostream>
using namespace std;

enum CellState{
    UNASSIGNED = 0, // belum ada aksi 
    FILLED = 1, // berisi blok warna
    CROSSED = 2 // berisi penanda bahwa blok kosong
};

class Nonogram { 
private : 
    int N;
    int M;

    vector<vector<CellState>> board;
    
    vector<vector<int>> rowClue;
    vector<vector<int>> colClue;
    
    bool boundingFunction(int row, int col) const;
    
public : 
    Nonogram(int n, int m, const vector<vector<int>>& rClue, const vector<vector<int>>& cClue);

    void infoBoard() const;
    bool solve(int row = 0, int col = 0);
    void preProcess();

};