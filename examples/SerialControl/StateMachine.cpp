#include "StateMachine.h"

void std::__throw_bad_function_call()
{
    pinMode(LED_BUILTIN, OUTPUT);
    while (1) {
        digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));
        delay(100);
    }
}
