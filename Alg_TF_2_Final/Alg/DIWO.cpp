#include "stdafx.h"
#include "DIWO.h"


DIWO::DIWO()
{
}

DIWO::~DIWO()
{
}

void DIWO::SetParameters(int PS0, int PSmax, int Smax, int Smin, int PLen,long TimeLimit)
{
	this->m_PSize0 = PS0;
	this->m_PSizemax = PSmax;
	this->m_Smax = Smax;
	this->m_Smin = Smin;
	this->m_PerLen = PLen;
	this->m_TimeLimit = TimeLimit;

}

void DIWO::Insert_JobSeqInSet(vector<vector<int>> & SeqofJob)
{
	int SetNo;
	do {
		SetNo = rand() % SeqofJob.size();
	} while (SeqofJob[SetNo].size() == 1);
	int pt0 = rand() % SeqofJob[SetNo].size();
	int pt1;
	do {
		pt1 = rand() % SeqofJob[SetNo].size();
	} while (pt1 == pt0);
	int Job = SeqofJob[SetNo][pt0];
	SeqofJob[SetNo].erase(SeqofJob[SetNo].begin() + pt0);
	SeqofJob[SetNo].insert(SeqofJob[SetNo].begin() + pt1, Job);
}

void DIWO::Swap_JobSeqInSet(vector<vector<int>> & SeqofJob)
{
	int SetNo;
	do {
		SetNo = rand() % SeqofJob.size();
	} while (SeqofJob[SetNo].size() == 1);
	int pt0 = rand() % SeqofJob[SetNo].size();
	int pt1;
	do {
		pt1 = rand() % SeqofJob[SetNo].size();
	} while (pt1 == pt0);
	int Job = SeqofJob[SetNo][pt0];
	SeqofJob[SetNo][pt0] = SeqofJob[SetNo][pt1];
	SeqofJob[SetNo][pt1] = Job;
}

void DIWO::Insert_SetSeq(vector<int> & SeqofSet)
{
	int pt0 = rand() % SeqofSet.size();
	int pt1;
	do {
		pt1 = rand() % SeqofSet.size();
	} while (pt1 == pt0);
	int Set = SeqofSet[pt0];
	SeqofSet.erase(SeqofSet.begin() + pt0);
	SeqofSet.insert(SeqofSet.begin() + pt1, Set);
}

void DIWO::Swap_SetSeq(vector<int> & SeqofSet)
{
	int pt0 = rand() % SeqofSet.size();
	int pt1;
	do {
		pt1 = rand() % SeqofSet.size();
	} while (pt1 == pt0);
	int Set = SeqofSet[pt0];
	SeqofSet[pt0] = SeqofSet[pt1];
	SeqofSet[pt1] = Set;
}

int DIWO::LocalSearch_JobInSet(vector<int> SeqofSet, vector <vector <int>> & JobSeqInSet, int CurSet, int TF)
{
	vector <int> Seq = JobSeqInSet[CurSet];
	random_shuffle(Seq.begin(), Seq.end()); //for extracting jobs without replications

	int nCnt = 0, h = 0;
	while (nCnt < Seq.size())
	{
		int Job = Seq[h%Seq.size()];
		vector<int>::iterator it = find(JobSeqInSet[CurSet].begin(), JobSeqInSet[CurSet].end(), Job);
		int OrgPos = it - JobSeqInSet[CurSet].begin();
		JobSeqInSet[CurSet].erase(it);

		int bestTF = INT_MAX, bestPos = -1;
		for (int i = 0; i <= JobSeqInSet[CurSet].size(); i++)
		{
			if (i == OrgPos) continue;
			JobSeqInSet[CurSet].insert(JobSeqInSet[CurSet].begin() + i, Job);
			int TempSeedTF = GetTF(SeqofSet, JobSeqInSet);
			if (TempSeedTF < bestTF)
			{
				bestTF = TempSeedTF;
				bestPos = i;
			}
			JobSeqInSet[CurSet].erase(JobSeqInSet[CurSet].begin() + i);
		}

		if (bestTF < TF)
		{
			TF = bestTF;
			JobSeqInSet[CurSet].insert(JobSeqInSet[CurSet].begin() + bestPos, Job);
			nCnt = 0;
		}
		else
		{
			JobSeqInSet[CurSet].insert(JobSeqInSet[CurSet].begin() + OrgPos, Job);
			nCnt++;
		}
		h++;
	}

	return TF;
}

int DIWO::LocalSearch_JobInAllSets(vector<int> SeqofSet, vector <vector <int>> & JobSeqInSet, int TF)
{
	int Cnt = 0;
	do
	{
		int TempTF = LocalSearch_JobInSet(SeqofSet, JobSeqInSet, SeqofSet[Cnt], TF);
		if (TempTF < TF)
		{
			TF = TempTF;
			Cnt = 0;
		}
		else
			Cnt++;
	} while (Cnt < SeqofSet.size());
	return TF;
}

int DIWO::LocalSearch_Set(vector <int> &SeqofSets, vector<vector<int>> SeqofJobsinEachSet, int Set, int TF)
{

	vector<int>::iterator it = find(SeqofSets.begin(), SeqofSets.end(), Set);
	int OrgPos = it - SeqofSets.begin();
	SeqofSets.erase(SeqofSets.begin() + OrgPos);


	int minTF = INT_MAX,minPt=-1;
	for (int i = 0; i <= SeqofSets.size(); i++)
	{
		if (i == OrgPos) continue;
		SeqofSets.insert(SeqofSets.begin() + i, Set);
		int TempTF = GetTF(SeqofSets, SeqofJobsinEachSet);
		if (TempTF < minTF)
		{
			minTF = TempTF;
			minPt = i;
		}
		SeqofSets.erase(SeqofSets.begin() + i);
	}


	if (minTF < TF)
	{
		TF = minTF;
		SeqofSets.insert(SeqofSets.begin() + minPt, Set);
	}
	else
		SeqofSets.insert(SeqofSets.begin() + OrgPos, Set);

	return TF;
}

int DIWO::LocalSearch_AllSets(vector <int> &SeqofSets, vector<vector<int>> SeqofJobsinEachSet, int TF)
{
	int Cnt = 0;
	while (Cnt < SeqofSets.size())
	{
		int CurSet = SeqofSets[Cnt];
		int TempTF = this->LocalSearch_Set(SeqofSets, SeqofJobsinEachSet, CurSet, TF);
		if (TempTF < TF)
		{
			TF = TempTF;
			Cnt = 0;
		}
		else
			Cnt++;
	}
	return TF;
}

int DIWO::LocalSearch(vector<int> &SeqofSets, vector <vector <int>> & SeqofJobsinEachSet, int TF) //for both sets and jobs
{
	int LastTF;

		LastTF = TF;
		TF = this->LocalSearch_AllSets(SeqofSets, SeqofJobsinEachSet, TF);

	return TF;
}

