#ifndef AL_ARRAY_ARRAY_1D_HPP_INCLUDED
#define AL_ARRAY_ARRAY_1D_HPP_INCLUDED

#include <cstddef>
#include <memory>

namespace al {
  
namespace array {

template <typename T, typename A = std::allocator<T> >
class array_1d
{
public:  
  typedef T value_type;
  typedef A allocator_type;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef pointer iterator;
  typedef const_pointer const_iterator;
  // constructors
  explicit array_1d(const allocator_type& = allocator_type());
  explicit array_1d(size_type, const value_type& = value_type(), const allocator_type& = allocator_type());  
  template <typename InputIterator> array_1d(InputIterator first, InputIterator last, const allocator_type& = allocator_type());
  array_1d(const array_1d<T, A>&);
  // destructor
  ~array_1d();
  // operator=
  array_1d<T, A>& operator=(const array_1d<T, A>&);
  // iterators
  iterator begin();
  const_iterator begin () const;
  iterator end();
  const_iterator end() const;
  // capacity
  size_type size() const;
  size_type max_size() const;
  void resize(size_type);
  bool empty() const;
  // element access
  reference operator[](size_type);
  const_reference operator[](size_type) const;
  const_reference at(size_type) const;
  reference at(size_type);
  // modifiers
  void assign(size_type, const value_type&);
  void swap(array_1d<T, A>&);
  void clear();
  // allocator
  allocator_type get_allocator() const;  
private:
  allocator_type alloc_;
  size_type size_;
  pointer elements_;
};

} // namespace al

} // namespace array

// -----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <stdexcept>

// -----------------------------------------------------------------------------

template <typename T, typename A>
inline al::array::array_1d<T, A>::array_1d(const allocator_type& alloc)
  : alloc_(alloc), size_(0), elements_(0)
{
}

template <typename T, typename A>
inline al::array::array_1d<T, A>::array_1d(size_type size, const value_type& value, const allocator_type& alloc)
  : alloc_(alloc), size_(size), elements_(0)
{
  if (size_ > 0) {
	// allocate elements
	elements_ = alloc_.allocate(size_);
	size_type i;
	try {
	  for (i = 0; i < size_; ++i) {
		alloc_.construct(&elements_[i], value);
	  }
	}
	catch(...) {
	  // destroy backwards
	  for ( ; i > 0; --i) {
		alloc_.destroy(&elements_[i - 1]);
	  }
	  alloc_.deallocate(elements_, size_);
	  throw;
	}
  }
}


// COPY CONSTRUCTOR
template <typename T, typename A>
inline al::array::array_1d<T, A>::array_1d(const array_1d<T, A>& that)
  : alloc_(that.alloc_), size_(that.size_), elements_(0)
{
  if (size_ > 0) {
	elements_ = alloc_.allocate(size_);
	size_type i;
	try {
	  for (i = 0; i < size_; ++i) {
		alloc_.construct(&elements_[i], that.elements_[i]);
	  }
	}
	catch(...) {
	  for ( ; i > 0; --i) {
		alloc_.destroy(&elements_[i - 1]);
	  }
	  alloc_.deallocate(elements_, size_);
	  throw;
	}
  }
}

template <typename T, typename A>
template <typename ForwardIterator>
inline al::array::array_1d<T, A>::array_1d(ForwardIterator first, ForwardIterator last, const allocator_type& alloc)
  : alloc_(alloc), size_(std::distance(first, last)), elements_(0)
{
  if (size_ > 0) {
	elements_ = alloc_.allocate(size_);
	size_type i;
	try {
	  for (i = 0; i < size_; ++i) {
		alloc_.construct(&elements_[i], *(first + i));
	  }
	}
	catch(...) {
	  for ( ; i > 0; --i) {
		alloc_.destroy(&elements_[i - 1]);
	  }
	  alloc_.deallocate(elements_, size_);
	  throw;
	}
  }
}

