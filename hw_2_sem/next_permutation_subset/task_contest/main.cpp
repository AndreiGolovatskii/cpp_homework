#include <algorithm>
#include <iostream>
#include <memory>


namespace {
namespace NMmaintence {
    template<typename T>
    struct Node {
        typedef size_t size_type;
        typedef T value_type;

        value_type val;
        value_type subSum;
        value_type extValue;
        value_type minValue;
        value_type maxValue;

        Node<T>* left;
        Node<T>* right;
        Node<T>* parent;
        size_type size;
        bool needRev;
        bool needSet;
        bool orderedDir;
        bool orderedRev;

        template<typename U>
        Node<T>(U&& value)
            : val(value), subSum(value), extValue(0), 
            minValue(value), maxValue(value),
            left(nullptr), right(nullptr), parent(nullptr), size(1), 
            needRev(false), needSet(false), orderedDir(true), orderedRev(true) {}
    };

    template<typename T, typename U, typename NodeAllocatorType>
    inline Node<T>* GetNode(U&& value, NodeAllocatorType& nodeAlloc) {
        using AllocatorTraits = std::allocator_traits<NodeAllocatorType>;

        Node<T>* n = AllocatorTraits::allocate(nodeAlloc, 1);
        AllocatorTraits::construct(nodeAlloc, n, value);
        return n;
    }

    template<typename T, typename NodeAllocatorType>
    inline void DestroyNode(Node<T>* node, NodeAllocatorType nodeAlloc) {
        using AllocatorTraits = std::allocator_traits<NodeAllocatorType>;

        AllocatorTraits::destroy(nodeAlloc, node);
        AllocatorTraits::deallocate(nodeAlloc, node, 1);
    }
        
    template<typename T, typename NodeAllocatorType>
    inline void RecursiveDestroy(Node<T>* node, NodeAllocatorType& nodeAlloc) {
        if (node) {
            RecursiveDestroy(node->left, nodeAlloc);
            RecursiveDestroy(node->right, nodeAlloc);
            DestroyNode(node, nodeAlloc);
        }
    }

    template<typename T>
    inline typename Node<T>::size_type NodeSize(Node<T>* v) {
        return v ? v->size : 0;
    }

    template<typename T>
    inline typename Node<T>::size_type NodeValue(Node<T>* v) {
        return v ? v->val : 0;
    }

    template<typename T>
    inline typename Node<T>::size_type NodeSubSum(Node<T>* v) {
        return v ? v->subSum : 0;
    }

    template<typename T>
    inline void NodePush(Node<T>* v) {
        if (!v) {
            return;
        }
        if (v->needSet) {
            v->val = v->maxValue = v->minValue = v->extValue;
            if (v->left) {
                v->left->extValue = v->extValue;
                v->left->needSet = true;
            }
            if (v->right) {
                v->right->extValue = v->extValue;
                v->right->needSet = true;
            }

            v->orderedDir = v->orderedRev = true;
            v->subSum = v->extValue * static_cast<typename Node<T>::value_type>(v->size);
            v->needSet = false;
            v->extValue = 0;
        } else {
            v->val += v->extValue;
            v->minValue += v->extValue;
            v->maxValue += v->extValue;
            if (v->left) {
                v->left->extValue += v->extValue;      
            }
            if (v->right) {
               v->right->extValue += v->extValue; 
            }
            v->subSum += v->extValue * static_cast<typename Node<T>::value_type>(v->size); 
            v->extValue = 0;
        }

        if (v->needRev) {
            std::swap(v->left, v->right);
            std::swap(v->orderedDir, v->orderedRev);

            if (v->left) {
                v->left->needRev ^= 1;
            }
            if (v->right) {
                v->right->needRev ^= 1;
            }
            v->needRev = false;
        }
    }

