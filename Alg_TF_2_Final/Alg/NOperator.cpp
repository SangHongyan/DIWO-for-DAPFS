#include "stdafx.h"
#include "NOperator.h"

NOperator::NOperator()
{
}

NOperator::~NOperator()
{
}

void NOperator::SPT_Set(vector <int> &SeqofSet)
{
	Pair <int> *ch = new Pair <int>[SeqofSet.size()];
	for (int i = 0; i < SeqofSet.size(); i++)
	{
		ch[i].dim = SeqofSet[i];
		ch[i].value = this->m_PTimeofSet[SeqofSet[i]];
	}
	sort(ch, ch + SeqofSet.size(), PairLess<int>());
	for (int i = 0; i < SeqofSet.size(); i++)
		SeqofSet[i] = ch[i].dim;
	delete[]ch;
}

void NOperator::LPT_Set(vector <int> &SeqofSet)
{
	Pair <int> *ch = new Pair <int>[SeqofSet.size()];
	for (int i = 0; i < SeqofSet.size(); i++)
	{
		ch[i].dim = SeqofSet[i];
		ch[i].value = this->m_PTimeofSet[SeqofSet[i]];
	}
	sort(ch, ch + SeqofSet.size(), PairGreater<int>());
	for (int i = 0; i < SeqofSet.size(); i++)
		SeqofSet[i] = ch[i].dim;
	delete[]ch;
}

void NOperator::LPT_Job(vector <int> &SeqofJob)
{
	Pair <int> *ch = new Pair <int>[SeqofJob.size()];
	for (int i = 0; i < SeqofJob.size(); i++)
	{
		ch[i].dim = SeqofJob[i];
		ch[i].value = this->m_PTimeofJob[SeqofJob[i]][0];
		for (int m = 1; m < this->m_Machines; m++)
			ch[i].value += this->m_PTimeofJob[SeqofJob[i]][m];
	}
	sort(ch, ch + SeqofJob.size(), PairGreater<int>());
	for (int i = 0; i < SeqofJob.size(); i++)
		SeqofJob[i] = ch[i].dim;
	delete[]ch;
}

void NOperator::SPT_Job(vector <int> &SeqofJob)
{
	Pair <int> *ch = new Pair <int>[SeqofJob.size()];
	for (int i = 0; i < SeqofJob.size(); i++)
	{
		ch[i].dim = SeqofJob[i];
		ch[i].value = this->m_PTimeofJob[SeqofJob[i]][0];
		for (int m = 1; m < this->m_Machines; m++)
			ch[i].value += this->m_PTimeofJob[SeqofJob[i]][m];
	}
	sort(ch, ch + SeqofJob.size(), PairLess<int>());
	for (int i = 0; i < SeqofJob.size(); i++)
		SeqofJob[i] = ch[i].dim;
	delete[]ch;
}

void NOperator::Swap(vector <int> &Seq, int Pos1, int Pos2)
{
	int Temp = Seq[Pos1];
	Seq[Pos1] = Seq[Pos2];
	Seq[Pos2] = Temp;
}

int NOperator::GetEij_InFactory(vector <int> SeqofJob, vector <vector <int>> &Eij)
{
	vector <int> mIdle(this->m_Machines, 0);
	for (int i = 0; i < SeqofJob.size(); i++)
	{
		int Job = SeqofJob[i];
		Eij[i][0] = mIdle[0] + this->m_PTimeofJob[Job][0];
		for (int m = 1; m < this->m_Machines; m++)
			Eij[i][m] = max(Eij[i][m - 1], mIdle[m]) + this->m_PTimeofJob[Job][m];
		mIdle = Eij[i];
	}
	return mIdle[this->m_Machines - 1];
}

int NOperator::GetQij_InFactory(vector <int> SeqofJob, vector <vector <int>> &Qij)
{
	vector <int> mIdle(this->m_Machines, 0);
	for (int i = SeqofJob.size() - 1; i >= 0; i--)
	{
		int Job = SeqofJob[i];
		Qij[i][this->m_Machines - 1] = mIdle[this->m_Machines - 1] + this->m_PTimeofJob[Job][this->m_Machines - 1];
		for (int m = this->m_Machines - 2; m >= 0; m--)
			Qij[i][m] = max(Qij[i][m + 1], mIdle[m]) + this->m_PTimeofJob[Job][m];
		mIdle = Qij[i];
	}
	return mIdle[0];
}

