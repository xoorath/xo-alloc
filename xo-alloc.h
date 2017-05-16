//////////////////////////////////////////////////////////////////////
//
// xo-alloc.h (version 0.2, May 2017) public domain
//
// A public domain single header file module for allocating memory. 
// C++11 or newer required.
//
// USAGE:
//   // Create an allocator of 1024 bytes.
//   BlockAllocator<1024> MyAlloc;
//
//   //...
//   Apple* apple = MyAlloc.New<Apple>("constructor params", 1, 2, 3);
//   Banana* banana = MyAlloc.New<Banana>();
//
//   //...
//
//   MyAlloc.Delete(apple);
//   MyAlloc.Delete(banana);
//
// IMPLEMENTATION NOTES:
//
//   There's a buffer of memory specified as a template parameter. 
//   The buffer starts off as a single large chunk marked as "free". 
//   When New is called, we start at the first buffer and see if it's 
//   free. If it isn't we skip ahead by that buffers size and check 
//   the next chunk onwards until a free buffer with an available 
//   size is found or the end of the buffer is reached. Once a free
//   chunk is found, it's split into two parts, the now-allocated
//   chunk and a new chunk following it, sized to indicate the 
//   remainin memory left over in that chunk.
//
//   Chunks are implemented as a 4 byte 31 bit integer and 1 bit 
//   "free" flag. This means the allocator is limited to be 2^31 
//   bytes large.
//
//   As memory is freed, we attempt to find other free buffers 
//   adjacent and join them together.
//
// AUTHOR
//   Jared Thomson <twitter: @xoorath> <email:jared@xoorath.com>
//
// STREET CRED
//   Inspired by Sean Barrett's stb.
//   https://github.com/nothings
//
// VERSION HISTORY
//   0.2  (2017-05-16) adding demo.cpp, public malloc/free, xo styling
//   0.1  (2017-05-15) initial commit
//
// TODO 1.0
//   - realloc, calloc, array new, array delete.
//   - unit tests.
//
// LICENSE
//   See end of file for license information.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// xo-alloc.h generic. included in most xo single header modules.
#pragma once

#if !defined(XO_NAMESPACE_BEGIN) && !defined(XO_NAMESPACE_END)
#define XO_NAMESPACE_BEGIN namespace xo {
#define XO_NAMESPACE_END }
#endif

//////////////////////////////////////////////////////////////////////
// xo-alloc.h declarations.

#define XO_ALLOC_VER "0.2"

#include <new>
#include <stdint.h>

XO_NAMESPACE_BEGIN

template<uint32_t SIZE>
class BlockAllocator {
  static_assert(SIZE < (1 << 31), "BlockAllocator doesn't support being larger than 1^31");
public:

  ////////////////////////////////////////////////////////////////////// BlockAllocator API

  template<typename T, typename...Args>
  T* New(Args...args) {
    void* mem = static_cast<void*>(InternalMallocT<sizeof(T)>());
    return mem ? new(mem) T(args...) : nullptr;
  }

  template<typename T>
  void Delete(T* m) {
    if(m) {
      m->~T();
      InternalFree(static_cast<void*>(m));
    }
  }

  void* Malloc(size_t size) {
    return InternalMalloc(size);
  }

  void Free(void* m) {
    if(m) {
      InternalFree(m);
    }
  }

  BlockAllocator() {
    Block* b = reinterpret_cast<Block*>(m_Buffer);
    b->Free = true;
    b->Size = SIZE - sizeof(Block);
  }

private:
  ////////////////////////////////////////////////////////////////////// BlockAllocator Internal

  char m_Buffer[SIZE];

  void* Begin() { return static_cast<void*>(m_Buffer); }
  void* End() { return static_cast<void*>(static_cast<char*>(m_Buffer)+SIZE); }

  struct Block {
    bool Free:1;
    uint32_t Size:31;

    Block* Next() const {
      return reinterpret_cast<Block*>((char*)(this) + Size + sizeof(Block));
    }

    Block* Previous(Block* s) const {
      while((char*)s + s->Size + sizeof(Block) < (char*)this) {
          s = s->Next();
      }
      return s;
    }
  };

  static void JoinBlocks(Block* b, Block* e, Block* m) {
    if(m->Free) {
      Block* n = m->Next();
      Block* p = m->Previous(b);
      // consume the next block, if it's free.
      if(n < e && n->Free) {
        m->Size += n->Size + sizeof(Block);
        // n is now invalid.
      }

      // consume the previous block.
      if(p != m && p->Free) {
        p->Size += m->Size + sizeof(Block);
        // m is now invalid.
      }
    }
  }

  void* InternalMalloc(uint32_t size) {
    Block* i = static_cast<Block*>(Begin());
    Block* e = static_cast<Block*>(End());
    for(;i < e; i = i->Next()) {
      if(i->Free && i->Size >= size) {
        i->Free = false;
        intptr_t oldSize = i->Size;
        i->Size = size;
        Block* n = i->Next();
        intptr_t nextSize = (oldSize-size-sizeof(Block));
        // if there's not enough space for the next block (meaning n is invalid)
        if(nextSize <= sizeof(Block)) {
          i->Size += nextSize + sizeof(Block);
          n = i;
        }
        // otherwise, break our block in half, creating a new next block. 
        else {
          n->Free = true;
          n->Size = nextSize;
        }
        return reinterpret_cast<char*>(i+1);
      }
    }
    return nullptr;
  }

  template<uint32_t size>
  void* InternalMallocT() {
    static_assert(size < SIZE-sizeof(Block), "Allocation requested is larger than the allocator.");
    return InternalMalloc(size);
  }

  void InternalFree(void* mem) {
    Block* m = reinterpret_cast<Block*>(mem)-1;
    Block* i = static_cast<Block*>(Begin());
    Block* e = static_cast<Block*>(End());

    if(m < i || m > e) {
      return;
    }
    m->Free = true;
    JoinBlocks(i, e, m);
  }
};

XO_NAMESPACE_END

//////////////////////////////////////////////////////////////////////
// This software is available under 2 licenses
// choose whichever you prefer.
//////////////////////////////////////////////////////////////////////

// MIT License
//////////////////////////////////////////////////////////////////////
/*
Copyright (c) 2017 Jared Thomson
Permission is hereby granted, free of charge, to any person obtaining 
a copy of this software and associated documentation files (the 
"Software"), to deal in the Software without restriction, including 
without limitation the rights to use, copy, modify, merge, publish, 
distribute, sublicense, and/or sell copies of the Software, and to 
permit persons to whom the Software is furnished to do so, subject to 
the following conditions:

The above copyright notice and this permission notice shall be 
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////////////////////////////////

// The Unlicense
//////////////////////////////////////////////////////////////////////
/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/
//////////////////////////////////////////////////////////////////////