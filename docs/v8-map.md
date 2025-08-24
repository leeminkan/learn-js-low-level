For major JavaScript environments like Google Chrome, Node.js, and Microsoft Edge, the `Map` data structure is implemented by the **V8 engine**.

V8 is Google's open-source, high-performance JavaScript and WebAssembly engine, written in C++. Other browsers have their own engines (like SpiderMonkey for Firefox and JavaScriptCore for Safari), which have their own C++ implementations, but they all must follow the same ECMAScript specification for how `Map` should behave.

---

## Links to the Real V8 Code ⚙️

The V8 codebase is constantly evolving, with a strong emphasis on writing engine builtins in **Torque**, a V8-specific language that compiles to optimized C++. Here's a guide to the key files that implement the `Map` data structure.

### 1. The Core Data Structure: `OrderedHashTable`

This is the low-level "engine room" that stores the data. It's a specialized hash table designed to preserve the original insertion order, effectively acting as a hash table and a linked list combined.

- **Header File (The "What"):** [`src/objects/ordered-hash-table.h`](<https://www.google.com/search?q=%5Bhttps://github.com/v8/v8/blob/main/src/objects/ordered-hash-table.h%5D(https://github.com/v8/v8/blob/main/src/objects/ordered-hash-table.h)>)
  This file defines the C++ class `OrderedHashTable`, which outlines its memory layout and methods.

- **Implementation File (The "How"):** [`src/objects/ordered-hash-table.cc`](<https://www.google.com/search?q=https/%5Bgithub.com/v8/v8/blob/main/src/objects/ordered-hash-table.cc%5D(https://github.com/v8/v8/blob/main/src/objects/ordered-hash-table.cc)>)
  This contains the C++ logic for adding, finding, re-hashing, and removing entries from the table.

### 2. The JavaScript Object: `JSMap`

This is the higher-level object that represents a `Map` within the JavaScript engine. It acts as a wrapper around the `OrderedHashTable`.

- **Header File:** [`src/objects/js-collection.h`](<https://www.google.com/search?q=%5Bhttps://github.com/v8/v8/blob/main/src/objects/js-collection.h%5D(https://github.com/v8/v8/blob/main/src/objects/js-collection.h)>)
  If you search for the `JSMap` class in this file, you'll see it contains a pointer to the `OrderedHashTable` that holds the actual data.

### 3. The Bridge Between JS and C++: Torque Builtins

When you call a method like `myMap.set(key, value)` in JavaScript, V8 executes a "builtin."

- **src/builtins/builtins-collections-gen.cc**