int DIWO::Evolution1(double alpha = 0.9) //first 'Set' based search then 'Job' based search
{
	long InitTime = ::GetTickCount();

	this->m_PSize = this->m_PSize0;
	this->m_SeqofSets.clear();
	this->m_JobSeqInSets.clear();
	this->m_TFArray.clear();
	this->m_SeqofSets.resize(this->m_PSize);
	this->m_JobSeqInSets.resize(this->m_PSize);
	this->m_TFArray.resize(this->m_PSize);


	int nHeu = 2;
	if (this->m_PSize < nHeu)
		nHeu = this->m_PSize;
	this->m_TFArray[0] = this->HR1(m_SeqofSets[0], m_JobSeqInSets[0]);
	if (this->m_PSize0>1)
		this->m_TFArray[1] = this->HR2(m_SeqofSets[1], m_JobSeqInSets[1]);


	for (int i = nHeu; i < this->m_PSize; i++)
	{
		int r = rand() % 2;
		this->m_SeqofSets[i] = m_SeqofSets[r];
		this->m_JobSeqInSets[i] = m_JobSeqInSets[r];
		this->Insert_SetSeq(this->m_SeqofSets[i]);
		this->Insert_SetSeq(this->m_SeqofSets[i]);
		this->Insert_JobSeqInSet(this->m_JobSeqInSets[i]);
		this->Insert_JobSeqInSet(this->m_JobSeqInSets[i]);
		this->m_TFArray[i] = GetTF(this->m_SeqofSets[i], this->m_JobSeqInSets[i]);
	}


	vector<vector<int>> SeqofSets = this->m_SeqofSets;
	vector<vector<vector<int>>> JobSeqInSet = this->m_JobSeqInSets;
	vector<int> TFArray = this->m_TFArray;

	Pair <int> *ch = new Pair <int>[this->m_PSize];
	for (int i = 0; i < this->m_PSize; i++)
	{
		ch[i].dim = i;
		ch[i].value = TFArray[i];
	}
	sort(ch, ch + this->m_PSize, PairLess<int>());
	for (int i = 0; i < this->m_PSize; i++)
	{
		this->m_SeqofSets[i] = SeqofSets[ch[i].dim];
		this->m_JobSeqInSets[i] = JobSeqInSet[ch[i].dim];
		this->m_TFArray[i] = TFArray[ch[i].dim];
	}
	delete[]ch;


	this->m_TFArray[0] = this->LocalSearch_AllSets(this->m_SeqofSets[0], this->m_JobSeqInSets[0], this->m_TFArray[0]);
	this->InitBestSol(this->m_SeqofSets[0], this->m_JobSeqInSets[0], this->m_TFArray[0]);



	int Gen = 1;
	while (::GetTickCount() - InitTime < alpha * m_TimeLimit)
	{
		int minTF = m_TFArray[0];
		int maxTF = m_TFArray[this->m_PSize-1];


		vector <int> pSeeds(m_TFArray.size(), 0);
		for (int i = 0; i < pSeeds.size(); i++)
		{
			if (minTF != maxTF)
				pSeeds[i] = (m_Smax - m_Smin)*(maxTF - m_TFArray[i]) / (maxTF - minTF) + m_Smin;
			else
				pSeeds[i] = (m_Smin+ m_Smax)/2;
		}


		vector<vector<vector<int>>> pSeedJobSeqInSets;
		vector <int> pSeedTF;
		vector<int> CurSetSeq, WeedSeqofSets;
		vector<vector<int>> CurJobSeqInSets, WeedJobSeqInSets;
		int WeedTF;
		for (int i = 0; i < this->m_PSize; i++)
		{
			WeedSeqofSets = this->m_SeqofSets[i];
			WeedJobSeqInSets = this->m_JobSeqInSets[i];
			WeedTF = this->m_TFArray[i];
			for (int nCnt = 0; nCnt < pSeeds[i]; nCnt++)
			{
				CurSetSeq = WeedSeqofSets;
				CurJobSeqInSets = WeedJobSeqInSets;
				int TF = WeedTF;

				for(int k=0;k<this->m_PerLen;k++)
					Insert_SetSeq(CurSetSeq);
				TF = GetTF(CurSetSeq, CurJobSeqInSets);

				pSeedSetSeq.push_back(CurSetSeq);
				pSeedJobSeqInSets.push_back(CurJobSeqInSets);
				pSeedTF.push_back(TF);

				if (TF < WeedTF)
				{
					WeedTF = TF;
					WeedSeqofSets = CurSetSeq;
					WeedJobSeqInSets = CurJobSeqInSets;
				}
			}
		}


		int bestpt;
		minTF = INT_MAX;
		for (int i = 0; i < pSeedTF.size(); i++)
		{
			if (pSeedTF[i] < minTF)
			{
				minTF = pSeedTF[i];
				bestpt = i;
			}
		}
		pSeedTF[bestpt] = this->LocalSearch_AllSets(pSeedSetSeq[bestpt], pSeedJobSeqInSets[bestpt], pSeedTF[bestpt]);


		pSeedTF.insert(pSeedTF.end(), this->m_TFArray.begin(), this->m_TFArray.end());
		pSeedSetSeq.insert(pSeedSetSeq.end(), this->m_SeqofSets.begin(), this->m_SeqofSets.end());
		pSeedJobSeqInSets.insert(pSeedJobSeqInSets.end(), this->m_JobSeqInSets.begin(), this->m_JobSeqInSets.end());


		Pair <int> *ch = new Pair <int>[pSeedTF.size()];
		for (int i = 0; i < pSeedTF.size(); i++)
		{
			ch[i].dim = i;
			ch[i].value = pSeedTF[i];
		}
		sort(ch, ch + pSeedTF.size(), PairLess<int>());


		for (int i = 0; i < this->m_PSize; i++)
		{
			m_SeqofSets[i] = pSeedSetSeq[ch[i].dim];
			m_JobSeqInSets[i] = pSeedJobSeqInSets[ch[i].dim];
			m_TFArray[i] = pSeedTF[ch[i].dim];
		}

		if (this->m_PSize < this->m_PSizemax)
		{
			int PSize = min(this->m_PSizemax, pSeedTF.size());
			for (int i = this->m_PSize; i < PSize; i++)
			{
				m_SeqofSets.push_back(pSeedSetSeq[ch[i].dim]);
				m_JobSeqInSets.push_back(pSeedJobSeqInSets[ch[i].dim]);
				m_TFArray.push_back(pSeedTF[ch[i].dim]);
			}
			this->m_PSize = PSize;
		}

		delete[]ch;
		this->UpdateBestSol(m_SeqofSets[0], m_JobSeqInSets[0], m_TFArray[0]);


		Gen++;
	}



	m_TFArray[0] = this->LocalSearch_JobInAllSets(m_SeqofSets[0], m_JobSeqInSets[0], m_TFArray[0]);
	this->UpdateBestSol(m_SeqofSets[0], m_JobSeqInSets[0], m_TFArray[0]);
	Gen = 1;
	while (::GetTickCount() - InitTime < m_TimeLimit)
	{
		int minTF = m_TFArray[0];
		int maxTF = m_TFArray[this->m_PSize - 1];

		vector <int> pSeeds(m_TFArray.size(), 0);
		for (int i = 0; i < pSeeds.size(); i++)
		{
			if (minTF != maxTF)
				pSeeds[i] = (m_Smax - m_Smin)*(maxTF - m_TFArray[i]) / (maxTF - minTF) + m_Smin;
			else
				pSeeds[i] = (m_Smin + m_Smax) / 2;
		}

		vector<vector<int>> pSeedSetSeq;
		vector<vector<vector<int>>> pSeedJobSeqInSets;
		vector <int> pSeedTF;
		vector<int> CurSetSeq, WeedSeqofSets;
		vector<vector<int>> CurJobSeqInSets, WeedJobSeqInSets;
		int WeedTF;
		for (int i = 0; i < this->m_PSize; i++)
		{
			WeedSeqofSets = this->m_SeqofSets[i];
			WeedJobSeqInSets = this->m_JobSeqInSets[i];
			WeedTF = this->m_TFArray[i];

			for (int nCnt = 0; nCnt < pSeeds[i]; nCnt++)
			{
				CurSetSeq = WeedSeqofSets;
				CurJobSeqInSets = WeedJobSeqInSets;
				int TF = WeedTF;

				for(int k=0;k<this->m_PerLen;k++)
					Insert_JobSeqInSet(CurJobSeqInSets);
				TF = GetTF(CurSetSeq, CurJobSeqInSets);
				pSeedSetSeq.push_back(CurSetSeq);
				pSeedJobSeqInSets.push_back(CurJobSeqInSets);
				pSeedTF.push_back(TF);
				if (TF < WeedTF)
				{
					WeedTF = TF;
					WeedSeqofSets = CurSetSeq;
					WeedJobSeqInSets = CurJobSeqInSets;
				}
			}
		}

		int bestpt;
		minTF = INT_MAX;
		for (int i = 0; i < pSeedTF.size(); i++)
		{
			if (pSeedTF[i] < minTF)
			{
				minTF = pSeedTF[i];
				bestpt = i;
			}
		}


		pSeedTF[bestpt] = this->LocalSearch_JobInAllSets(pSeedSetSeq[bestpt], pSeedJobSeqInSets[bestpt], pSeedTF[bestpt]);


		pSeedTF.insert(pSeedTF.end(), this->m_TFArray.begin(), this->m_TFArray.end());
		pSeedSetSeq.insert(pSeedSetSeq.end(), this->m_SeqofSets.begin(), this->m_SeqofSets.end());
		pSeedJobSeqInSets.insert(pSeedJobSeqInSets.end(), this->m_JobSeqInSets.begin(), this->m_JobSeqInSets.end());


		Pair <int> *ch = new Pair <int>[pSeedTF.size()];
		for (int i = 0; i < pSeedTF.size(); i++)
		{
			ch[i].dim = i;
			ch[i].value = pSeedTF[i];
		}
		sort(ch, ch + pSeedTF.size(), PairLess<int>());


		for (int i = 0; i < this->m_PSize; i++)
		{
			m_SeqofSets[i] = pSeedSetSeq[ch[i].dim];
			m_JobSeqInSets[i] = pSeedJobSeqInSets[ch[i].dim];
			m_TFArray[i] = pSeedTF[ch[i].dim];
		}

		if (this->m_PSize < this->m_PSizemax)
		{
			int PSize = min(this->m_PSizemax, pSeedTF.size());
			for (int i = this->m_PSize; i < PSize; i++)
			{
				m_SeqofSets.push_back(pSeedSetSeq[ch[i].dim]);
				m_JobSeqInSets.push_back(pSeedJobSeqInSets[ch[i].dim]);
				m_TFArray.push_back(pSeedTF[ch[i].dim]);
			}
			this->m_PSize = PSize;
		}

		delete[]ch;

		this->UpdateBestSol(m_SeqofSets[0], m_JobSeqInSets[0], m_TFArray[0]);

		Gen++;
	}

	return this->m_bestTF;
}

