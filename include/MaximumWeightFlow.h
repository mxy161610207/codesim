#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<string.h>
#include<queue>
#include<iostream>
#define INF 0x3f3f3f3f
using namespace std;

class Graph {
public:
	struct Edge
	{
		int to, next, cap, flow;
		double cost;
	}edge[100100];
	int head[10010], pre[10010];
	double dis[10010];
	bool vis[10010];
	int tol;
	int N;
	int init(int n)
	{
		N = n;
		tol = 0;
		memset(head, -1, sizeof(head));
	}
	void addedge(int u, int v, int cap, double cost)
	{
		edge[tol].to = v;
		edge[tol].cap = cap;
		edge[tol].cost = cost;
		edge[tol].flow = 0;
		edge[tol].next = head[u];
		head[u] = tol++;
		edge[tol].to = u;
		edge[tol].cap = 0;
		edge[tol].cost = -cost;
		edge[tol].flow = 0;
		edge[tol].next = head[v];
		head[v] = tol++;
	}
	bool spfa(int s, int t)
	{
		queue<int>q;
		for (int i = 0; i < N; i++)
		{
			dis[i] = (double)INF;
			vis[i] = false;
			pre[i] = -1;
		}
		dis[s] = 0;
		vis[s] = true;
		q.push(s);
		while (!q.empty())
		{
			int u = q.front();
			q.pop();
			vis[u] = false;
			for (int i = head[u]; i != -1; i = edge[i].next)
			{
				int v = edge[i].to;
				if (edge[i].cap > edge[i].flow &&
					dis[v] > dis[u] + edge[i].cost)
				{
					dis[v] = dis[u] + edge[i].cost;
					pre[v] = i;
					if (!vis[v])
					{
						vis[v] = true;
						q.push(v);
					}
				}
			}
		}
		if (pre[t] == -1)return false;
		else return true;
	}
	int minCostMaxflow(int s, int t, double& cost)
	{
		int flow = 0;
		cost = 0;
		while (spfa(s, t))
		{
			int Min = INF;
			for (int i = pre[t]; i != -1; i = pre[edge[i ^ 1].to])
			{
				if (Min > edge[i].cap - edge[i].flow)
					Min = edge[i].cap - edge[i].flow;
			}
			for (int i = pre[t]; i != -1; i = pre[edge[i ^ 1].to])
			{
				edge[i].flow += Min;
				edge[i ^ 1].flow -= Min;
				cost += edge[i].cost * Min;
			}
			flow += Min;
			//cout << "|" << cost << endl;
		}
		return flow;
	}
};