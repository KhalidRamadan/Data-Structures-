

#include<vector>
#include<iostream>
#include<functional>
#include<string>
#include<algorithm>
#include "segment_tree.hpp"
#include <boost/test/unit_test.hpp>


// used to test preformance of segment tree 
template<typename T>
T brute_force(int l, int r, const std::vector<T> &vec,
	std::function<T(T, T)> fun = [](T a, T b) { return std::min(a, b); })
{
	T ret = vec[l];
	for (int i = l + 1; i <= r; i++)
	{
		ret = fun(ret, vec[i]);
	}
	return ret;
}


// Unit Test
int main()
{
	std::vector<int> x = { 345, 45 , 54, 45, 3432, -123, 123, 14, -33 };

	data_structure::segment_tree<int> mysegment(x, [](int a, int b)
	{
		return std::min(a, b);
	});


	while (1)
	{
		int q;
		std::cin >> q;
		int a, b;
		std::cin >> a >> b;
		if (q == 0)
		{
			x[a] = b;
			mysegment.update(a, b);
		}
		else if (q == 1)
		{

			int bf = brute_force(a, b, x);
			int st = mysegment.query(a, b);
			BOOST_CHECK_MESSAGE(bf == st, "add(..) result: " << bf);
			std::cout << bf << " " << st << std::endl;

		}
		else if (q == 2)
		{
			x.push_back(a);
			mysegment.push_back(a);
		}
		else
		{
			x.pop_back();
			mysegment.pop_back();
		}


		std::cout << mysegment.size() << std::endl;
		for (int i = 0; i < mysegment.size(); i++)
		{
			std::cout << mysegment.get_element(i) << " ";
			BOOST_REQUIRE(mysegment.get_element(i) == x[i]);
		}
		std::cout << std::endl << std::endl;


		mysegment.print();

	}



	system("PAUSE");
}




