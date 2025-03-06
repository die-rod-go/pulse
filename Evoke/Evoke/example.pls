byte n = 255 : init; // Number of Fibonacci numbers to generate
byte a = 0 : init;
byte b = 1 : init;
byte count = 0 : init;
emit init;

// Print the current Fibonacci number
print a : fibonacci_loop;

// Calculate the next Fibonacci number
byte next = a + b : fibonacci_loop;
a = b : fibonacci_loop;
b = next : fibonacci_loop;

// Increment the counter
count = count + 1 : fibonacci_loop;

// Emit fibonacci_loop until count >= n
emit fibonacci_loop ?? count < n;