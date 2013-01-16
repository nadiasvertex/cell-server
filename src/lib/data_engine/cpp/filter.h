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
class predicate
{
public:
  virtual bool operator()(const T& column_value)=0;
};

template<typename T>
class contains : public predicate<T>
{
  std::set<T> s;
public:
  contains(const std::set<T>& _s):s(_s) {}
  virtual ~contains() {}

  virtual bool operator()(const T& column_value) {
    return s.find(column_value) != s.end();
  }

};

template<typename T, filter_op op>
class filter : public predicate<T>
{
  T predicate_value;
public:
  filter(const T& _predicate_value):predicate_value(_predicate_value) {}
  virtual ~filter() {}
};

template<typename T>
class filter<T, filter_op::EQ>
{
  T predicate_value;
public:
  filter(const T& _predicate_value):predicate_value(_predicate_value) {}
  virtual ~filter() {}
  bool operator()(const T& column_value) {
    return (predicate_value == column_value);
  }
};

template<typename T>
class filter<T, filter_op::NE>
{
  T predicate_value;
public:
  filter(const T& _predicate_value):predicate_value(_predicate_value) {}
  virtual ~filter() {}

  bool operator()(const T& column_value) {
    return (predicate_value != column_value);
  }
};

template<typename T>
class filter<T, filter_op::LE>
{
  T predicate_value;
public:
  filter(const T& _predicate_value):predicate_value(_predicate_value) {}
  virtual ~filter() {}

  bool operator()(const T& column_value) {
    return (predicate_value <= column_value);
  }
};

template<typename T>
class filter<T, filter_op::GE>
{
  T predicate_value;
public:
  filter(const T& _predicate_value):predicate_value(_predicate_value) {}
  virtual ~filter() {}

  bool operator()(const T& column_value) {
    return (predicate_value >= column_value);
  }
};

template<typename T>
class filter<T, filter_op::LT>
{
  T predicate_value;
public:
  filter(const T& _predicate_value):predicate_value(_predicate_value) {}
  virtual ~filter() {}

  bool operator()(const T& column_value) {
    return (predicate_value < column_value);
  }
};

template<typename T>
class filter<T, filter_op::GT>
{
  T predicate_value;
public:
  filter(const T& _predicate_value):predicate_value(_predicate_value) {}
  virtual ~filter() {}

  bool operator()(const T& column_value) {
    return (predicate_value > column_value);
  }
};

template<typename T>
class filter<T, filter_op::TRUE>
{
  T predicate_value;
public:
  filter(const T& _predicate_value):predicate_value(_predicate_value) {}
  virtual ~filter() {}

  bool operator()(const T& column_value) {
    return true;
  }
};

template<typename T>
class filter<T, filter_op::FALSE>
{
  T predicate_value;
public:
  filter(const T& _predicate_value):predicate_value(_predicate_value) {}
  virtual ~filter() {}

  bool operator()(const T& column_value) {
    return false;
  }
};

template<typename T, bool _and_>
class binary_predicate : public predicate<T>
{
  predicate<T> left, right;
public:
  binary_predicate(const predicate<T>& _left, const predicate<T>& _right)
    : left(_left), right(_right) {}

  bool operator()(const T& column_value);
};

template<typename T>
class binary_predicate<T, true>
{
  predicate<T> left, right;
public:
  binary_predicate(const predicate<T>& _left, const predicate<T>& _right)
    : left(_left), right(_right) {}
  bool operator()(const T& column_value) {
    return left(column_value) && right(column_value);
  }

};

template<typename T>
class binary_predicate<T, false>
{
  predicate<T> left, right;
public:
  binary_predicate(const predicate<T>& _left, const predicate<T>& _right)
    : left(_left), right(_right) {}
  bool operator()(const T& column_value) {
    return left(column_value) || right(column_value);
  }

};


} // end namespace engine
} // end namespace cell

#endif // __CELL_ENGINE_FILTER_H__
