#pragma once

#include <boost/json.hpp>
#include <functional>
#include <vector>

namespace plsm {
namespace utils {

template <typename T> inline auto mapToJson(const std::vector<T> &vector) {
  boost::json::array result(vector.size());
  for (size_t i = 0; i < vector.size(); i++)
    result[i] = vector[i]->toJson();
  return result;
}

template <typename T, typename Mapper>
inline auto mapToJson(const std::vector<T> &vector, const Mapper &mapper) {
  boost::json::array result(vector.size());
  for (size_t i = 0; i < vector.size(); i++)
    result[i] = mapper(vector[i]);
  return result;
}

template <typename B, typename A> inline bool is(const A *value) {
  return value && dynamic_cast<const B *>(value);
}

template <typename A, typename B>
inline std::unique_ptr<A> ptrcast(std::unique_ptr<B> &ptr) {
  return std::unique_ptr<A>(static_cast<A *>(ptr.release()));
}

template <typename A, typename B>
inline std::shared_ptr<A> ptrcast(std::shared_ptr<B> &ptr) {
  return std::static_pointer_cast<A>(ptr);
}

// template <typename B, typename A> inline bool is(const A &value) {
//   return dynamic_cast<const B *>(&value) != nullptr;
// }

} // namespace utils
} // namespace plsm
