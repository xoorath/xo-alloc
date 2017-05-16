//////////////////////////////////////////////////////////////////////////////////////////
//
//  xo-alloc.h (version 0.1, May 2017)
//
//  A public domain single header file module for allocating memory. C++11 or newer 
//  required.
//
//  AUTHOR
//    Jared Thomson (@xoorath)
//
//  STREET CRED
//    Inspired by Sean Barrett's stb.
//    https://github.com/nothings
//
//  LICENSE
//
//    This software is dual-licensed to the public domain and under the following
//    license: you are granted a perpetual, irrevocable license to copy, modify,
//    publish, and distribute this file as you see fit.
//
//  USAGE:
//    // Create an allocator of 1024 bytes.
//    BlockAllocator<1024> MyAlloc;
//
//    //...
//    Apple* apple = MyAlloc.New<Apple>("constructor params", 1, 2, 3);
//    Banana* banana = MyAlloc.New<Banana>();
//
//    //...
//
//    MyAlloc.Delete(apple);
//    MyAlloc.Delete(banana);
//
//  IMPLEMENTATION NOTES:
//
//    There's a buffer of memory specified as a template parameter. The buffer starts off
//    as a single large chunk marked as "free". When New is called, we start at the first
//    buffer and see if it's free. If it isn't we skip ahead by that buffers size and 
//    check the next chunk onwards until a free buffer with an available size is found
//    or the end of the buffer is reached.
//
//    Chunks are implemented as a 4 byte 31 bit integer and 1 bit "free" flag.
//
//    As memory is freed, we attempt to find other free buffers adjacent and join them 
//    together.
//
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <new>
#include <stdint.h>

namespace xo {
  template<uint32_t SIZE>
  class BlockAllocator {
    static_assert(SIZE < (1 << 31), "BlockAllocator doesn't support being larger than 1^31");
    struct Block;
  public:
    BlockAllocator() {
      Block* b = reinterpret_cast<Block*>(m_Buffer);
      b->Free = true;
      b->Size = SIZE - sizeof(Block);
    }

    template<typename T, typename...Args>
    T* New(Args...args) {
      void* mem = static_cast<void*>(Malloc<sizeof(T)>());
      return mem ? new(mem) T(args...) : nullptr;
    }

    template<typename T>
    void Delete(T* m) {
      if(m) {
        m->~T();
        Free(static_cast<void*>(m));
      }
    }

  private:
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

    template<uint32_t size>
    void* Malloc() {
      static_assert(size < SIZE-sizeof(Block), "Allocation requested is larger than the allocator.");
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

    void Free(void* mem) {
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

}
