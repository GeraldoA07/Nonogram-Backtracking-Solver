#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../include/Nonogram.hpp"

using namespace std;

bool readInput(const string& filename,int& N, int& M,vector<vector<int>>& rowClue,vector<vector<int>>& colClue){
    ifstream f(filename);
    if (!f.is_open()) {
        cerr << "[ERROR] File tidak ditemukan: " << filename << "\n";
        return false;
    }

    string line;

    if (!getline(f, line)) {
        cerr << "[ERROR] File kosong atau format tidak valid.\n";
        return false;
    }
    {
        istringstream ss(line);
        if (!(ss >> N >> M) || N <= 0 || M <= 0) {
            cerr << "[ERROR] Baris pertama harus berisi dua bilangan bulat positif (N M).\n";
            return false;
        }
    }

    rowClue.clear();
    for (int i = 0; i < N; i++) {
        if (!getline(f, line)) {
            cerr << "[ERROR] Clue baris ke-" << i+1 << " tidak ditemukan.\n";
            return false;
        }
        vector<int> clue;
        istringstream ss(line);
        int val;
        while (ss >> val) {
            if (val <= 0) {
                cerr << "[ERROR] Clue harus berupa bilangan positif.\n";
                return false;
            }
            clue.push_back(val);
        }
        if (clue.empty()) {
            cerr << "[ERROR] Clue baris ke-" << i+1 << " kosong.\n";
            return false;
        }
        rowClue.push_back(clue);
    }

    colClue.clear();
    for (int j = 0; j < M; j++) {
        if (!getline(f, line)) {
            cerr << "[ERROR] Clue kolom ke-" << j+1 << " tidak ditemukan.\n";
            return false;
        }
        vector<int> clue;
        istringstream ss(line);
        int val;
        while (ss >> val) {
            if (val <= 0) {
                cerr << "[ERROR] Clue harus berupa bilangan positif.\n";
                return false;
            }
            clue.push_back(val);
        }
        if (clue.empty()) {
            cerr << "[ERROR] Clue kolom ke-" << j+1 << " kosong.\n";
            return false;
        }
        colClue.push_back(clue);
    }

    for (int i = 0; i < N; i++) {
        int total = 0;
        for (int c : rowClue[i]) total += c;
        total += (int)rowClue[i].size() - 1; // spasi minimum antar blok
        if (total > M) {
            cerr << "[ERROR] Clue baris ke-" << i+1
                 << " tidak valid (total melebihi panjang baris).\n";
            return false;
        }
    }
    for (int j = 0; j < M; j++) {
        int total = 0;
        for (int c : colClue[j]) total += c;
        total += (int)colClue[j].size() - 1;
        if (total > N) {
            cerr << "[ERROR] Clue kolom ke-" << j+1
                 << " tidak valid (total melebihi tinggi kolom).\n";
            return false;
        }
    }

    return true;
}

void printClueRow(const vector<int>& clue) {
    for (int i = 0; i < (int)clue.size(); i++) {
        if (i > 0) cout << " ";
        cout << clue[i];
    }
}

int main(int argc, char* argv[]) {
    string filename;
    if (argc >= 2) {
        filename = argv[1];
    } else {
        cout << "Masukkan path file input: ";
        cin >> filename;
    }

    cout << "\n[INFO] Membaca file: " << filename << " ...\n";
    int N, M;
    vector<vector<int>> rowClue, colClue;

    if (!readInput("../input/" + filename, N, M, rowClue, colClue)) {
        return 1;
    }

    cout << "[INFO] Ukuran papan: " << N << " baris x " << M << " kolom\n";

    cout << "\n--- Clue Baris ---\n";
    for (int i = 0; i < N; i++) {
        cout << "  Baris " << i+1 << ": ";
        printClueRow(rowClue[i]);
        cout << "\n";
    }

    cout << "\n--- Clue Kolom ---\n";
    for (int j = 0; j < M; j++) {
        cout << "  Kolom " << j+1 << ": ";
        printClueRow(colClue[j]);
        cout << "\n";
    }

    Nonogram nonogram(N, M, rowClue, colClue);

    cout << "\n[INFO] Menjalankan pre-processing...\n";
    nonogram.preProcess();

    cout << "[INFO] Menjalankan backtracking solver...\n\n";

    bool solved = nonogram.solve();


    if (solved) {
        cout << "SOLUSI DITEMUKAN!\n";
        nonogram.infoBoard();
    } else {
        cout << "TIDAK ADA SOLUSI YANG VALID\n";
    }

    return 0;
}