int DIWO::Evolution1(double alpha, vector<int> &MetaTFArray, vector<int> &ElapsedTimeArray)
{
	long InitTime = ::GetTickCount();

	this->m_PSize = this->m_PSize0;
	this->m_SeqofSets.clear();
	this->m_JobSeqInSets.clear();
	this->m_TFArray.clear();
	this->m_SeqofSets.resize(this->m_PSize);
	this->m_JobSeqInSets.resize(this->m_PSize);
	this->m_TFArray.resize(this->m_PSize);

	int nHeu = 2;
	if (this->m_PSize < nHeu)
		nHeu = this->m_PSize;
	this->m_TFArray[0] = this->HR1(m_SeqofSets[0], m_JobSeqInSets[0]);
	if (this->m_PSize0>1)
		this->m_TFArray[1] = this->HR2(m_SeqofSets[1], m_JobSeqInSets[1]);

	for (int i = nHeu; i < this->m_PSize; i++)
	{
		int r = rand() % 2;
		this->m_SeqofSets[i] = m_SeqofSets[r];
		this->m_JobSeqInSets[i] = m_JobSeqInSets[r];
		this->Insert_SetSeq(this->m_SeqofSets[i]);
		this->Insert_SetSeq(this->m_SeqofSets[i]);
		this->Insert_JobSeqInSet(this->m_JobSeqInSets[i]);
		this->Insert_JobSeqInSet(this->m_JobSeqInSets[i]);
		this->m_TFArray[i] = GetTF(this->m_SeqofSets[i], this->m_JobSeqInSets[i]);
	}


	vector<vector<int>> SeqofSets = this->m_SeqofSets;
	vector<vector<vector<int>>> JobSeqInSet = this->m_JobSeqInSets;
	vector<int> TFArray = this->m_TFArray;

	Pair <int> *ch = new Pair <int>[this->m_PSize];
	for (int i = 0; i < this->m_PSize; i++)
	{
		ch[i].dim = i;
		ch[i].value = TFArray[i];
	}
	sort(ch, ch + this->m_PSize, PairLess<int>());
	for (int i = 0; i < this->m_PSize; i++)
	{
		this->m_SeqofSets[i] = SeqofSets[ch[i].dim];
		this->m_JobSeqInSets[i] = JobSeqInSet[ch[i].dim];
		this->m_TFArray[i] = TFArray[ch[i].dim];
	}
	delete[]ch;

	MetaTFArray.clear();
	ElapsedTimeArray.clear();
	MetaTFArray.push_back(m_TFArray[0]);
	ElapsedTimeArray.push_back(0);


	this->m_TFArray[0] = this->LocalSearch_AllSets(this->m_SeqofSets[0], this->m_JobSeqInSets[0], this->m_TFArray[0]);
	this->InitBestSol(this->m_SeqofSets[0], this->m_JobSeqInSets[0], this->m_TFArray[0]);



	MetaTFArray.push_back(this->m_bestTF);
	ElapsedTimeArray.push_back(::GetTickCount() - InitTime);

	int Gen = 1;
	while (::GetTickCount() - InitTime < alpha * m_TimeLimit)
	{
		int minTF = m_TFArray[0];
		int maxTF = m_TFArray[this->m_PSize - 1];

		vector <int> pSeeds(m_TFArray.size(), 0);
		for (int i = 0; i < pSeeds.size(); i++)
		{
			if (minTF != maxTF)
				pSeeds[i] = (m_Smax - m_Smin)*(maxTF - m_TFArray[i]) / (maxTF - minTF) + m_Smin;
			else
				pSeeds[i] = (m_Smin + m_Smax) / 2;
		}


		vector<vector<int>> pSeedSetSeq;
		vector<vector<vector<int>>> pSeedJobSeqInSets;
		vector <int> pSeedTF;
		vector<int> CurSetSeq, WeedSeqofSets;
		vector<vector<int>> CurJobSeqInSets, WeedJobSeqInSets;
		int WeedTF;
		for (int i = 0; i < this->m_PSize; i++)
		{
			WeedSeqofSets = this->m_SeqofSets[i];
			WeedJobSeqInSets = this->m_JobSeqInSets[i];
			WeedTF = this->m_TFArray[i];
			for (int nCnt = 0; nCnt < pSeeds[i]; nCnt++)
			{
				CurSetSeq = WeedSeqofSets;
				CurJobSeqInSets = WeedJobSeqInSets;
				int TF = WeedTF;

				for (int k = 0; k<this->m_PerLen; k++)
					Insert_SetSeq(CurSetSeq);
				TF = GetTF(CurSetSeq, CurJobSeqInSets);

				pSeedSetSeq.push_back(CurSetSeq);
				pSeedJobSeqInSets.push_back(CurJobSeqInSets);
				pSeedTF.push_back(TF);

				if (TF < WeedTF)
				{
					WeedTF = TF;
					WeedSeqofSets = CurSetSeq;
					WeedJobSeqInSets = CurJobSeqInSets;
				}
			}
		}

		int bestpt;
		minTF = INT_MAX;
		for (int i = 0; i < pSeedTF.size(); i++)
		{
			if (pSeedTF[i] < minTF)
			{
				minTF = pSeedTF[i];
				bestpt = i;
			}
		}
		pSeedTF[bestpt] = this->LocalSearch_AllSets(pSeedSetSeq[bestpt], pSeedJobSeqInSets[bestpt], pSeedTF[bestpt]);


		pSeedTF.insert(pSeedTF.end(), this->m_TFArray.begin(), this->m_TFArray.end());
		pSeedSetSeq.insert(pSeedSetSeq.end(), this->m_SeqofSets.begin(), this->m_SeqofSets.end());
		pSeedJobSeqInSets.insert(pSeedJobSeqInSets.end(), this->m_JobSeqInSets.begin(), this->m_JobSeqInSets.end());


		Pair <int> *ch = new Pair <int>[pSeedTF.size()];
		for (int i = 0; i < pSeedTF.size(); i++)
		{
			ch[i].dim = i;
			ch[i].value = pSeedTF[i];
		}
		sort(ch, ch + pSeedTF.size(), PairLess<int>());


		for (int i = 0; i < this->m_PSize; i++)
		{
			m_SeqofSets[i] = pSeedSetSeq[ch[i].dim];
			m_JobSeqInSets[i] = pSeedJobSeqInSets[ch[i].dim];
			m_TFArray[i] = pSeedTF[ch[i].dim];
		}

		if (this->m_PSize < this->m_PSizemax)
		{
			int PSize = min(this->m_PSizemax, pSeedTF.size());
			for (int i = this->m_PSize; i < PSize; i++)
			{
				m_SeqofSets.push_back(pSeedSetSeq[ch[i].dim]);
				m_JobSeqInSets.push_back(pSeedJobSeqInSets[ch[i].dim]);
				m_TFArray.push_back(pSeedTF[ch[i].dim]);
			}
			this->m_PSize = PSize;
		}

		delete[]ch;
		this->UpdateBestSol(m_SeqofSets[0], m_JobSeqInSets[0], m_TFArray[0]);


		MetaTFArray.push_back(this->m_bestTF);
		ElapsedTimeArray.push_back(min(::GetTickCount() - InitTime, m_TimeLimit));
		Gen++;
	}

	m_TFArray[0] = this->LocalSearch_JobInAllSets(m_SeqofSets[0], m_JobSeqInSets[0], m_TFArray[0]);
	this->UpdateBestSol(m_SeqofSets[0], m_JobSeqInSets[0], m_TFArray[0]);
	Gen = 1;

	while (::GetTickCount() - InitTime < m_TimeLimit)
	{
		int minTF = m_TFArray[0];
		int maxTF = m_TFArray[this->m_PSize - 1];

		vector <int> pSeeds(m_TFArray.size(), 0);
		for (int i = 0; i < pSeeds.size(); i++)
		{
			if (minTF != maxTF)
				pSeeds[i] = (m_Smax - m_Smin)*(maxTF - m_TFArray[i]) / (maxTF - minTF) + m_Smin;
			else
				pSeeds[i] = (m_Smin + m_Smax) / 2;
		}

		vector<vector<int>> pSeedSetSeq;
		vector<vector<vector<int>>> pSeedJobSeqInSets;
		vector <int> pSeedTF;
		vector<int> CurSetSeq, WeedSeqofSets;
		vector<vector<int>> CurJobSeqInSets, WeedJobSeqInSets;
		int WeedTF;
		for (int i = 0; i < this->m_PSize; i++)
		{
			WeedSeqofSets = this->m_SeqofSets[i];
			WeedJobSeqInSets = this->m_JobSeqInSets[i];
			WeedTF = this->m_TFArray[i];

			for (int nCnt = 0; nCnt < pSeeds[i]; nCnt++)
			{
				CurSetSeq = WeedSeqofSets;
				CurJobSeqInSets = WeedJobSeqInSets;
				int TF = WeedTF;

				for (int k = 0; k<this->m_PerLen; k++)
					Insert_JobSeqInSet(CurJobSeqInSets);
				TF = GetTF(CurSetSeq, CurJobSeqInSets);
				pSeedSetSeq.push_back(CurSetSeq);
				pSeedJobSeqInSets.push_back(CurJobSeqInSets);
				pSeedTF.push_back(TF);
				if (TF < WeedTF)
				{
					WeedTF = TF;
					WeedSeqofSets = CurSetSeq;
					WeedJobSeqInSets = CurJobSeqInSets;
				}
			}
		}

		int bestpt;
		minTF = INT_MAX;
		for (int i = 0; i < pSeedTF.size(); i++)
		{
			if (pSeedTF[i] < minTF)
			{
				minTF = pSeedTF[i];
				bestpt = i;
			}
		}

		pSeedTF[bestpt] = this->LocalSearch_JobInAllSets(pSeedSetSeq[bestpt], pSeedJobSeqInSets[bestpt], pSeedTF[bestpt]);

		pSeedTF.insert(pSeedTF.end(), this->m_TFArray.begin(), this->m_TFArray.end());
		pSeedSetSeq.insert(pSeedSetSeq.end(), this->m_SeqofSets.begin(), this->m_SeqofSets.end());
		pSeedJobSeqInSets.insert(pSeedJobSeqInSets.end(), this->m_JobSeqInSets.begin(), this->m_JobSeqInSets.end());


		Pair <int> *ch = new Pair <int>[pSeedTF.size()];
		for (int i = 0; i < pSeedTF.size(); i++)
		{
			ch[i].dim = i;
			ch[i].value = pSeedTF[i];
		}
		sort(ch, ch + pSeedTF.size(), PairLess<int>());


		for (int i = 0; i < this->m_PSize; i++)
		{
			m_SeqofSets[i] = pSeedSetSeq[ch[i].dim];
			m_JobSeqInSets[i] = pSeedJobSeqInSets[ch[i].dim];
			m_TFArray[i] = pSeedTF[ch[i].dim];
		}

		if (this->m_PSize < this->m_PSizemax)
		{
			int PSize = min(this->m_PSizemax, pSeedTF.size());
			for (int i = this->m_PSize; i < PSize; i++)
			{
				m_SeqofSets.push_back(pSeedSetSeq[ch[i].dim]);
				m_JobSeqInSets.push_back(pSeedJobSeqInSets[ch[i].dim]);
				m_TFArray.push_back(pSeedTF[ch[i].dim]);
			}
			this->m_PSize = PSize;
		}

		delete[]ch;

		this->UpdateBestSol(m_SeqofSets[0], m_JobSeqInSets[0], m_TFArray[0]);


		MetaTFArray.push_back(this->m_bestTF);
		ElapsedTimeArray.push_back(min(::GetTickCount() - InitTime, m_TimeLimit));

		Gen++;
	}

	return this->m_bestTF;
}