int NOperator::GetSpan_InFactory(vector <int> SeqofJob)
{
	vector <vector <int>> Eij(SeqofJob.size());
	for (int j = 0; j < SeqofJob.size(); j++)
		Eij[j].resize(this->m_Machines, 0);
	return this->GetEij_InFactory(SeqofJob, Eij);
}

int NOperator::FindBestPosition_InFactory(vector <int> SeqofJob, vector<vector<int>> Eij, vector<vector<int>> Qij, int Job, int &bestPosition) // donot consider assembly phase
{
	vector <int> Fij(this->m_Machines);
	int minTSpan = INT_MAX,Span;
	for (int i = 0; i <= SeqofJob.size(); i++)
	{
		if (i == 0)
		{
			Fij[0] = this->m_PTimeofJob[Job][0];
			for (int m = 1; m < this->m_Machines; m++)
				Fij[m] = Fij[m - 1] + this->m_PTimeofJob[Job][m];
			Span = Fij[0] + Qij[0][0];
			for (int m = 1; m < this->m_Machines; m++)
			{
				int xx = Fij[m] + Qij[0][m];
				if (xx>Span)
					Span = xx;
			}
		}
		else if (i < SeqofJob.size())
		{
			Fij[0] = Eij[i - 1][0] + this->m_PTimeofJob[Job][0];
			for (int m = 1; m < this->m_Machines; m++)
				Fij[m] = max(Fij[m - 1], Eij[i - 1][m]) + this->m_PTimeofJob[Job][m];
			Span = Fij[0] + Qij[i][0];
			for (int m = 1; m < this->m_Machines; m++)
			{
				int xx = Fij[m] + Qij[i][m];
				if (xx>Span)
					Span = xx;
			}
		}
		else
		{
			Fij[0] = Eij[i - 1][0] + this->m_PTimeofJob[Job][0];
			for (int m = 1; m < this->m_Machines; m++)
				Fij[m] = max(Fij[m - 1], Eij[i - 1][m]) + this->m_PTimeofJob[Job][m];
			Span = Fij[this->m_Machines - 1];
		}

		if (Span < minTSpan)
		{
			minTSpan = Span;
			bestPosition = i;
		}
	}
	return minTSpan;
}

int NOperator::LocalSearch_InFactory_Swap(vector <int> &SeqofJob, int mSpan)
{
	vector <int> TempSeq = SeqofJob;
	int minTSpan=mSpan,Pos1, Pos2;
	for (int i = 0; i < SeqofJob.size() - 1; i++)
	{
		for (int j = i + 1; j < SeqofJob.size(); j++)
		{
			TempSeq = SeqofJob;
			this->Swap(TempSeq, i, j);
			int Span= this->GetSpan_InFactory(TempSeq);
			if (Span < minTSpan)
			{
				minTSpan = Span;
				Pos1 = i;
				Pos2 = j;
			}
		}
	}

	if (minTSpan < mSpan)
	{
		mSpan = minTSpan;
		this->Swap(SeqofJob, Pos1, Pos2);
	}

	return mSpan;
}

int NOperator::GetSpan(vector <int> SeqofSet, vector<vector<int>> SeqofJobInEachSet, vector <vector<int>> &SeqofJobInEachFac, vector<vector<int>> &CTimeofJob, vector <int> &CTimeofSet) //Assignment rule: minimize completion time of each job; // used in heuristics
{
	SeqofJobInEachFac.clear();
	SeqofJobInEachFac.resize(this->m_Factories);
	CTimeofJob.clear();
	CTimeofJob.resize(this->m_Jobs);
	for (int i = 0; i < CTimeofJob.size(); i++)
		CTimeofJob[i].resize(this->m_Machines);
	CTimeofSet.clear();
	CTimeofSet.resize(this->m_Sets);


	vector <vector <int>> mIdle(this->m_Factories), TempCTime(this->m_Factories);
	for (int i = 0; i < this->m_Factories; i++)
		mIdle[i].resize(this->m_Machines, 0);
	TempCTime = mIdle;
	vector <int> RTimeofSet(this->m_Sets, 0);//Release time of Sets
	int mIdle_Assemble = 0;

	for (int i = 0; i < SeqofSet.size(); i++)
	{
		int CurSet = SeqofSet[i];
		for (int j = 0; j < SeqofJobInEachSet[CurSet].size(); j++)
		{
			int Job = SeqofJobInEachSet[CurSet][j], minTempCTime = INT_MAX, SelFac = -1;
			for (int k = 0; k < this->m_Factories; k++)
			{
				TempCTime[k][0] = mIdle[k][0] + this->m_PTimeofJob[Job][0];
				for (int m = 1; m < this->m_Machines; m++)
					TempCTime[k][m] = max(TempCTime[k][m - 1], mIdle[k][m]) + this->m_PTimeofJob[Job][m];
				if (TempCTime[k][this->m_Machines - 1]<minTempCTime)
				{
					minTempCTime = TempCTime[k][this->m_Machines - 1];
					SelFac = k;
				}
			}

			mIdle[SelFac] = TempCTime[SelFac];
			CTimeofJob[Job] = TempCTime[SelFac];
			SeqofJobInEachFac[SelFac].push_back(Job);
			if (CTimeofJob[Job][this->m_Machines - 1] > RTimeofSet[CurSet])
				RTimeofSet[CurSet] = CTimeofJob[Job][this->m_Machines - 1];
		}

		CTimeofSet[CurSet] = max(mIdle_Assemble, RTimeofSet[CurSet]) + this->m_PTimeofSet[CurSet];
		mIdle_Assemble = CTimeofSet[CurSet];
	}
	return mIdle_Assemble;
}