    template<typename T>
    inline void NodeOrderUpdate(Node<T>* v) {
        v->orderedDir = v->orderedRev = true;
        if (v->left) {
            if (!v->left->orderedDir || v->left->maxValue > v->val) {
                v->orderedDir = false;  
            }
            if (!v->left->orderedRev || v->left->minValue < v->val) {
                v->orderedRev = false;
            }
        }
        if (v->right) {
            if (!v->right->orderedDir || v->right->minValue < v->val) {
                v->orderedDir = false;
            }
            if (!v->right->orderedRev || v->right->maxValue > v->val) {
                v->orderedRev = false;
            }
        }
    } 

    template<typename T>
    inline void NodeUpdate(Node<T>* v) {
        if (v) {
            NodePush(v->left);
            NodePush(v->right);
            
            NodeOrderUpdate(v); 

            v->size = NodeSize(v->left) + NodeSize(v->right) + 1;
            v->subSum = NodeSubSum(v->left) + NodeSubSum(v->right) + v->val;
           
            v->minValue = v->maxValue = v->val;

            if (v->left) {
                v->minValue = std::min(v->minValue, v->left->minValue);
                v->maxValue = std::max(v->maxValue, v->left->maxValue);
            }
            if (v->right) {
                v->minValue = std::min(v->minValue, v->right->minValue);
                v->maxValue = std::max(v->maxValue, v->right->maxValue);
            }
        }
    }

    template<typename T>
    inline void RotateLeft(Node<T>* v) {
        Node<T>* vParent = v->parent;
        Node<T>* right = v->right;

        if (vParent) {
            if (IsLeftChild(v)) {
                vParent->left = right;
            } else {
                vParent->right = right;
            }
        }
        Node<T> *tmp = right->left;
        right->left = v;

        v->right = tmp;

        UpdateParent(v->right, v);
        UpdateParent(v, right);
        UpdateParent(right, vParent);

        NodeUpdate(v);
        NodeUpdate(right);
        NodeUpdate(vParent);
    }

    template<typename T>
    inline void RotateRight(Node<T>* v) {
        Node<T>* vParent = v->parent;
        Node<T>* left = v->left;

        if (vParent) {
            if (IsRightChild(v)) {
                vParent->right = left;
            } else {
                vParent->left = left;
            }
        }
        Node<T> *tmp = left->right;
        left->right = v;

        v->left = tmp;

        UpdateParent(v->left, v);
        UpdateParent(v, left);
        UpdateParent(left, vParent);

        NodeUpdate(v);
        NodeUpdate(left);
        NodeUpdate(vParent);
    }

    template<typename T, typename U>
    inline void UpdateParent(Node<T>* v, U parent) {
        if (v) {
            v->parent = parent;
        }
    }
    
    template<typename T>
    inline bool HasParent(Node<T>* v) {
        return v->parent;
    }

    template<typename T>
    inline bool HasGrand(Node<T>* v) {
        return v->parent->parent;
    }

    template<typename T>
    inline bool IsLeftChild(Node<T>* v) {
        return v->parent->left == v;
    }

    template<typename T>
    inline bool IsRightChild(Node<T>* v) {
        return v->parent->right == v;
    }

    template<typename T>
    inline Node<T>* Splay(Node<T>* v) {
        while (HasParent(v)) {
            if (IsLeftChild(v)) {
                if (!HasGrand(v)) {
                    RotateRight(v->parent);
                } else if(IsLeftChild(v->parent)) {
                    RotateRight(v->parent->parent);
                    RotateRight(v->parent);
                } else {
                    RotateRight(v->parent);
                    RotateLeft(v->parent);
                }
            } else {
                if (!HasGrand(v)) {
                    RotateLeft(v->parent);
                } else if (IsRightChild(v->parent)) {
                    RotateLeft(v->parent->parent);
                    RotateLeft(v->parent);
                } else {
                    RotateLeft(v->parent);
                    RotateRight(v->parent);
                }
            }
        }
        return v;
    }

