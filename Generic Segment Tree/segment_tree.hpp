#ifndef SEGMENT_TREE_HPP
#define SEGMENT_TREE_HPP

#include<iostream>
#include<memory>
#include<vector>
#include<functional>




namespace data_structure
{
	// Node (small unit of segment tree) every node contain a info about range(start, end) 
	// have a value and have left, right childs
	template<typename T>
	class node
	{
		typedef T value_type;
		typedef std::shared_ptr<node> node_ptr;

	public:
		value_type value;
		bool have_value;
		node_ptr left;
		node_ptr right;

		node() :
			value(value_type()),
			have_value(false),
			left(nullptr),
			right(nullptr)
		{}

		node(value_type value, bool have_value = true) :
			value(value),
			have_value(have_value),
			left(nullptr),
			right(nullptr)
		{}
	};

	// dynamic segment tree 
	// T type of segment tree 
	// Container type you want to use, ex (vector, linked list, etc...)
	template<typename T, typename container = std::vector<T> >
	class segment_tree
	{
	public:
		// Types 
		typedef T value_type;
		typedef node<value_type> node;
		typedef std::shared_ptr<node> node_ptr;
		typedef typename container::size_type size_type;
		typedef typename container::iterator iterator;
		typedef typename container::const_iterator const_iterator;
		typedef std::function <value_type(const value_type&, const value_type&)> function;

		// This a Default fuction for segment tree , you can send your own function in the constructor
		static function plus_func;

	private:
		node_ptr m_root; // Root of the Segment tree
		container m_data; // Container to contain the raw data 
		size_type m_real_size; // contain the real size of the segment treee 
		function m_function; // your marge and query function 

	public:
		// empty constructor
		segment_tree() :
			m_root(),
			m_data(),
			m_real_size(0),
			m_function(plus_func)
		{
			build();
		}

		// constructor have a size and initial value for all nodes 
		segment_tree(size_type size, const value_type &initVal, function func = plus_func) :
			m_root(),
			m_data(size, initVal),
			m_real_size(0),
			m_function(func)
		{
			build();
		}

		// constructor take a begin iterator and end iterator 
		segment_tree(const_iterator begin, const_iterator end, function func = plus_func) :
			m_root(),
			m_data(begin, end),
			m_real_size(0),
			m_function(func)
		{
			build();
		}

		// constructor take array 
		segment_tree(size_type size, const value_type *arr, function func = plus_func) :
			m_root(),
			m_data(arr, arr + size),
			m_real_size(0),
			m_function(func)
		{
			build();
		}

		// constructor take Container 
		segment_tree(const container &data, function func = plus_func) :
			m_root(),
			m_data(data),
			m_real_size(0),
			m_function(func)
		{
			build();
		}



	protected:
		// This function build a segment tree with prv given data in constructor 
		void build()
		{
			calculate_tree_size();
			m_root = create_tree(0, m_real_size - 1);
		}
	
		// calculate real size of segment tree 
		void calculate_tree_size()
		{
			size_type size = m_data.size();
			m_real_size = 1;
			while (m_real_size < size) m_real_size <<= 1;
			

		}

		// create segment tree recursively
		node_ptr create_tree(size_type start_interval, size_type end_interval)
		{

			if (start_interval == end_interval)
			{
				value_type value = value_type();
				bool have_value = true;
				size_type arr_index = start_interval;
				if (arr_index < m_data.size())
					value = m_data[arr_index];
				else have_value = false;

				return create_node(value, have_value);
			}
			node_ptr root = create_node();
			
			size_type start_left = start_interval;
			size_type end_left = get_mid(start_interval, end_interval);
			size_type start_right = end_left + 1;
			size_type end_right = end_interval;


			root->left = create_tree(start_left, end_left);
			root->right = create_tree(start_right, end_right);

			calculate_value(root);

			return root;

		}


		// create a new node 
		node_ptr create_node(const value_type &value = value_type(), bool have_val = true)
		{
			return std::make_shared<node>(node(value, have_val));
		}

		// This function is used to update tree valued after change the your function 
		void rebuild()
		{
			update_values(m_root, 0, m_real_size - 1);
		}

		void update_values(const node_ptr &cur_root, size_type start_interval, size_type end_interval)
		{

			if (start_interval == end_interval)
			{
				return;
			}
			size_type start_left = start_interval;
			size_type end_left = get_mid(start_interval, end_interval);
			size_type start_right = end_left + 1;
			size_type end_right = end_interval;

			
			update_values(cur_root->left, start_left, end_left);
			update_values(cur_root->right, start_right, end_right);

			calculate_value(cur_root);
		}

		// get The middle of the interval
		size_type get_mid(size_type start, size_type end) const  
		{
			return (start + end) >> 1;
		}

