
#include "stdafx.h"
#include "Heuristic.h"
#include "DIWO.h"

void DIWO_EvType1_Ins(string InstanceDirectory, string InstanceFileName, string OutFileName, int RandSeeds, int CPUFactor, int PS0, int PSmax, int PLen, int Smax, double alpha)
{
	int Jobs;
	int Machines;
	int Factories;
	vector<vector<int>> JobPTime;
	int Sets;
	vector <int> SetPTime;
	vector <int> SetofJob;

	string FileName = InstanceDirectory + "\\" + InstanceFileName;
	GetInstances(Jobs, Machines, Factories, JobPTime, Sets, SetPTime, SetofJob, FileName);

	long TimeLimit = CPUFactor * Jobs * Machines;
	srand(RandSeeds);
	long STime = ::GetTickCount();
	DIWO diwo;
	diwo.SetInstance(Jobs, Machines, Factories, JobPTime, Sets, SetPTime, SetofJob);
	diwo.SetParameters(PS0,PSmax, Smax, 1, PLen, TimeLimit);
	int TF = diwo.Evolution1(alpha);
	double RealRuingTime = (::GetTickCount() - STime) / 1000.0;

	cout << InstanceFileName << "\t" << Jobs << "\t" << Machines << "\t" << Factories << "\t" << Sets << "\t" << CPUFactor << "\t" << TF << "\t" << RealRuingTime << endl;
	ofstream ofile;
	ofile.open(OutFileName, ios_base::app);
	ofile << InstanceFileName << "\t" << Jobs << "\t" << Machines << "\t" << Factories << "\t" << Sets << "\t" << CPUFactor << "\t" << TF << "\t" << RealRuingTime << endl;
	ofile.close();
}

void DIWO_EvType2_Ins(string InstanceDirectory, string InstanceFileName, string OutFileName, int RandSeeds, int CPUFactor, int PS0, int PSmax, int PLen, int Smax)
{
	int Jobs;
	int Machines;
	int Factories;
	vector<vector<int>> JobPTime;
	int Sets;
	vector <int> SetPTime;
	vector <int> SetofJob;

	string FileName = InstanceDirectory + "\\" + InstanceFileName;
	GetInstances(Jobs, Machines, Factories, JobPTime, Sets, SetPTime, SetofJob, FileName);

	long TimeLimit = CPUFactor * Jobs * Machines;
	srand(RandSeeds);
	long STime = ::GetTickCount();
	DIWO diwo;
	diwo.SetInstance(Jobs, Machines, Factories, JobPTime, Sets, SetPTime, SetofJob);
	diwo.SetParameters(PS0, PSmax, Smax, 1, PLen, TimeLimit);
	int TF = diwo.Evolution2();
	double RealRuingTime = (::GetTickCount() - STime) / 1000.0;

	cout << InstanceFileName << "\t" << Jobs << "\t" << Machines << "\t" << Factories << "\t" << Sets << "\t" << CPUFactor << "\t" << TF << "\t" << RealRuingTime << endl;
	ofstream ofile;
	ofile.open(OutFileName, ios_base::app);
	ofile << InstanceFileName << "\t" << Jobs << "\t" << Machines << "\t" << Factories << "\t" << Sets << "\t" << CPUFactor << "\t" << TF << "\t" << RealRuingTime << endl;
	ofile.close();
}

