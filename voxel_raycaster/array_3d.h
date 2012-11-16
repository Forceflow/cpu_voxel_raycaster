#ifndef AL_ARRAY_ARRAY_3D_HPP_INCLUDED
#define AL_ARRAY_ARRAY_3D_HPP_INCLUDED

#include <cstddef>
#include <memory>

#include "array_1d.h"

namespace al {
  
namespace array {
    
template <typename T, typename A = std::allocator<T> >
class array_3d
{
public:
  // typedefs
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
  explicit array_3d(const allocator_type& = allocator_type());
  array_3d(size_type, size_type, size_type, const value_type& = value_type(), const allocator_type& = allocator_type());
  // iterators
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;
  // capacity
  size_type size(size_type) const;
  size_type max_size() const;
  void resize(size_type, size_type, size_type);
  bool empty() const;
  // element access
  reference operator()(size_type, size_type, size_type);
  const_reference operator()(size_type, size_type, size_type) const;
  const_reference at(size_type, size_type, size_type) const;
  reference at(size_type, size_type, size_type);
  // modifiers
  void assign(size_type, size_type, size_type, const value_type&);
  void swap(array_3d<T, A>&);
  void clear();
  // allocator
  allocator_type get_allocator() const;  
private:
  array_1d<value_type, allocator_type> array_1d_;
  size_type size_[3];
};

} // namespace al

} // namespace array

// -----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <stdexcept>

// -----------------------------------------------------------------------------

template <typename T, typename A>
inline al::array::array_3d<T, A>::array_3d(const allocator_type& alloc)
  : array_1d_(alloc)
{
  size_[0] = 0;
  size_[1] = 0;
  size_[2] = 0;
}

template <typename T, typename A>
inline al::array::array_3d<T, A>::array_3d(size_type size_2, size_type size_1, size_type size_0, const value_type& value, const allocator_type& alloc)
  : array_1d_(size_0 * size_1 * size_2, value, alloc)
{
  size_[0] = size_0;
  size_[1] = size_1;
  size_[2] = size_2;
}

// -----------------------------------------------------------------------------

template <typename T, typename A>
inline typename al::array::array_3d<T, A>::iterator al::array::array_3d<T, A>::begin()
{
  return array_1d_.begin();
}

template <typename T, typename A>
inline typename al::array::array_3d<T, A>::const_iterator al::array::array_3d<T, A>::begin() const
{
  return array_1d_.begin();
}

template <typename T, typename A>
inline typename al::array::array_3d<T, A>::iterator al::array::array_3d<T, A>::end()
{
  return array_1d_.end();
}

template <typename T, typename A>
inline typename al::array::array_3d<T, A>::const_iterator al::array::array_3d<T, A>::end() const
{
  return array_1d_.end();
}

// -----------------------------------------------------------------------------

template <typename T, typename A>
inline typename al::array::array_3d<T, A>::size_type al::array::array_3d<T, A>::size(size_type dimension) const
{
  assert(dimension < 3);
  return size_[dimension];
}

template <typename T, typename A>
inline typename al::array::array_3d<T, A>::size_type al::array::array_3d<T, A>::max_size() const
{
  return array_1d_.max_size();
}

template <typename T, typename A>
inline void al::array::array_3d<T, A>::resize(size_type size_2, size_type size_1, size_type size_0)
{
  size_[0] = size_0;
  size_[1] = size_1;
  size_[2] = size_2;
  array_1d_.resize(size_[0] * size_[1] * size_[2]);
}

template <typename T, typename A>
inline bool al::array::array_3d<T, A>::empty() const
{
  return array_1d_.empty();
}

// -----------------------------------------------------------------------------



template <typename T, typename A>
inline typename al::array::array_3d<T, A>::reference al::array::array_3d<T, A>::operator()(size_type index_2, size_type index_1, size_type index_0)
{
  assert(index_0 < size(0));
  assert(index_1 < size(1));
  assert(index_2 < size(2));
  return array_1d_[(((index_2 * size_[1]) + index_1) * size_[0]) + index_0];
}

template <typename T, typename A>
inline typename al::array::array_3d<T, A>::const_reference al::array::array_3d<T, A>::operator()(size_type index_2, size_type index_1, size_type index_0) const
{
  assert(index_0 < size(0));
  assert(index_1 < size(1));
  assert(index_2 < size(2));
  return array_1d_[(((index_2 * size_[1]) + index_1) * size_[0]) + index_0];
}

template <typename T, typename A>
inline typename al::array::array_3d<T, A>::const_reference al::array::array_3d<T, A>::at(size_type index_2, size_type index_1, size_type index_0) const
{
  if ((index_0 >= size_[0]) || (index_1 >= size_[1]) || (index_2 >= size_[2])) {
    throw std::out_of_range(__FUNCTION__);
  }
  return array_1d_[(((index_2 * size_[1]) + index_1) * size_[0]) + index_0];
}

template <typename T, typename A>
inline typename al::array::array_3d<T, A>::reference al::array::array_3d<T, A>::at(size_type index_2, size_type index_1, size_type index_0)
{
  if ((index_0 >= size_[0]) || (index_1 >= size_[1]) || (index_2 >= size_[2])) {
    throw std::out_of_range(__FUNCTION__);
  }
  return array_1d_[(((index_2 * size_[1]) + index_1) * size_[0]) + index_0];
}

// -----------------------------------------------------------------------------

template <typename T, typename A>
inline void al::array::array_3d<T, A>::assign(size_type size_2, size_type size_1, size_type size_0, const value_type& value)
{
  swap(array_3d(size_2, size_1, size_0, value));
}

template <typename T, typename A>
inline void al::array::array_3d<T, A>::swap(array_3d<T, A>& that)
{
  std::swap(array_1d_, that.array_1d_);
  std::swap(size_[0], that.size_[0]);
  std::swap(size_[1], that.size_[1]);
  std::swap(size_[2], that.size_[2]);
}

template <typename T, typename A>
inline void al::array::array_3d<T, A>::clear()
{
  array_1d_.clear();
  size_[0] = 0;
  size_[1] = 0;
  size_[2] = 0;
}

// -----------------------------------------------------------------------------

template <typename T, typename A>
inline typename al::array::array_3d<T, A>::allocator_type al::array::array_3d<T, A>::get_allocator() const
{
  return array_1d_.get_allocator();
}

// -----------------------------------------------------------------------------

#endif // AL_ARRAY_ARRAY_3D_HPP_INCLUDED
