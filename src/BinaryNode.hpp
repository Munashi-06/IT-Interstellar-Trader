#pragma once

#include <memory>

// Clase Base para ABs
template <typename Key>
class BinNode
{
public:
    using KeyType = Key;
    using KeyRefType = Key&;
    using KeyCRefType = const Key&;
    using KeyURefType = Key&&;
    using PointerType = std::shared_ptr<BinNode<Key>>;
    using PointerRefType = std::shared_ptr<BinNode<Key>>&;
    
    BinNode(KeyURefType k) noexcept;

    BinNode(KeyURefType k, PointerType l, PointerType r) noexcept;

    virtual ~BinNode() = default;

    // Non-copyable, non-movable, and non-assignable
    BinNode(const BinNode& node) = delete;
    BinNode(BinNode&& node) = delete;
    BinNode operator = (BinNode node) = delete;

    KeyRefType get_key() noexcept;

    KeyCRefType get_key() const noexcept;

    PointerRefType get_lchild() noexcept;

    PointerRefType get_rchild() noexcept;

    bool is_leaf() const noexcept;

private:
    KeyType key;
    PointerType lchild;
    PointerType rchild;
};

template <typename Key>
BinNode<Key>::BinNode(KeyURefType k) noexcept
    : key{k}, lchild{nullptr}, rchild{nullptr}
{
    // empty
}

template <typename Key>
BinNode<Key>::BinNode(KeyURefType k, PointerType l, PointerType r) noexcept
    : key{k}, lchild{l}, rchild{r}
{
    // empty
}

template <typename Key>
Key& BinNode<Key>::get_key() noexcept
{
    return this->key;
}

template <typename Key>
const Key& BinNode<Key>::get_key() const noexcept
{
    return this->key;
}

template <typename Key>
std::shared_ptr<BinNode<Key>>& BinNode<Key>::get_lchild() noexcept
{
    return this->lchild;
}

template <typename Key>
std::shared_ptr<BinNode<Key>>& BinNode<Key>::get_rchild() noexcept
{
    return this->rchild;
}

template <typename Key>
bool BinNode<Key>::is_leaf() const noexcept
{
    return this->lchild == nullptr && this->rchild == nullptr;
}

// Helper functions
template <typename BinNodeType>
inline typename BinNodeType::KeyRefType K(std::shared_ptr<BinNodeType> node) noexcept
{
    return node->get_key();
}

template <typename BinNodeType>
inline typename BinNodeType::KeyRefType K(BinNodeType* node) noexcept
{
    return node->get_key();
}

template <typename BinNodeType>
inline typename BinNodeType::PointerRefType L(std::shared_ptr<BinNodeType> node) noexcept
{
    return node->get_lchild();
}

template <typename BinNodeType>
inline typename BinNodeType::PointerRefType L(BinNodeType* node) noexcept
{
    return node->get_lchild();
}

template <typename BinNodeType>
inline typename BinNodeType::PointerRefType R(std::shared_ptr<BinNodeType> node) noexcept
{
    return node->get_rchild();
}

template <typename BinNodeType>
inline typename BinNodeType::PointerRefType R(BinNodeType* node) noexcept
{
    return node->get_rchild();
}