    template<typename T, typename NodeAllocatorType>
    Node<T>* Build(NodeAllocatorType& nodeAlloc, typename Node<T>::size_type count, 
                   const typename Node<T>::value_type& value, Node<T>* parent = nullptr) {
        if (count == 0) {
            return nullptr;
        }
        Node<T>* r = GetNode<T>(value, nodeAlloc);
        r->parent = parent;
        r->left = Build<T>(nodeAlloc, (count) / 2, value, r);
        r->right = Build<T>(nodeAlloc, (count - 1) / 2, value, r);
        NodeUpdate(r);
        return r;
    }

    template<typename T>
    inline Node<T>* Find(Node<T>* root, typename Node<T>::size_type index) {
        NodePush(root);
        while (NodeSize(root->left) != index) {
            if (NodeSize(root->left) > index) {
                root = root->left;
            } else {
                index -= NodeSize(root->left) + 1;
                root = root->right;
            }
            NodePush(root);
        }
        return root;
    }

    template<typename T>
    inline Node<T>* MakeRoot(Node<T>* root, typename Node<T>::size_type index) {
        return Splay(Find(root, index)); 
    }

    template<typename T>
    inline Node<T>* Merge(Node<T>* lhs, Node<T> *rhs) {
        NodePush(lhs);
        NodePush(rhs);
        if (!lhs || !rhs) {
            return lhs ? lhs : rhs;
        }
        typename Node<T>::size_type lsh_size = NodeSize(lhs);
        lhs = MakeRoot(lhs, lsh_size - 1);
        lhs->right = rhs;
        UpdateParent(rhs, lhs);
        NodeUpdate(lhs);

        return lhs;
    }

    template<typename T>
    inline std::pair<Node<T>*, Node<T>*> Split(Node<T>* root, 
                                               typename Node<T>::size_type leftSize) {
        if (!root) {
            return std::make_pair(nullptr, nullptr);
        }

        if (leftSize == NodeSize(root)) {
            return std::make_pair(root, nullptr);
        }
        root = MakeRoot(root, leftSize);

        Node<T>* l = root->left;
        UpdateParent(l, nullptr);

        Node<T>* r = root;
        r->left = nullptr;
        NodeUpdate(r);

        return std::make_pair(l, r);
    }

    template<typename T, typename U, typename NodeAllocatorType>
    inline Node<T>* Insert(Node<T>* root, typename Node<T>::size_type pos, U&& val,
                           NodeAllocatorType& nodeAlloc) {

        Node<T>* new_node = GetNode<T>(std::move(val), nodeAlloc);
        auto [l, r] = Split(root, pos);
        new_node->left = l;
        new_node->right = r;

        UpdateParent(l, new_node);
        UpdateParent(r, new_node);

        NodeUpdate(new_node);

        return new_node;
    }

    template<typename T, typename NodeAllocatorType>
    inline Node<T>* Erase(Node<T>* root, typename Node<T>::size_type pos,
                          NodeAllocatorType& nodeAlloc) {
        root = MakeRoot(root, pos);
        
        Node<T>* l = root->left;
        UpdateParent(l, nullptr);

        Node<T>* r = root->right;
        UpdateParent(r, nullptr);

        DestroyNode(root, nodeAlloc);
        return Merge(l, r);
    }

    template<typename T, typename F>
    inline Node<T>* SubsegmentOperation(Node<T>* root, typename Node<T>::size_type start_pos,
                                 typename Node<T>::size_type len, 
                                 F func) {
        auto [left, tmp] = Split(root, start_pos);
        auto [target, right] = Split(tmp, len);

        target = func(target);

        return Merge(Merge(left, target), right);
    }

