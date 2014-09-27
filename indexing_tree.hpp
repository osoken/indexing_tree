/* --
Copyright (c) 2012--2014 Takeshi OSOEKAWA

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-- */

#ifndef INDEXING_TREE_HPP_
#define INDEXING_TREE_HPP_

#include <stdexcept>

#ifdef INDEXING_TREE_USES_TR1
#  include <type_traits>
#else
#  ifdef INDEXING_TREE_USES_BOOST
#    include <boost/type_traits.hpp>
#  endif
#endif

namespace osoken
{
#ifdef INDEXING_TREE_USES_TR1
  namespace integral_trait_name_space = std::tr1;
#else
# ifdef INDEXING_TREE_USES_BOOST
    namespace integral_trait_name_space = boost;
# else
    namespace integral_trait_name_space
    {
      template<class T>
      struct is_integral
      {
        static const bool value_ = false;
      };
      template<> struct is_integral<char> { static const bool value_ = true; };
      template<> struct is_integral<int> { static const bool value_ = true; };
      template<> struct is_integral<short> { static const bool value_ = true; };
      template<> struct is_integral<long> { static const bool value_ = true; };
      template<> struct is_integral<unsigned char> { static const bool value_ = true; };
      template<> struct is_integral<unsigned int> { static const bool value_ = true; };
      template<> struct is_integral<unsigned short> { static const bool value_ = true; };
      template<> struct is_integral<unsigned long> { static const bool value_ = true; };
      template<> struct is_integral<signed char> { static const bool value_ = true; };
      template<> struct is_integral<bool> { static const bool value_ = true; };
    }
# endif
#endif

template<class T, class Alloc = ::std::allocator<T> >
class indexing_tree
{
public:
  typedef typename Alloc::reference reference;
  typedef typename Alloc::pointer pointer;
  typedef typename Alloc::const_reference const_reference;
  typedef typename Alloc::const_pointer const_pointer;
  typedef Alloc allocator_type;
  typedef T value_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
private:
  template<class U>
  struct node
  {
    node *next_, *prev_, *left_, *right_, *parent_;
    size_type size_;
    U value_;
  };
  typedef node<T> node_type;
  typedef typename allocator_type::template rebind< node_type >::other node_allocator_type;

  class iterator_base : public std::iterator<std::random_access_iterator_tag, typename indexing_tree::value_type, typename indexing_tree::difference_type, typename indexing_tree::pointer, typename indexing_tree::reference>
  {
  public:
    bool operator == (const iterator_base& i) const;
    bool operator != (const iterator_base& i) const;
    typename indexing_tree::difference_type operator - (const iterator_base& i) const;
  protected:
    iterator_base(const iterator_base& i);
    iterator_base(typename indexing_tree::node_type* node);
    iterator_base();

    typename indexing_tree::node_type *node_;

    void advance_forward(difference_type diff);
    typename indexing_tree::size_type index_of() const;
  };
public:

  class iterator : public iterator_base
  {
  public:
    iterator(const iterator& i);
    iterator();
    iterator& operator++();
    iterator operator++(int);
    iterator& operator--();
    iterator operator--(int);
    iterator operator+(difference_type diff) const;
    iterator operator-(difference_type diff) const;
    iterator& operator+=(difference_type diff);
    iterator& operator-=(difference_type diff);
    bool operator < (const iterator& i) const;
    bool operator <= (const iterator& i) const;
    bool operator > (const iterator& i) const;
    bool operator >= (const iterator& i) const;
    typename indexing_tree::reference operator [] (difference_type diff);
    typename indexing_tree::const_reference operator [] (difference_type diff) const;
    typename indexing_tree::reference operator*();
    typename indexing_tree::const_reference operator*() const;
    typename indexing_tree::pointer operator->();
    typename indexing_tree::const_pointer operator->() const;
  private:
    iterator(typename indexing_tree::node_type* node);

    friend class indexing_tree;
  };

  class const_iterator : public iterator_base
  {
  public:
    const_iterator(const iterator& i);
    const_iterator(const const_iterator& i);
    const_iterator();
    const_iterator& operator++();
    const_iterator operator++(int);
    const_iterator& operator--();
    const_iterator operator--(int);
    const_iterator operator+(difference_type diff) const;
    const_iterator operator-(difference_type diff) const;
    const_iterator& operator+=(difference_type diff);
    const_iterator& operator-=(difference_type diff);
    bool operator < (const const_iterator& i) const;
    bool operator <= (const const_iterator& i) const;
    bool operator > (const const_iterator& i) const;
    bool operator >= (const const_iterator& i) const;
    typename indexing_tree::const_reference operator [] (difference_type diff) const;
    typename indexing_tree::const_reference operator*() const;
    typename indexing_tree::const_pointer operator->() const;
  private:
    const_iterator(typename indexing_tree::node_type* node);

    friend class indexing_tree;
  };

  class reverse_iterator: public iterator_base
  {
  public:
    reverse_iterator(const reverse_iterator& i);
    reverse_iterator();
    reverse_iterator& operator++();
    reverse_iterator operator++(int);
    reverse_iterator& operator--();
    reverse_iterator operator--(int);
    reverse_iterator operator+(difference_type diff) const;
    reverse_iterator operator-(difference_type diff) const;
    reverse_iterator& operator+=(difference_type diff);
    reverse_iterator& operator-=(difference_type diff); 
    bool operator < (const reverse_iterator& i) const;
    bool operator <= (const reverse_iterator& i) const;
    bool operator > (const reverse_iterator& i) const;
    bool operator >= (const reverse_iterator& i) const;
    typename indexing_tree::reference operator [] (difference_type diff);
    typename indexing_tree::const_reference operator [] (difference_type diff) const;
    typename indexing_tree::reference operator*();
    typename indexing_tree::const_reference operator*() const;
    typename indexing_tree::pointer operator->();
    typename indexing_tree::const_pointer operator->() const;
  private:
    reverse_iterator(typename indexing_tree::node_type* node);

