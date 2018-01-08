

#ifndef ARI_NODE_TRAITS_HPP
#define ARI_NODE_TRAITS_HPP

namespace ari {

template <typename TNode>
struct node_traits {
	using value_type      = typename TNode::value_type;
	using difference_type = typename TNode::difference_type;
	using pointer         = typename TNode::node_pointer;
	using reference       = typename TNode::node_reference;
};

}

#endif