    template<typename T>
    inline typename Node<T>::size_type GetOrderedSuffixLen(Node<T>* root, bool dirOrder) {
        typename Node<T>::value_type requiredVal;
        bool isVal = false;

        typename Node<T>::size_type len = 0;
        NodePush(root);
        if (dirOrder) {
            while (root) {
                if (isVal && root->val > requiredVal) {
                    root = root->right;
                    NodePush(root);
                    continue;
                }

                NodePush(root->right);
                if (!root->right || (root->right->orderedDir && 
                                     (!isVal || root->right->maxValue <= requiredVal) &&
                                     root->right->minValue >= root->val)) {
                    requiredVal = root->val;
                    isVal = true;
                    len += 1 + NodeSize(root->right);

                    root = root->left;
                    NodePush(root);
                    continue;
                }
                root = root->right;
                NodePush(root);
            }
        } else { 
            while (root) {
                if (isVal && root->val < requiredVal) {
                    root = root->right;
                    NodePush(root);
                    continue;
                }

                NodePush(root->right);
                if (!root->right || (root->right->orderedRev && 
                                     (!isVal || root->right->minValue >= requiredVal) &&
                                     root->right->maxValue <= root->val)) {
                    requiredVal = root->val;
                    isVal = true;
                    len += 1 + NodeSize(root->right);

                    root = root->left;
                    NodePush(root);
                    continue;
                }
                root = root->right;
                NodePush(root);
            }
        }
        return len;
    }

    template<typename T>
    typename Node<T>::size_type GLValPrefixLen(Node<T>* root, 
                                               typename Node<T>::value_type& val,
                                               bool dirOrder) {
        NodePush(root);
        typename Node<T>::size_type len = 0;
        while(root) {
            if ((dirOrder && root->val < val) ||
                (!dirOrder && root->val > val)) {
                len += 1 + NodeSize(root->left);
                root = root->right;
            } else {
                root = root->left;
            }
            NodePush(root);
        }
        return len;
    }

    template<typename T>
    inline Node<T>* Permutation(Node<T>* root, bool isNext) {
        if (!root) {
            return nullptr;
        }
        if ((isNext && root->orderedRev) ||
            (!isNext && root->orderedDir)) {
            root->needRev ^= 1;
            return root;
        }

        typename Node<T>::size_type suffixLen = GetOrderedSuffixLen(root, !isNext);
        auto [l, r] = Split(root, NodeSize(root) - suffixLen);
        l = MakeRoot(l, NodeSize(l) - 1);
        NodePush(l);

        typename Node<T>::size_type swapPos = GLValPrefixLen(r, l->val, !isNext) - 1;
        r = MakeRoot(r, swapPos);
        NodePush(r);

        std::swap(l->val, r->val);
        NodeUpdate(r);
        NodeUpdate(l);

        r->needRev = true;
        return Merge(l, r);
    }
}
}


template<typename T, class Allocator = std::allocator<T>>
class TSplayTree {
public:
    typedef NMmaintence::Node<T> node_type;
    typedef T value_type;
    typedef Allocator allocator_type;
    typedef typename node_type::size_type size_type;
    
    explicit TSplayTree(const Allocator& alloc=Allocator())
        :nodeAlloc_(alloc), root_(nullptr) {}
    
    TSplayTree(size_type count, 
               const value_type& value = value_type(), 
               const Allocator& alloc=Allocator())
        :nodeAlloc_(alloc), root_(NMmaintence::Build<value_type>(nodeAlloc_, count, value)) {}

    TSplayTree(const TSplayTree& src) = delete;
    TSplayTree& operator=(const TSplayTree& src) = delete;
    
    value_type& operator[](size_type i) {
        root_ = MakeRoot(root_, i);
        return root_->val;
    }
    
    const value_type& operator[](size_type i) const {
        root_ = NMmaintence::MakeRoot(root_, i);
        return root_->val;
    }

    template<typename U>
    void Insert(size_type pos, U&& value) {
        root_ = NMmaintence::Insert(root_, pos, std::move(value), nodeAlloc_);
    }

    void Erase(size_type pos) {
        root_ = NMmaintence::Erase(root_, pos, nodeAlloc_);
    }

    value_type SubsegmentSum(size_type start_pos, size_type len) {
        value_type res;
        root_ = NMmaintence::SubsegmentOperation(root_, start_pos, len, 
                [&res](node_type* t) -> node_type* {
                    res = NMmaintence::NodeSubSum(t);
                    return t;
                });
        return res;
    }

