#ifndef SAPI_ARG_ARGUMENT_HPP
#define SAPI_ARG_ARGUMENT_HPP

#include <utility>
#include <pthread.h>
#include <signal.h>

#if !defined __link || defined __macosx
#include <unistd.h>
#include <sched.h>
#endif

#include "../api/ArgumentObject.hpp"

namespace arg {

template <typename T, typename Parameter>
class Argument {
public:
	explicit Argument(T const& value) : m_value(value) {}

	template<typename T_ = T>
	explicit Argument(T&& value,
							typename std::enable_if<!std::is_reference<T_>{},
							std::nullptr_t>::type = nullptr)
		: m_value(std::move(value)) {}

	T& argument() { return m_value; }
	T const& argument() const {return m_value; }
private:
	T m_value;
};


template <typename T, typename Parameter, typename A>
class ImplicitArgument {
public:
	ImplicitArgument(T const& value) : m_value(value) {}
	ImplicitArgument(A const& value) : m_value(value.argument()) {}

	template<typename T_ = T>
	ImplicitArgument(T&& value,
						  typename std::enable_if<!std::is_reference<T_>{},
						  std::nullptr_t>::type = nullptr)
		: m_value(std::move(value)) {}

	T& argument() { return m_value; }
	T const& argument() const {return m_value; }
private:
	T m_value;
};

}


#endif // SAPI_ARG_ARGUMENT_HPP
