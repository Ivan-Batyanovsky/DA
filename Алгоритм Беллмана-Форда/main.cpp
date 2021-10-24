#include<bits/stdc++.h>

typedef long long int li;

using namespace std; 

const li INF = LONG_MAX;

struct edge
{
	li u, v, weight;
};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	li n, m, s, f;
	cin >> n >> m >> s >> f;
	vector<edge> e(m);
	for (int i = 0; i < m; ++i)
		cin >> e[i].u >> e[i].v >> e[i].weight;
	bool signal;
	vector<li> d(n + 1, INF);
	d[s] = 0;

	for (int i = 0; i < n; ++i)
	{
		signal = false;
		for (int j = 0; j < m; ++j)
		{
			if (d[e[j].u] < INF)
			{
				if (d[e[j].v] > d[e[j].u] + e[j].weight)
				{
					d[e[j].v] = d[e[j].u] + e[j].weight;
					signal = true;
				}
			}
		}
		if (signal == false) break;
	}
	if (d[f] == INF)
	{
		cout << "No solution\n";
	} else {
		cout << d[f] << '\n';
	}
	return 0;
}
