**This is the native node.js module for the encoder, based on wiringOp library**

***to run compilation just exec "npm install" in root of the project. Required the wiringOp to have been installed***

**Using:**

```const Encoder = require('./encoder-pi')```

  class Encoder expects 3 parameters:

  * 1 - configuration
  * 2 - event callback
  * 3 - error handler

  configuration includes 3 properties:
  * buttonPin - wiringOp pin for encoder button
  * pin1, pin2 - worongOp pins for encoder outputs

  for example:

```const encoder = new Encoder({ buttonPin: 8, pin1: 7, pin2: 9 })```

encoder includes method `onAction` that allows assigning event callback directly

```encoder.onAction(action => { console.log({action}) })```
