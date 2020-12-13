#include "MyPaser.h"
#include <string.h>
#include <string>
#include <iostream>     
#include <fstream>     
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>

bool TouchFile(const char* filename) {
	std::ifstream ifs(filename);
	if (!ifs.is_open()) {
		fprintf(stderr, "File [%s] not exsit.\n", filename);
		exit(-1);
	}
	ifs.close();
}


int ExecCmd(string cmd, string& res) {
	if (cmd.size() == 0) {  //cmd is empty 
		return -1;
	}

	char buffer[1024] = { 0 };
	string result = "";
	FILE* pin = popen(cmd.c_str(), "r");
	if (!pin) { //popen failed 
		return -1;
	}

	res.clear();
	while (!feof(pin)) {
		if (fgets(buffer, sizeof(buffer), pin) != NULL) {
			result += buffer;
		}
	}

	res = result;
	return pclose(pin); //-1:pclose failed; else shell ret
}

int SplitStringToVect(const string& srcStr, vector<string>& destVect, const string& strFlag)
{
	int pos = srcStr.find(strFlag, 0);
	int startPos = 0;
	int splitN = pos;
	string lineText(strFlag);

	while (pos > -1)
	{
		lineText = srcStr.substr(startPos, splitN);
		startPos = pos + 1;
		pos = srcStr.find(strFlag, pos + 1);
		splitN = pos - startPos;
		destVect.push_back(lineText);
	}

	lineText = srcStr.substr(startPos, srcStr.length() - startPos);
	destVect.push_back(lineText);

	return destVect.size();
}

int MyPaser::Program::GetFuncNameFromStr(const string& context) {
	vector<string> lines;
	lines.clear();
	SplitStringToVect(context, lines, "\n");

	for (auto &l : lines) {
		if (l.find("g     F .text") != string::npos) {
			int pos = l.find_last_of(" ");
			string funcName = l.substr(pos + 1, l.size() - pos);
			//cout << l << ":" << funcName << endl;
			//cout << funcName << endl;
			this->funcName_.push_back(funcName);
		}
	}
	return 0;
}

int MyPaser::Program::GetFuncOpFromStr(const string& context) {
	vector<string> lines;
	lines.clear();
	SplitStringToVect(context, lines, "\n");

	bool inFunc = 0;
	string curFunc = "";
	for (auto &l : lines) {
		//cout << l<< endl;
		if (!inFunc) {
			for (auto &func : this->funcName_) {
				//string afunc = "<" + func + ">";
				if (l.find(func) != string::npos) {
					inFunc = 1;
					curFunc = func;
				}
			}
		}
		else {
			//  0:   lea    0x0(%rip),%rsi        # 7 <main+0x7>
			//	7:   sub    $0x8, % rsp
			// 12:   callq  17 <main+0x17>
			// 1d:   retq

			int pos = l.find(":\t");
			if (pos != string::npos) {
				string op = l.substr(pos + 2, l.size() - pos - 2);
				int pos = op.find_first_of(" ");
				op = op.substr(0, pos);
				//cout << curFunc << ":" << op << endl;
				this->functOp_[curFunc].push_back(op);
			}
			else
			{
				inFunc = 0;
			}
		}
	}
	return 0;
}


/*int MakeTokenList(const char* filename,vector<) {
	char cmd[1000];
	sprintf(cmd, "clang++ -O2 -Xclang -dump-tokens %s 2>&1 | grep \"%s\"", filename, filename);

	string res;
	ExecCmd(cmd, res);
	cout << "-------------------------------" << endl;
	cout << res << endl;
	
	vector<string> lines;
	SplitStringToVect(res, lines, "\n");
	printf("%d\n", (int)lines.size());
	for (auto i : lines) {
		printf("%s]]]\n", i.c_str());
	}
}//*/

string SplitDirectory(string fulldir) {
	int pos = fulldir.find_last_of("/");
	//cout << pos << endl;
	if (pos == string::npos) {
		return fulldir;
	}
	else {
		return fulldir.substr(pos+1, fulldir.size() - pos);
	}
}