void DIWO_EvType3_Ins(string InstanceDirectory, string InstanceFileName, string OutFileName, int RandSeeds, int CPUFactor, int PS0, int PSmax, int PLen, int Smax, double Prob)
{
	int Jobs;
	int Machines;
	int Factories;
	vector<vector<int>> JobPTime;
	int Sets;
	vector <int> SetPTime;
	vector <int> SetofJob;

	string FileName = InstanceDirectory + "\\" + InstanceFileName;
	GetInstances(Jobs, Machines, Factories, JobPTime, Sets, SetPTime, SetofJob, FileName);

	long TimeLimit = CPUFactor * Jobs * Machines;
	srand(RandSeeds);
	long STime = ::GetTickCount();
	DIWO diwo;
	diwo.SetInstance(Jobs, Machines, Factories, JobPTime, Sets, SetPTime, SetofJob);
	diwo.SetParameters(PS0, PSmax, Smax, 1, PLen, TimeLimit);
	int TF = diwo.Evolution3(Prob);
	double RealRuingTime = (::GetTickCount() - STime) / 1000.0;

	cout << InstanceFileName << "\t" << Jobs << "\t" << Machines << "\t" << Factories << "\t" << Sets << "\t" << CPUFactor << "\t" << TF << "\t" << RealRuingTime << endl;
	ofstream ofile;
	ofile.open(OutFileName, ios_base::app);
	ofile << InstanceFileName << "\t" << Jobs << "\t" << Machines << "\t" << Factories << "\t" << Sets << "\t" << CPUFactor << "\t" << TF << "\t" << RealRuingTime << endl;
	ofile.close();
}

void DIWO_EvType1_Ins(string InstanceDirectory, string InstanceFileName, string OutFileName, int RandSeeds, int CPUFactor, int Para_PSize, int Smax, double alpha)
{
	int PLen = 2;
	int Jobs;
	int Machines;
	int Factories;
	vector<vector<int>> JobPTime;
	int Sets;
	vector <int> SetPTime;
	vector <int> SetofJob;

	string FileName = InstanceDirectory + "\\" + InstanceFileName;
	GetInstances(Jobs, Machines, Factories, JobPTime, Sets, SetPTime, SetofJob, FileName);

	long TimeLimit = CPUFactor * Jobs * Machines;
	srand(RandSeeds);
	long STime = ::GetTickCount();
	DIWO diwo;
	diwo.SetInstance(Jobs, Machines, Factories, JobPTime, Sets, SetPTime, SetofJob);
//	diwo.SetParameters(Para_PSize,Smax,1,PLen,TimeLimit);
	int TF = diwo.Evolution1(alpha);
	double RealRuingTime = (::GetTickCount() - STime) / 1000.0;

	cout << InstanceFileName << "\t" << Jobs << "\t" << Machines << "\t" << Factories << "\t" << Sets << "\t" << CPUFactor << "\t" << TF << "\t" << RealRuingTime << endl;
	ofstream ofile;
	ofile.open(OutFileName, ios_base::app);
	ofile << InstanceFileName << "\t" << Jobs << "\t" << Machines << "\t" << Factories << "\t" << Sets << "\t" << CPUFactor << "\t" << TF << "\t" << RealRuingTime << endl;
	ofile.close();
}

void DIWO_EvType2_Ins(string InstanceDirectory, string InstanceFileName, string OutFileName, int RandSeeds, int CPUFactor, int Para_PSize, int Smax)
{
	int PLen = 2;
	int Jobs;
	int Machines;
	int Factories;
	vector<vector<int>> JobPTime;
	int Sets;
	vector <int> SetPTime;
	vector <int> SetofJob;

	string FileName = InstanceDirectory + "\\" + InstanceFileName;
	GetInstances(Jobs, Machines, Factories, JobPTime, Sets, SetPTime, SetofJob, FileName);

	long TimeLimit = CPUFactor * Jobs * Machines;
	srand(RandSeeds);
	long STime = ::GetTickCount();
	DIWO diwo;
	diwo.SetInstance(Jobs, Machines, Factories, JobPTime, Sets, SetPTime, SetofJob);
	//diwo.SetParameters(Para_PSize, Smax, 1, PLen,TimeLimit);
	int TF = diwo.Evolution2();
	double RealRuingTime = (::GetTickCount() - STime) / 1000.0;

	cout << InstanceFileName << "\t" << Jobs << "\t" << Machines << "\t" << Factories << "\t" << Sets << "\t" << CPUFactor << "\t" << TF << "\t" << RealRuingTime << endl;
	ofstream ofile;
	ofile.open(OutFileName, ios_base::app);
	ofile << InstanceFileName << "\t" << Jobs << "\t" << Machines << "\t" << Factories << "\t" << Sets << "\t" << CPUFactor << "\t" << TF << "\t" << RealRuingTime << endl;
	ofile.close();
}