		// to update
		void do_update(const node_ptr &cur_node, size_type index, const value_type &new_value, bool have_value, size_type start, size_type end)
		{
			if (start == end)
			{
				cur_node->value = new_value;
				cur_node->have_value = have_value;
				return;
			}
			size_type mid = get_mid(start, end);

			if (index <= mid)
				do_update(cur_node->left, index, new_value, have_value, start, mid);
			else
				do_update(cur_node->right, index, new_value, have_value, mid + 1, end);

			calculate_value(cur_node);
		}


		value_type calculate_query(const node_ptr &root, const value_type &left_half, const value_type &right_half) const
		{
			if (root->left->have_value == true && root->right->have_value == true)
				return m_function(left_half, right_half);

			if (root->left->have_value == true)
				return left_half;

			if (root->right->have_value == true)
				return right_half;

			return value_type();
		}

		// do query
		value_type do_query(const node_ptr &cur_node, size_type from, size_type to, size_type start, size_type end) const
		{
			if (start >= from && end <= to)
				return cur_node->value;

			size_type mid = get_mid(start, end);

			if (to <= mid)
				return do_query(cur_node->left, from, to, start, mid);
			else if(mid < from)
				return do_query(cur_node->right, from, to, mid + 1, end);

			value_type left_half = do_query(cur_node->left, from, to, start, mid);
			value_type right_half = do_query(cur_node->right, from, to, mid + 1, end);

			return calculate_query(cur_node, left_half, right_half);

		}

		// calculate value 
		void calculate_value(const node_ptr &root)
		{
			if (root->left->have_value == true && root->right->have_value == true)
				root->value = m_function(root->left->value, root->right->value), root->have_value = true;
			else if (root->left->have_value == true)
				root->value = root->left->value, root->have_value = true;
			else if (root->right->have_value == true)
				root->value = root->right->value, root->have_value = true;
			else
				root->have_value = false;
		}

		// This function just add a new nodes to the tree , when tree is full , increase tree size 
		void upgrade()
		{
			if (m_real_size > m_data.size()) return;

			size_type start_right = m_real_size;
			size_type end_right = (2 * m_real_size) - 1;

			m_real_size *= 2;
			node_ptr root = create_node();

			root->left = m_root;
			root->right = create_tree(start_right, end_right);

			m_root = root;
		}

		// This fuction free memory when you don't use it (delete free nodes in the tree)
		void downgrade()
		{
			if (m_data.size() > (m_real_size >> 1)) return;
			if (m_real_size == 1) return;
			m_real_size >>= 1;
			m_root = m_root->left;
		}



	public: // user methods 

		// Set A new Function for your segment tree (Notice: tree will rebuild values every time you use This Function)
		void set_function(function func)
		{
			m_function = func;
			rebuild();
		}

			// This function insert a new element in the back of segment tree (push a new element to the end of the segment tree) (log(n))
		void push_back(const value_type &value)
		{
			m_data.push_back(value);
			if (m_real_size < m_data.size())
				upgrade();

			update(m_data.size() - 1, value);
		}

		// This function delete the last value of semgment tree (log(n))
		void pop_back()
		{
			update(m_data.size() - 1, value_type(), false);
			m_data.pop_back();
			downgrade();
		}

		// This query function take interval(from, to) and make a query in it base on your function (log(n))
		value_type query(size_type from, size_type to) const
		{
			return do_query(m_root, from, to, 0, m_real_size - 1);
		}

		// This update function take index and new value, update old value with the new value (log(n))
		void update(size_type index, const value_type &new_value, bool have_value = true)
		{
			m_data[index] = new_value;
			do_update(m_root, index, new_value, have_value, 0, m_real_size - 1);
		}

		const_iterator begin() const
		{
			return m_data.begin();
		}

		const_iterator end() const
		{
			return m_data.end();
		}

		size_type size() const
		{
			return m_data.size();
		}

		bool empty() const
		{
			return size() == 0;
		}

		value_type get_element(size_type index)
		{
			if (index >= size()) return value_type();
			return m_data[index];
		}


		// testing 
		// for debugging
		std::vector<std::pair<size_type, std::pair<value_type, bool>> > test_print;

		void print_tree(const node_ptr &node, size_type l, size_type r, size_type pos = 1)
		{
			test_print.push_back({ pos, {node->value, node->have_value} });
			if (l == r) return;
			print_tree(node->left, l, get_mid(l, r), 2 * pos);
			print_tree(node->right, get_mid(l, r) + 1, r, (2 * pos) + 1);
		}

		void print()
		{
			test_print.clear();
			print_tree(m_root, 0, m_real_size - 1);
			sort(test_print.begin(), test_print.end());
			int ff = 2;
			for (int i = 0; i < test_print.size(); i++)
			{
				if (i == ff - 1)
				{
					std::cout << std::endl;
					ff *= 2;
				}
				std::cout << "{" << test_print[i].first  << "," << test_print[i].second.first << ","<< test_print[i].second.second << "} ";
			}
			std::cout << std::endl;
		}

		// for debugging
	};


	template<typename T, typename container = std::vector<T> >
	typename segment_tree<T, container>::function segment_tree<T, container>::plus_func = [](const T &first,const T &second)
	{
		return first + second;
	};

}


#endif