#ifndef SYSTEM_TYPE_DETECTOR_HPP
#define SYSTEM_TYPE_DETECTOR_HPP

#include <boost/type_traits.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/array.hpp>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <array>
#include <map>

namespace mps {
namespace type_traits {
namespace type_detector {
template <class T_object> struct is_shared_ptr : boost::false_type {};
template <class T_object> struct is_shared_ptr<boost::shared_ptr<T_object>> : boost::true_type {};

template <class T_object> struct is_std_vector : boost::false_type {};
template <class T_object, class Alloc> struct is_std_vector<std::vector<T_object,Alloc>> : boost::true_type {};

template <class T_object> struct is_boost_array : public boost::false_type {};
template <class T_object, std::size_t N> struct is_boost_array <boost::array<T_object,N>> : public boost::true_type {};

template <class T_object> struct is_std_array : public boost::false_type {};
template <class T_object, std::size_t N> struct is_std_array<std::array<T_object,N>> : public boost::true_type {};

template <class T_object> struct is_std_queue : boost::false_type {};
template <class T_object, class Seq> struct is_std_queue<std::queue<T_object,Seq>> : boost::true_type {};

template <class T_object> struct is_std_stack : boost::false_type {};
template <class T_object, class Seq> struct is_std_stack<std::stack<T_object,Seq>> : boost::true_type {};

template <class T_object> struct is_std_list : boost::false_type {};
template <class T_object, class Alloc> struct is_std_list<std::list<T_object,Alloc>> : boost::true_type {};

template <class T_object> struct is_std_map : boost::false_type {};
template <class T_object, class Compare, class Alloc> struct is_std_map<
        std::map<T_object,Compare,Alloc>> : boost::true_type {};

template <class T_object> struct is_boost_graph : boost::false_type {};
template <class T_object> struct is_boost_graph<boost::adjacency_list<T_object>> : boost::true_type {};
}           /// <--- type_detector
}       /// <--- type_traits
}   /// <--- mps

#endif
