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
//

#include <cstddef>   // std::size_t
#include <cstring>   // std::memcmp
#include <cassert>

#include "buffer.hpp"

using namespace std;

int main ()
{
  const char* null = 0;
  char data[] = "simple buffer class";
  char data2[] = "simple buffer class\0simple buffer class";
  char data3[] = "aaaaaaaaaa";

  // buffer (size_type = 0)
  //
  {
    buffer b;
    assert (b.size () == 0);
    assert (b.empty ());
  }

  {
    buffer b (0);
    assert (b.size () == 0);
    assert (b.empty ());
  }

  {
    buffer b (256);
    assert (b.size () == 256);
    assert (b.capacity () >= 256);
    assert (!b.empty ());
    assert (b.data () != 0);
  }

  // buffer (size_type, size_type)
  //
  {
    size_t n (0);
    buffer b (n, n);
    assert (b.size () == 0);
  }

  {
    buffer b (256, 1024);
    assert (b.size () == 256);
    assert (b.capacity () >= 1024);
    assert (b.data () != 0);
  }

  // buffer (const void*, size_type)
  //
  {
    buffer b (null, 0);
    assert (b.size () == 0);
  }

  {
    size_t n (sizeof (data));
    buffer b (data, n);
    assert (b.size () == n);
    assert (memcmp (b.data (), data, n) == 0);
  }

  // buffer (const void*, size_type, size_type)
  //
  {
    buffer b (null, 0, 0);
    assert (b.size () == 0);
  }

  {
    size_t n (sizeof (data));
    buffer b (data, n, 2 * n);
    assert (b.size () == n);
    assert (b.capacity () >= 2 * n);
    assert (memcmp (b.data (), data, n) == 0);
  }

  // buffer (const void*, size_type, size_type, bool)
  //
  {
    size_t n (sizeof (data));
    buffer b (data, n, n, false);
    assert (b.size () == n);
    assert (b.capacity () == n);
    assert (b.data () == data);
  }

  {
    size_t n (sizeof (data));
    char* buf (new char[n]);
    buffer b (buf, n, n, true);
    assert (b.size () == n);
    assert (b.capacity () == n);
    assert (b.data () == buf);
  }

  // buffer (const buffer&)
  //
  {
    buffer b;
    buffer c (b);
    assert (c.size () == 0);
  }

  {
    size_t n (sizeof (data));
    buffer b (data, n);
    buffer c (b);
    assert (c.size () == n);
    assert (memcmp (c.data (), data, n) == 0);
  }

  // operator= (const buffer&)
  //
  {
    buffer b;
    buffer c;
    c = b;
    assert (c.size () == 0);
  }

  {
    buffer b;
    buffer c (data, sizeof (data));
    c = b;
    assert (c.size () == 0);
  }

  {
    size_t n (sizeof (data));
    buffer b (data, n);
    buffer c (size_t (0), n);
    char* d (c.data ());
    c = b;
    assert (c.size () == n);
    assert (c.data () == d);
  }

  {
    size_t n (sizeof (data));
    buffer b (data, n);
    buffer c (size_t (0), n / 2);
    c = b;
    assert (c.size () == n);
  }

  // swap (buffer&)
  //
  {
    size_t n (sizeof (data));
    buffer b (data, n);
    char* d (b.data ());
    buffer c;
    c.swap (b);
    assert (b.size () == 0);
    assert (c.size () == n);
    assert (c.data () == d);
  }

  // detach ()
  //
  {
    size_t n (sizeof (data));
    buffer b (data, n);
    char* buf (b.detach ());
    assert (b.size () == 0);
    assert (b.capacity () == 0);
    delete[] buf;
  }

  // assign (const void*, size_type)
  //
  {
    size_t n (sizeof (data2));
    buffer b (data, sizeof (data));
    b.assign (data2, n);
    assert (b.size () == n);
    assert (memcmp (b.data (), data2, n) == 0);
  }

  // assign (const void*, size_type, size_type, bool)
  //
  {
    size_t n (sizeof (data));
    char* buf (new char[n]);
    buffer b;
    b.assign (buf, n, n, true);
    assert (b.size () == n);
    assert (b.capacity () == n);
    assert (b.data () == buf);
  }

  // append (const void*, size_type)
  //
  {
    size_t n (sizeof (data));
    buffer b (data, n);
    b.append (data, n);
    assert (b.size () == 2 * n);
    assert (memcmp (b.data (), data2, sizeof (data2)) == 0);
  }

  // fill (char = 0)
  //
  {
    buffer b (10);
    b.fill ('a');
    assert (memcmp (b.data (), data3, sizeof (data3) - 1) == 0);
  }

  // size (size_type)
  //
  {
    buffer b (256, 512);
    assert (!b.size (512));
    assert (b.size () == 512);
  }

  {
    buffer b (256, 512);
    assert (b.size (1024));
    assert (b.size () == 1024);
  }

  // capacity (size_type)
  //
  {
    buffer b (256, 512);
    assert (!b.capacity (512));
  }

  {
    buffer b (256, 512);
    b.size (1024);
    assert (b.capacity () >= 1024);
  }

  // clear ()
  //
  {
    buffer b (256);
    b.clear ();
    assert (b.empty ());
  }

  // find (char, size_type = 0)
  //
  {
    buffer b (data, sizeof (data));
    assert (b.find ('e') == 5);
  }

  // rfind (char, size_type = npos)
  //
  {
    buffer b (data, sizeof (data));
    assert (b.rfind ('e') == 11);
  }

  // operator== (const buffer&, const buffer&)
  //
  {
    buffer b1, b2;
    assert (b1 == b2);
  }

  {
    size_t n (sizeof (data));
    buffer b1 (data, n), b2 (data, n);
    assert (b1 == b2);
  }

  {
    size_t n (sizeof (data));
    buffer b1 (data, n), b2;
    assert (b1 != b2);
  }
}