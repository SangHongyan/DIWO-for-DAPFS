#include "stdafx.h"
#include "Problem.h"


Problem::Problem()
{
}

Problem::~Problem()
{
}

void Problem::SetInstance(int Jobs, int Machines, int Factories, vector<vector<int>> PTimeofJob, int Sets, vector <int> PTimeofSet, vector <int> SetofJob)
{
	this->m_Jobs = Jobs;
	this->m_Machines = Machines;
	this->m_Factories = Factories;
	this->m_PTimeofJob = PTimeofJob;
	this->m_Sets = Sets;
	this->m_PTimeofSet = PTimeofSet;
	this->m_SetofJob = SetofJob;

	for (int j = 0; j < this->m_SetofJob.size(); j++)
		this->m_SetofJob[j] --;
	this->m_JobsofSet.clear();
	this->m_JobsofSet.resize(this->m_Sets);
	for (int j = 0; j < this->m_Jobs; j++)
		this->m_JobsofSet[this->m_SetofJob[j]].push_back(j);
}

void Problem::DumpInstance(string FileName)
{
	ofstream ofile;
	ofile.open(FileName);
	if (!ofile)
		exit(0);

	ofile << "----------Problem Information---------" << endl;
	for (int j = 0; j < this->m_Jobs; j++)
	{
		for (int m = 0; m < this->m_Machines; m++)
		{
			ofile << this->m_PTimeofJob[j][m] << "\t";
		}
		ofile << endl;
	}

	ofile << "PTime of Sets" << endl;
	for (int j = 0; j < this->m_Sets; j++)
		ofile << this->m_PTimeofSet[j] << endl;

	ofile << "Job in Set" << endl;
	for (int j = 0; j < this->m_Jobs; j++)
	{
		ofile << this->m_SetofJob[j] << endl;
	}
	ofile.close();
}

void Problem::InitBestSol(vector <int> SeqofSet, vector<vector<int>> JobSeqinEachSet, int TF)
{
	this->m_bestSeqofSet = SeqofSet;
	this->m_bestJobSeqofEachSet = JobSeqinEachSet;
	this->m_bestTF= TF;
}

bool Problem::UpdateBestSol(vector <int> SeqofSet, vector<vector<int>> JobSeqinEachSet, int TF)
{
	bool bImp = TF < this->m_bestTF;
	if (TF < this->m_bestTF)
	{
		this->m_bestSeqofSet = SeqofSet;
		this->m_bestJobSeqofEachSet = JobSeqinEachSet;
		this->m_bestTF= TF;
	}
	return bImp;
}