int DIWO::Evolution2()
{
	long InitTime = ::GetTickCount();
	this->m_PSize = this->m_PSize0;
	this->m_SeqofSets.clear();
	this->m_JobSeqInSets.clear();
	this->m_TFArray.clear();
	this->m_SeqofSets.resize(this->m_PSize);
	this->m_JobSeqInSets.resize(this->m_PSize);
	this->m_TFArray.resize(this->m_PSize);

	int nHeu = 2;
	if (this->m_PSize < nHeu)
		nHeu = this->m_PSize;
	this->m_TFArray[0] = this->HR1(m_SeqofSets[0], m_JobSeqInSets[0]);
	if (this->m_PSize>1)
		this->m_TFArray[1] = this->HR2(m_SeqofSets[1], m_JobSeqInSets[1]);


	for (int i = nHeu; i < this->m_PSize; i++)
	{
		int r = rand() % 2;
		this->m_SeqofSets[i] = m_SeqofSets[r];
		this->m_JobSeqInSets[i] = m_JobSeqInSets[r];
		this->Insert_SetSeq(this->m_SeqofSets[i]);
		this->Insert_SetSeq(this->m_SeqofSets[i]);
		this->Insert_JobSeqInSet(this->m_JobSeqInSets[i]);
		this->Insert_JobSeqInSet(this->m_JobSeqInSets[i]);
		this->m_TFArray[i] = GetTF(this->m_SeqofSets[i], this->m_JobSeqInSets[i]);
	}


	vector<vector<int>> SeqofSets = this->m_SeqofSets;
	vector<vector<vector<int>>> JobSeqInSet = this->m_JobSeqInSets;
	vector<int> TFArray = this->m_TFArray;
	Pair <int> *ch = new Pair <int>[TFArray.size()];
	for (int i = 0; i < TFArray.size(); i++)
	{
		ch[i].dim = i;
		ch[i].value = TFArray[i];
	}
	sort(ch, ch + TFArray.size(), PairLess<int>());
	for (int i = 0; i < this->m_PSize; i++)
	{
		this->m_SeqofSets[i] = SeqofSets[ch[i].dim];
		this->m_JobSeqInSets[i] = JobSeqInSet[ch[i].dim];
		this->m_TFArray[i] = TFArray[ch[i].dim];
	}
	delete[]ch;

	this->m_TFArray[0] = this->LocalSearch_AllSets(this->m_SeqofSets[0], this->m_JobSeqInSets[0], this->m_TFArray[0]);
	this->m_TFArray[0] = this->LocalSearch_JobInAllSets(this->m_SeqofSets[0], this->m_JobSeqInSets[0], this->m_TFArray[0]);
	this->InitBestSol(this->m_SeqofSets[0], this->m_JobSeqInSets[0], this->m_TFArray[0]);


	int Gen = 1;
	while (::GetTickCount() - InitTime < m_TimeLimit)
	{
		int minTF = m_TFArray[0];
		int maxTF = m_TFArray[this->m_PSize - 1];

		vector <int> pSeeds(m_TFArray.size(), 0);
		for (int i = 0; i < pSeeds.size(); i++)
		{
			if (minTF != maxTF)
				pSeeds[i] = (m_Smax - m_Smin)*(maxTF - m_TFArray[i]) / (maxTF - minTF) + m_Smin;
			else
				pSeeds[i] = (m_Smin + m_Smax) / 2;
		}

		vector<vector<int>> pSeedSetSeq;
		vector<vector<vector<int>>> pSeedJobSeqInSets;
		vector <int> pSeedTF;
		vector<int> CurSetSeq, WeedSeqofSets;
		vector<vector<int>> CurJobSeqInSets, WeedJobSeqInSets;
		int WeedTF;
		for (int i = 0; i < this->m_PSize; i++)
		{
			WeedSeqofSets = this->m_SeqofSets[i];
			WeedJobSeqInSets = this->m_JobSeqInSets[i];
			WeedTF = this->m_TFArray[i];

			for (int nCnt = 0; nCnt < pSeeds[i]; nCnt++)
			{
				CurSetSeq = WeedSeqofSets;
				CurJobSeqInSets = WeedJobSeqInSets;
				int TF = WeedTF;
				for (int k = 0; k < this->m_PerLen; k++)
				{
					Insert_SetSeq(CurSetSeq);
					Insert_JobSeqInSet(CurJobSeqInSets);
				}
				TF = GetTF(CurSetSeq, CurJobSeqInSets);
				pSeedSetSeq.push_back(CurSetSeq);
				pSeedJobSeqInSets.push_back(CurJobSeqInSets);
				pSeedTF.push_back(TF);
				if (TF < WeedTF)
				{
					WeedTF = TF;
					WeedSeqofSets = CurSetSeq;
					WeedJobSeqInSets = CurJobSeqInSets;
				}
			}
		}

		int bestpt;
		minTF = INT_MAX;
		for (int i = 0; i < pSeedTF.size(); i++)
		{
			if (pSeedTF[i] < minTF)
			{
				minTF = pSeedTF[i];
				bestpt = i;
			}
		}
		pSeedTF[bestpt] = this->LocalSearch_AllSets(pSeedSetSeq[bestpt], pSeedJobSeqInSets[bestpt], pSeedTF[bestpt]);
		pSeedTF[bestpt] = this->LocalSearch_JobInAllSets(pSeedSetSeq[bestpt], pSeedJobSeqInSets[bestpt], pSeedTF[bestpt]);

		pSeedTF.insert(pSeedTF.end(), this->m_TFArray.begin(), this->m_TFArray.end());
		pSeedSetSeq.insert(pSeedSetSeq.end(), this->m_SeqofSets.begin(), this->m_SeqofSets.end());
		pSeedJobSeqInSets.insert(pSeedJobSeqInSets.end(), this->m_JobSeqInSets.begin(), this->m_JobSeqInSets.end());

		Pair <int> *ch = new Pair <int>[pSeedTF.size()];
		for (int i = 0; i < pSeedTF.size(); i++)
		{
			ch[i].dim = i;
			ch[i].value = pSeedTF[i];
		}
		sort(ch, ch + pSeedTF.size(), PairLess<int>());

		for (int i = 0; i < this->m_PSize; i++)
		{
			m_SeqofSets[i] = pSeedSetSeq[ch[i].dim];
			m_JobSeqInSets[i] = pSeedJobSeqInSets[ch[i].dim];
			m_TFArray[i] = pSeedTF[ch[i].dim];
		}
		if (this->m_PSize < this->m_PSizemax)
		{
			int PSize = min(this->m_PSizemax, pSeedTF.size());
			for (int i = this->m_PSize; i < PSize; i++)
			{
				m_SeqofSets.push_back(pSeedSetSeq[ch[i].dim]);
				m_JobSeqInSets.push_back(pSeedJobSeqInSets[ch[i].dim]);
				m_TFArray.push_back(pSeedTF[ch[i].dim]);
			}
			this->m_PSize = PSize;
		}
		delete[]ch;

		this->UpdateBestSol(m_SeqofSets[0], m_JobSeqInSets[0], m_TFArray[0]);

		Gen++;
	}

	return this->m_bestTF;
}

