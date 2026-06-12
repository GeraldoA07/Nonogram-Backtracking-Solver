#include "../include/Nonogram.hpp"

Nonogram::Nonogram(int n, int m, const vector<vector<int>>& rClue, const vector<vector<int>>& cClue) : N(n), M(m), rowClue(rClue), colClue(cClue) {
    board.assign(N, vector<CellState>(M,UNASSIGNED));
}

void Nonogram::infoBoard()const{
    for (int i = 0; i < N; i++){
        for (int j = 0; j < M ; j++){
            if (board[i][j] == UNASSIGNED) cout << " ";
            if (board[i][j] == FILLED) cout << "█";
            if (board[i][j] == CROSSED) cout << "X";
        }
        cout << endl;
    }
}

bool Nonogram::solve(int row, int col){
    if (row == N){
        return true; 
    }

    int nextRow = (col == M-1) ? row + 1:row;
    int nextCol = (col == M-1) ? 0 : col+1;

    if (board[row][col] != UNASSIGNED){
        return solve(nextRow,nextCol);
    }

    board[row][col] = FILLED; // Pembangkit cabang pertama

    if (boundingFunction(row, col)){
        if (solve(nextRow,nextCol)){
            return true;
        }
    }

    board[row][col] = CROSSED;
    if (boundingFunction(row,col)){
        if (solve(nextRow,nextCol)){
            return true;
        }
    }

    board[row][col] = UNASSIGNED;
    return false;
}

bool Nonogram::boundingFunction(int row, int col) const{
    // Pelanggaran penjumlahan petunjuk
    int curFilledRow = 0;
    int maxAllowedRow = 0; 

    for(int clue : rowClue[row]){
        maxAllowedRow += clue;
    }

    for (int i = 0; i <= col; i++){
        if (board[row][i] == FILLED){
            curFilledRow++;
        }
    }

    if (curFilledRow > maxAllowedRow){
        return false;
    }

    int curFilledCol = 0; 
    int maxAllowedCol = 0;
    for (int clue : colClue[col]){
        maxAllowedCol+=clue;
    }

    for (int i = 0; i <= row; i++){
        if (board[i][col] == FILLED){
            curFilledCol++;
        }
    }
    if (curFilledCol > maxAllowedCol){
        return false;
    }

    // pelanggaran ukuran blok & jarak baris

    int idxRow = 0; // indeks clue yang sedang dievaluasi
    int curBlockRow = 0; // panjang deretan filled saat ini / temp

    for (int i = 0; i<= col; i++){
        if (board[row][i] == FILLED){
            curBlockRow++;
        } else if (board[row][i] == CROSSED){
            if (curBlockRow > 0){
                if (idxRow >= (int)rowClue[row].size() || curBlockRow != rowClue[row][idxRow]){
                    return false;
                }
                idxRow++;
                curBlockRow = 0;
            }
        }
    }

    if (curBlockRow > 0){
        if (idxRow >= (int)rowClue[row].size() || curBlockRow > rowClue[row][idxRow]){
            return false;
        }
    }

    // pelanggaran ukuran blok & jarak kolom

    int idxCol = 0;
    int curBlockCol = 0;
    for (int i = 0; i <= row; i++){
        if (board[i][col] == FILLED){
            curBlockCol++;
        } else if (board[i][col] == CROSSED){
            if (curBlockCol > 0){
                if (idxCol >= (int)colClue[col].size() || curBlockCol != colClue[col][idxCol]){
                    return false;
                }
                idxCol++;
                curBlockCol = 0;
            }
        }
    }
    if (curBlockCol > 0){
        if (idxCol >= (int)colClue[col].size() || curBlockCol > colClue[col][idxCol]){
            return false;
        }
    }

    return true;
}

void Nonogram::preProcess(){
    // baris
    for (int i=0; i < N; i++){
        int L = M; // panjang baris = jumlah kolom

        if (rowClue[i].size() == 1){
            int C = rowClue[i][0];

            if (C > L/2){
                int startIdx = L - C;
                int endIdx = C - 1;

                for (int j = startIdx; j <= endIdx; j++){
                    board[i][j] = FILLED;
                }
            }
        }

        int sumClue = 0;
        for (int c : rowClue[i]){
            sumClue += c;
        }
        int minSpace = (int)rowClue[i].size() - 1;

        if (sumClue + minSpace == L){
            int j = 0;
            for (int c : rowClue[i]){
                for (int k = 0; k < c; k++){
                    board[i][j++] = FILLED;
                }

                if (j < L){
                    board[i][j++] = CROSSED;
                }
            }
        }
    }


    // kolom
    for (int j = 0; j < M; j++){
        int L = N; // panjang kolom = jumlah baris

        if (colClue[j].size() == 1){
            int C = colClue[j][0];

            if (C > L/2){
                int startIdx = L - C;
                int endIdx = C - 1;

                for (int i = startIdx; i <= endIdx; i++){
                    board[i][j] = FILLED;
                }
            }
        }

        int sumClue = 0;
        for (int c : colClue[j]){
            sumClue += c;
        }
        int minSpace = (int)colClue[j].size() - 1;

        if (sumClue + minSpace == L){
            int i = 0;
            for (int c : colClue[j]){
                for (int k = 0; k < c; k++){
                    board[i++][j] = FILLED;
                }
                if (i < L){
                    board[i++][j] = CROSSED;
                }
            }
        }
    }

}