#ifndef __CELL_ENGINE_FILTER_H__
#define __CELL_ENGINE_FILTER_H__

#include <set>

namespace cell
{
namespace engine
{

enum class filter_op
{
  EQ,
  NE,
  LE,
  LT,
  GE,
  GT,
  TRUE,
  FALSE
};

template<typename T>
class contains
  {
    std::set<T> s;
  public:
  contains(const std::set<T>& _s):s(_s) {}

  bool operator()(const T& column_value)
  {
    return s.find(column_value) != s.end();
  }

  };

template<typename T, filter_op op>
class filter
{
  T predicate_value;
public:
 filter(const T& _predicate_value):predicate_value(_predicate_value) {}

  /**
   * Runs a simple filter over the column store and returns a list
   * of matching columns.
   *
   * @param column_value: The value to compare with.
   *
   * @returns: True if the predicate matches.
   */
  bool operator()(const T& column_value);
};

class filter<T, filter_op::EQ>
{
 public:
  bool operator()(const T& column_value)
  {   
    return (predicate_value == column_value);
  }    
};

class filter<T, filter_op::NE>
{
 public:
  bool operator()(const T& column_value)
  {   
    return (predicate_value != column_value);
  }    
};

class filter<T, filter_op::LE>
{
 public:
  bool operator()(const T& column_value)
  {   
    return (predicate_value <= column_value);
  }    
};

class filter<T, filter_op::GE>
{
 public:
  bool operator()(const T& column_value)
  {   
    return (predicate_value >= column_value);
  }    
};

class filter<T, filter_op::LT>
{
 public:
  bool operator()(const T& column_value)
  {   
    return (predicate_value < column_value);
  }    
};

class filter<T, filter_op::GT>
{
 public:
  bool operator()(const T& column_value)
  {   
    return (predicate_value > column_value);
  }    
};

class filter<T, filter_op::TRUE>
{
 public:
  bool operator()(const T& column_value)
  {   
    return true;
  }    
};

class filter<T, filter_op::FALSE>
{
 public:
  bool operator()(const T& column_value)
  {   
    return false;
  }    
};

template<typename T, bool and>
class binary_predicate
{
  filter left, right;
 public:
 binary_predicate(const filter& _left, const filter& _right) 
   : left(_left), right(_right) {}
 
  bool operator()(const T& column_value);
};

 class binary_predicate<T, true>
{
 public:
  bool operator()(const T& column_value)
  {
    return left(column_value) && right(column_value);
  }

};

 class binary_predicate<T, false>
{
 public:
  bool operator()(const T& column_value)
  {
    return left(column_value) || right(column_value);
  }

};


} // end namespace engine
} // end namespace cell