void DIWO_EvType3_Ins(string InstanceDirectory, string InstanceFileName, string OutFileName, int RandSeeds, int CPUFactor, int Para_PSize, int Smax,double Prob)
{
	int PLen = 2;
	int Jobs;
	int Machines;
	int Factories;
	vector<vector<int>> JobPTime;
	int Sets;
	vector <int> SetPTime;
	vector <int> SetofJob;

	string FileName = InstanceDirectory + "\\" + InstanceFileName;
	GetInstances(Jobs, Machines, Factories, JobPTime, Sets, SetPTime, SetofJob, FileName);

	long TimeLimit = CPUFactor * Jobs * Machines;
	srand(RandSeeds);
	long STime = ::GetTickCount();
	DIWO diwo;
	diwo.SetInstance(Jobs, Machines, Factories, JobPTime, Sets, SetPTime, SetofJob);
	//diwo.SetParameters(Para_PSize, Smax, 1, PLen,TimeLimit);
	int TF= diwo.Evolution3(Prob);
	double RealRuingTime = (::GetTickCount() - STime) / 1000.0;

	cout << InstanceFileName << "\t" << Jobs << "\t" << Machines << "\t" << Factories << "\t" << Sets << "\t" << CPUFactor << "\t" << TF << "\t" << RealRuingTime << endl;
	ofstream ofile;
	ofile.open(OutFileName, ios_base::app);
	ofile << InstanceFileName << "\t" << Jobs << "\t" << Machines << "\t" << Factories << "\t" << Sets << "\t" << CPUFactor << "\t" << TF << "\t" << RealRuingTime << endl;
	ofile.close();
}

void DIWO_EvType1_RunLarge(string InstanceDirectory, int CPU, string ResultFileName, int InsReps = 10, int ExpReps = 5, int PS0 = 10, int PSmax = 50, int PLen = 5, int Smax = 20,double alpha = 0.9)
{
	int JobArray[] = { 100, 200, 500 };
	int MachineArray[] = { 5, 10, 20 };
	int FactoryArray[] = { 4, 6, 8 };
	int ProductArray[] = { 30, 40, 50 };

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				for (int l = 0; l < 3; l++)
					for (int rep = 0; rep < InsReps; rep++)
					{
						ostringstream ostr;
						ostr << "I_" << JobArray[i] << "_" << MachineArray[j] << "_" << FactoryArray[k] << "_" << ProductArray[l] << "_" << rep + 1 << ".txt";
						string InsName = ostr.str();
						for (int Rep_Exp = 0; Rep_Exp < ExpReps; Rep_Exp++)
							DIWO_EvType1_Ins(InstanceDirectory, InsName, ResultFileName, Rep_Exp, CPU, PS0, PSmax,PLen,Smax,alpha);
					}
}

void DIWO_EvType1_TestPerbLen(string InstanceDirectory, int CPU, string ResultFileName, int PLen, int InsReps = 1, int ExpReps = 1, int Para_PSize = 50, int Smax = 20, double alpha = 0.9)
{
	int JobArray[] = { 100, 200, 500 };
	int MachineArray[] = { 5, 10, 20 };
	int FactoryArray[] = { 4, 6, 8 };
	int ProductArray[] = { 30, 40, 50 };

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				for (int l = 0; l < 3; l++)
					for (int rep = 0; rep < InsReps; rep++)
					{
						ostringstream ostr;
						ostr << "I_" << JobArray[i] << "_" << MachineArray[j] << "_" << FactoryArray[k] << "_" << ProductArray[l] << "_" << rep + 1 << ".txt";
						string InsName = ostr.str();
						for (int Rep_Exp = 0; Rep_Exp < ExpReps; Rep_Exp++)
							DIWO_EvType1_Ins(InstanceDirectory, InsName, ResultFileName, Rep_Exp, CPU, Para_PSize, Smax, alpha);
					}
}

