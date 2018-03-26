#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE segment_tree_test

#include<vector>
#include<iostream>
#include<functional>
#include<string>
#include<algorithm>
#include<random>
#include <time.h> 
#include "segment_tree.hpp"
#include <boost/test/unit_test.hpp>

// used to test preformance of segment tree 
template<typename T>
T brute_force(int l, int r, const std::vector<T> &vec,
	std::function<T(T, T)> fun = [](T a, T b) { return a + b; })
{
	T ret = vec[l];
	for (int i = l + 1; i <= r; i++)
	{
		ret = fun(ret, vec[i]);
	}
	return ret;
}

// feed vector<int> for segment tree and make some update and some qurey in it 
BOOST_AUTO_TEST_CASE(st_vector_int_test)
{
	srand(time(NULL));

	std::vector<int> bf = { 345, 45 , 54, 5,  -12321, 232, 23243, 23, -343, -234, 343, -12321 };
	std::function<int(int, int)> lamda = [](int a, int b) { return std::max(a, b); }; // Tested +, ^, |, *, max, min
	data_structure::segment_tree<int> st(bf, [](int a, int b) { return std::max(a, b); });

	for (int i = 0; i < 1000; i++)
	{
		int type = rand() % 4;
		if (type == 0)
		{
			if (bf.size() == 0) break;
			int l = rand() % (bf.size());
			int r = rand() % (bf.size());
			if (l > r) std::swap(l, r);
			int bf_ans = brute_force(l, r, bf, lamda);
			int st_ans = st.query(l, r);
			//std::cout << "qurey: " << l << " " << r << std::endl;
			BOOST_CHECK(bf_ans == st_ans);
			if (bf_ans != st_ans)
			{
				st.print();

				std::cout << bf_ans << " " << st_ans << std::endl;
			}

		}
		else if (type == 1)
		{
			if (bf.size() == 0) break;
			int pos = rand() % (bf.size());
			int new_val = (rand() % (1000000)) - 500000;
			bf[pos] = new_val;
			st.update(pos, new_val);
			//std::cout << "update: " << pos << " " << new_val << std::endl;
			BOOST_CHECK(bf[pos] == st.get_element(pos)); 

		}
		else if (type == 2)
		{
			int new_val = (rand() % (1000000)) - 500000;
			bf.push_back(new_val);
			st.push_back(new_val);
			//std::cout << "push: " << new_val << std::endl;
			BOOST_CHECK(bf.back() == st.get_element(st.size() - 1));
		}
		else if (type == 3)
		{
			bf.pop_back();
			st.pop_back();
			//std::cout << "pop" << std::endl;
		}
		BOOST_CHECK(bf.size() == st.size());

		for (int i = 0; i < bf.size(); i++)
		{
			BOOST_CHECK(bf[i] == st.get_element(i));
			
		}

	}
	system("PAUSE");
}



