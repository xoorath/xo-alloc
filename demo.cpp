#include "xo-alloc.h"

#include <iostream>
#include <stdio.h>
#include <string>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

struct LevelData {
  LevelData(string name) 
    : LevelName(name)
    , FileContents(nullptr)
    , Size(0) {
    cout << "Creating level data...\n";
  }

  ~LevelData() {
    cout << "Destroying level data...\n";
  }

  string LevelName;
  char* FileContents;
  size_t Size;
};

int main() {
  xo::BlockAllocator<2048> Alloc;

  cout << "demo for xo-alloc version: " << XO_ALLOC_VER << endl;

  // Types can be allocated with the New function.
  // Parameters can be passed to the constructor.
  // If New returns null, the allocation failed.
  LevelData* level = Alloc.New<LevelData>("My Level");

  if(FILE* f = fopen("demo.cpp", "r")) {
    fseek(f, 0, SEEK_END);
    level->Size = ftell(f);
    rewind(f);

    // Data can be allocated with the Malloc function as well,
    // which calls no constructor, and returns a void*
    // If Malloc returns null, the allocation failed.
    level->FileContents = static_cast<char*>(Alloc.Malloc(level->Size+1));

    fread(level->FileContents, 1, level->Size, f);
    level->FileContents[level->Size] = '\0';
    fclose(f);
    cout << "Level file \"" << level->LevelName << 
      "\" opened and read. Length: " << level->Size << endl;
  } else {
    cerr << "Couldn't open level file." << endl;
  }

  // Memory allocated with the allocators New or Malloc methods can be
  // returned with the Free method. No destructor is called.
  // Free safely accepts nullptr/NULL pointers. 
  // WARNING: When a pointer not allocated by this allocator is passed 
  // to Free, it will return without doing anything.
  Alloc.Free(level->FileContents);

  // Memory allocated with the allocators New or Malloc methods can be
  // returned with the Delete method. The types destructor is called.
  // Delete safely accepts nullptr/NULL pointers. 
  // WARNING: When a pointer not allocated by this allocator is passed 
  // to Delete, it will call the destructor and return without freeing 
  // any memory.
  Alloc.Delete(level);
  return 0;
}