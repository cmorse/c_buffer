//
//  test.cpp
//
//  Copyright (c) 2011 Boris Kolpackov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  Tests for the buffer class.

#define BOOST_TEST_MODULE buffertest
#include <boost/test/included/unit_test.hpp>

#include "buffer.hpp"

using namespace std;

const char *null = 0;
char data[] = "simple buffer class";
char data2[] = "simple buffer class\0simple buffer class";
char data3[] = "aaaaaaaaaa";

// buffer(size_type = 0)
BOOST_AUTO_TEST_SUITE(buffer_constructor_default)
  BOOST_AUTO_TEST_CASE(buffer1)
  {
    buffer b;
    BOOST_CHECK(b.size() == 0);
    BOOST_CHECK(b.empty());
  }

  BOOST_AUTO_TEST_CASE(buffer2)
  {
    buffer b(0);
    BOOST_CHECK(b.size() == 0);
    BOOST_CHECK(b.empty());
  }

  BOOST_AUTO_TEST_CASE(buffer3)
  {
    buffer b(256);
    BOOST_CHECK(b.size() == 256);
    BOOST_CHECK(b.capacity() >= 256);
    BOOST_CHECK(!b.empty());
    BOOST_CHECK(b.data() != 0);
  }
BOOST_AUTO_TEST_SUITE_END( ) // buffer_constructor_default