int DIWO::Evolution2(vector<int> &MetaTFArray, vector<int> &ElapsedTimeArray) //Combine Set and Job local searches
{
	long InitTime = ::GetTickCount();
	this->m_PSize = this->m_PSize0;
	this->m_SeqofSets.clear();
	this->m_JobSeqInSets.clear();
	this->m_TFArray.clear();
	this->m_SeqofSets.resize(this->m_PSize);
	this->m_JobSeqInSets.resize(this->m_PSize);
	this->m_TFArray.resize(this->m_PSize);

	int nHeu = 2;
	if (this->m_PSize < nHeu)
		nHeu = this->m_PSize;
	this->m_TFArray[0] = this->HR1(m_SeqofSets[0], m_JobSeqInSets[0]);
	if (this->m_PSize>1)
		this->m_TFArray[1] = this->HR2(m_SeqofSets[1], m_JobSeqInSets[1]);

	for (int i = nHeu; i < this->m_PSize; i++)
	{
		int r = rand() % 2;
		this->m_SeqofSets[i] = m_SeqofSets[r];
		this->m_JobSeqInSets[i] = m_JobSeqInSets[r];
		this->Insert_SetSeq(this->m_SeqofSets[i]);
		this->Insert_SetSeq(this->m_SeqofSets[i]);
		this->Insert_JobSeqInSet(this->m_JobSeqInSets[i]);
		this->Insert_JobSeqInSet(this->m_JobSeqInSets[i]);
		this->m_TFArray[i] = GetTF(this->m_SeqofSets[i], this->m_JobSeqInSets[i]);
	}

	vector<vector<int>> SeqofSets = this->m_SeqofSets;
	vector<vector<vector<int>>> JobSeqInSet = this->m_JobSeqInSets;
	vector<int> TFArray = this->m_TFArray;
	Pair <int> *ch = new Pair <int>[TFArray.size()];
	for (int i = 0; i < TFArray.size(); i++)
	{
		ch[i].dim = i;
		ch[i].value = TFArray[i];
	}
	sort(ch, ch + TFArray.size(), PairLess<int>());
	for (int i = 0; i < this->m_PSize; i++)
	{
		this->m_SeqofSets[i] = SeqofSets[ch[i].dim];
		this->m_JobSeqInSets[i] = JobSeqInSet[ch[i].dim];
		this->m_TFArray[i] = TFArray[ch[i].dim];
	}
	delete[]ch;

	MetaTFArray.clear();
	ElapsedTimeArray.clear();
	MetaTFArray.push_back(m_TFArray[0]);
	ElapsedTimeArray.push_back(0);

	this->m_TFArray[0] = this->LocalSearch_AllSets(this->m_SeqofSets[0], this->m_JobSeqInSets[0], this->m_TFArray[0]);
	this->m_TFArray[0] = this->LocalSearch_JobInAllSets(this->m_SeqofSets[0], this->m_JobSeqInSets[0], this->m_TFArray[0]);
	this->InitBestSol(this->m_SeqofSets[0], this->m_JobSeqInSets[0], this->m_TFArray[0]);

	MetaTFArray.push_back(m_bestTF);
	ElapsedTimeArray.push_back(::GetTickCount() - InitTime);

	int Gen = 1;
	while (::GetTickCount() - InitTime < m_TimeLimit)
	{
		int minTF = m_TFArray[0];
		int maxTF = m_TFArray[this->m_PSize - 1];

		vector <int> pSeeds(m_TFArray.size(), 0);
		for (int i = 0; i < pSeeds.size(); i++)
		{
			if (minTF != maxTF)
				pSeeds[i] = (m_Smax - m_Smin)*(maxTF - m_TFArray[i]) / (maxTF - minTF) + m_Smin;
			else
				pSeeds[i] = (m_Smin + m_Smax) / 2;
		}

		vector<vector<int>> pSeedSetSeq;
		vector<vector<vector<int>>> pSeedJobSeqInSets;
		vector <int> pSeedTF;
		vector<int> CurSetSeq, WeedSeqofSets;
		vector<vector<int>> CurJobSeqInSets, WeedJobSeqInSets;
		int WeedTF;
		for (int i = 0; i < this->m_PSize; i++)
		{
			WeedSeqofSets = this->m_SeqofSets[i];
			WeedJobSeqInSets = this->m_JobSeqInSets[i];
			WeedTF = this->m_TFArray[i];

			for (int nCnt = 0; nCnt < pSeeds[i]; nCnt++)
			{
				CurSetSeq = WeedSeqofSets;
				CurJobSeqInSets = WeedJobSeqInSets;
				int TF = WeedTF;
				for (int k = 0; k < this->m_PerLen; k++)
				{
					Insert_SetSeq(CurSetSeq);
					Insert_JobSeqInSet(CurJobSeqInSets);
				}
				TF = GetTF(CurSetSeq, CurJobSeqInSets);
				pSeedSetSeq.push_back(CurSetSeq);
				pSeedJobSeqInSets.push_back(CurJobSeqInSets);
				pSeedTF.push_back(TF);
				if (TF < WeedTF)
				{
					WeedTF = TF;
					WeedSeqofSets = CurSetSeq;
					WeedJobSeqInSets = CurJobSeqInSets;
				}
			}
		}


		int bestpt;
		minTF = INT_MAX;
		for (int i = 0; i < pSeedTF.size(); i++)
		{
			if (pSeedTF[i] < minTF)
			{
				minTF = pSeedTF[i];
				bestpt = i;
			}
		}
		pSeedTF[bestpt] = this->LocalSearch_AllSets(pSeedSetSeq[bestpt], pSeedJobSeqInSets[bestpt], pSeedTF[bestpt]);
		pSeedTF[bestpt] = this->LocalSearch_JobInAllSets(pSeedSetSeq[bestpt], pSeedJobSeqInSets[bestpt], pSeedTF[bestpt]);


		pSeedTF.insert(pSeedTF.end(), this->m_TFArray.begin(), this->m_TFArray.end());
		pSeedSetSeq.insert(pSeedSetSeq.end(), this->m_SeqofSets.begin(), this->m_SeqofSets.end());
		pSeedJobSeqInSets.insert(pSeedJobSeqInSets.end(), this->m_JobSeqInSets.begin(), this->m_JobSeqInSets.end());


		Pair <int> *ch = new Pair <int>[pSeedTF.size()];
		for (int i = 0; i < pSeedTF.size(); i++)
		{
			ch[i].dim = i;
			ch[i].value = pSeedTF[i];
		}
		sort(ch, ch + pSeedTF.size(), PairLess<int>());


		for (int i = 0; i < this->m_PSize; i++)
		{
			m_SeqofSets[i] = pSeedSetSeq[ch[i].dim];
			m_JobSeqInSets[i] = pSeedJobSeqInSets[ch[i].dim];
			m_TFArray[i] = pSeedTF[ch[i].dim];
		}
		if (this->m_PSize < this->m_PSizemax)
		{
			int PSize = min(this->m_PSizemax, pSeedTF.size());
			for (int i = this->m_PSize; i < PSize; i++)
			{
				m_SeqofSets.push_back(pSeedSetSeq[ch[i].dim]);
				m_JobSeqInSets.push_back(pSeedJobSeqInSets[ch[i].dim]);
				m_TFArray.push_back(pSeedTF[ch[i].dim]);
			}
			this->m_PSize = PSize;
		}
		delete[]ch;

		this->UpdateBestSol(m_SeqofSets[0], m_JobSeqInSets[0], m_TFArray[0]);

		Gen++;

		MetaTFArray.push_back(m_bestTF);
		ElapsedTimeArray.push_back(min(::GetTickCount() - InitTime,m_TimeLimit));
	}

	return this->m_bestTF;
}