int NOperator::GetSpan(vector <int> SeqofSet, vector<vector<int>> SeqofJobInEachSet)// used in heuristics
{
	vector <vector <int>> mIdle(this->m_Factories), CTimeofJob(this->m_Jobs), TempCTime(this->m_Factories);
	for (int i = 0; i < this->m_Factories; i++)
		mIdle[i].resize(this->m_Machines, 0);
	TempCTime = mIdle;
	for (int i = 0; i < this->m_Jobs; i++)
		CTimeofJob[i].resize(this->m_Machines, 0);
	vector <int> CTimeofSet(this->m_Sets, 0), RTimeofSet(this->m_Sets, 0);
	int mIdle_Assemble = 0;

	for (int i = 0; i < SeqofSet.size(); i++)
	{
		int CurSet = SeqofSet[i];
		for (int j = 0; j < SeqofJobInEachSet[CurSet].size(); j++)
		{
			int Job = SeqofJobInEachSet[CurSet][j], minTempCTime = INT_MAX, SelFac = -1;
			for (int k = 0; k < this->m_Factories; k++)
			{
				TempCTime[k][0] = mIdle[k][0] + this->m_PTimeofJob[Job][0];
				for (int m = 1; m < this->m_Machines; m++)
					TempCTime[k][m] = max(TempCTime[k][m - 1], mIdle[k][m]) + this->m_PTimeofJob[Job][m];
				if (TempCTime[k][this->m_Machines - 1]<minTempCTime)
				{
					minTempCTime = TempCTime[k][this->m_Machines - 1];
					SelFac = k;
				}
			}

			mIdle[SelFac] = TempCTime[SelFac];
			CTimeofJob[Job] = TempCTime[SelFac];
			if (CTimeofJob[Job][this->m_Machines - 1] > RTimeofSet[CurSet])
				RTimeofSet[CurSet] = CTimeofJob[Job][this->m_Machines - 1];
		}

		CTimeofSet[CurSet] = max(mIdle_Assemble, RTimeofSet[CurSet]) + this->m_PTimeofSet[CurSet];
		mIdle_Assemble = CTimeofSet[CurSet];
	}
	return mIdle_Assemble;
}

