const getDefaultEnv = () => ({
  memoryBase: 0,
  tableBase: 0,
  __memory_base: 0,
  memory: new WebAssembly.Memory({
    initial: 256
  }),
  table: new WebAssembly.Table({
    initial: 2,
    element: "anyfunc"
  }),
  abort: console.log
});

function loadWasm(fileName, importObj = {
  env: {}
}) {
  const allEnv = Object.assign({}, getDefaultEnv(), importObj.env);
  const allImports = Object.assign({}, importObj, {
    env: allEnv
  });
  return fetch(fileName)
    .then(response => {
      if (response.ok) return response.arrayBuffer();
      throw new Error(`Unable to fetch WebAssembly file ${fileName}`);
    })
    .then(bytes => WebAssembly.instantiate(bytes, allImports));
}