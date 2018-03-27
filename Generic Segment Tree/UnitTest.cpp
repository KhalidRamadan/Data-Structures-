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
#include <iomanip> 
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
	clock_t time_brute_force = 0;
	clock_t time_segment_tree = 0;
	srand(time(NULL));

	std::vector<int> bf = { 345, 45 , 54, 5,  -12321, 232, 23243, 23, -343, -234, 343, -12321 };
	std::function<int(int, int)> lamda = [](int a, int b) { return std::max(a, b); }; // Tested +, ^, |, *, max, min
	data_structure::segment_tree<int> st(bf, [](int a, int b) { return std::max(a, b); });

	clock_t before, after;
	for (int i = 0; i < 100000; i++)
	{
		int type = rand() % 4;
		//std::cout << i << " " << type << std::endl;
		if (type == 0)
		{
			if (bf.size() == 0) continue;
			int l = rand() % (bf.size());
			int r = rand() % (bf.size());
			if (l > r) std::swap(l, r);
			before = clock();
			int bf_ans = brute_force(l, r, bf, lamda);
			after = clock();
			time_brute_force = time_brute_force + (after - before);
			before = clock();
			int st_ans = st.query(l, r);
			time_segment_tree = time_segment_tree + (after - before);
			BOOST_CHECK(bf_ans == st_ans);

		}
		else if (type == 1)
		{
			if (bf.size() == 0) continue;
			int pos = rand() % (bf.size());
			int new_val = (rand() % (1000000)) - 500000;
			bf[pos] = new_val;
			st.update(pos, new_val);
			BOOST_CHECK(bf[pos] == st.get_element(pos)); 

		}
		else if (type == 2)
		{
			int new_val = (rand() % (1000000)) - 500000;
			bf.push_back(new_val);
			st.push_back(new_val);
			BOOST_CHECK(bf.back() == st.get_element(st.size() - 1));
		}
		else if (type == 3 && st.size() != 0 && bf.size() != 0)
		{
			bf.pop_back();
			st.pop_back();
		}
		BOOST_CHECK(bf.size() == st.size());

		for (int i = 0; i < bf.size(); i++)
		{
			BOOST_CHECK(bf[i] == st.get_element(i));
			
		}

	}
	std::cout << std::fixed << std::setprecision(9);
	std::cout << "brute force time : " << time_brute_force << std::endl;
	std::cout << "segment tree time : " << time_segment_tree << std::endl;

	system("PAUSE");
}