int NOperator::GetTF(vector <int> SeqofSet, vector<vector<int>> SeqofJobInEachSet)
{
	vector <vector <int>> mIdle(this->m_Factories), CTimeofJob(this->m_Jobs), TempCTime(this->m_Factories);
	for (int i = 0; i < this->m_Factories; i++)
		mIdle[i].resize(this->m_Machines, 0);
	TempCTime = mIdle;
	for (int i = 0; i < this->m_Jobs; i++)
		CTimeofJob[i].resize(this->m_Machines, 0);
	vector <int> CTimeofSet(this->m_Sets, 0), RTimeofSet(this->m_Sets, 0);
	int mIdle_Assemble = 0;

	for (int i = 0; i < SeqofSet.size(); i++)
	{
		int CurSet = SeqofSet[i];
		for (int j = 0; j < SeqofJobInEachSet[CurSet].size(); j++)
		{
			int Job = SeqofJobInEachSet[CurSet][j], minTempCTime = INT_MAX, SelFac = -1;
			for (int k = 0; k < this->m_Factories; k++)
			{
				TempCTime[k][0] = mIdle[k][0] + this->m_PTimeofJob[Job][0];
				for (int m = 1; m < this->m_Machines; m++)
					TempCTime[k][m] = max(TempCTime[k][m - 1], mIdle[k][m]) + this->m_PTimeofJob[Job][m];
				if (TempCTime[k][this->m_Machines - 1]<minTempCTime)
				{
					minTempCTime = TempCTime[k][this->m_Machines - 1];
					SelFac = k;
				}
			}

			mIdle[SelFac] = TempCTime[SelFac];
			CTimeofJob[Job] = TempCTime[SelFac];
			if (CTimeofJob[Job][this->m_Machines - 1] > RTimeofSet[CurSet])
				RTimeofSet[CurSet] = CTimeofJob[Job][this->m_Machines - 1];
		}

		CTimeofSet[CurSet] = max(mIdle_Assemble, RTimeofSet[CurSet]) + this->m_PTimeofSet[CurSet];
		mIdle_Assemble = CTimeofSet[CurSet];
	}
	int TF = accumulate(CTimeofSet.begin(), CTimeofSet.end(), 0);
	return TF;
}

int NOperator::GetTF_Gantt(vector <int> SeqofSet, vector<vector<int>> SeqofJobInEachSet, string FileName) //Assignment rule: minimize completion time of each job; Forward Calculation
{
	vector <vector <int>> mIdle(this->m_Factories), STimeofJob(this->m_Jobs), CTimeofJob(this->m_Jobs), TempCTime(this->m_Factories);
	for (int i = 0; i < this->m_Factories; i++)
		mIdle[i].resize(this->m_Machines, 0);
	TempCTime = mIdle;
	for (int i = 0; i < this->m_Jobs; i++)
		CTimeofJob[i].resize(this->m_Machines, 0);
	STimeofJob = CTimeofJob;
	vector <int> CTimeofSet(this->m_Sets, 0), RTimeofSet(this->m_Sets, 0), AssignedFac(this->m_Jobs);
	int mIdle_Assemble = 0;

	for (int i = 0; i < SeqofSet.size(); i++)
	{
		int CurSet = SeqofSet[i];
		for (int j = 0; j < SeqofJobInEachSet[CurSet].size(); j++)
		{
			int Job = SeqofJobInEachSet[CurSet][j], minTempCTime = INT_MAX, SelFac = -1;
			for (int k = 0; k < this->m_Factories; k++)
			{
				TempCTime[k][0] = mIdle[k][0] + this->m_PTimeofJob[Job][0];
				for (int m = 1; m < this->m_Machines; m++)
					TempCTime[k][m] = max(TempCTime[k][m - 1], mIdle[k][m]) + this->m_PTimeofJob[Job][m];
				if (TempCTime[k][this->m_Machines - 1]<minTempCTime)
				{
					minTempCTime = TempCTime[k][this->m_Machines - 1];
					SelFac = k;
				}
			}

			mIdle[SelFac] = TempCTime[SelFac];
			CTimeofJob[Job] = TempCTime[SelFac];
			AssignedFac[Job] = SelFac;
			for (int m = 0; m < this->m_Machines; m++)
				STimeofJob[Job][m] = CTimeofJob[Job][m] - this->m_PTimeofJob[Job][m];

			if (CTimeofJob[Job][this->m_Machines - 1] > RTimeofSet[CurSet])
				RTimeofSet[CurSet] = CTimeofJob[Job][this->m_Machines - 1];
		}

		CTimeofSet[CurSet] = max(mIdle_Assemble, RTimeofSet[CurSet]) + this->m_PTimeofSet[CurSet];
		mIdle_Assemble = CTimeofSet[CurSet];
	}

	ofstream ofile;
	ofile.open(FileName, ios_base::ate);
	ofile << "figure" << endl;
	for (int Job = 0; Job < this->m_Jobs; Job++)
	{
		int Fac = AssignedFac[Job];
		for (int m = 0; m < this->m_Machines; m++)
		{
			int st = STimeofJob[Job][m];
			int et = CTimeofJob[Job][m];
			int w = et - st;
			int x = st;
			int y = (Fac*this->m_Machines + m) * 15;
			int h = 10;

			ofile << "rectangle('Position', [";
			ofile << x << "\t";
			ofile << y << "\t";
			ofile << w << "\t";
			ofile << h;
			ofile << "]);";

			ofile << "text(" << x + w / 2 << "," << y + 5 << "," << "'" << Job + 1 << "');";

		}
		ofile << endl;
	}

	for (int Set = 0; Set < this->m_Sets; Set++)
	{
		int st = CTimeofSet[Set] - this->m_PTimeofSet[Set];
		int et = CTimeofSet[Set];
		int w = et - st;
		int x = st;
		int y = this->m_Factories*this->m_Machines * 15;
		int h = 10;

		ofile << "rectangle('Position', [";
		ofile << x << "\t";
		ofile << y << "\t";
		ofile << w << "\t";
		ofile << h;
		ofile << "]);";
		ofile << "text(" << x + w / 2 << "," << y + 5 << "," << "'" << Set + 1 << "');";
	}
	ofile.close();
	int TF = accumulate(CTimeofSet.begin(), CTimeofSet.end(), 0);
	return TF;
}

