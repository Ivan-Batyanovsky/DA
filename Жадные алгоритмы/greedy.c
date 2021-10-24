#include<bits/stdc++.h>

typedef long long int li;

using namespace std; 

int main()
{
	li n, res = 0;

	cin >> n;
	vector<deque<li> > cons(3);
	li temp;
	for (li i = 0; i < n; ++i) // Считываем цифры
	{
		cin >> temp;
		cons[temp - 1].push_back(i);
	}

	li size = cons[0].size();
	while (!cons[0].empty() && cons[0].front() < size) // Выкидываем все 1 стоящие на своих позициях
		cons[0].pop_front();

	while (!cons[1].empty() && !cons[0].empty() && cons[1].front() < size) // Выкидываем из оч1 эл-ты пока в оч2 есть эл-ты входящие в отрезок единиц
	{
		cons[1].push_back(cons[0].front());
		cons[0].pop_front();
		cons[1].pop_front();
		res++;
	}

	while (!cons[2].empty() && !cons[0].empty() && cons[2].front() < size) // Выкидываем из оч1 эл-ты пока в оч3 есть эл-ты входящие в отрезок единиц
	{
		cons[2].push_back(cons[0].front());
		cons[0].pop_front();
		cons[2].pop_front();
		res++;
	}
  
	size += cons[1].size();
	while (!cons[1].empty())
	{
		if (cons[1].front() >= size)
			res++;
		cons[1].pop_front();
	}		
	cout << res << endl;
	return 0;
}
