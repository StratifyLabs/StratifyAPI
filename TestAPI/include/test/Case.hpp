/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_TEST_CASE_HPP_
#define SAPI_TEST_CASE_HPP_

/*! \cond */

#include "../var/String.hpp"
namespace test {

class Test;

class Case
{
public:
	Case(Test * test, const var::String& name);
	~Case();

private:
	Test * m_test;
};

}

/*! \endcond */


#endif // SAPI_TEST_CASE_HPP_
