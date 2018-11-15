#pragma once
#include "Heuristic.h"
class DIWO :
	public Heuristic
{
public:
	DIWO();
	virtual ~DIWO();	
	int Evolution1(double alpha);
	int Evolution1(double alpha, vector<int> &MetaTFArray, vector<int> &ElapsedTimeArray);
	int Evolution2();
	int Evolution2(vector<int> &MetaTFArray, vector<int> &ElapsedTimeArray);
	int Evolution3(double Prop);
	int Evolution3(double Prop,vector<int> &MetaTFArray, vector<int> &ElapsedTimeArray);
	void SetParameters(int PS0, int PSmax, int Smax, int Smin, int PLen, long TimeLimit);

private:
	int LocalSearch_JobInSet(vector<int> SeqofSet, vector <vector <int>> & JobSeqofSet, int Set,int TF);
	int LocalSearch_JobInAllSets(vector<int> SeqofSet, vector <vector <int>> & JobSeqofSet,int TF);
	int LocalSearch_Set(vector <int> &SeqofSets, vector<vector<int>> SeqofJobsinEachSet, int Set, int TF);
	int LocalSearch_AllSets(vector <int> &SeqofSets, vector<vector<int>> SeqofJobsinEachSet, int TF);
	int LocalSearch(vector<int> &SeqofSet, vector <vector <int>> & JobSeqofSet, int TF);

	void Insert_SetSeq(vector<int> & SeqofSet);
	void Swap_SetSeq(vector<int> & SeqofSet);
	void Insert_JobSeqInSet(vector<vector<int>> & SeqofJob);
	void Swap_JobSeqInSet(vector<vector<int>> & SeqofJob);

	vector <int> m_TFArray;
	vector <vector <int>> m_SeqofSets;
	vector<vector <vector <int>>> m_JobSeqInSets;

	int m_PSize;
	int m_PSize0;
	int m_PSizemax;
	int m_PerLen;
	long m_TimeLimit;
	int m_Smax;
	int m_Smin;
};

