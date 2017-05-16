# xo-alloc 0.2
A public domain single header file module for allocating memory in a block. C++11 or newer required.

# Example: Create a block allocator

``` cpp
// Create an allocator of 1024 bytes.
xo::BlockAllocator<1024> MyAlloc;

//...
Apple* apple = MyAlloc.New<Apple>("constructor params", 1, 2, 3);
Banana* banana = MyAlloc.New<Banana>();

//...
MyAlloc.Delete(apple);
MyAlloc.Delete(banana);
```

# Todo 1.0:
- ~Create a consistent "xo-lib" look and feel~ added in 0.2
- realloc, calloc, array new, array delete.
- unit tests.
- decide how visualization might be implemented, and do that.
- do cleanup on casts, and use of char*

# Versions:
### 0.2 (2017-05-16) adding demo.cpp, public malloc/free, xo styling
### 0.1 (2017-05-15) initial commit

# Street Cred
Inspired by [Sean Barrett's stb](https://github.com/nothings).

# LICENSE
This software is available under 2 licenses - choose whichever you prefer

### MIT Licence
> Copyright (c) 2017 Jared Thomson
> Permission is hereby granted, free of charge, to any person obtaining 
> a copy of this software and associated documentation files (the 
> "Software"), to deal in the Software without restriction, including 
> without limitation the rights to use, copy, modify, merge, publish, 
> distribute, sublicense, and/or sell copies of the Software, and to 
> permit persons to whom the Software is furnished to do so, subject to 
> the following conditions:
> 
> The above copyright notice and this permission notice shall be 
> included in all copies or substantial portions of the Software.
> 
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
> EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
> MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
> IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
> CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
> TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
> SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


### The Unlicence
> This is free and unencumbered software released into the public domain.
> 
> Anyone is free to copy, modify, publish, use, compile, sell, or
> distribute this software, either in source code form or as a compiled
> binary, for any purpose, commercial or non-commercial, and by any
> means.
> 
> In jurisdictions that recognize copyright laws, the author or authors
> of this software dedicate any and all copyright interest in the
> software to the public domain. We make this dedication for the benefit
> of the public at large and to the detriment of our heirs and
> successors. We intend this dedication to be an overt act of
> relinquishment in perpetuity of all present and future rights to this
> software under copyright law.
> 
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
> EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
> MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
> IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
> OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
> ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
> OTHER DEALINGS IN THE SOFTWARE.
> 
> For more information, please refer to <http://unlicense.org/>