// buffer(size_type, size_type)
BOOST_AUTO_TEST_SUITE(buffer_constructor_2)
  BOOST_AUTO_TEST_CASE(buffer1)
  {
    const size_t n = 0;
    buffer b(n, n);
    BOOST_CHECK(b.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(buffer2)
  {
    buffer b(256, 1024);
    BOOST_CHECK(b.size() == 256);
    BOOST_CHECK(b.capacity() >= 1024);
    BOOST_CHECK(b.data() != 0);
  }
BOOST_AUTO_TEST_SUITE_END( ) // buffer_constructor_2

// buffer(const void*, size_type)
BOOST_AUTO_TEST_SUITE(buffer_constructor_3)
  BOOST_AUTO_TEST_CASE(buffer1)
  {
    buffer b(null, 0);
    BOOST_CHECK(b.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(buffer2)
  {
    const size_t n = sizeof(data);
    buffer b(data, n);
    BOOST_CHECK(b.size() == n);
    BOOST_CHECK(memcmp(b.data(), data, n) == 0);
  }
BOOST_AUTO_TEST_SUITE_END( ) // buffer_constructor_3

// buffer(const void*, size_type, size_type)
BOOST_AUTO_TEST_SUITE(buffer_constructor_4)
  BOOST_AUTO_TEST_CASE(buffer1)
  {
    buffer b(null, 0, 0);
    BOOST_CHECK(b.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(buffer2)
  {
    const size_t n = sizeof(data);
    buffer b(data, n, 2 * n);
    BOOST_CHECK(b.size() == n);
    BOOST_CHECK(b.capacity() >= 2 * n);
    BOOST_CHECK(memcmp(b.data(), data, n) == 0);
  }
BOOST_AUTO_TEST_SUITE_END( ) // buffer_constructor_4

// buffer(const void*, size_type, size_type, bool)
BOOST_AUTO_TEST_SUITE(buffer_constructor_5)
  BOOST_AUTO_TEST_CASE(buffer1)
  {
    const size_t n = sizeof(data);
    buffer b(data, n, n, false);
    BOOST_CHECK(b.size() == n);
    BOOST_CHECK(b.capacity() == n);
    BOOST_CHECK(b.data() == data);
  }

  BOOST_AUTO_TEST_CASE(buffer2)
  {
    const size_t n = sizeof(data);
    char *buf = new char[n];
    buffer b(buf, n, n, true);
    BOOST_CHECK(b.size() == n);
    BOOST_CHECK(b.capacity() == n);
    BOOST_CHECK(b.data() == buf);
  }
BOOST_AUTO_TEST_SUITE_END( ) // buffer_constructor_5

// buffer(const buffer&)
BOOST_AUTO_TEST_SUITE(buffer_constructor_copy)
  BOOST_AUTO_TEST_CASE(buffer1)
  {
    buffer b;
    buffer c(b);
    BOOST_CHECK(c.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(buffer2)
  {
    const size_t n = sizeof(data);
    buffer b(data, n);
    buffer c(b);
    BOOST_CHECK(c.size() == n);
    BOOST_CHECK(memcmp(c.data(), data, n) == 0);
  }
BOOST_AUTO_TEST_SUITE_END( ) // buffer_constructor_copy

// operator=(const buffer&)
BOOST_AUTO_TEST_SUITE(buffer_operator_assignment)
  // Assign empty buffer
  BOOST_AUTO_TEST_CASE(assign_empty)
  {
    buffer b;
    buffer c;
    c = b;
    BOOST_CHECK(c.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(buffer2)
  {
    buffer b;
    buffer c(data, sizeof(data));
    c = b;
    BOOST_CHECK(c.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(buffer3)
  {
    const size_t n = sizeof(data);
    buffer b(data, n);
    buffer c(size_t(0), n);
    char *d = c.data();
    c = b;
    BOOST_CHECK(c.size() == n);
    BOOST_CHECK(c.data() == d);
  }

  BOOST_AUTO_TEST_CASE(buffer4)
  {
    const size_t n = sizeof(data);
    buffer b(data, n);
    buffer c(size_t(0), n / 2);
    c = b;
    BOOST_CHECK(c.size() == n);
  }
BOOST_AUTO_TEST_SUITE_END( ) // buffer_operator_assignment

// swap(buffer&)
BOOST_AUTO_TEST_CASE(buffer_swap)
{
  const size_t n = sizeof(data);
  buffer b(data, n);
  char *d = b.data();
  buffer c;
  c.swap(b);
  BOOST_CHECK(b.size() == 0);
  BOOST_CHECK(c.size() == n);
  BOOST_CHECK(c.data() == d);
}

// detach()
BOOST_AUTO_TEST_CASE(buffer_detach)
{
  const size_t n(sizeof(data));
  buffer b(data, n);
  char *buf(b.detach());
  BOOST_CHECK(b.size() == 0);
  BOOST_CHECK(b.capacity() == 0);
  delete[] buf;
}

BOOST_AUTO_TEST_SUITE(buffer_assign)
  // assign(const void*, size_type)
  BOOST_AUTO_TEST_CASE(buffer_assign_1)
  {
    const size_t n = sizeof(data2);
    buffer b(data, sizeof(data));
    b.assign(data2, n);
    BOOST_CHECK(b.size() == n);
    BOOST_CHECK(memcmp(b.data(), data2, n) == 0);
  }

  // assign(const void*, size_type, size_type, bool)
  BOOST_AUTO_TEST_CASE(buffer_assign_2)
  {
    const size_t n = sizeof(data);
    char *buf = new char[n];
    buffer b;
    b.assign(buf, n, n, true);
    BOOST_CHECK(b.size() == n);
    BOOST_CHECK(b.capacity() == n);
    BOOST_CHECK(b.data() == buf);
  }
BOOST_AUTO_TEST_SUITE_END( ) // buffer_assign

// append(const void*, size_type)
BOOST_AUTO_TEST_CASE(buffer_append)
{
  const size_t n = sizeof(data);
  buffer b(data, n);
  b.append(data, n);
  BOOST_CHECK(b.size() == 2 * n);
  BOOST_CHECK(memcmp(b.data(), data2, sizeof(data2)) == 0);
}

// fill(char = 0)
BOOST_AUTO_TEST_CASE(buffer_fill)
{
  buffer b(10);
  b.fill('a');
  BOOST_CHECK(memcmp(b.data(), data3, sizeof(data3) - 1) == 0);
}

// size(size_type)
BOOST_AUTO_TEST_SUITE(buffer_size)
  BOOST_AUTO_TEST_CASE(buffer_size1)
  {
    buffer b(256, 512);
    BOOST_CHECK(!b.size(512));
    BOOST_CHECK(b.size() == 512);
  }

  BOOST_AUTO_TEST_CASE(buffer_size2)
  {
    buffer b(256, 512);
    BOOST_CHECK(b.size(1024));
    BOOST_CHECK(b.size() == 1024);
  }
BOOST_AUTO_TEST_SUITE_END( ) // buffer_size

// capacity(size_type)
BOOST_AUTO_TEST_SUITE(buffer_capacity)
  BOOST_AUTO_TEST_CASE(buffer_capacity1)
  {
    buffer b(256, 512);
    BOOST_CHECK(!b.capacity(512));
    BOOST_CHECK(b.size() == 256);
  }

  BOOST_AUTO_TEST_CASE(buffer_capacity2)
  {
    buffer b(256, 512);
    b.size(1024);
    BOOST_CHECK(b.capacity() >= 1024);
    BOOST_CHECK(b.size() == 1024);
  }
BOOST_AUTO_TEST_SUITE_END( ) // buffer_capacity

// clear()
BOOST_AUTO_TEST_CASE(buffer_clear)
{
  buffer b(256);
  b.clear();
  BOOST_CHECK(b.empty());
}

// operator==(const buffer&, const buffer&)
BOOST_AUTO_TEST_SUITE(buffer_operator_compare)
  BOOST_AUTO_TEST_CASE(buffer_operator_compare1)
  {
    buffer b1, b2;
    BOOST_CHECK(b1 == b2);
  }

  BOOST_AUTO_TEST_CASE(buffer_operator_compare2)
  {
    const size_t n = sizeof(data);
    buffer b1(data, n), b2(data, n);
    BOOST_CHECK(b1 == b2);
  }

  BOOST_AUTO_TEST_CASE(buffer_operator_compare3)
  {
    buffer b1(data, sizeof(data)), b2;
    BOOST_CHECK(b1 != b2);
  }
BOOST_AUTO_TEST_SUITE_END( ) // buffer_operator_compare

