#pragma once
#include "NOperator.h"
class Heuristic :
	public NOperator
{
public:
	Heuristic();
	virtual ~Heuristic();
	int HR1(vector <int> &SeqofSet, vector<vector<int>> & SeqofJobinEachSet);
	int HR2(vector <int> &SeqofSet, vector<vector<int>> & SeqofJobinEachSet);
	int HR3(vector <int> &SeqofSet, vector<vector<int>> & SeqofJobinEachSet);
	int VND1(vector <int> &SeqofSet, vector<vector<int>> & SeqofJobinEachSet);
	int VND2(vector <int> &SeqofSet, vector<vector<int>> & SeqofJobinEachSet);
	int VND3(vector <int> &SeqofSet, vector<vector<int>> & SeqofJobinEachSet);
	int VND_LocalSearch(vector <int> &SeqofSet, vector<vector<int>> & SeqofJobinEachSet,int TF);
protected:
	int ProductLocalSearch_VND(vector <int> &SeqofSets, vector<vector<int>> SeqofJobsinEachSet, int mSpan); //VND LocalSearch1
	int JobLocalSearch_VND(vector <int> SeqofSets, vector<vector<int>> &SeqofJobsinEachSet, int mSpan); //VND LocalSearch2
private:
	int NEH_InFactory(vector <int> &SeqofJob);
	int FL_InFactory(vector <int> &SeqofJob);
	int NEHInsert_InFactory(vector <int> SeqofJob_Seed, vector <int> & SeqofJob_New);

