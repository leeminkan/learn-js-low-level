// The wrapper class is the same
class WasmMap {
  constructor(module, numBuckets = 16) {
    this.module = module;
    this._create = module.cwrap("map_create", "number", ["number"]);
    this._set = module.cwrap("map_set", null, ["number", "number", "number"]);
    this._get = module.cwrap("map_get", "number", ["number", "number"]);
    this._free = module.cwrap("map_free", null, ["number"]);
    this.mapPtr = this._create(numBuckets);
    console.log("Wasm Map created at memory address:", this.mapPtr);
  }
  set(key, value) {
    this._set(this.mapPtr, key, value);
  }
  get(key) {
    const result = this._get(this.mapPtr, key);
    return result === -1 ? undefined : result;
  }
  destroy() {
    console.log("Freeing Wasm Map memory at address:", this.mapPtr);
    this._free(this.mapPtr);
    this.mapPtr = null;
  }
}

// 1. Directly require the module. This returns an object and starts loading.
const Module = require("./cmap/cmap.js");

// 2. Use the onRuntimeInitialized callback. This function will be executed
//    once the WebAssembly module is fully compiled, loaded, and ready.
Module.onRuntimeInitialized = () => {
  console.log("WebAssembly module loaded in Node.js.");

  const myMap = new WasmMap(Module);

  console.log("Setting key-value pairs...");
  myMap.set(101, 500);
  myMap.set(205, 999);

  console.log("Getting values:");
  console.log("myMap.get(101) ->", myMap.get(101)); // Expected: 500
  console.log("myMap.get(205) ->", myMap.get(205)); // Expected: 999
  console.log("myMap.get(99)  ->", myMap.get(99)); // Expected: undefined

  myMap.destroy();
};