int DIWO::Evolution3(double Prop = 0.8)
{
	long InitTime = ::GetTickCount();
	this->m_PSize = this->m_PSize0;
	this->m_SeqofSets.clear();
	this->m_JobSeqInSets.clear();
	this->m_TFArray.clear();
	this->m_SeqofSets.resize(this->m_PSize);
	this->m_JobSeqInSets.resize(this->m_PSize);
	this->m_TFArray.resize(this->m_PSize);

	int nHeu = 2;
	if (this->m_PSize < nHeu)
		nHeu = this->m_PSize;
	this->m_TFArray[0] = this->HR1(m_SeqofSets[0], m_JobSeqInSets[0]);
	if (this->m_PSize>1)
		this->m_TFArray[1] = this->HR2(m_SeqofSets[1], m_JobSeqInSets[1]);

	for (int i = nHeu; i < this->m_PSize; i++)
	{
		int r = rand() % 2;
		this->m_SeqofSets[i] = m_SeqofSets[r];
		this->m_JobSeqInSets[i] = m_JobSeqInSets[r];
		this->Insert_SetSeq(this->m_SeqofSets[i]);
		this->Insert_SetSeq(this->m_SeqofSets[i]);
		this->Insert_JobSeqInSet(this->m_JobSeqInSets[i]);
		this->Insert_JobSeqInSet(this->m_JobSeqInSets[i]);
		this->m_TFArray[i] = GetTF(this->m_SeqofSets[i], this->m_JobSeqInSets[i]);
	}

	//Sort the solutions
	vector<vector<int>> SeqofSets = this->m_SeqofSets;
	vector<vector<vector<int>>> JobSeqInSet = this->m_JobSeqInSets;
	vector<int> TFArray = this->m_TFArray;
	Pair <int> *ch = new Pair <int>[TFArray.size()];
	for (int i = 0; i < TFArray.size(); i++)
	{
		ch[i].dim = i;
		ch[i].value = TFArray[i];
	}
	sort(ch, ch + TFArray.size(), PairLess<int>());
	for (int i = 0; i < this->m_PSize; i++)
	{
		this->m_SeqofSets[i] = SeqofSets[ch[i].dim];
		this->m_JobSeqInSets[i] = JobSeqInSet[ch[i].dim];
		this->m_TFArray[i] = TFArray[ch[i].dim];
	}
	delete[]ch;

	this->m_TFArray[0] = this->LocalSearch_AllSets(this->m_SeqofSets[0], this->m_JobSeqInSets[0], this->m_TFArray[0]);
	this->m_TFArray[0] = this->LocalSearch_JobInAllSets(this->m_SeqofSets[0], this->m_JobSeqInSets[0], this->m_TFArray[0]);
	this->InitBestSol(this->m_SeqofSets[0], this->m_JobSeqInSets[0], this->m_TFArray[0]);


	int Gen = 1;
	while (::GetTickCount() - InitTime < m_TimeLimit)
	{
		int minTF = m_TFArray[0];
		int maxTF = m_TFArray[this->m_PSize - 1];


		vector <int> pSeeds(m_TFArray.size(), 0);
		for (int i = 0; i < pSeeds.size(); i++)
		{
			if (minTF != maxTF)
				pSeeds[i] = (m_Smax - m_Smin)*(maxTF - m_TFArray[i]) / (maxTF - minTF) + m_Smin;
			else
				pSeeds[i] = (m_Smin + m_Smax) / 2;
		}


		vector<vector<int>> pSeedSetSeq;
		vector<vector<vector<int>>> pSeedJobSeqInSets;
		vector <int> pSeedTF;

		vector<int> CurSetSeq, WeedSeqofSets;
		vector<vector<int>> CurJobSeqInSets, WeedJobSeqInSets;
		int WeedTF;
		for (int i = 0; i < this->m_PSize; i++)
		{
			WeedSeqofSets = this->m_SeqofSets[i];
			WeedJobSeqInSets = this->m_JobSeqInSets[i];
			WeedTF = this->m_TFArray[i];

			for (int nCnt = 0; nCnt < pSeeds[i]; nCnt++)
			{
				CurSetSeq = WeedSeqofSets;
				CurJobSeqInSets = WeedJobSeqInSets;
				int TF = WeedTF;

				double r = rand() / (double)RAND_MAX;
				if (r < Prop)
				{
					for(int k=0;k<this->m_PerLen;k++)
						Insert_SetSeq(CurSetSeq);
				}
				else
				{
					for(int k=0;k<this->m_PerLen;k++)
						Insert_JobSeqInSet(CurJobSeqInSets);
				}
				TF = GetTF(CurSetSeq, CurJobSeqInSets);

				pSeedSetSeq.push_back(CurSetSeq);
				pSeedJobSeqInSets.push_back(CurJobSeqInSets);
				pSeedTF.push_back(TF);

				if (TF < WeedTF)
				{
					WeedTF = TF;
					WeedSeqofSets = CurSetSeq;
					WeedJobSeqInSets = CurJobSeqInSets;
				}
			}
		}

		int bestpt;
		minTF = INT_MAX;
		for (int i = 0; i < pSeedTF.size(); i++)
		{
			if (pSeedTF[i] < minTF)
			{
				minTF = pSeedTF[i];
				bestpt = i;
			}
		}

		double r = rand() / (double)RAND_MAX;
		if (r < Prop)
			pSeedTF[bestpt] = this->LocalSearch_AllSets(pSeedSetSeq[bestpt], pSeedJobSeqInSets[bestpt], pSeedTF[bestpt]);
		else
			pSeedTF[bestpt] = this->LocalSearch_JobInAllSets(pSeedSetSeq[bestpt], pSeedJobSeqInSets[bestpt], pSeedTF[bestpt]);


		pSeedTF.insert(pSeedTF.end(), this->m_TFArray.begin(), this->m_TFArray.end());
		pSeedSetSeq.insert(pSeedSetSeq.end(), this->m_SeqofSets.begin(), this->m_SeqofSets.end());
		pSeedJobSeqInSets.insert(pSeedJobSeqInSets.end(), this->m_JobSeqInSets.begin(), this->m_JobSeqInSets.end());


		Pair <int> *ch = new Pair <int>[pSeedTF.size()];
		for (int i = 0; i < pSeedTF.size(); i++)
		{
			ch[i].dim = i;
			ch[i].value = pSeedTF[i];
		}
		sort(ch, ch + pSeedTF.size(), PairLess<int>());


		for (int i = 0; i < this->m_PSize; i++)
		{
			m_SeqofSets[i] = pSeedSetSeq[ch[i].dim];
			m_JobSeqInSets[i] = pSeedJobSeqInSets[ch[i].dim];
			m_TFArray[i] = pSeedTF[ch[i].dim];
		}

		if (this->m_PSize < this->m_PSizemax)
		{
			int PSize = min(this->m_PSizemax, pSeedTF.size());
			for (int i = this->m_PSize; i < PSize; i++)
			{
				m_SeqofSets.push_back(pSeedSetSeq[ch[i].dim]);
				m_JobSeqInSets.push_back(pSeedJobSeqInSets[ch[i].dim]);
				m_TFArray.push_back(pSeedTF[ch[i].dim]);
			}
			this->m_PSize = PSize;
		}
		delete[]ch;

		this->UpdateBestSol(m_SeqofSets[0], m_JobSeqInSets[0], m_TFArray[0]);

		Gen++;
	}

	return this->m_bestTF;
}