// -----------------------------------------------------------------------------

// DESTROYER
template <typename T, typename A>
inline al::array::array_1d<T, A>::~array_1d()
{
  if (size_ > 0) {
	for (size_type i = size_; i > 0; --i) {
	  alloc_.destroy(&elements_[i - 1]);
	}
	alloc_.deallocate(elements_, size_);
  }
}

// -----------------------------------------------------------------------------

// ASSIGNMENT OPERATOR
template <typename T, typename A>
inline al::array::array_1d<T, A>& al::array::array_1d<T, A>::operator=(const array_1d<T, A>& that)
{
  if (&that != this) {
	array_1d temp(that);
	swap(temp);
  }
  return *this;
}

// -----------------------------------------------------------------------------

// ITERATOR STUFF
template <typename T, typename A>
inline typename al::array::array_1d<T, A>::iterator al::array::array_1d<T, A>::begin()
{
  return elements_;
}

template <typename T, typename A>
inline typename al::array::array_1d<T, A>::const_iterator al::array::array_1d<T, A>::begin () const
{
  return elements_;
}

template <typename T, typename A>
inline typename al::array::array_1d<T, A>::iterator al::array::array_1d<T, A>::end()
{
  return elements_ + size_;
}

template <typename T, typename A>
inline typename al::array::array_1d<T, A>::const_iterator al::array::array_1d<T, A>::end() const
{
  return elements_ + size_;
}

// -----------------------------------------------------------------------------

template <typename T, typename A>
inline typename al::array::array_1d<T, A>::size_type al::array::array_1d<T, A>::size() const
{
  return size_;
}

template <typename T, typename A>
inline typename al::array::array_1d<T, A>::size_type al::array::array_1d<T, A>::max_size() const
{
  return alloc_.max_size();
}
  
template <typename T, typename A>
inline void al::array::array_1d<T, A>::resize(size_type size)
{
  array_1d temp(size);
  swap(temp);
}

template <typename T, typename A>
inline bool al::array::array_1d<T, A>::empty() const
{
  return size_ == 0;
}

// -----------------------------------------------------------------------------

template <typename T, typename A>
inline typename al::array::array_1d<T, A>::reference al::array::array_1d<T, A>::operator[](size_type index)
{
  assert(index < size());
  return elements_[index];
}

template <typename T, typename A>
inline typename al::array::array_1d<T, A>::const_reference al::array::array_1d<T, A>::operator[](size_type index) const
{
  assert(index < size());
  return elements_[index];
}

template <typename T, typename A>
inline typename al::array::array_1d<T, A>::const_reference al::array::array_1d<T, A>::at(size_type index) const
{
  if (index >= size_) {
	throw std::out_of_range(__FUNCTION__);
  }
  return elements_[index];
}

template <typename T, typename A>
inline typename al::array::array_1d<T, A>::reference al::array::array_1d<T, A>::at(size_type index)
{
  if (index >= size_) {
	throw std::out_of_range(__FUNCTION__);
  }
  return elements_[index];
}

// -----------------------------------------------------------------------------

template <typename T, typename A>
inline void al::array::array_1d<T, A>::assign(size_type size, const value_type& value)
{
  swap(array_1d(size, value));
}

template <typename T, typename A>
inline void al::array::array_1d<T, A>::swap(array_1d<T, A>& that)
{
  std::swap(alloc_, that.alloc_);
  std::swap(size_, that.size_);
  std::swap(elements_, that.elements_);
}

template <typename T, typename A>
inline void al::array::array_1d<T, A>::clear()
{
  array_1d temp();
  swap(temp);
}

// -----------------------------------------------------------------------------

template <typename T, typename A>
inline typename al::array::array_1d<T, A>::allocator_type al::array::array_1d<T, A>::get_allocator() const
{
  return alloc_;
}


// -----------------------------------------------------------------------------

#endif // AL_ARRAY_ARRAY_1D_HPP_INCLUDED
