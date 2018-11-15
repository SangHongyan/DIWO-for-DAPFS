#include "stdafx.h"
#include "Heuristic.h"


Heuristic::Heuristic()
{
}

Heuristic::~Heuristic()
{
}

int Heuristic::NEHInsert_InFactory(vector <int> SeqofJob_Seed, vector <int> & SeqofJob_New)
{
	SeqofJob_New.clear();
	SeqofJob_New.push_back(SeqofJob_Seed[0]);
	int Span;
	for (int i = 1; i < SeqofJob_Seed.size(); i++)
	{
		vector <vector<int>> Eij(SeqofJob_New.size()), Qij;
		for (int j = 0; j < Eij.size(); j++)
			Eij[j].resize(this->m_Machines);
		Qij = Eij;
		this->GetEij_InFactory(SeqofJob_New, Eij);
		this->GetQij_InFactory(SeqofJob_New, Qij);
		int Job = SeqofJob_Seed[i], bestPosition;
		Span = this->FindBestPosition_InFactory(SeqofJob_New, Eij, Qij, Job, bestPosition);
		SeqofJob_New.insert(SeqofJob_New.begin()+bestPosition, Job);
	}
	return Span;
}

int Heuristic::NEH_InFactory(vector <int> &SeqofJob)
{
	this->LPT_Job(SeqofJob);
	int Span = this->NEHInsert_InFactory(SeqofJob, SeqofJob);
	return Span;
}

int Heuristic::FL_InFactory(vector <int> &SeqofJob)
{
	vector <int> SeqofJob_New;
	this->SPT_Job(SeqofJob);
	SeqofJob_New.push_back(SeqofJob[0]);
	int Span;
	for (int i = 1; i < SeqofJob.size(); i++)
	{
		vector <vector<int>> Eij(SeqofJob_New.size()), Qij;
		for (int j = 0; j < Eij.size(); j++)
			Eij[j].resize(this->m_Machines);
		Qij = Eij;
		this->GetEij_InFactory(SeqofJob_New, Eij);
		this->GetQij_InFactory(SeqofJob_New, Qij);
		int Job = SeqofJob[i], bestPosition;
		Span = this->FindBestPosition_InFactory(SeqofJob_New, Eij, Qij, Job, bestPosition);
		SeqofJob_New.insert(SeqofJob_New.begin() + bestPosition, Job);
		Span = this->LocalSearch_InFactory_Swap(SeqofJob_New, Span);
	}
	SeqofJob = SeqofJob_New;
	return Span;
}

int Heuristic::HR1(vector <int> &SeqofSet, vector<vector<int>> & SeqofJobinEachSet)
{
	SeqofSet.clear();
	SeqofSet.resize(this->m_Sets);
	for (int i = 0; i < this->m_Sets; i++)
		SeqofSet[i] = i;
	this->SPT_Set(SeqofSet);

	SeqofJobinEachSet = this->m_JobsofSet;
	for (int i = 0; i < this->m_Sets; i++)
		this->FL_InFactory(SeqofJobinEachSet[i]);


	int TF=this->GetTF(SeqofSet, SeqofJobinEachSet);
	return TF;
}

int Heuristic::HR2(vector <int> &SeqofSet, vector<vector<int>> & SeqofJobinEachSet)
{
	SeqofJobinEachSet = this->m_JobsofSet;
	for (int i = 0; i < this->m_Sets; i++)
		this->FL_InFactory(SeqofJobinEachSet[i]);

	vector <int> Eh(this->m_Sets, 0);
	SeqofSet.resize(1);
	for (int i = 0; i < this->m_Sets; i++)
	{
		SeqofSet[0]=i;
		Eh[i] = this->GetSpan(SeqofSet, SeqofJobinEachSet) - this->m_PTimeofSet[i];
	}


	Pair<int> *ch=new Pair <int>[this->m_Sets];
	for (int i = 0; i < this->m_Sets; i++)
	{
		ch[i].dim = i;
		ch[i].value =  Eh[i];
	}
	sort(ch, ch + this->m_Sets, PairLess<int>());
	SeqofSet.resize(this->m_Sets);
	for (int i = 0; i < this->m_Sets; i++)
		SeqofSet[i] = ch[i].dim;
	delete[] ch;


	int TF = this->GetTF(SeqofSet, SeqofJobinEachSet);
	return TF;
}

int Heuristic::HR3(vector <int> &SeqofSet, vector<vector<int>> & SeqofJobinEachSet)
{
	SeqofJobinEachSet = this->m_JobsofSet;
	for (int i = 0; i < this->m_Sets; i++)
		this->SPT_Job(SeqofJobinEachSet[i]);

	vector <int> Eh(this->m_Sets, 0);
	SeqofSet.resize(1);
	for (int i = 0; i < this->m_Sets; i++)
	{
		SeqofSet[0] = i;
		Eh[i] = this->GetSpan(SeqofSet, SeqofJobinEachSet) - this->m_PTimeofSet[i];
	}


	Pair<int> *ch = new Pair <int>[this->m_Sets];
	for (int i = 0; i < this->m_Sets; i++)
	{
		ch[i].dim = i;
		ch[i].value = Eh[i];
	}
	sort(ch, ch + this->m_Sets, PairLess<int>());
	SeqofSet.resize(this->m_Sets);
	for (int i = 0; i < this->m_Sets; i++)
		SeqofSet[i] = ch[i].dim;
	delete[] ch;


	int TF = this->GetTF(SeqofSet, SeqofJobinEachSet);
	return TF;
}

