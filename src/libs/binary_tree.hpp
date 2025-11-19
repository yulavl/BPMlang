
template<typename value_type>
struct TreeNode {
	value_type data;
	TreeNode<value_type> *left;
	TreeNode<value_type> *right;
	TreeNode() = default;
	TreeNode(TreeNode<value_type>&& other)
	: data(std::exchange(other.data, 0)),
	  left(std::exchange(other.left, nullptr)),
	  right(std::exchange(other.right, nullptr))
	{}
	TreeNode(const TreeNode<value_type>& other) {
		left = nullptr;
		right = nullptr;
		data = other.data;
		if(other.left != nullptr)
			left = new TreeNode<value_type>(other.left);
		if(other.right != nullptr)
			right = new TreeNode<value_type>(right.right);
	}
	void operator=(const TreeNode<value_type>& other) {
		left = nullptr;
		right = nullptr;
		data = other.data;
		if(other.left != nullptr) {
			if(left == nullptr) left = new TreeNode<value_type>;
			*left = *other.left;
		}
		if(other.right != nullptr) {
			if(right == nullptr) right = new TreeNode<value_type>;
			*right = *other.right;
		}
	}
	size_t size() const noexcept {
		size_t res {1};
		if(left != nullptr) res += left->size();
		if(right != nullptr) res += right->size();
		return res;
	}
	bool operator==(const TreeNode<value_type>& other) const {
		if(size() != other.size()) return false;
		if(data != other.data) return false;
		if(other.left != nullptr)
			if(*left != *other.left) return false;
		if(other.right != nullptr)
			if(*right != *other.right) return false;
		return true;
	}
	bool operator!=(const TreeNode<value_type>& other) {
		return !this->operator==(other);
	}
	TreeNode(value_type&& _value) : data(std::move(_value)) {
		left = nullptr;
		right = nullptr;
	}
	TreeNode(const value_type& _value) : data(_value) {
		left = nullptr;
		right = nullptr;
	}
};

template<typename T, typename size_type = size_t>
class BinaryTree {
public:
	using value_type = T;
	BinaryTree() : root(nullptr) {};
	BinaryTree(const BinaryTree<T, size_type>& other) {
		constexpr bool has_eq = requires(T a, T b){ 
    		a == b;
  		};
  		static_assert(has_eq, "BinaryTree<T>, T need to be have operator==");
		root = new TreeNode<value_type>;
		*root = *other.root;
	}
	BinaryTree(BinaryTree<T, size_type>&& other)
	: root(std::exchange(other.root, nullptr))
	{}

	void operator=(const BinaryTree<T, size_type>& other) {
		this->~BinaryTree();
		root = new TreeNode<value_type>;
		*root = *other.root;
	}

	void operator=(BinaryTree<T, size_type>&& other) {
		root = std::exchange(other.root, nullptr);
	}

	bool operator==(const BinaryTree<T, size_type>& other) const {
		if(root == nullptr && other.root == nullptr) return true;
		if(root == nullptr && other.root != nullptr) return false;
		if(root != nullptr && other.root == nullptr) return false;
		return *root == *other.root;
	}

	bool operator!=(const BinaryTree<T, size_type>& other) {
		return !this->operator==(other);
	}

	void insert_data(const value_type& val, TreeNode<value_type>** node) {
		assert(node != nullptr);
		if(*node == nullptr){
	        *node = new TreeNode(val);
	    } else {
	        (*node)->data = val;
	    }
	}

	void insert_right(const value_type& val, TreeNode<value_type>* node) {
	    if(node->right == nullptr){
	        node->right = new TreeNode(val);
	    } else{
	        insert_data(val, &(node->right));
	    }
	}
	void insert_left(const value_type& val, TreeNode<value_type>* node) {
	    if(node->left == nullptr){
	        node->left = new TreeNode(val);
	    } else{
	        insert_data(val, &(node->left));
	    }
	}
	~BinaryTree() {
		delete_node(root);
	}
	TreeNode<value_type>* get_root() const noexcept {
		return root;
	}
	TreeNode<value_type>** get_root_ptr() noexcept {
		return &root;
	}
private:
	void delete_node(TreeNode<value_type>* node) {
		if(node != nullptr) {
			delete_node(node->left);
			delete_node(node->right);
			delete node;
		}
	}
	TreeNode<value_type>* root = nullptr;
};

template<typename T>
void dump_tree(size_t level, TreeNode<T>* node) {
	if(node == nullptr) return;
	for(size_t i = 0ULL;i < level;++i) {
		std::cout << "    ";
	}
	std::cout << '{' << std::endl;
	for(size_t i = 0ULL;i < level + 1;++i) {
		std::cout << "    ";
	}
	std::cout << node->data << ',' << std::endl;
	dump_tree(level + 1ULL, node->left);
	dump_tree(level + 1ULL, node->right);
	for(size_t i = 0ULL;i < level;++i) {
		std::cout << "    ";
	}
	std::cout << '}' << std::endl;
}