void DIWO_EvType2_RunLarge(string InstanceDirectory, int CPU, string ResultFileName, int InsReps = 10, int ExpReps = 5, int PS0 = 10, int PSmax = 50, int PLen = 5, int Smax = 20)
{
	int JobArray[] = { 100, 200, 500 };
	int MachineArray[] = { 5, 10, 20 };
	int FactoryArray[] = { 4, 6, 8 };
	int ProductArray[] = { 30, 40, 50 };

	for (int i = 2; i < 3; i++)
		for (int j = 2; j < 3; j++)
			for (int k = 2; k < 3; k++)
				for (int l = 2; l < 3; l++)
					for (int rep = 0; rep < InsReps; rep++)
					{
						ostringstream ostr;
						ostr << "I_" << JobArray[i] << "_" << MachineArray[j] << "_" << FactoryArray[k] << "_" << ProductArray[l] << "_" << rep + 1 << ".txt";
						string InsName = ostr.str();
						for (int Rep_Exp = 0; Rep_Exp < ExpReps; Rep_Exp++)
							DIWO_EvType2_Ins(InstanceDirectory, InsName, ResultFileName, Rep_Exp, CPU, PS0, PSmax, PLen, Smax);
					}
}

void DIWO_EvType3_RunLarge(string InstanceDirectory, int CPU, string ResultFileName, int InsReps = 10, int ExpReps = 5, int PS0 = 10, int PSmax = 50, int PLen = 5, int Smax = 20, double Prob = 0.9)
{
	int JobArray[] = { 100, 200, 500 };
	int MachineArray[] = { 5, 10, 20 };
	int FactoryArray[] = { 4, 6, 8 };
	int ProductArray[] = { 30, 40, 50 };

	for (int i = 2; i < 3; i++)
		for (int j = 2; j < 3; j++)
			for (int k = 2; k < 3; k++)
				for (int l = 2; l < 3; l++)
					for (int rep = 0; rep < InsReps; rep++)
					{
						ostringstream ostr;
						ostr << "I_" << JobArray[i] << "_" << MachineArray[j] << "_" << FactoryArray[k] << "_" << ProductArray[l] << "_" << rep + 1 << ".txt";
						string InsName = ostr.str();
						for (int Rep_Exp = 0; Rep_Exp < ExpReps; Rep_Exp++)
							DIWO_EvType3_Ins(InstanceDirectory, InsName, ResultFileName, Rep_Exp, CPU, PS0, PSmax,PLen,Smax,Prob);
					}
}

void DIWO_EvType1_Large_Calibration(string InstanceDirectory, int AllParts, int PartNo)
{
	int CPU = 10;
	int PSizeArray[][2] = { {10,30}, {20, 30}, {30,30},{10, 40},{20,40},{30,40},{40,40},{10, 50},{20,50},{30,50},{40,50},{50,50} };
	int SmaxArray[] = {10,15, 20 };
	int PLenArray[] = { 2,3,5 };
	double AlphaArray[] = { 0.8,0.9 };
	int InsReps = 1;
	int ExpReps = 5;
	int Confs = 12 * 3 * 3 * 2;

	vector<int> RunNo(Confs);
	for (int i = 0; i < RunNo.size(); i++)
		RunNo[i] = i;
	srand(1981);
	random_shuffle(RunNo.begin(), RunNo.end());
	int div = Confs / AllParts;
	int Start = div*PartNo;
	int End = div*(PartNo + 1);
	if (End > Confs)
		End = Confs;
	if (End<Confs && Confs - End < div)
		End = Confs;

	for (int i = Start + 33; i < End; i++)
	{
		int CurNo = RunNo[i];
		int PS0 = PSizeArray[CurNo / (3 * 3 * 2)][0];
		int PSmax = PSizeArray[CurNo / (3 * 3 * 2)][1];

		CurNo = CurNo % (3 * 3 * 2 );
		int Smax = SmaxArray[CurNo / (3 * 2)];
		CurNo = CurNo % (3 * 2);
		int PLen = PLenArray[CurNo / 2];
		CurNo = CurNo % 2;
		double Alpha = AlphaArray[CurNo];

		ostringstream ostr;
		ostr << "Diwo_EvType1_Large_" << RunNo[i] << ".txt";
		string ResultFileName = ostr.str();
		DIWO_EvType1_RunLarge(InstanceDirectory, CPU, ResultFileName, InsReps, ExpReps, PS0, PSmax, PLen, Smax, Alpha);
	}
}

