#ifndef _HASHMAP_HPP_
#define _HASHMAP_HPP_

#include <iostream>
#include <vector>

#define DEF_CAP 16
#define MAX_LOAD 0.75
#define MIN_LOAD 0.25
#define OUT_OF_RANGE "error: out of range"
#define LENGHT_ERROR "error: vectors are not at the same size"

template<typename KeyT, typename ValueT>
class HashMap
{
  typedef std::vector<std::pair<KeyT, ValueT>> hashmap_bucket;
 private:
  int _size;
  int _capacity;
  hashmap_bucket *_buckets;

  /**
   * hash func
   * @param key KeyT-> calculate the hash value
   * @return hash value
   */
  int hash (const KeyT &key, int hash_capacity) const;
  /**
  * decrease the capacity while current load factor is under the lower bound
  * @param new_capacity
  */
  void re_hash (int new_capacity);

 public:
  /**
  * Default constructor
  */
  HashMap ();

  /**
   * Constructor
   * @param keys vector of  keys
   * @param values vector of values
   */
  HashMap (const std::vector<KeyT> &keys, const std::vector<ValueT> &values);

  /**
   * Copy-Constructor
   * @param other HashMap reference that we will copy from
   */
  HashMap (const HashMap &other);

  /**
  * @return size of the Hash map
  */
  int size () const
  {
    return _size;
  }

  /**
  * @return capacity of the Hash map
  */
  int capacity () const
  {
    return _capacity;
  }

  /**
   * @return if the Hash map is empty -> true, else false
   */
  bool empty () const
  {
    return _size == 0;
  }

  /**
  * @return load factor of the map (size/capacity)
  */
  double get_load_factor () const
  {
    return (double) _size / (double) _capacity;
  }

  /**
   * @param key
   * @return if the key is in the hashmap -> true, else false
   */
  bool contains_key (const KeyT &key) const;

  /**
   * @param key
   * @param value
   * @return if the key not in the hashmap, we add the value ->true
   * else false.
   */
  bool insert (const KeyT &key, const ValueT &value);

  /**
   * @param key const reference to a KeyT key
   * @return  if key in the hashmap r-> value, else insert the key-value
   */
  ValueT &operator[] (const KeyT &key);

  /**
  * @param key const reference to a KeyT key
  * @return  if key in the hashmap r-> value, else insert the key-value
  */
  ValueT operator[] (const KeyT &key) const;

  /**
   * @param key
   * @return if key not in the hashmap->out of range else, ->value
   */
  ValueT &at (const KeyT &key);

  /**
  * @param key
  * @return if key not in the hashmap->out of range else, ->value
  */
  const ValueT &at (const KeyT &key) const;

  /**
   * @param key
   * @return key is in the hashmap, erased->true, else -> false
   */
  virtual bool erase (const KeyT &key);

  /**
   * @param key
   * @return the size of the bucket that the key is located in
   */
  int bucket_size (const KeyT &key) const;

  /**
   * @param key const reference to a KeyT key
   * @return the bucket index that the key is located in, else-> out of range
   */
  int bucket_index (const KeyT &key) const;

  /**
   *
   * @param other hashmap
   * @return if the current hashmap == other->true, else ->false
   */
  bool operator== (const HashMap &other) const;

  /**
   *
   * @param other hashmap
   * @return if the current hashmap != other->true, else ->false
   */
  bool operator!= (const HashMap &other) const;

  /**
  * @param other hashmap
  * @return this HashMap after assigning the other hashmap
  */
  HashMap &operator= (const HashMap &other);

  /**
   * Destructor
   */
  virtual ~ HashMap ();

  /**
   * clear the hashmap from data
   */
  void clear ();

//******
  class ConstIterator
  {
   private:
    std::pair<KeyT, ValueT> *_kv;
    const HashMap<KeyT, ValueT> &_hash_buckets;
    int _bucket;
    int _index;

   public:
    typedef std::pair<KeyT, ValueT> value_type;
    typedef const std::pair<KeyT, ValueT> &reference;
    typedef const std::pair<KeyT, ValueT> *pointer;

