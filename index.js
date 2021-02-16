const { fork } = require('child_process')

/**
 *@author Viktor Limishenko 
 */


/**
 * @typedef{Object} EncoderPins
 * @property{Number} pin1
 * @property{Number} pin2
 * @property{Number} buttonPin
 */

/**
 * @typedef{Function} OnAction
 * @param{string} action
 */

/**
 * @typedef{Function} OnError
 * @param{*} error
 */

/**
 * @class EncoderPi
 */
class EncoderPi {
  pin1 = null
  pin2 = null
  buttonPin = null
  callback = () => {}
  onErr = () => {}
  _onError = (err) => this.onErr(err)

  /**
   * @constructor
   * @param {EncoderPins} options
   * @param {OnAction} cb
   * @param {OnError} onErr
   */
  constructor(options, cb, onErr) {
    const { buttonPin, pin1, pin2 } = options
    if(!pin1 || !pin2) throw new Error('Encoder pins required')
    this.buttonPin = buttonPin
    if(buttonPin === undefined) this.buttonPin = -1
    this.pin1 = pin1
    this.pin2 = pin2
    this.callback = cb
    this.onErr = onErr
    const addonProcess = fork(__dirname+'/addon-process.js', [this.pin1, this.pin2, this.buttonPin])
    addonProcess.on('message', message => {
      this.callback(message)
    })
  }
  
  /**
   * @param {OnAction} cb
   */
  onAction = (cb) => {
    this.callback = cb
  }

}



module.exports = EncoderPi