int DIWO::Evolution3(double Prop, vector<int> &MetaTFArray, vector<int> &ElapsedTimeArray) //Serach Set and Job randomly
{
	long InitTime = ::GetTickCount();
	this->m_PSize = this->m_PSize0;
	this->m_SeqofSets.clear();
	this->m_JobSeqInSets.clear();
	this->m_TFArray.clear();
	this->m_SeqofSets.resize(this->m_PSize);
	this->m_JobSeqInSets.resize(this->m_PSize);
	this->m_TFArray.resize(this->m_PSize);


	int nHeu = 2;
	if (this->m_PSize < nHeu)
		nHeu = this->m_PSize;
	this->m_TFArray[0] = this->HR1(m_SeqofSets[0], m_JobSeqInSets[0]);
	if (this->m_PSize>1)
		this->m_TFArray[1] = this->HR2(m_SeqofSets[1], m_JobSeqInSets[1]);

	for (int i = nHeu; i < this->m_PSize; i++)
	{
		int r = rand() % 2;
		this->m_SeqofSets[i] = m_SeqofSets[r];
		this->m_JobSeqInSets[i] = m_JobSeqInSets[r];
		this->Insert_SetSeq(this->m_SeqofSets[i]);
		this->Insert_SetSeq(this->m_SeqofSets[i]);
		this->Insert_JobSeqInSet(this->m_JobSeqInSets[i]);
		this->Insert_JobSeqInSet(this->m_JobSeqInSets[i]);
		this->m_TFArray[i] = GetTF(this->m_SeqofSets[i], this->m_JobSeqInSets[i]);
	}


	vector<vector<int>> SeqofSets = this->m_SeqofSets;
	vector<vector<vector<int>>> JobSeqInSet = this->m_JobSeqInSets;
	vector<int> TFArray = this->m_TFArray;
	Pair <int> *ch = new Pair <int>[TFArray.size()];
	for (int i = 0; i < TFArray.size(); i++)
	{
		ch[i].dim = i;
		ch[i].value = TFArray[i];
	}
	sort(ch, ch + TFArray.size(), PairLess<int>());
	for (int i = 0; i < this->m_PSize; i++)
	{
		this->m_SeqofSets[i] = SeqofSets[ch[i].dim];
		this->m_JobSeqInSets[i] = JobSeqInSet[ch[i].dim];
		this->m_TFArray[i] = TFArray[ch[i].dim];
	}
	delete[]ch;

	MetaTFArray.clear();
	ElapsedTimeArray.clear();
	MetaTFArray.push_back(m_TFArray[0]);
	ElapsedTimeArray.push_back(0);


	this->m_TFArray[0] = this->LocalSearch_AllSets(this->m_SeqofSets[0], this->m_JobSeqInSets[0], this->m_TFArray[0]);
	this->m_TFArray[0] = this->LocalSearch_JobInAllSets(this->m_SeqofSets[0], this->m_JobSeqInSets[0], this->m_TFArray[0]);
	this->InitBestSol(this->m_SeqofSets[0], this->m_JobSeqInSets[0], this->m_TFArray[0]);

	MetaTFArray.push_back(m_bestTF);
	ElapsedTimeArray.push_back(::GetTickCount() - InitTime);


	int Gen = 1;
	while (::GetTickCount() - InitTime < m_TimeLimit)
	{
		int minTF = m_TFArray[0];
		int maxTF = m_TFArray[this->m_PSize - 1];


		vector <int> pSeeds(m_TFArray.size(), 0);
		for (int i = 0; i < pSeeds.size(); i++)
		{
			if (minTF != maxTF)
				pSeeds[i] = (m_Smax - m_Smin)*(maxTF - m_TFArray[i]) / (maxTF - minTF) + m_Smin;
			else
				pSeeds[i] = (m_Smin + m_Smax) / 2;
		}

		vector<vector<int>> pSeedSetSeq;
		vector<vector<vector<int>>> pSeedJobSeqInSets;
		vector <int> pSeedTF;

		vector<int> CurSetSeq, WeedSeqofSets;
		vector<vector<int>> CurJobSeqInSets, WeedJobSeqInSets;
		int WeedTF;
		for (int i = 0; i < this->m_PSize; i++)
		{
			WeedSeqofSets = this->m_SeqofSets[i];
			WeedJobSeqInSets = this->m_JobSeqInSets[i];
			WeedTF = this->m_TFArray[i];

			for (int nCnt = 0; nCnt < pSeeds[i]; nCnt++)
			{
				CurSetSeq = WeedSeqofSets;
				CurJobSeqInSets = WeedJobSeqInSets;
				int TF = WeedTF;

				double r = rand() / (double)RAND_MAX;
				if (r < Prop)
				{
					for (int k = 0; k<this->m_PerLen; k++)
						Insert_SetSeq(CurSetSeq);
				}
				else
				{
					for (int k = 0; k<this->m_PerLen; k++)
						Insert_JobSeqInSet(CurJobSeqInSets);
				}
				TF = GetTF(CurSetSeq, CurJobSeqInSets);

				pSeedSetSeq.push_back(CurSetSeq);
				pSeedJobSeqInSets.push_back(CurJobSeqInSets);
				pSeedTF.push_back(TF);

				if (TF < WeedTF)
				{
					WeedTF = TF;
					WeedSeqofSets = CurSetSeq;
					WeedJobSeqInSets = CurJobSeqInSets;
				}
			}
		}

		int bestpt;
		minTF = INT_MAX;
		for (int i = 0; i < pSeedTF.size(); i++)
		{
			if (pSeedTF[i] < minTF)
			{
				minTF = pSeedTF[i];
				bestpt = i;
			}
		}

		double r = rand() / (double)RAND_MAX;
		if (r < Prop)
			pSeedTF[bestpt] = this->LocalSearch_AllSets(pSeedSetSeq[bestpt], pSeedJobSeqInSets[bestpt], pSeedTF[bestpt]);
		else
			pSeedTF[bestpt] = this->LocalSearch_JobInAllSets(pSeedSetSeq[bestpt], pSeedJobSeqInSets[bestpt], pSeedTF[bestpt]);

		pSeedTF.insert(pSeedTF.end(), this->m_TFArray.begin(), this->m_TFArray.end());
		pSeedSetSeq.insert(pSeedSetSeq.end(), this->m_SeqofSets.begin(), this->m_SeqofSets.end());
		pSeedJobSeqInSets.insert(pSeedJobSeqInSets.end(), this->m_JobSeqInSets.begin(), this->m_JobSeqInSets.end());

		Pair <int> *ch = new Pair <int>[pSeedTF.size()];
		for (int i = 0; i < pSeedTF.size(); i++)
		{
			ch[i].dim = i;
			ch[i].value = pSeedTF[i];
		}
		sort(ch, ch + pSeedTF.size(), PairLess<int>());


		for (int i = 0; i < this->m_PSize; i++)
		{
			m_SeqofSets[i] = pSeedSetSeq[ch[i].dim];
			m_JobSeqInSets[i] = pSeedJobSeqInSets[ch[i].dim];
			m_TFArray[i] = pSeedTF[ch[i].dim];
		}

		if (this->m_PSize < this->m_PSizemax)
		{
			int PSize = min(this->m_PSizemax, pSeedTF.size());
			for (int i = this->m_PSize; i < PSize; i++)
			{
				m_SeqofSets.push_back(pSeedSetSeq[ch[i].dim]);
				m_JobSeqInSets.push_back(pSeedJobSeqInSets[ch[i].dim]);
				m_TFArray.push_back(pSeedTF[ch[i].dim]);
			}
			this->m_PSize = PSize;
		}
		delete[]ch;

		this->UpdateBestSol(m_SeqofSets[0], m_JobSeqInSets[0], m_TFArray[0]);

		Gen++;
		MetaTFArray.push_back(m_bestTF);
		ElapsedTimeArray.push_back(min(::GetTickCount() - InitTime, m_TimeLimit));
	}

	return this->m_bestTF;
}
