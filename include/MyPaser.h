#include<cstdio>
#include<vector>
#include<string>
#include<map>
#include"MaximumWeightFlow.h"
using namespace std;

namespace MyPaser {
	class Program {
	public:
		vector<string> funcName_;
		map<string, vector<string>> functOp_;
		Program() {
			funcName_.clear();
			functOp_.clear();
		}
		int GetFuncNameFromStr(const string& context);
		int GetFuncOpFromStr(const string& context);
		void PrintfFuncOptoFile(const char* filename);
	};

	int FuncCompare(vector<string>& a, vector<string>& b);
	int LCS(vector<string>& a, vector<string>& b);
	int GetFuncList(char* filename, Program& a);
	double PaserCompare(char* filename1, char* filename2);

	double MaxFlow(Program& A, Program& B);
	double MaxWeightFlow(Program& A, Program& B);	//Needle
	double Compare(Program& A, Program& B);
}
