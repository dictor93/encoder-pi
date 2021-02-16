const addon = require('bindings')('encoder-pi')

console.log(process.argv)


addon(
  Number(process.argv[2]),
  Number(process.argv[3]),
  Number(process.argv[4]),
  ev => process.send(ev),
  (msg) => {throw new Error(msg)},
);