    /**
     * constructor
     * @param kv pair of key, value
     * @param hashmap
     * @param bucket
     * @param bucket_index
     */
    explicit ConstIterator (value_type *kv,
                            const HashMap<KeyT, ValueT> &hashmap, int bucket,
                            int bucket_index) :
        _kv (kv), _hash_buckets (hashmap), _bucket (bucket),
        _index (bucket_index)
    {

    }

    /**
     * Pre increment
     * @return ref to the iterator after the incres
     */
    ConstIterator &operator++ ()
    {
      _index++;
      if (_index >= (int) _hash_buckets._buckets[_bucket].size ())
        {
          _index = 0;
          _bucket++;
          while (_hash_buckets._buckets[_bucket].empty ()
                 && _bucket < _hash_buckets._capacity)
            {
              _bucket++;
            }

        }
      if (_bucket < _hash_buckets._capacity)
        {
          _kv = &_hash_buckets._buckets[_bucket][_index];
        }
      else
        {
          _kv = nullptr;
        }
      return *this;
    }

    /**
     * post increment
     * @return ref to the iterator before the incres
     */
    ConstIterator operator++ (int)
    {
      ConstIterator tmp = *this;
      operator++ ();
      return tmp;
    }

    /**
   * @param other constiterator
   * @return if rhs == this ->true, else -> false
   */
    bool operator== (const ConstIterator &rhs) const
    {
      return _kv == rhs._kv && (&_hash_buckets == &rhs._hash_buckets);
    }

    /**
     *
     * @param other constiterator
     * @return if rhs != this ->true, else -> false
     */
    bool operator!= (const ConstIterator &other) const
    {
      return !(*this == other);

    }

    /**
     * @return pointer to kv
     */
    pointer operator-> () const
    {
      return _kv;
    }

    /**
     * @return de-reference of kv
     */
    reference operator* () const
    {
      return *_kv;
    }
  };
  using const_iterator = ConstIterator;

  /**
   *
   * @return iterator to the begin of the map, if empty-> to the end
   */
  const_iterator begin () const
  {
    for (int i = 0; i < _capacity; i++)
      {
        if (!_buckets[i].empty ())
          {
            return ConstIterator (&_buckets[i][0], *this, i, 0);
          }
      }
    return end ();
  }

  /**
   *
   * @return pointer to the element that after the last item
   */
  const_iterator end () const
  {
    return ConstIterator (nullptr, *this, 0, _capacity);
  }

  /**
   *
   * @return iterator to the begin of the map, if empty-> to the end
   */
  const_iterator cbegin () const
  {
    for (int i = 0; i < _capacity; i++)
      {
        if (!_buckets[i].empty ())
          {
            return ConstIterator (&_buckets[i][0], *this, i, 0);
          }
      }
    return cend ();
  }

  /**
  *
  * @return pointer to the element that after the last item
  */
  const_iterator cend () const
  {
    return ConstIterator (nullptr, *this, 0, _capacity);
  }
};

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap ()
{
  _capacity = DEF_CAP;
  _size = 0;
  _buckets = new hashmap_bucket[DEF_CAP];
}

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap (const std::vector<KeyT> &keys,
                                const std::vector<ValueT> &values): HashMap ()
{
  if (keys.size () != values.size ())
    {
      throw std::length_error (LENGHT_ERROR);
    }
  for (size_t i = 0; i < keys.size (); ++i)
    {
      insert (keys[i], values[i]);
    }
}

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap (const HashMap &other)
{
  _buckets = new hashmap_bucket[other._capacity];
  for (int i = 0; i < other._capacity; ++i)
    {
      _buckets[i] = other._buckets[i];
    }
  _size = other._size;
  _capacity = other._capacity;
}

template<typename KeyT, typename ValueT>
ValueT &HashMap<KeyT, ValueT>::operator[] (const KeyT &key)
{
  {
    if (!contains_key (key))
      {
        insert (key, ValueT ());
      }
    return at (key);
  }
}

template<typename KeyT, typename ValueT>
ValueT HashMap<KeyT, ValueT>::operator[] (const KeyT &key) const
{
  if (!contains_key (key))
    {
      return ValueT ();
    }
  return at (key);
}

