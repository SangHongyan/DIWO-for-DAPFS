#pragma once
class Problem
{
public:
	void SetInstance(int Jobs, int Machines, int Factories, vector<vector<int>> PTimeofJob, int Sets, vector <int> PTimeofSet, vector <int> SetsofJob);
	void DumpInstance(string FileName);
protected:
	void InitBestSol(vector <int> SeqofSet, vector<vector<int>> JobSeqinEachSet, int TF);
	bool UpdateBestSol(vector <int> SeqofSet, vector<vector<int>> JobSeqinEachSet, int TF);

	int m_Jobs;
	int m_Machines;
	int m_Factories;
	vector <vector<int>> m_PTimeofJob;
	int m_Sets;
	vector<int> m_PTimeofSet;
	vector<int> m_SetofJob;
	vector <vector <int>> m_JobsofSet;


	vector <int> m_bestSeqofSet;
	vector<vector<int>> m_bestJobSeqofEachSet;
	int m_bestTF;

public:
	Problem();
	virtual ~Problem();
};

