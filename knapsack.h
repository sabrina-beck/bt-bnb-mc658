/*******************************************************
 * MC658 - Projeto e Analise de Algoritmo III - 1s2017
 * Prof: Flavio Keidi Miyazawa
 * PED: Edson Ticona Zegarra
 ******************************************************/
#ifndef KNAPSACK_H
#define KNAPSACK_H
#include<string>
#include<vector>
#include<algorithm>
#include<iostream>
#include<set>
using namespace std;
int read_knapsack(string);
int show_usage();
int show_input();
bool bt(int n, int d, int B, vector<int>&p, vector<int>&w, vector<int>&c, vector<int>&sol, int t);
bool bnb(int n, int d, int B, vector<int>&p, vector<int>&w, vector<int>&c, vector<int>&sol, int t);
bool is_feasible_solution();
#endif
