# xo-alloc
A public domain single header file module for allocating memory in a block. C++11 or newer required.

# Example: Create a block allocator

``` cpp
// Create an allocator of 1024 bytes.
BlockAllocator<1024> MyAlloc;

//...
Apple* apple = MyAlloc.New<Apple>("constructor params", 1, 2, 3);
Banana* banana = MyAlloc.New<Banana>();

//...
MyAlloc.Delete(apple);
MyAlloc.Delete(banana);
```

# Todo 1.0:
- Create a consistent "xo-lib" look and feel
- Publish test cases
- Decide how a visualization might be implemented, and do that.
- Do a pass cleaning up your casts, and remove unnecessary casts to char*

# Versions:
### 0.1 (may 2017): Initial commit.

# Street Cred
Inspired by [Sean Barrett's stb](https://github.com/nothings).

# LICENSE
This software is dual-licensed to the public domain and under the following license: 

>You are granted a perpetual, irrevocable license to copy, modify, publish, and distribute this file as you see fit.