void NOperator::CheckSol(vector <int> SeqofSet, vector<vector<int>> SeqofJobInEachSet, int TF)
{
	//Check Seq of Set;
	vector <bool> bSetArray(this->m_Sets, false);
	for (int i = 0; i < this->m_Sets; i++)
		bSetArray[SeqofSet[i]] = true;
	for (int i = 0; i < this->m_Sets; i++)
	{
		if (!bSetArray[i])
		{
			cout << "Set " << i << " is lost!" << endl;
			getchar();
			exit(0);
		}
	}

	//Check Jobs
	vector <bool> bJobArray(this->m_Jobs, false);
	for (int i = 0; i < this->m_Sets; i++)
	{
		int CurSet = SeqofSet[i];
		for (int j = 0; j < SeqofJobInEachSet[CurSet].size(); j++)
		{
			int Job = SeqofJobInEachSet[CurSet][j];
			bJobArray[Job] = true;
		}
	}

	for (int i = 0; i < this->m_Jobs; i++)
	{
		if (!bJobArray[i])
		{
			cout << "Job " << i << " is Error!" << endl;
			getchar();
			exit(0);
		}
	}

	//Check Jobs in Sets
	for (int i = 0; i < this->m_Sets; i++)
	{
		int CurSet = SeqofSet[i];
		for (int j = 0; j < SeqofJobInEachSet[CurSet].size(); j++)
		{
			int Job = SeqofJobInEachSet[CurSet][j];
			if (m_JobsofSet[CurSet].end() == find(m_JobsofSet[CurSet].begin(), m_JobsofSet[CurSet].end(), Job))
			{
				cout << "Job is not found in the Set:" << CurSet << endl;
				getchar();
				exit(0);
			}
		}
	}

	//----- Check Computation of total flowtime---------

	int TempTF= this->GetTF(SeqofSet, SeqofJobInEachSet);
	if (TempTF != TF)
	{
		cout << "Total flowtime is Error!" << TF << "\t" << TempTF<< endl;
		getchar();
		exit(0);
	}
}

void NOperator::GetBestSol(vector <int> &SeqofSets, vector<vector<int>> &SeqofJobsInEachSet, int &TF)
{
	SeqofSets = this->m_bestSeqofSet;
	SeqofJobsInEachSet = this->m_bestJobSeqofEachSet;
	TF = this->m_bestTF;
}

void NOperator::GetRTimeofSet(vector <vector<int>> CTimeofJobs, vector<int> &RTimeofSet) //Compute Release time of Sets according to the completion time of jobs
{
	for (int i = 0; i < this->m_Sets; i++)
	{
		for (int j = 0; j < this->m_JobsofSet[i].size(); j++)
		{
			int Job = this->m_JobsofSet[i][j];
			if (CTimeofJobs[Job][this->m_Machines - 1] > RTimeofSet[i])
				RTimeofSet[i] = CTimeofJobs[Job][this->m_Machines - 1];
		}
	}
}

int NOperator::GetCTimeofSet(vector<int> SeqofSets, vector<int> RTimeofSet, vector <int> &CTimeofSet) //Compute Completion time of Sets
{
	int mIdle = 0;
	for (int i = 0; i < SeqofSets.size(); i++)
	{
		int Set = SeqofSets[i];
		CTimeofSet[Set] = max(mIdle, RTimeofSet[Set]) + this->m_PTimeofSet[Set];
		mIdle = CTimeofSet[Set];
	}
	return mIdle;
}