    friend class indexing_tree;
  };
  class const_reverse_iterator : public iterator_base
  {
  public:
    const_reverse_iterator(const reverse_iterator& i);
    const_reverse_iterator(const const_reverse_iterator& i);
    const_reverse_iterator();
    const_reverse_iterator& operator++();
    const_reverse_iterator operator++(int);
    const_reverse_iterator& operator--();
    const_reverse_iterator operator--(int);
    const_reverse_iterator operator+(difference_type diff) const;
    const_reverse_iterator operator-(difference_type diff) const;
    const_reverse_iterator& operator+=(difference_type diff);
    const_reverse_iterator& operator-=(difference_type diff);
    bool operator < (const const_reverse_iterator& i) const;
    bool operator <= (const const_reverse_iterator& i) const;
    bool operator > (const const_reverse_iterator& i) const;
    bool operator >= (const const_reverse_iterator& i) const;
    typename indexing_tree::const_reference operator [] (difference_type diff) const;
    typename indexing_tree::const_reference operator*() const;
    typename indexing_tree::const_pointer operator->() const;
  private:
    const_reverse_iterator(typename indexing_tree::node_type* const node);

    friend class indexing_tree;
  };
  friend class iterator_base;
  friend class iterator;
  friend class const_iterator;
  friend class reverse_iterator;
  friend class const_reverse_iterator;
  // member functions
  explicit indexing_tree(const Alloc& alloc = Alloc());
  explicit indexing_tree(size_type n, const T& x, const Alloc& alloc = Alloc());
  template<class InIter>
  indexing_tree(InIter first, InIter last, const Alloc& alloc = Alloc());
  indexing_tree(const indexing_tree& that);
  ~indexing_tree();

  indexing_tree& operator = (const indexing_tree& that);
  template<class InIter>
  void assign(InIter first, InIter last);
  void assign(size_type n, const T& x);
  Alloc get_allocator() const;

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;
  reverse_iterator rbegin();
  const_reverse_iterator rbegin() const;
  reverse_iterator rend();
  const_reverse_iterator rend() const;
  size_type size() const;
  size_type max_size() const;
  void resize(size_type sz, const T& x = T());
  size_type capacity() const;
  bool empty() const;
  void reserve(size_type n);

  reference operator [] (size_type n);
  const_reference operator [] (size_type n) const;
  const_reference at(size_type n) const;
  reference at(size_type n);
  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

  void push_back(const T& x);
  void pop_back();
  void push_front(const T& x);
  void pop_front();
  iterator insert(iterator position, const T& x);
  void insert(iterator position, size_type n, const T& x);
  template<class InIter>
  void insert(iterator position, InIter first, InIter last);
//  iterator insert(size_type position, const T& x);
//  iterator insert(size_type position, size_type n, const T& x);
  iterator erase(iterator position);
  iterator erase(iterator first, iterator last);
//  iterator erase(size_type position);
//  iterator erase(size_type position, size_type n);
  void swap(indexing_tree& that) throw();

  void clear();
private:
  node_type* sentinel_;
  allocator_type alloc_;
  node_allocator_type nodealloc_;

  void init_sentinel_();
  node_type* select(size_type n);
  void range_check_lt(size_type n) const;
  void range_check_leq(size_type n) const;
  void fix_up_incr(node_type* p);
  void fix_up_decr(node_type* p);
  void fix_up(node_type* p);
  bool is_balanced(node_type* p) const;
  void rebalance(node_type* p);
  void ll_rotation(node_type* p);
  void rr_rotation(node_type* p);
  void lr_rotation(node_type* p);
  void rl_rotation(node_type* p);
  node_type* newitem(const T& x);
  void deleteitem(node_type* p);
  void put_first_element(node_type* p);
  static bool is_sentinel(node_type* p);

  template<bool Is_integral, class InIter>
  class private_insert
  {
    friend class indexing_tree;
  public:
    private_insert(indexing_tree& that, iterator position, InIter first, InIter last);
  };

