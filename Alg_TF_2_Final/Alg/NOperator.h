#pragma once
#include "Problem.h"
class NOperator :
	public Problem
{
public:
	NOperator();
	virtual ~NOperator();
	int GetSpan_InFactory(vector <int> JobSeq);
	int GetSpan(vector <int> SeqofSet, vector<vector<int>> SeqofJobInEachSet, vector <vector<int>> &SeqofJobinEachFac, vector<vector<int>> &CTimeofJobs, vector <int> &CTimeofSets); //
	int GetSpan(vector <int> SeqofSets, vector<vector<int>> SeqofJobsInEachSet);
	int GetTF(vector <int> SeqofSets, vector<vector<int>> SeqofJobsInEachSet);
	int GetTF_Gantt(vector <int> SeqofSets, vector<vector<int>> SeqofJobsInEachSet, string FileName);
	void CheckSol(vector <int> SeqofSets, vector<vector<int>> SeqofJobsInEachSet, int TF);
	void GetBestSol(vector <int> &SeqofSets, vector<vector<int>> &SeqofJobsInEachSet, int &TF);
	void GetRTimeofSet(vector <vector<int>> CTimeofJob, vector<int> &RTimeofSet);
	int GetCTimeofSet(vector<int> SeqofSets, vector<int> RTimeofSet, vector <int> &CTimeofSet);

protected:
	void LPT_Job(vector <int> &SeqofJob);
	void SPT_Job(vector <int> &SeqofJob);
	void SPT_Set(vector <int> &SeqofSet);
	void LPT_Set(vector <int> &SeqofSet);
	void Swap(vector <int> &Seq, int Pos1, int Pos2);
	int GetEij_InFactory(vector <int> SeqofJob, vector <vector <int>> &Eij);
	int GetQij_InFactory(vector <int> SeqofJob, vector <vector <int>> &Qij);
	int FindBestPosition_InFactory(vector <int> SeqofJob, vector<vector<int>> Eij, vector<vector<int>> Qij, int Job, int &bestPosition); //inside a factory
	int LocalSearch_InFactory_Swap(vector <int> &SeqofJob, int mSpan);
};