void MyPaser::Program::PrintfFuncOptoFile(const char* filename) {
	FILE* f = fopen(filename, "w");
	for (auto &name : this->funcName_) {
		fprintf(f, "[%s]:\n", name.c_str());
		for (auto &op : this->functOp_[name]) {
			fprintf(f, "%s,", op.c_str());
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

int MyPaser::LCS(vector<string>& a, vector<string>& b) {
	int na = a.size();
	int nb = b.size();

	int** dp = new int* [na+10];
	for (int i = 0; i < na+10; ++i)
	{
		dp[i] = new int[nb+10];
	}

	for (int i = 0; i <= na; i++) {
		for (int j = 0; j <= nb; j++) {
			dp[i][j] = 0;
		}
	}

	for (int i = 1; i <= na; i++) {
		for (int j = 1; j <= nb; j++) {
			if (a[i - 1] == b[j - 1]) {
				dp[i][j] = dp[i - 1][j - 1] + 1;
			}
			else {
				dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
			}
		}
	}

	int ans = dp[na][nb];

	for (int i = 0; i < na+10; ++i)
	{
		delete[] dp[i];
	}
	delete[] dp;

	/*
	for (auto& i : a) {
		cout << i << ",";
	}
	cout << endl;
	for (auto& i : b) {
		cout << i << ",";
	}
	cout << endl;
	cout << dp[na][nb] << endl;
	//*/

	return ans;
}

int MyPaser::FuncCompare(vector<string>& a, vector<string>& b) {

	/*
	for (auto& i : a) {
		cout << i << ",";
	}
	cout << endl;
	for (auto& i : b) {
		cout << i << ",";
	}
	cout << endl;
	//*/

	int ans = 0;
	int w = (int)((double)a.size() * 3.0 / 2.0);
	int na = a.size();
	int nb = b.size();
	vector<string> c; c.clear();
	for (int k = 0; k < nb; k++) {
		if (k + 1 + w < nb) {
			c.assign(b.begin() + k, b.begin() + k + w + 1);
		}
		else {
			c.assign(b.begin() + k, b.end());
		}
		int tmp = LCS(a, c);
		ans = max(ans, tmp);
	}
	return ans;
}

double MyPaser::MaxWeightFlow(Program& A, Program& B) {
	int na = A.funcName_.size();
	int nb = B.funcName_.size();
	int N = na + nb + 2;

	Graph G;

	double alpha = 2.0;
	double beta = 1.0 / 2.0;

	G.init(N);
	for (int i = 0; i < na; i++) {
		for (int j = 0; j < nb; j++) {
			vector<string>& a_op = A.functOp_[A.funcName_[i]];
			vector<string>& b_op = B.functOp_[B.funcName_[j]];
			int cap1 = FuncCompare(a_op, b_op);
			int cap2 = FuncCompare(b_op, a_op);
			double fac = (double)max(cap1, cap2) / (double)min(a_op.size(), b_op.size());
			double w = 1.0 / (1 + exp(-alpha * fac + beta));

			G.addedge(i + 1, j + na + 1, cap1, -w);
			//cout << i + 1 << " " << j + na + 1 << " " << cap1 << " " << -w << endl;
		}
	}
	int st = 0;
	int ed = na + nb + 1;

	int fa = 0;

	for (int i = 0; i < na; i++) {
		G.addedge(st, i + 1, A.functOp_[A.funcName_[i]].size(), 0);
		fa += A.functOp_[A.funcName_[i]].size();
	}
	for (int i = 0; i < nb; i++) {
		G.addedge(i + na + 1, ed, B.functOp_[B.funcName_[i]].size(), 0);
	}
	double cost = 0, flow = 0;
	flow = G.minCostMaxflow(st, ed, cost);

	cost = -cost;
	//cout << "cost:" << cost << endl;

	double ans = (double)cost / (double)fa;
	ans = ans / 0.82;
	//cout << flow << "/" << fa << endl;
	return ans;
}

double MyPaser::MaxFlow(Program& A, Program& B) {
	int na = A.funcName_.size();
	int nb = B.funcName_.size();
	int N = na + nb + 2;
	
	Graph G;

	G.init(N);
	for (int i = 0; i < na; i++) {
		for (int j = 0; j < nb; j++) {
			vector<string>& a_op = A.functOp_[A.funcName_[i]];
			vector<string>& b_op = B.functOp_[B.funcName_[j]];
			int w = FuncCompare(a_op, b_op);
			G.addedge(i + 1, j + na + 1, w, 0);
			//cout << i + 1 << " " << j + na + 1 << " " << w << endl;
		}
	}
	int st = 0;
	int ed = na + nb + 1;

	int fa = 0;

	for (int i = 0; i < na; i++) {
		G.addedge(st, i + 1, A.functOp_[A.funcName_[i]].size(), 0);
		fa += A.functOp_[A.funcName_[i]].size();
	}
	for (int i = 0; i < nb; i++) {
		G.addedge(i + na + 1, ed, B.functOp_[B.funcName_[i]].size(), 0);
	}
	double cost = 0, flow = 0;
	flow = G.minCostMaxflow(st, ed, cost);
	//cout << cost << endl;
	cost = -cost;
	//TODO change MaxFlow to MaxCostMaxflow

	double ans = (double)flow / (double)fa;
	//cout << flow << "/" << fa << endl;
	return ans;
}

int MyPaser::GetFuncList(char* filename, Program& Prog) {
	char cmd[1000];
	string cmdres;

	sprintf(cmd, "mktemp");
	ExecCmd(cmd, cmdres);
	char tempFilename[50];
	strcpy(tempFilename, cmdres.c_str());
	//cout << tempFilename << endl;

	sprintf(cmd, "g++ -std=c++11 -O2 -c %s -o %s", filename, tempFilename);
	ExecCmd(cmd, cmdres);

	sprintf(cmd, "objdump -t %s", tempFilename);
	ExecCmd(cmd, cmdres);
	Prog.GetFuncNameFromStr(cmdres);
	//printf("%s\n", cmdres.c_str());

	//FILE* f = fopen("1.txt", "w");
	//fprintf(f, "%s", cmdres.c_str());
	//fclose(f);

	sprintf(cmd, "objdump --no-show-raw-insn -d %s", tempFilename);
	ExecCmd(cmd, cmdres);
	Prog.GetFuncOpFromStr(cmdres);

	/*string t = filename;
	if (t.find("a.cpp") != string::npos) {
		Prog.PrintfFuncOptoFile("A_op.txt");
	}
	else {
		Prog.PrintfFuncOptoFile("B_op.txt");
	}

	//f = fopen("2.txt", "w");
	//fprintf(f, "%s", cmdres.c_str());
	//fclose(f);
	//*/
}

double MyPaser::Compare(Program& A, Program& B) {
	//return MaxFlow(A, B);
	return MaxWeightFlow(A, B);
}

double MyPaser::PaserCompare(char* filename1, char* filename2) {
	MyPaser::Program A;
	MyPaser::Program B;

	if (TouchFile(filename1) && TouchFile(filename2)) {
		GetFuncList(filename1, A);
		GetFuncList(filename2, B);

		return max(Compare(A, B), Compare(B, A));
		//9return  Compare(A, B);

		//printf("%s\n", filename1);
		//string s = SplitDirectory(filename1);
		//printf("%s\n", s.c_str());
	}
}