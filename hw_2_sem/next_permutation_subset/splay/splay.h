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
        value_type addValue;
        value_type setValue; 
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
            : val(value), subSum(value), addValue(0), setValue(0), 
            minValue(value), maxValue(value),
            left(nullptr), right(nullptr), parent(nullptr), size(1), 
            needRev(false), needSet(false), orderedDir(true), orderedRev(true) {}
    };

    template<typename T, typename U, typename NodeAllocatorType>
    Node<T>* GetNode(U&& value, NodeAllocatorType& nodeAlloc) {
        using AllocatorTraits = std::allocator_traits<NodeAllocatorType>;

        Node<T>* n = AllocatorTraits::allocate(nodeAlloc, 1);
        AllocatorTraits::construct(nodeAlloc, n, value);
        return n;
    }

    template<typename T, typename NodeAllocatorType>
    void DestroyNode(Node<T>* node, NodeAllocatorType nodeAlloc) {
        using AllocatorTraits = std::allocator_traits<NodeAllocatorType>;

        AllocatorTraits::destroy(nodeAlloc, node);
        AllocatorTraits::deallocate(nodeAlloc, node, 1);
    }
        
    template<typename T, typename NodeAllocatorType>
    void RecursiveDestroy(Node<T>* node, NodeAllocatorType& nodeAlloc) {
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
    inline bool NodeOrderedDir(Node<T>* v) {
        return !v || v->orderedDir;
    }

    template<typename T>
    inline bool NodeOrderedRev(Node<T>* v) {
        return !v || v->orderedDir;
    }
    
    template<typename T>
    inline void NodePush(Node<T>* v) {
        if (v) {
            {
                v->val += v->addValue;
                if (v->left) {
                    v->left->addValue += v->addValue;      
                }

                if (v->right) {
                   v->right->addValue += v->addValue; 
                }
                v->subSum += v->addValue * static_cast<typename Node<T>::value_type>(v->size); 
                v->minValue += v->addValue;
                v->maxValue += v->addValue;
                v->addValue = 0;
            }

            if (v->needSet) {
                v->val = v->setValue; 
                if (v->left) {
                    v->left->setValue = v->setValue;
                }
                if (v->right) {
                    v->right->setValue = v->setValue;
                }

                v->maxValue = v->minValue = v->setValue;
                v->orderedDir = v->orderedRev = true;
                v->subSum = v->setValue * static_cast<typename Node<T>::value_type>(v->size);
                v->needSet = false;
                v->setValue = 0;
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
    }

    template<typename T>
    inline void NodeOrderUpdate(Node<T>* v) {
        v->orderedDir = v->orderedRev = false;
        if (NodeOrderedDir(v->left) && NodeOrderedDir(v->right)) {
            if (v->left && v->left->maxValue > v->val) {
                return;
            }
            if (v->right && v->right->minValue < v->val) {
                return;
            }
            v->orderedDir = true;
        }

        if (NodeOrderedRev(v->left) && NodeOrderedRev(v->right)) {
            if (v->left && v->left->minValue < v->val) {
                return;
            }
            if (v->right && v->right->maxValue > v->val) {
                return;
            }
            v->orderedRev = true;
        }
    } 

    template<typename T>
    inline void NodeUpdate(Node<T>* v) {
        if (v) { // TODO
            NodePush(v);
            NodePush(v->left);
            NodePush(v->right);
            
            NodeOrderUpdate(v); 

            v->size = NodeSize(v->left) + NodeSize(v->right) + 1;
            v->subSum = NodeSubSum(v->left) + NodeSubSum(v->right) + v->val;
           
            Node<T>* l = v;
            Node<T>* r = v;

            if (v->left) {
                l = v->left;
            }
            if (v->right) {
                r = v->right;
            }

            v->minValue = std::min(v->val, std::min(l->minValue, r->minValue));
            v->maxValue = std::max(v->val, std::max(l->maxValue, r->maxValue));
        }
    }

    template<typename T>
    inline void RotateLeft(Node<T>* v) {
        Node<T>* vParent = v->parent;
        NodePush(vParent);
        NodePush(v);

        Node<T>* right = v->right;
        NodePush(right);


        if (vParent) {
            if (vParent->left == v) {
                vParent->left = right;
            } else {
                vParent->right = right;
            }
        }
        Node<T> *tmp = right->left;
        NodePush(tmp);

        right->left = v;

        v->right = tmp;
        v->parent = right;
        right->parent = vParent;

        if (v->right) 
            v->right->parent = v;

        NodeUpdate(v);
        NodeUpdate(right);
        NodeUpdate(vParent);
    }

    template<typename T>
    inline void RotateRight(Node<T>* v) {
        Node<T>* vParent = v->parent;
        NodePush(vParent);
        NodePush(v);

        Node<T>* left = v->left;
        NodePush(left);

        if (vParent) {
            if (vParent->right == v) {
                vParent->right = left;
            } else {
                vParent->left = left;
            }
        }
        Node<T> *tmp = left->right;
        NodePush(tmp);
        left->right = v;

        v->left = tmp;

        v->parent = left;
        left->parent = vParent;

        if (v->left) 
            v->left->parent = v;

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

