#include <boost/json.hpp>
#include <functional>
#include <vector>

namespace plsm {
namespace utils {

template <typename T> auto mapToJson(const std::vector<T> &vector) {
  boost::json::array result(vector.size());
  for (size_t i = 0; i < vector.size(); i++)
    result[i] = vector[i]->toJson();
  return result;
}

template <typename T, typename Mapper>
auto mapToJson(const std::vector<T> &vector, const Mapper &mapper) {
  boost::json::array result(vector.size());
  for (size_t i = 0; i < vector.size(); i++)
    result[i] = mapper(vector[i]);
  return result;
}

} // namespace utils
} // namespace plsm