int Heuristic::VND1(vector <int> &SeqofSet, vector<vector<int>> & SeqofJobinEachSet)
{
	int TF = this->HR1(SeqofSet, SeqofJobinEachSet);
	int LastTF = TF;
	do
	{
		LastTF = TF;
		TF = this->ProductLocalSearch_VND(SeqofSet, SeqofJobinEachSet, TF);
		TF = this->JobLocalSearch_VND(SeqofSet, SeqofJobinEachSet, TF);
	} while (TF<LastTF);

	this->CheckSol(SeqofSet, SeqofJobinEachSet, TF);
	return TF;
}

int Heuristic::VND2(vector <int> &SeqofSet, vector<vector<int>> & SeqofJobinEachSet)
{
	int TF = this->HR2(SeqofSet, SeqofJobinEachSet);
	int LastTF = TF;
	do
	{
		LastTF = TF;
		TF = this->ProductLocalSearch_VND(SeqofSet, SeqofJobinEachSet, TF);
		TF = this->JobLocalSearch_VND(SeqofSet, SeqofJobinEachSet, TF);
	} while (TF<LastTF);

	this->CheckSol(SeqofSet, SeqofJobinEachSet, TF);
	return TF;
}

int Heuristic::VND3(vector <int> &SeqofSet, vector<vector<int>> & SeqofJobinEachSet)
{
	int TF = this->HR3(SeqofSet, SeqofJobinEachSet);
	int LastTF = TF;
	do
	{
		LastTF = TF;
		TF = this->ProductLocalSearch_VND(SeqofSet, SeqofJobinEachSet, TF);
		TF = this->JobLocalSearch_VND(SeqofSet, SeqofJobinEachSet, TF);
	} while (TF<LastTF);

	this->CheckSol(SeqofSet, SeqofJobinEachSet, TF);
	return TF;
}

int Heuristic::VND_LocalSearch(vector <int> &SeqofSet, vector<vector<int>> & SeqofJobinEachSet, int TF)
{
	int LastTF = TF;
	do
	{
		LastTF = TF;
		TF = this->ProductLocalSearch_VND(SeqofSet, SeqofJobinEachSet, TF);
		TF = this->JobLocalSearch_VND(SeqofSet, SeqofJobinEachSet, TF);
	} while (TF<LastTF);

	return TF;
}

int Heuristic::ProductLocalSearch_VND(vector <int> &SeqofSets, vector<vector<int>> SeqofJobsinEachSet, int TF)
{
	vector <int> TempSeq, TempSeq1;
	int i = 0;
	while (i < SeqofSets.size())
	{
		TempSeq = SeqofSets;
		int CurSet = TempSeq[i];
		TempSeq.erase(TempSeq.begin() + i);

		int TminTF = INT_MAX, pt = -1;
		for (int Pos = 0; Pos <= TempSeq.size(); Pos++)
		{
			if (Pos == i) continue;
			TempSeq1 = TempSeq;
			TempSeq1.insert(TempSeq1.begin() + Pos, CurSet);
			int TempTF = GetTF(TempSeq1, SeqofJobsinEachSet);
			if (TempTF < TminTF)
			{
				TminTF = TempTF;
				pt = Pos;
			}
		}

		if (TminTF< TF)
		{
			TF = TminTF;
			TempSeq.insert(TempSeq.begin() + pt, CurSet);
			SeqofSets = TempSeq;
			i = 0;
		}
		else
			i++;
	}
	return TF;
}

int Heuristic::JobLocalSearch_VND(vector <int> SeqofSets, vector<vector<int>> &SeqofJobsinEachSet, int TF)
{
	vector<vector <int>> TempSeq, TempSeq1;
	int h = 0;
	while (h < SeqofSets.size())
	{
		int CurSet = SeqofSets[h];
		int j = 0;
		while (j < SeqofJobsinEachSet[CurSet].size())
		{
			TempSeq = SeqofJobsinEachSet;
			int CurJob = TempSeq[CurSet][j];
			TempSeq[CurSet].erase(TempSeq[CurSet].begin() + j);
			int TminTF = INT_MAX, pt = -1;
			for (int Pos = 0; Pos <= TempSeq[CurSet].size(); Pos++)
			{
				if (Pos == j) continue;
				TempSeq1 = TempSeq;
				TempSeq1[CurSet].insert(TempSeq1[CurSet].begin() + Pos, CurJob);
				int TempTF= GetTF(SeqofSets, TempSeq1);
				if (TempTF < TminTF)
				{
					TminTF= TempTF;
					pt = Pos;
				}
			}

			if (TminTF < TF)
			{
				TF = TminTF;
				TempSeq[CurSet].insert(TempSeq[CurSet].begin() + pt, CurJob);
				SeqofJobsinEachSet = TempSeq;
				j = 0;
			}
			else
				j++;
		}
		h++;
	}
	return TF;
}