  template<class InIter>
  class private_insert<true,InIter>
  {
    friend class indexing_tree;
  public:
    private_insert(indexing_tree& that, iterator position, InIter first, InIter last);
  };
  // for debug
public:
  static std::ostream& print_node(std::ostream& s, iterator p)
  {
    node_type* pp = p.node_;
    return s << pp->prev_->value_ << " " << pp->left_->value_ << " [" << pp->left_->size_ << "] " << pp->value_ << " [" << pp->size_ << "] " << pp->parent_->value_ << " [" << pp->right_->size_ << "] " << pp->right_->value_ << " " << pp->next_->value_ << std::endl;
  }
};

//////////////////
// iterator_base
//////////////////
template<class T,class A>
inline bool indexing_tree<T,A>::iterator_base::operator == (const iterator_base& i) const
{
  return node_ == i.node_;
}

template<class T,class A>
inline bool indexing_tree<T,A>::iterator_base::operator != (const iterator_base& i) const
{
  return node_ != i.node_;
}


template<class T,class A>
inline typename indexing_tree<T,A>::difference_type indexing_tree<T,A>::iterator_base::operator - (const iterator_base& i) const
{
  return static_cast<difference_type>(index_of()) - static_cast<difference_type>(i.index_of());
}


template<class T,class A>
inline indexing_tree<T,A>::iterator_base::iterator_base(const iterator_base& i):
node_(i.node_)
{
}

template<class T,class A>
inline indexing_tree<T,A>::iterator_base::iterator_base(node_type* node):
node_(node)
{
}

template<class T,class A>
inline indexing_tree<T,A>::iterator_base::iterator_base()
{
}

template<class T,class A>
void indexing_tree<T,A>::iterator_base::advance_forward(difference_type diff)
{
  difference_type d = diff;
  while (!indexing_tree::is_sentinel(this->node_))
  {
    if (d == 0)
    {
      return;
    }
    if ( 0 < d )
    {
      if ( this->node_->right_->size_ < static_cast<size_type>(d) )
      {
        if ( this->node_->parent_->left_ == this->node_)
        {
          d -= (this->node_->right_->size_ + 1);
        }
        else
        {
          d += (this->node_->left_->size_ + 1);
        }
        this->node_ = this->node_->parent_;
      }
      else
      {
        d -= this->node_->right_->left_->size_ + 1;
        this->node_ = this->node_->right_;
      }
    }
    else
    {
      if ( this->node_->left_->size_ < static_cast<size_type>(-d) )
      {
        if ( this->node_->parent_->left_ == this->node_ )
        {
          d -= (this->node_->right_->size_ + 1);
        } 
        else
        {
          d += (this->node_->left_->size_ + 1);
        }
        this->node_ = this->node_->parent_;
      }
      else
      {
        d += (this->node_->left_->right_->size_ + 1);
        this->node_ = this->node_->left_;
      }
    }
  }
  throw std::out_of_range("indexing_tree::out_of_range");
}

template<class T,class A>
typename indexing_tree<T,A>::size_type indexing_tree<T,A>::iterator_base::index_of() const
{
  difference_type ret = 0;
  node_type* nd = node_;
  while (!indexing_tree::is_sentinel(nd))
  {
    if (nd->parent_->left_ == nd)
    {
      ret -= (nd->right_->size_ + 1);
    }
    else
    {
      ret += (nd->left_->size_ + 1);
    }
    nd = nd->parent_;
  }
  ret += nd->left_->size_;
  return static_cast<size_type>(ret);
}

//////////////////
// iterator
//////////////////
template<class T,class A>
inline indexing_tree<T,A>::iterator::iterator(const iterator& i):
iterator_base(i)
{
}

template<class T,class A>
inline indexing_tree<T,A>::iterator::iterator():
iterator_base()
{
}

template<class T,class A>
inline indexing_tree<T,A>::iterator::iterator(node_type* node):
iterator_base(node)
{
}

template<class T,class A>
inline typename indexing_tree<T,A>::iterator& indexing_tree<T,A>::iterator::operator++()
{
  this->node_ = this->node_->next_;
  return *this;
}

template<class T,class A>
inline typename indexing_tree<T,A>::iterator indexing_tree<T,A>::iterator::operator++(int)
{
  iterator tmp = *this;
  operator++();
  return tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::iterator& indexing_tree<T,A>::iterator::operator--()
{
  this->node_ = this->node_->prev_;
  return *this;
}

template<class T,class A>
inline typename indexing_tree<T,A>::iterator indexing_tree<T,A>::iterator::operator--(int)
{
  iterator tmp = *this;
  operator--();
  return tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::iterator indexing_tree<T,A>::iterator::operator + (difference_type diff) const
{
  iterator tmp = *this;
  tmp += diff;
  return tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::iterator indexing_tree<T,A>::iterator::operator - (difference_type diff) const
{
  iterator tmp = *this;
  tmp -= diff;
  return tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::iterator& indexing_tree<T,A>::iterator::operator += (difference_type diff)
{
  iterator_base::advance_forward(diff);
  return *this;
}

template<class T,class A>
inline typename indexing_tree<T,A>::iterator& indexing_tree<T,A>::iterator::operator -= (difference_type diff)
{
  iterator_base::advance_forward(-diff);
  return *this;
}

template<class T,class A>
inline bool indexing_tree<T,A>::iterator::operator < (const iterator& i) const
{
  return this->index_of() < i.index_of();
}

template<class T,class A>
inline bool indexing_tree<T,A>::iterator::operator <= (const iterator& i) const
{
  return this->index_of() <= i.index_of();
}

template<class T,class A>
inline bool indexing_tree<T,A>::iterator::operator > (const iterator& i) const
{
  return this->index_of() > i.index_of();
}

template<class T,class A>
inline bool indexing_tree<T,A>::iterator::operator >= (const iterator& i) const
{
  return this->index_of() >= i.index_of();
}

template<class T,class A>
inline typename indexing_tree<T,A>::reference indexing_tree<T,A>::iterator::operator*()
{
  return this->node_->value_;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_reference indexing_tree<T,A>::iterator::operator*() const
{
  return this->node_value_;
}

template<class T,class A>
inline typename indexing_tree<T,A>::pointer indexing_tree<T,A>::iterator::operator->()
{
  return &(this->node_->value_);
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_pointer indexing_tree<T,A>::iterator::operator->() const
{
  return &(this->node_->value_);
}

template<class T,class A>
inline typename indexing_tree<T,A>::reference indexing_tree<T,A>::iterator::operator [] (difference_type diff)
{
  iterator tmp = *this;
  tmp += diff;
  return *tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_reference indexing_tree<T,A>::iterator::operator [] (difference_type diff) const
{
  const_iterator tmp = *this;
  tmp += diff;
  return *tmp;
}

//////////////////
// const_iterator
//////////////////
template<class T,class A>
inline indexing_tree<T,A>::const_iterator::const_iterator(const iterator& i):
iterator_base(i)
{
}

template<class T,class A>
inline indexing_tree<T,A>::const_iterator::const_iterator(const const_iterator& i):
iterator_base(i)
{
}

template<class T,class A>
inline indexing_tree<T,A>::const_iterator::const_iterator(node_type* node):
iterator_base(node)
{
}

template<class T,class A>
inline indexing_tree<T,A>::const_iterator::const_iterator():
iterator_base()
{
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_iterator& indexing_tree<T,A>::const_iterator::operator++()
{
  this->node_ = this->node_->next_;
  return *this;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_iterator indexing_tree<T,A>::const_iterator::operator++(int)
{
  const_iterator tmp = *this;
  operator++();
  return tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_iterator& indexing_tree<T,A>::const_iterator::operator--()
{
  this->node_ = this->node_->prev_;
  return *this;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_iterator indexing_tree<T,A>::const_iterator::operator--(int)
{
  const_iterator tmp = *this;
  operator--();
  return tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_iterator indexing_tree<T,A>::const_iterator::operator + (difference_type diff) const
{
  const_iterator tmp = *this;
  tmp += diff;
  return tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_iterator indexing_tree<T,A>::const_iterator::operator - (difference_type diff) const
{
  const_iterator tmp = *this;
  tmp -= diff;
  return tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_iterator& indexing_tree<T,A>::const_iterator::operator += (difference_type diff)
{
  iterator_base::advance_forward(diff);
  return *this;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_iterator& indexing_tree<T,A>::const_iterator::operator -= (difference_type diff)
{
  iterator_base::advance_forward(-diff);
  return *this;
}

template<class T,class A>
inline bool indexing_tree<T,A>::const_iterator::operator < (const const_iterator& i) const
{
  return this->index_of() < i.index_of();
}

template<class T,class A>
inline bool indexing_tree<T,A>::const_iterator::operator <= (const const_iterator& i) const
{
  return this->index_of() <= i.index_of();
}

template<class T,class A>
inline bool indexing_tree<T,A>::const_iterator::operator > (const const_iterator& i) const
{
  return this->index_of() > i.index_of();
}

template<class T,class A>
inline bool indexing_tree<T,A>::const_iterator::operator >= (const const_iterator& i) const
{
  return this->index_of() >= i.index_of();
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_reference indexing_tree<T,A>::const_iterator::operator*() const
{
  return this->node_->value_;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_pointer indexing_tree<T,A>::const_iterator::operator->() const
{
  return &(this->node_->value_);
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_reference indexing_tree<T,A>::const_iterator::operator [] (difference_type diff) const
{
  const_iterator tmp = *this;
  tmp += diff;
  return *tmp;
}

//////////////////
// reverse_iterator
//////////////////
template<class T,class A>
inline indexing_tree<T,A>::reverse_iterator::reverse_iterator(const reverse_iterator& i):
iterator_base(i)
{
}

template<class T,class A>
inline indexing_tree<T,A>::reverse_iterator::reverse_iterator():
iterator_base()
{
}

template<class T,class A>
inline indexing_tree<T,A>::reverse_iterator::reverse_iterator(node_type* node):
iterator_base(node)
{
}

template<class T,class A>
inline typename indexing_tree<T,A>::reverse_iterator& indexing_tree<T,A>::reverse_iterator::operator++()
{
  this->node_ = this->node_->prev_;
  return *this;
}

template<class T,class A>
inline typename indexing_tree<T,A>::reverse_iterator indexing_tree<T,A>::reverse_iterator::operator++(int)
{
  reverse_iterator tmp = *this;
  operator++();
  return tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::reverse_iterator& indexing_tree<T,A>::reverse_iterator::operator--()
{
  this->node_ = this->node_->next_;
  return *this;
}

template<class T,class A>
inline typename indexing_tree<T,A>::reverse_iterator indexing_tree<T,A>::reverse_iterator::operator--(int)
{
  reverse_iterator tmp = *this;
  operator--();
  return tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::reverse_iterator indexing_tree<T,A>::reverse_iterator::operator + (difference_type diff) const
{
  reverse_iterator tmp = *this;
  tmp += diff;
  return tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::reverse_iterator indexing_tree<T,A>::reverse_iterator::operator - (difference_type diff) const
{
  reverse_iterator tmp = *this;
  tmp -= diff;
  return tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::reverse_iterator& indexing_tree<T,A>::reverse_iterator::operator += (difference_type diff)
{
  iterator_base::advance_forward(-diff);
  return *this;
}

template<class T,class A>
inline typename indexing_tree<T,A>::reverse_iterator& indexing_tree<T,A>::reverse_iterator::operator -= (difference_type diff)
{
  iterator_base::advance_forward(diff);
  return *this;
}

template<class T,class A>
inline bool indexing_tree<T,A>::reverse_iterator::operator < (const reverse_iterator& i) const
{
  return this->index_of() < i.index_of();
}

template<class T,class A>
inline bool indexing_tree<T,A>::reverse_iterator::operator <= (const reverse_iterator& i) const
{
  return this->index_of() <= i.index_of();
}

template<class T,class A>
inline bool indexing_tree<T,A>::reverse_iterator::operator > (const reverse_iterator& i) const
{
  return this->index_of() > i.index_of();
}

template<class T,class A>
inline bool indexing_tree<T,A>::reverse_iterator::operator >= (const reverse_iterator& i) const
{
  return this->index_of() >= i.index_of();
}

template<class T,class A>
inline typename indexing_tree<T,A>::reference indexing_tree<T,A>::reverse_iterator::operator*()
{
  return this->node_->value_;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_reference indexing_tree<T,A>::reverse_iterator::operator*() const
{
  return this->node_->value_;
}

template<class T,class A>
inline typename indexing_tree<T,A>::pointer indexing_tree<T,A>::reverse_iterator::operator->()
{
  return &(this->node_->value_);
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_pointer indexing_tree<T,A>::reverse_iterator::operator->() const
{
  return &(this->node_->value_);
}

template<class T,class A>
inline typename indexing_tree<T,A>::reference indexing_tree<T,A>::reverse_iterator::operator [] (difference_type diff)
{
  reverse_iterator tmp = *this;
  tmp += diff;
  return *tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_reference indexing_tree<T,A>::reverse_iterator::operator [] (difference_type diff) const
{
  const_reverse_iterator tmp = *this;
  tmp += diff;
  return *tmp;
}

//////////////////
// const_reverse_iterator
//////////////////
template<class T,class A>
inline indexing_tree<T,A>::const_reverse_iterator::const_reverse_iterator(const reverse_iterator& i):
iterator_base(i)
{
}

template<class T,class A>
inline indexing_tree<T,A>::const_reverse_iterator::const_reverse_iterator(const const_reverse_iterator& i):
iterator_base(i)
{
}

template<class T,class A>
inline indexing_tree<T,A>::const_reverse_iterator::const_reverse_iterator(node_type* node):
iterator_base(node)
{
}

template<class T,class A>
inline indexing_tree<T,A>::const_reverse_iterator::const_reverse_iterator():
iterator_base()
{
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_reverse_iterator& indexing_tree<T,A>::const_reverse_iterator::operator++()
{
  this->node_ = this->node_->prev_;
  return *this;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_reverse_iterator indexing_tree<T,A>::const_reverse_iterator::operator++(int)
{
  const_reverse_iterator tmp = *this;
  operator++();
  return tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_reverse_iterator& indexing_tree<T,A>::const_reverse_iterator::operator--()
{
  this->node_ = this->node_->next_;
  return *this;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_reverse_iterator indexing_tree<T,A>::const_reverse_iterator::operator--(int)
{
  const_reverse_iterator tmp = *this;
  operator--();
  return tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_reverse_iterator indexing_tree<T,A>::const_reverse_iterator::operator + (difference_type diff) const
{
  const_reverse_iterator tmp = *this;
  tmp += diff;
  return tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_reverse_iterator indexing_tree<T,A>::const_reverse_iterator::operator - (difference_type diff) const
{
  const_reverse_iterator tmp = *this;
  tmp -= diff;
  return tmp;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_reverse_iterator& indexing_tree<T,A>::const_reverse_iterator::operator += (difference_type diff)
{
  iterator_base::advance_forward(-diff);
  return *this;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_reverse_iterator& indexing_tree<T,A>::const_reverse_iterator::operator -= (difference_type diff)
{
  iterator_base::advance_forward(diff);
  return *this;
}

template<class T,class A>
inline bool indexing_tree<T,A>::const_reverse_iterator::operator < (const const_reverse_iterator& i) const
{
  return this->index_of() < i.index_of();
}

template<class T,class A>
inline bool indexing_tree<T,A>::const_reverse_iterator::operator <= (const const_reverse_iterator& i) const
{
  return this->index_of() <= i.index_of();
}

template<class T,class A>
inline bool indexing_tree<T,A>::const_reverse_iterator::operator > (const const_reverse_iterator& i) const
{
  return this->index_of() > i.index_of();
}

template<class T,class A>
inline bool indexing_tree<T,A>::const_reverse_iterator::operator >= (const const_reverse_iterator& i) const
{
  return this->index_of() >= i.index_of();
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_reference indexing_tree<T,A>::const_reverse_iterator::operator*() const
{
  return this->node_value_;
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_pointer indexing_tree<T,A>::const_reverse_iterator::operator->() const
{
  return &(this->node_->value_);
}

template<class T,class A>
inline typename indexing_tree<T,A>::const_reference indexing_tree<T,A>::const_reverse_iterator::operator [] (difference_type diff) const
{
  const_reverse_iterator tmp = *this;
  tmp += diff;
  return *tmp;
}

//////////////////
// indexing_tree
//////////////////
// private member functions
template<class T, class A>
inline void indexing_tree<T,A>::init_sentinel_()
{
  sentinel_ = nodealloc_.allocate(1);
  sentinel_->left_ = sentinel_;
  sentinel_->next_ = sentinel_;
  sentinel_->parent_ = sentinel_;
  sentinel_->prev_ = sentinel_;
  sentinel_->right_ = sentinel_;
  sentinel_->size_ = 0;
}

template<class T, class A>
typename indexing_tree<T,A>::node_type* indexing_tree<T,A>::select(size_type n)
{
  size_type i = n;
  node_type *p = sentinel_->left_;
  while ( !is_sentinel(p) && (i != p->left_->size_))
  {
    if ( i < p->left_->size_ )
    {
      p = p->left_;
    }
    else
    {
      i -= p->left_->size_ + 1;
      p = p->right_;
    }
  }
  return p;
}

template<class T, class A>
inline void indexing_tree<T,A>::range_check_lt(size_type n) const
{
  if ( sentinel_->left_->size_ < n )
  {
    throw std::out_of_range("indexing_tree::out_of_range");
  }
}

template<class T, class A>
inline void indexing_tree<T,A>::range_check_leq(size_type n) const
{
  if ( sentinel_->left_->size_ <= n )
  {
    throw std::out_of_range("indexing_tree::out_of_range");
  }
}

template<class T, class A>
inline bool indexing_tree<T,A>::is_balanced(node_type* p) const
{
  if (p->left_->size_ < p->right_->size_)
  {
    return ( (p->right_->size_ - p->left_->size_) <= (p->left_->size_ + 1) );
  }
  return ( (p->left_->size_ - p->right_->size_) <= (p->right_->size_ + 1) );
}

template<class T, class A>
void indexing_tree<T,A>::fix_up(node_type* p)
{
  while (!is_sentinel(p))
  {
    node_type *parent = p->parent_;
    rebalance(p);
    p = parent;
  }
}

template<class T, class A>
void indexing_tree<T,A>::fix_up_incr(node_type* p)
{
  while (!is_sentinel(p))
  {
    node_type *parent = p->parent_;
    ++p->size_;
    rebalance(p);
    p = parent;
  }
}

template<class T, class A>
void indexing_tree<T,A>::fix_up_decr(node_type* p)
{
  while (!is_sentinel(p))
  {
    node_type *parent = p->parent_;
    --p->size_;
    rebalance(p);
    p = parent;
  }
}

template<class T, class A>
void indexing_tree<T,A>::rebalance(node_type* p)
{
  while (!is_balanced(p))
  {
    if (p->right_->size_ < p->left_->size_)
    {
      if (p->right_->size_ + p->left_->right_->size_ <= 2*p->left_->left_->size_)
      {
        ll_rotation(p);
        return;
      }
      node_type* pp = p;
      if (p->left_->right_->left_->size_ < p->left_->right_->right_->size_)
      {
        pp = p->left_;
      }
      lr_rotation(p);
      p = pp;
    }
    else
    {
      if (p->left_->size_ + p->right_->left_->size_ <= 2*p->right_->right_->size_)
      {
        rr_rotation(p);
        return;
      }
      node_type* pp = p;
      if (p->right_->left_->right_->size_ < p->right_->left_->left_->size_)
      {
        pp = p->right_;
      }
      rl_rotation(p);
      p = pp;
    }
  }
}

template<class T, class A>
inline void indexing_tree<T,A>::ll_rotation(node_type* p)
{
  node_type *q = p->left_;
  q->size_ = p->size_;
  p->size_ -= (q->left_->size_ + 1);
  if (p->parent_->left_ == p)
  {
    p->parent_->left_ = q;
  }
  else
  {
    p->parent_->right_ = q;
  }
  q->parent_ = p->parent_;
  p->left_ = q->right_;
  if (!is_sentinel(p->left_))
  {
    p->left_->parent_ = p;
  }
  q->right_ = p;
  p->parent_ = q;
}

template<class T, class A>
inline void indexing_tree<T,A>::rr_rotation(node_type* p)
{
  node_type *q = p->right_;
  q->size_ = p->size_;
  p->size_ -= (q->right_->size_ + 1);
  if (p->parent_->left_ == p)
  {
    p->parent_->left_ = q;
  }
  else
  {
    p->parent_->right_ = q;
  }
  q->parent_ = p->parent_;
  p->right_ = q->left_;
  if (!is_sentinel(p->right_))
  {
    p->right_->parent_ = p;
  }
  q->left_ = p;
  p->parent_ = q;
}

template<class T, class A>
inline void indexing_tree<T,A>::lr_rotation(node_type* p)
{
  node_type *q = p->left_;
  node_type *r = q->right_;
  r->size_ = p->size_;
  p->size_ -= (q->size_ - r->right_->size_);
  q->size_ -= (r->right_->size_ + 1);
  if (p->parent_->left_ == p)
  {
    p->parent_->left_ = r;
  }
  else
  {
    p->parent_->right_ = r;
  }
  r->parent_ = p->parent_;
  p->left_ = r->right_;
  if (!is_sentinel(p->left_))
  {
    p->left_->parent_ = p;
  }
  q->right_ = r->left_;
  if (!is_sentinel(q->right_))
  {
    q->right_->parent_ = q;
  }
  p->parent_ = r;
  q->parent_ = r;
  r->left_ = q;
  r->right_ = p;
}

template<class T, class A>
inline void indexing_tree<T,A>::rl_rotation(node_type* p)
{
  node_type *q = p->right_;
  node_type *r = q->left_;
  r->size_ = p->size_;
  p->size_ -= (q->size_ - r->left_->size_);
  q->size_ -= (r->left_->size_ + 1);
  if (p->parent_->left_ == p)
  {
    p->parent_->left_ = r;
  }
  else
  {
    p->parent_->right_ = r;
  }
  r->parent_ = p->parent_;
  p->right_ = r->left_;
  if (!is_sentinel(p->right_))
  {
    p->right_->parent_ = p;
  }
  q->left_ = r->right_;
  if (!is_sentinel(q->left_))
  {
    q->left_->parent_ = q;
  }
  p->parent_ = r;
  q->parent_ = r;
  r->right_ = q;
  r->left_ = p;
}

template<class T, class A>
inline typename indexing_tree<T,A>::node_type* indexing_tree<T,A>::newitem(const T& x)
{
  node_type* item = nodealloc_.allocate(1);
  try
  {
    alloc_.construct(&(item->value_), x);
    return item;
  }
  catch (...)
  {
    nodealloc_.deallocate(item,1);
    throw;
  }
}

template<class T, class A>
void indexing_tree<T,A>::put_first_element(node_type* p)
{
  sentinel_->left_ = p;
  sentinel_->next_ = p;
  sentinel_->prev_ = p;
  sentinel_->right_ = p;
  p->parent_ = sentinel_;
  p->left_ = sentinel_;
  p->right_ = sentinel_;
  p->next_ = sentinel_;
  p->prev_ = sentinel_;
  p->size_ = 1;
}

template<class T, class A>
void indexing_tree<T,A>::deleteitem(node_type* p)
{
  alloc_.destroy(get_allocator().address(p->value_));
  nodealloc_.deallocate(p,1);
}

template<class T, class A>
inline bool indexing_tree<T,A>::is_sentinel(node_type* p)
{
  return p->parent_ == p;
}

template<class T, class A>
template<bool Is_integral, class InIter>
indexing_tree<T,A>::private_insert<Is_integral,InIter>::private_insert(indexing_tree<T,A>& that, iterator position, InIter first, InIter last)
{
  InIter it(first);
  indexing_tree<T,A>::iterator prev = position;
  indexing_tree<T,A>::iterator p = prev--;
  try
  {
    for (;it != last;++it)
    {
      p = that.insert(position, *it);
    }
  }
  catch (...)
  {
    while (p != prev)
    {
      p = that.erase(p);
    }
    throw;
  }
}

template<class T, class A>
template<class InIter>
indexing_tree<T,A>::private_insert<true,InIter>::private_insert(indexing_tree<T,A>& that, iterator position, InIter first, InIter last)
{
  that.insert(position, static_cast<size_type>(first), static_cast<T>(last));
}

// public member functions
template<class T, class A>
indexing_tree<T,A>::indexing_tree(const A& alloc)
  : alloc_(alloc),sentinel_(0),nodealloc_(node_allocator_type())
{
  init_sentinel_();
}

template<class T, class A>
indexing_tree<T,A>::indexing_tree(size_type n,const T& x, const A& alloc)
  : alloc_(alloc),sentinel_(0),nodealloc_(node_allocator_type())
{
  init_sentinel_();
  insert(begin(), n, x);
}

template<class T, class A>
template<class InIter>
indexing_tree<T,A>::indexing_tree(InIter first, InIter last, const A& alloc)
  : alloc_(alloc),sentinel_(0),nodealloc_(node_allocator_type())
{
  init_sentinel_();
  insert(begin(), first, last);
}

template<class T, class A>
indexing_tree<T,A>::indexing_tree(const indexing_tree& that)
  : alloc_(that.get_allocator()),sentinel_(0),nodealloc_(node_allocator_type())
{
  init_sentinel_();
  insert(that.begin(), that.end());
}

template<class T, class A>
indexing_tree<T,A>::~indexing_tree()
{
  clear();
  nodealloc_.deallocate(sentinel_,1);
}

template<class T, class A>
indexing_tree<T,A>& indexing_tree<T,A>::operator=(const indexing_tree& that)
{
  if (this == &that)
  {
    return *this;
  }
  assign(that.begin(), that.end());
  return *this;
}

template<class T, class A>
template<class InIter>
void indexing_tree<T,A>::assign(InIter first, InIter last)
{
  indexing_tree tmp(first,last);
  swap(tmp);
}

template<class T, class A>
void indexing_tree<T,A>::assign(size_type n, const T& x)
{
  indexing_tree tmp(n,x);
  swap(tmp);
}

template<class T, class A>
inline typename indexing_tree<T,A>::allocator_type indexing_tree<T,A>::get_allocator() const
{
  return alloc_;
}

template<class T, class A>
inline typename indexing_tree<T,A>::iterator indexing_tree<T,A>::begin()
{
  return iterator(sentinel_->next_);
}

template<class T, class A>
inline typename indexing_tree<T,A>::const_iterator indexing_tree<T,A>::begin() const
{
  return const_iterator(sentinel_->next_);
}

template<class T, class A>
inline typename indexing_tree<T,A>::iterator indexing_tree<T,A>::end()
{
  return iterator(sentinel_);
}

template<class T, class A>
inline typename indexing_tree<T,A>::const_iterator indexing_tree<T,A>::end() const
{
  return const_iterator(sentinel_);
}

template<class T, class A>
inline typename indexing_tree<T,A>::reverse_iterator indexing_tree<T,A>::rbegin()
{
  return reverse_iterator(sentinel_->prev_);
}

template<class T, class A>
inline typename indexing_tree<T,A>::const_reverse_iterator indexing_tree<T,A>::rbegin() const
{
  return const_reverse_iterator(sentinel_->prev_);
}

template<class T, class A>
inline typename indexing_tree<T,A>::reverse_iterator indexing_tree<T,A>::rend()
{
  return reverse_iterator(sentinel_);
}

template<class T, class A>
inline typename indexing_tree<T,A>::const_reverse_iterator indexing_tree<T,A>::rend() const
{
  return const_reverse_iterator(sentinel_);
}

template<class T, class A>
inline typename indexing_tree<T,A>::size_type indexing_tree<T,A>::size() const
{
  return sentinel_->left_->size_;
}

template<class T, class A>
inline typename indexing_tree<T,A>::size_type indexing_tree<T,A>::max_size() const
{
  return alloc_.max_size();
}

template<class T, class A>
void indexing_tree<T,A>::resize(size_type sz, const T& x)
{
  if (size() < sz)
  {
    insert(end(),sz-size(),x);
    return;
  }
  while (sz < size())
  {
    pop_back();
  }
}

template<class T, class A>
typename indexing_tree<T,A>::size_type indexing_tree<T,A>::capacity() const
{
  return 0;
}

template<class T, class A>
inline bool indexing_tree<T,A>::empty() const
{
  return (sentinel_->left_->size_ == 0);
}

template<class T, class A>
inline void indexing_tree<T,A>::reserve(size_type n)
{
}

template<class T, class A>
typename indexing_tree<T,A>::reference indexing_tree<T,A>::operator[](size_type n)
{
  range_check_leq(n);
  return select(n)->value_;
}

template<class T, class A>
inline typename indexing_tree<T,A>::const_reference indexing_tree<T,A>::operator[](size_type n) const
{
  range_check_leq(n);
  return select(n)->value_;
}

template<class T, class A>
inline typename indexing_tree<T,A>::reference indexing_tree<T,A>::at(size_type n)
{
  range_check_leq(n);
  return select(n)->value_;
}

template<class T, class A>
inline typename indexing_tree<T,A>::const_reference indexing_tree<T,A>::at(size_type n) const
{
  range_check_leq(n);
  return select(n)->value_;
}

template<class T, class A>
inline typename indexing_tree<T,A>::reference indexing_tree<T,A>::front()
{
  return sentinel_->next_->value_;
}

template<class T, class A>
inline typename indexing_tree<T,A>::const_reference indexing_tree<T,A>::front() const
{
  return sentinel_->next_->value_;
}

template<class T, class A>
inline typename indexing_tree<T,A>::reference indexing_tree<T,A>::back()
{
  return sentinel_->prev_->value_;
}

template<class T, class A>
inline typename indexing_tree<T,A>::const_reference indexing_tree<T,A>::back() const
{
  return sentinel_->prev_->value_;
}

template<class T, class A>
void indexing_tree<T,A>::push_back(const T& x)
{
  node_type* n = newitem(x);
  node_type* p = sentinel_->prev_;
  if (is_sentinel(p))
  {
    put_first_element(n);
    return;
  }
  n->size_ = 1;
  n->next_ = sentinel_;
  n->prev_ = p;
  n->parent_ = p;
  n->left_ = sentinel_;
  n->right_ = sentinel_;
  p->next_ = n;
  p->right_ = n;
  sentinel_->prev_ = n;
  fix_up_incr(p);
}

template<class T, class A>
void indexing_tree<T,A>::pop_back()
{
  node_type* p = sentinel_->prev_;
  if (is_sentinel(p))
  {
    return;
  }
  sentinel_->prev_ = p->prev_;
  p->prev_->next_ = sentinel_;
  node_type* pp = p->parent_;
  node_type* l = p->left_;
  if (l != sentinel_)
  {
    l->parent_ = pp;
  }
  pp->right_ = l;
  if (pp == sentinel_)
  {
    pp->left_ = l;
  }
  deleteitem(p);
  fix_up_decr(pp);
}

template<class T, class A>
void indexing_tree<T,A>::push_front(const T& x)
{
  node_type* n = newitem(x);
  node_type* p = sentinel_->next_;
  if (is_sentinel(p))
  {
    put_first_element(n);
    return;
  }
  n->size_ = 1;
  n->prev_ = sentinel_;
  n->next_ = p;
  n->parent_ = p;
  n->left_ = sentinel_;
  n->right_ = sentinel_;
  p->prev_ = n;
  p->left_ = n;
  sentinel_->next_ = n;
  fix_up_incr(p);
}

template<class T, class A>
void indexing_tree<T,A>::pop_front()
{
  node_type* p = sentinel_->next_;
  if (is_sentinel(p))
  {
    return;
  }
  sentinel_->next_ = p->next_;
  p->next_->prev_ = sentinel_;
  node_type* pp = p->parent_;
  node_type* r = p->right_;
  if (r != sentinel_)
  {
    r->parent_ = pp;
  }
  pp->left_ = r;
  if (pp == sentinel_)
  {
    pp->right_ = r;
  }
  deleteitem(p);
  fix_up_decr(pp);
}

template<class T, class A>
void indexing_tree<T,A>::clear()
{
  node_type* p = sentinel_->next_;
  while (p != sentinel_)
  {
    node_type* next = p->next_;
    deleteitem(p);
    p = next;
  }
  sentinel_->left_ = sentinel_;
  sentinel_->right_ = sentinel_;
  sentinel_->next_ = sentinel_;
  sentinel_->prev_ = sentinel_;
}

template<class T, class A>
typename indexing_tree<T,A>::iterator indexing_tree<T,A>::insert(iterator position, const T& x)
{
  if (is_sentinel(position.node_))
  {
    push_back(x);
    return end().node_->prev_;
  }
  node_type* p = newitem(x);
  node_type* n = position.node_;
  node_type* m = n->prev_;
  p->size_ = 1;
  p->next_ = n;
  p->prev_ = m;
  p->left_ = sentinel_;
  p->right_ = sentinel_;
  n->prev_ = p;
  m->next_ = p;
  if (is_sentinel(n->left_))
  {
    n->left_ = p;
    p->parent_ = n;
    fix_up_incr(n);
  }
  else
  {
    m->right_ = p;
    p->parent_ = m;
    fix_up_incr(m);
  }
  return iterator(p);
}

template<class T, class A>
void indexing_tree<T,A>::insert(iterator position, size_type n, const T& x)
{
  size_type i;
  iterator p = position;
  try
  {
    for (i=0;i<n;++i)
    {
      p = insert(p,x);
    }
  }
  catch (...)
  {
    while (i-- != 0)
    {
      p = erase(p);
    }
    throw;
  }
}

template<class T, class A>
template<class InIter>
void indexing_tree<T,A>::insert(iterator position, InIter first, InIter last)
{
  private_insert<integral_trait_name_space::is_integral<InIter>::value_,InIter> temp(*this, position,first,last);
}

template<class T, class A>
typename indexing_tree<T,A>::iterator indexing_tree<T,A>::erase(iterator position)
{
  if (is_sentinel(position.node_))
  {
    return position;
  }
  node_type* del = position.node_;
  node_type* pp = del->parent_;
  bool leftchild = del->parent_->left_==del;
  node_type* l = del->left_;
  node_type* r = del->right_;
  node_type* p = del->prev_;
  node_type* n = del->next_;
  size_type sz = del->size_;
  deleteitem(del);
  p->next_ = n;
  n->prev_ = p;
  if (is_sentinel(l))
  {
    if (leftchild)
    {
      pp->left_ = r;
    }
    else
    {
      pp->right_ = r;
    }
    if (!is_sentinel(r))
    {
      r->parent_ = pp;
    }
    fix_up_decr(pp);
    return iterator(n);
  }
  if (is_sentinel(r))
  {
    if (leftchild)
    {
      pp->left_ = l;
    }
    else
    {
      pp->right_ = l;
    }
    if (!is_sentinel(l))
    {
      l->parent_ = pp;
    }
    fix_up_decr(pp);
    return iterator(n);
  }
  if (p == l)
  {
    p->size_ = sz-1;
    p->right_ = r;
    r->parent_ = p;
    p->parent_ = pp;
    if (leftchild)
    {
      pp->left_ = p;
    }
    else
    {
      pp->right_ = p;
    }
    fix_up_decr(pp);
    return iterator(n);
  }
  if (is_sentinel(p->right_))
  {
    node_type* ppp = p->parent_;
    if (ppp->left_ == p)
    {
      ppp->left_ = p->left_;
    }
    else
    {
      ppp->right_ = p->left_;
    }
    if (!is_sentinel(p->left_))
    {
      p->left_->parent_ = ppp;
    }
    p->size_ = sz;
    p->left_ = l;
    p->right_ = r;
    p->parent_ = pp;
    l->parent_ = p;
    r->parent_ = p;
    if (leftchild)
    {
      pp->left_ = p;
    }
    else
    {
      pp->right_ = p;
    }
    fix_up_decr(ppp);
    return iterator(n);
  }
  node_type* ppp = n->parent_;
  if (ppp->left_ == n)
  {
    ppp->left_ = n->right_;
  }
  else
  {
    ppp->right_ = n->right_;
  }
  if (!is_sentinel(n->right_))
  {
    n->right_->parent_ = ppp;
  }
  n->size_ = sz;
  n->left_ = l;
  n->right_ = r;
  n->parent_ = pp;
  l->parent_ = n;
  r->parent_ = n;
  if (leftchild)
  {
    pp->left_ = n;
  }
  else
  {
    pp->right_ = n;
  }
  fix_up_decr(ppp);
  return iterator(n);
}

template<class T, class A>
void indexing_tree<T,A>::swap(indexing_tree& that) throw()
{
  node_type* p = sentinel_;
  sentinel_ = that.sentinel_;
  that.sentinel_ = p;
}

} // end of namespace osoken

#endif // INDEXING_TREE_HPP_


