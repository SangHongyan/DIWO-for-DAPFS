
#pragma once

namespace Base
{
	template <typename T>
	struct Pair
	{
		int dim;
		T value;
	};

	template <typename T>
	struct PairGreater {
		bool operator () (Pair <T> a, Pair <T> b)
		{
			return a.value > b.value;
		}
	};

	template <typename T>
	struct PairLess {
		bool operator () (Pair <T> a, Pair <T> b)
		{
			return a.value < b.value;
		}
	};

	void GetInstances(int &Jobs, int &Machines, int &Factories, vector<vector<int>> &JobPTime, int &Set, vector <int> &SetPTime, vector <int> &JobinSet, string FileName);
	void GenerateInstances(int Jobs, int Machines, int Factories, int Set, string FileName, int Seeds);
	void GenerateTestSet_Large();
	void GenerateTestSet_Small();
}