template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::hash (const KeyT &key, int hash_capacity) const
{
  return ((int) (std::hash<KeyT> () (key))) & (hash_capacity - 1);
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::contains_key (const KeyT &key) const
{
  for (const auto &i: _buckets[hash (key, _capacity)])
    {
      if (i.first == key)
        { return true; }
    }
  return false;
}
template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::insert (const KeyT &key, const ValueT &value)
{
  if (contains_key (key))
    {
      return false;
    }
  else
    {
      _buckets[hash (key, _capacity)].push_back ({key, value});
      _size++;

      if (get_load_factor () >= MAX_LOAD)
        {
          re_hash (_capacity * 2);
        }
      return true;
    }
}

template<typename KeyT, typename ValueT>
void HashMap<KeyT, ValueT>::re_hash (int new_capacity)
{
  if (new_capacity == _capacity)
    {
      return;
    }
  auto n_buckets = new hashmap_bucket[new_capacity];
  for (const auto &kv: *this)
    {
      n_buckets[hash (kv.first, new_capacity)].push_back (kv);
    }
  delete[] _buckets;
  _buckets = n_buckets;
  _capacity = new_capacity;
}

template<typename KeyT, typename ValueT>
ValueT &HashMap<KeyT, ValueT>::at (const KeyT &key)
{
  for (auto &i: _buckets[hash (key, _capacity)])
    {
      if (i.first == key)
        { return i.second; }
    }
  throw std::out_of_range (OUT_OF_RANGE);
}

template<typename KeyT, typename ValueT>
const ValueT &HashMap<KeyT, ValueT>::at (const KeyT &key) const
{
  for (auto &i: _buckets[hash (key, _capacity)])
    {
      if (i.first == key)
        { return i.second; }
    }
  throw std::out_of_range (OUT_OF_RANGE);
}
template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::erase (const KeyT &key)
{
  if (!contains_key (key))
    {
      return false;
    }
  else
    {
      auto k = hash (key, _capacity);
      int index;
      for (int i = 0; i < bucket_size (key); ++i)
        {
          if (key == _buckets[k][i].first)
            {
              index = i;
            }
        }
      _buckets[k].erase (_buckets[k].begin () + index);
      _size--;
      //****
      int n_capacity = _capacity;
      while ((double) _size / n_capacity < MIN_LOAD)
        {
          n_capacity /= 2;
        }
      n_capacity = std::max (n_capacity, 1);
      re_hash (n_capacity);
      return true;
    }
}
template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::bucket_size (const KeyT &key) const
{
  if (!contains_key (key))
    {
      throw std::out_of_range (OUT_OF_RANGE);
    }
  return (_buckets[hash (key, _capacity)]).size ();
}
template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::bucket_index (const KeyT &key) const
{
  if (!contains_key (key))
    {
      throw std::out_of_range (OUT_OF_RANGE);
    }
  return hash (key, _capacity);
}
template<typename KeyT, typename ValueT>
void HashMap<KeyT, ValueT>::clear ()
{
  delete[] _buckets;
  _buckets = new std::vector<std::pair<KeyT, ValueT>>[_capacity];
  _size = 0;
}
template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::operator== (const HashMap &other) const
{
  if (_size != other._size)
    {
      return false;
    }
  for (const auto &kv: *this)
    {
      if (!other.contains_key (kv.first) || other.at (kv.first) != kv.second)
        { return false; }
    }
  return true;
}
template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::operator!= (const HashMap &other) const
{
  return !(*this == other);
}
template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT> &
HashMap<KeyT, ValueT>::operator= (const HashMap &other)
{
  if (this != &other)
    {
      delete[] _buckets;
      _buckets = new hashmap_bucket[other._capacity];
      for (int i = 0; i < other._capacity; ++i)
        {
          _buckets[i] = other._buckets[i];
        }
      _size = other._size;
      _capacity = other._capacity;
    }
  return *this;
}
template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::~HashMap ()
{
  delete[] _buckets;
}

#endif //_HASHMAP_HPP_