    value_type SubsegmentMin(size_type start_pos, size_type len) {
        value_type res;
        root_ = NMmaintence::SubsegmentOperation(root_, start_pos, len, 
                [&res](node_type* t) -> node_type* {
                    res = t->minValue;
                    return t;
                });
        return res;
    }

    value_type SubsegmentMax(size_type start_pos, size_type len) {
        value_type res;
        root_ = NMmaintence::SubsegmentOperation(root_, start_pos, len, 
                [&res](node_type* t) -> node_type* {
                    res = t->maxValue;
                    return t;
                });
        return res;
    }

    void SubsegmentSet(size_type start_pos, size_type len, value_type value) {
        root_ = NMmaintence::SubsegmentOperation(root_, start_pos, len, 
                [&value](node_type* t) -> node_type* {
                    t->extValue = value;
                    t->needSet = true;
                    return t;
                });
    }

    void SubsegmentAdd(size_type start_pos, size_type len, value_type value) {
        root_ = NMmaintence::SubsegmentOperation(root_, start_pos, len, 
                [&value](node_type* t) -> node_type* {
                    t->extValue += value;
                    return t;
                });
    }

    void SubsegmentReverse(size_type start_pos, size_type len) {
        root_ = NMmaintence::SubsegmentOperation(root_, start_pos, len, 
                [](node_type* t) -> node_type* {
                    t->needRev ^= 1;
                    return t;
                });
    }

    void SubsegmentNextPermutation(size_type start_pos, size_type len) {
        root_ = NMmaintence::SubsegmentOperation(root_, start_pos, len,
                [](node_type* t) -> node_type* {
                    return NMmaintence::Permutation(t, true);
                });
    }

    void SubsegmentPrevPermutation(size_type start_pos, size_type len) {
        root_ = NMmaintence::SubsegmentOperation(root_, start_pos, len,
                [](node_type* t) -> node_type* {
                    return NMmaintence::Permutation(t, false);
                });
    }

    size_type size() const {
        return NMmaintence::NodeSize(root_);
    }

    ~TSplayTree() {
        NMmaintence::RecursiveDestroy(root_, nodeAlloc_);
    }
private:
    using NodeAllocatorType_ = typename std::allocator_traits<Allocator>::
                               template rebind_alloc<NMmaintence::Node<T>>;
    using AllocatorTraits_ = typename std::allocator_traits<NodeAllocatorType_>;

    NodeAllocatorType_ nodeAlloc_;
    mutable NMmaintence::Node<value_type>* root_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    size_t n;
    std::cin >> n;
    
    TSplayTree<int64_t> s(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> s[i];
    }
    size_t q;
    std::cin >> q;
    for (size_t i = 0; i < q; ++i) {
        int t;
        std::cin >> t;
        if (t == 1) {
            size_t l, r;
            std::cin >> l >> r;
            std::cout << s.SubsegmentSum(l, r - l + 1) << "\n";
        } else if (t == 2) {
            int x;
            size_t pos;
            std::cin >> x >> pos;
            s.Insert(pos, x);
        } else if (t == 3) {
            size_t pos;
            std::cin >> pos;
            s.Erase(pos);
        } else if (t == 4) {
            int x;
            size_t l, r;
            std::cin >> x >> l >> r;
            s.SubsegmentSet(l, r - l + 1, x);
        } else if (t == 5) {
            int x;
            size_t l, r;
            std::cin >> x >> l >> r;
            s.SubsegmentAdd(l, r - l + 1, x);
        } else if (t == 6) {
            size_t l, r;
            std::cin >> l >> r;
            s.SubsegmentNextPermutation(l, r - l + 1);
        } else if (t == 7) {
            size_t l, r;
            std::cin >> l >> r;
            s.SubsegmentPrevPermutation(l, r - l + 1);
        }
    }

    for (size_t i = 0; i < s.size(); ++i) {
        std::cout << s[i] << " ";
    }
    std::cout << std::endl;
}
