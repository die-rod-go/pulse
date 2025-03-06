# Pulse
Pulse is an event-driven programming language where every statement is triggered by a named event. Execution follows a strict event subscription order, and control flow is managed through event calls rather than traditional loops or function calls.

Example:

```
//  two statements are created and subscribe to the init event.
byte num1 = 1 : init;
byte num2 = 1 : init;

//  the init event is called, creating num1 and num2
emit init;

//  a new statement is created and subscribes to the add event
byte sum = num1 + num2 : add;
//  the add event is called and the two numbers are added together
emit add;
```

# Variables

*Declaration*
```
//  creates a byte variable named example
byte example : event; // set to 0 by default
```

*Reassignment*
```
//  creates a byte variable named example initialized to 10
byte example = 10 : event;
//  reassigns example to 5
example = 5 : event;
```
Note that EVERY statement must be tied to an event
```
//  WRONG
byte example; <- syntax error. expecting : and event name
```

# Events

There's three ways to trigger events.

*Once, unconditionally*
```
emit event_name;
```
Usage:
```
print 1 : print_event;
emit print_event;

output:
1
```

*Once, conditionally*

```
emit event_name ? condition;
```

Usage:
```
byte num = 10 : init;
emit init;

print 1 : print_event;
emit print_event ? num > 5;

output:
1
```

*Looping, conditionally*
```
emit event_name ?? condition;
```

Usage:
```
byte num = 10 : init;
emit init;

print 1 : print_event;
emit print_event ? num > 5;

output:
1
1
1
1
...
```

# Event chaining
Emit statements can also be subscribed to events. 
```
//  event_name will NOT be emitted by default. it will be emitted for the first time when subscribed_event is emitted 
emit event_name : subscribed_event;

//  works with conditionals
emit event_name ? condition : subscribed_event;

// works with looping conditionals
emit event_name ?? condition : subcribed_event;
```
Take this code for a binary search for example.
```
byte secretNumber = 33 : init;
byte numGuesses = 0 : init;
byte low = 0 : init;
byte high = 255 : init;
byte numGuessed : init;
evoke init;

numGuessed = (high + low) / 2 : guess;
numGuesses = numGuesses + 1 : guess;

low = numGuessed : guessed_too_high;
high = numGuessed : guessed_too_low;

//  the guessed_too_high and guessed_too_low events will be emitted every time the guess event is emitted
evoke guessed_too_high ? numGuessed > secretNumber : guess;
evoke guessed_too_low ? numGuessed < secretNumber : guess;

//  updates numGuessed, numGuessed, AND low and high since their event
//  (guessed_too_high and guessed_too_low are also subscribed to the guess event)
evoke guess ?? numGuessed != secretNumber;

print numGuessed : done;
print numGuesses : done;
evoke done;
```

# Initialization Order
Event chaining can cause confusing initialization order errors. For example, look at this code.
```
byte test = 10 : init;
emit init ? test == 10 : chain; // [line 2] Error: Undefined variable 'test'.
emit chain;
```
chain calls 'emit init', which has a conditional that relies on the variable 'test' which is only initialized after init is called once. So when test == 10  is checked, test does not exist. It's fairly easy to see the error here, but with longer event chains, it can get a little confusing.
