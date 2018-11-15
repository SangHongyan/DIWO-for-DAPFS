//Base.cpp
#include "stdafx.h"
#include "Base.h"

void  Base::GetInstances(int &Jobs, int &Machines, int &Factories, vector<vector<int>> &JobPTime, int &Sets, vector <int> &SetPTime, vector <int> &JobinSet, string FileName)
{
	ifstream infile;
	infile.open(FileName);
	if (!infile)
	{
		cout << "open instance file is error!\t" << FileName;
		getchar();
		exit(0);
	}

	infile >> Jobs >> Machines >> Factories;

	JobPTime.resize(Jobs);
	for (int j = 0; j < Jobs; j++)
		JobPTime[j].resize(Machines);
	for (int j = 0; j < Jobs; j++)
	for (int m = 0; m < Machines; m++)
		infile >> JobPTime[j][m] >> JobPTime[j][m];

	string str;
	infile >> str;
	infile >> Sets;
	infile >> str;
	SetPTime.resize(Sets);
	for (int j = 0; j < Sets; j++)
		infile >> SetPTime[j] >> SetPTime[j];
	infile >> str;
	JobinSet.resize(Jobs);
	for (int j = 0; j < Jobs; j++)
		infile >> JobinSet[j] >> JobinSet[j];

	infile.close();
}

void Base::GenerateInstances(int Jobs, int Machines, int Factories, int Set, string FileName, int Seeds)
{
	srand(Seeds);
	ofstream ofile;
	ofile.open(FileName);
	ofile << Jobs << "\t" << Machines << endl;
	ofile << Factories << endl;
	for (int j = 0; j < Jobs; j++)
	{
		for (int m = 0; m < Machines; m++)
		{
			ofile << m << "\t" << rand() % 99 + 1 << "\t";
		}
		ofile << endl;
	}

	ofile << "NumAssemblySet" << endl;
	ofile << Set << endl;
	vector <int> JobofSet(Jobs);
	for (int j = 0; j < Jobs; j++)
		JobofSet[j] = j%Set;
	random_shuffle(JobofSet.begin(), JobofSet.end());

	vector<int> Num_Jobs(Set, 0);
	for (int j = 0; j < Jobs; j++)
		Num_Jobs[JobofSet[j]]++;

	ofile << "ProTimeAssemblySet" << endl;
	for (int j = 0; j < Set; j++)
	{
		ofile << j + 1 << "\t" << rand() % (99 * Num_Jobs[j]) + Num_Jobs[j]<<endl;
	}

	ofile << "NumJob_NumSet" << endl;
	for (int j = 0; j < Jobs; j++)
	{
		ofile << j + 1 << "\t" << JobofSet[j] + 1 << endl;
	}
	ofile.close();
}

void Base::GenerateTestSet_Large()
{
	int JobArray[] = { 100,200,500 };
	int MachineArray[] = { 5,10,20 };
	int FactoryArray[] = { 4,6,8 };
	int SetArray[]={ 30,40,50 };
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			for(int k=0;k<3;k++)
				for (int l = 0; l < 3; l++)
				{
					for (int rep = 0; rep < 1; rep++)
					{
						ostringstream ostr;
						ostr << "I_" << JobArray[i] << "_" << MachineArray[j] << "_" << FactoryArray[k] << "_" << SetArray[l]<<"_" << rep + 1 << ".txt";
						GenerateInstances(JobArray[i], MachineArray[j], FactoryArray[k], SetArray[l], ostr.str(), rep);
					}
				}
}

void Base::GenerateTestSet_Small()
{
	int JobArray[] = { 8,12,16,20,24 };
	int MachineArray[] = { 2,3,4,5 };
	int FactoryArray[] = { 2,3,4 };
	int SetArray[] = { 2,3,4 };
	for (int i = 0; i<5; i++)
		for (int j = 0; j<4; j++)
			for (int k = 0; k<3; k++)
				for (int l = 0; l < 3; l++)
				{
					for (int rep = 0; rep < 1; rep++)
					{
						ostringstream ostr;
						ostr << "I_" << JobArray[i] << "_" << MachineArray[j] << "_" << FactoryArray[k] << "_" << SetArray[l] << "_" << rep + 1 << ".txt";
						GenerateInstances(JobArray[i], MachineArray[j], FactoryArray[k], SetArray[l], ostr.str(), rep);
					}
				}
}
