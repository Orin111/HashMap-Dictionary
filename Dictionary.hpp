
#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_

#define INVALID_KEY "ERROR: Invalid Key"

#include "HashMap.hpp"

class InvalidKey : public std::invalid_argument
{
 public:
  /**
 * Construct
 * @param msg an error message
 */
  explicit InvalidKey (const std::string &msg) :
      std::invalid_argument (msg)
  {}

  /**
 * Def construct
 */
  InvalidKey () : std::invalid_argument ("")
  {}
};

class Dictionary : public HashMap<std::string, std::string>
{
 public:
  using HashMap<std::string, std::string>::HashMap;
  /**
 * erases the key from the dictionary. (using erase func of hashmap)
  * if  key isn't exist -> throw InvalidKey exception.
 * @param key string
 * @return if key erased->true, else throw exception
 */
  bool erase (const std::string &key) override
  {
    if (!HashMap<std::string, std::string>::erase (key))
      {
        throw InvalidKey (INVALID_KEY);
      }
    return true;
  }
  template<class Iterator>
  /**
 * add all of the elements in the iterator to the dictionary
 * @tparam Iterator
 * @param start- iterator`s begin, of pairs of strings
 * @param end
 */
  void update (Iterator start, Iterator end)
  {
    while (start != end)
      {
        (*this)[start->first] = start->second;
        start++;
      }
  }
};
#endif //_DICTIONARY_HPP_
