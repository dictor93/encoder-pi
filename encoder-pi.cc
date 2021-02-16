#include <napi.h>
#include <wiringPi.h>
#include <stdio.h>

int PIN1, PIN2, BUTTON_PIN;

volatile int status1 = 0;
volatile int status2 = 0;
volatile int buttonStatus = 0;

int oldStatus1 = 0;
int oldButtonStatus = buttonStatus;

void onPin1Isr(void) {
  status1 = digitalRead(PIN1);
}
void onPin2Isr(void) {
  status2 = digitalRead(PIN2);
}

void onButtonIsr() {
  buttonStatus = digitalRead(BUTTON_PIN);
}

void RunCallback(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Number pin1 = info[0].As<Napi::Number>();
  Napi::Number pin2 = info[1].As<Napi::Number>();
  Napi::Number buttonPin = info[2].As<Napi::Number>();
  Napi::Function cb = info[3].As<Napi::Function>();
  Napi::Function errHandler = info[4].As<Napi::Function>();

  PIN1 = pin1.Int32Value();
  PIN2 = pin2.Int32Value();
  BUTTON_PIN = buttonPin.Int32Value();

  wiringPiSetup ();
  pinMode(PIN1, INPUT);
  pinMode(PIN2, INPUT);
  pullUpDnControl(PIN1, PUD_UP);
  pullUpDnControl(PIN2, PUD_UP);
  wiringPiISR(PIN1, INT_EDGE_BOTH, &onPin1Isr);
  wiringPiISR(PIN2, INT_EDGE_BOTH, &onPin2Isr);

  if(BUTTON_PIN != -1) {
    pinMode(BUTTON_PIN, INPUT);
    pullUpDnControl(BUTTON_PIN, PUD_UP);
    wiringPiISR(BUTTON_PIN, INT_EDGE_BOTH, &onButtonIsr);
  }

  for (;;)
  {
    if(status1 != oldStatus1) {
      int next;
      oldStatus1 = status1;
      if(status1 != status2) {
        next =1;
      } else {
        next = 0;
      }
      cb.Call(env.Global(), {Napi::Number::New(env, next)});
    }
    if(buttonStatus != oldButtonStatus) {
      oldButtonStatus = buttonStatus;
      if(!buttonStatus) {
        cb.Call(env.Global(), {Napi::Number::New(env, -1)});
      }
    }
  }
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  return Napi::Function::New(env, RunCallback);
}

NODE_API_MODULE(addon, Init)