void DIWO_EvType1_Large_Calibration_Temp(string InstanceDirectory, int AllParts, int PartNo)//标定遗漏参数组合，完成后删除20180522
{
	int CPU = 20;
	int PSizeArray[][2] = { { 10,30 },{ 20, 30 },{ 30,30 },{ 10, 40 },{ 20,40 },{ 30,40 },{ 40,40 },{ 10, 50 },{ 20,50 },{ 30,50 },{ 40,50 },{ 50,50 } };
	int SmaxArray[] = { 10,15, 20 };
	int PLenArray[] = { 2,3,5 };
	double AlphaArray[] = { 0.8,0.9 };
	int InsReps = 1;
	int ExpReps = 5;
	int Confs = 12 * 3 * 3 * 2;

	vector<int> RunNo = { 0,4,9,13,15,22,25,38,43,53,55, 72,75,89,94,97,100,105,109,121,128,132,143,161,166,174,179,182,183,188,201,202,207 };
	srand(1981);
	random_shuffle(RunNo.begin(), RunNo.end());
	int div = 33 / AllParts;
	int Start = div * PartNo;
	int End = div * (PartNo + 1);
	for (int i = Start; i < End; i++)
	{
		int CurNo = RunNo[i];
		int PS0 = PSizeArray[CurNo / (3 * 3 * 2)][0];
		int PSmax = PSizeArray[CurNo / (3 * 3 * 2)][1];

		CurNo = CurNo % (3 * 3 * 2);
		int Smax = SmaxArray[CurNo / (3 * 2)];
		CurNo = CurNo % (3 * 2);
		int PLen = PLenArray[CurNo / 2];
		CurNo = CurNo % 2;
		double Alpha = AlphaArray[CurNo];

		ostringstream ostr;
		ostr << "Diwo_EvType1_Large_" << RunNo[i] << ".txt";
		string ResultFileName = ostr.str();
		DIWO_EvType1_RunLarge(InstanceDirectory, CPU, ResultFileName, InsReps, ExpReps, PS0, PSmax, PLen, Smax, Alpha);
	}
}

void DIWO_EvType2_Large_Calibration(string InstanceDirectory, int AllParts, int PartNo)
{
	int CPU = 10;
	int PSizeArray[][2] = { { 10,30 },{ 20, 30 },{ 30,30 },{ 10, 40 },{ 20,40 },{ 30,40 },{ 40,40 },{ 10, 50 },{ 20,50 },{ 30,50 },{ 40,50 },{ 50,50 } };
	int SmaxArray[] = { 10,15, 20 };
	int PLenArray[] = { 2,3,5 };
	int InsReps = 1;
	int ExpReps = 5;
	int Confs = 12 * 3 * 3;

	vector<int> RunNo(Confs);
	for (int i = 0; i < RunNo.size(); i++)
		RunNo[i] = i;
	srand(1981);
	random_shuffle(RunNo.begin(), RunNo.end());
	int div = Confs / AllParts;
	int Start = div * PartNo;
	int End = div * (PartNo + 1);
	if (End > Confs)
		End = Confs;
	if (End<Confs && Confs - End < div)
		End = Confs;

	for (int i = 0; i < RunNo.size(); i++)
	{
		int CurNo = RunNo[i];
		int PS0 = PSizeArray[CurNo / (3 * 3)][0];
		int PSmax = PSizeArray[CurNo / (3 * 3)][1];

		CurNo = CurNo % (3 * 3);
		int Smax = SmaxArray[CurNo / 3];
		CurNo = CurNo % 3;
		int PLen = PLenArray[CurNo];

		ostringstream ostr;
		ostr << "Diwo_EvType2_Large_" << RunNo[i] << ".txt";
		string ResultFileName = ostr.str();
		DIWO_EvType2_RunLarge(InstanceDirectory, CPU, ResultFileName,InsReps,ExpReps, PS0, PSmax, PLen, Smax);
	}
}

