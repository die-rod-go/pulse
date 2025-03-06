byte secretNumber = 33 : init;
byte numGuesses = 0 : init;
byte low = 0 : init;
byte high = 255 : init;
byte numGuessed : init;
emit init;

numGuessed = (high + low) / 2 : guess;
numGuesses = numGuesses + 1 : guess;

low = numGuessed : guessed_too_high;
high = numGuessed : guessed_too_low;

emit guessed_too_high ? numGuessed > secretNumber : guess;
emit guessed_too_low ? numGuessed < secretNumber : guess;

emit guess ?? numGuessed != secretNumber;

print numGuessed : done;
print numGuesses : done;
emit done;