void DIWO_EvType3_Large_Calibration(string InstanceDirectory, int AllParts, int PartNo)
{
	int CPU = 10;
	int PSizeArray[][2] = { { 10,30 },{ 20, 30 },{ 30,30 },{ 10, 40 },{ 20,40 },{ 30,40 },{ 40,40 },{ 10, 50 },{ 20,50 },{ 30,50 },{ 40,50 },{ 50,50 } };
	int SmaxArray[] = { 10,15, 20 };
	int PLenArray[] = { 2,3,5 };
	double ProbArray[] = { 0.5,0.7,0.9 };
	int InsReps = 1;
	int ExpReps = 5;
	int Confs = 12 * 3 * 3 * 3;

	vector<int> RunNo(Confs);
	for (int i = 0; i < RunNo.size(); i++)
		RunNo[i] = i;
	srand(1981);
	random_shuffle(RunNo.begin(), RunNo.end());
	int div = Confs / AllParts;
	int Start = div*PartNo;
	int End = div*(PartNo + 1);
	if (End > Confs)
		End = Confs;
	if (End < Confs && Confs - End < div)
		End = Confs;

	for (int i = Start; i < End; i++)
	{
		int CurNo = RunNo[i];
		int PS0 = PSizeArray[CurNo / (3 * 3 * 3)][0];
		int PSmax = PSizeArray[CurNo / (3 * 3 * 3)][1];

		CurNo = CurNo % (3 * 3 * 3);
		int Smax = SmaxArray[CurNo / (3 * 3)];
		CurNo = CurNo % (3 * 3);
		int PLen = PLenArray[CurNo / 3];
		CurNo = CurNo % 3;
		double Prob = ProbArray[CurNo];
		ostringstream ostr;
		ostr << "Diwo_EvType3_Large_" << RunNo[i] << ".txt";
		string ResultFileName = ostr.str();
		DIWO_EvType3_RunLarge(InstanceDirectory, CPU, ResultFileName, InsReps,ExpReps, PS0,PSmax,PLen,Smax, Prob);
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	string Dir_Large = "CalibrationInstances\\Large";

	Dir_Large = "TestBed\\Large";
	DIWO_EvType1_RunLarge(Dir_Large, 20,"DIWO_EvType1_20.txt");
//	DIWO_EvType1_RunLarge(Dir_Large, 40, "DIWO_EvType1_40.txt");
//	DIWO_EvType1_RunLarge(Dir_Large, 60, "DIWO_EvType1_60.txt");
//	DIWO_EvType2_RunLarge(Dir_Large, 20, "DIWO_EvType2_20.txt");
//	DIWO_EvType2_RunLarge(Dir_Large, 40, "DIWO_EvType2_40.txt");
//	DIWO_EvType2_RunLarge(Dir_Large, 60, "DIWO_EvType2_60.txt");
//	DIWO_EvType3_RunLarge(Dir_Large, 20, "DIWO_EvType3_20.txt");
//	DIWO_EvType3_RunLarge(Dir_Large, 40, "DIWO_EvType3_40.txt");
//	DIWO_EvType3_RunLarge(Dir_Large, 60, "DIWO_EvType3_60.txt");


	return 0;
}
