/*
 * Morse code translator
 * 
 * Gets morse code input from button and translates to ASCII.
 *
 * 
 * This implementation uses Trie structure to achieve this.
 * It works but the timings aren't ideal.
 * It should be reimplemented in different way 
 * because this is way too hard to understand and implement.
 *
 * Trie object is rewrite of: 
 * https://upload.wikimedia.org/wikipedia/commons/thumb/6/62/Morse_code_tree3.svg/770px-Morse_code_tree3.svg.png
 * where dot = 0 and dash = 1
 */

struct Trie
{
  char val;
  Trie *t[2];
};

Trie * words = new Trie{
  '\0',
  {
  new Trie{
    'E',
    {
      new Trie{
        'I',
        {
          new Trie{
            'S',
            {
              new Trie{
                'H',
                nullptr
              },
              new Trie{
                'V',
                nullptr
              }
            }
          },
          new Trie{
            'U',
            {
              new Trie{
                'F',
                nullptr
              },
              nullptr
            }
          }
        }
      },
      new Trie{
        'A',
        {
          new Trie{
            'R',
            {
              new Trie{
                'L',
                nullptr
              },
              nullptr
            }
          },
          new Trie{
            'W',
            {
              new Trie{
                'P',
                nullptr
              },
              new Trie{
                'J',
                nullptr
              }
            }
          }
        }
      }
    }
  },
new Trie{
    'T',
    {
      new Trie{
        'N',
        {
          new Trie{
            'D',
            {
              new Trie{
                'B',
                nullptr
              },
              new Trie{
                'X',
                nullptr
              }
            }
          },
          new Trie{
            'K',
            {
              new Trie{
                'C',
                nullptr
              },
              new Trie{
                'Y',
                nullptr
              }
            }
          }
        }
      },
      new Trie{
        'M',
        {
          new Trie{
            'G',
            {
              new Trie{
                'Z',
                nullptr
              },
              new Trie{
                'Q',
                nullptr
              }
            }
          },
          new Trie{
            'O',
            nullptr
          }
        }
      }
    }
  }
  }
};

// constants
const int pushButtonPin = 2;
const int boardLedPin = 13;
const int timeCheck = 10;

const int unitTime = 100;
const int dotTime = unitTime;
const int dashTime = 3*unitTime;
const int spaceInPartsInLetter = unitTime;
const int spaceInLetters = 3*unitTime;
const int spaceInWords = 7*unitTime;

// variables
int buttonState = 0;
int lastButtonState = 0;
int timeInSameState = 0;
int letters = 0;
int morseCount = 0;
bool morseWord[4];
char sentence[20];
bool sentencePrinted = false;

// functions
void printSentence(const char * sentence, const int letters);
char parseWord(const bool word[], const int size);

// the setup routine runs once when you press reset:
void setup() {
  // makes pushButton INPUT
  pinMode(pushButtonPin, INPUT);
  // makes boardLedPin OUTPUT
  pinMode(boardLedPin, OUTPUT);
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  buttonState = digitalRead(pushButtonPin);
  // Serial.print("Last: ");
  // Serial.println(lastButtonState);
  // Serial.print(" Current: ");
  // Serial.println(buttonState);

  if (buttonState != lastButtonState) {
    sentencePrinted = false;

    if (lastButtonState == HIGH) {
      // button is on
      if (dashTime + unitTime <= timeInSameState) {
	// Not sure what to do here
      } else if ( (dotTime + unitTime <= timeInSameState) && (timeInSameState < dashTime + unitTime) ) {
        // this is *dash* (1)
        morseWord[morseCount++] = 1;
      } else if ( (0 <= timeInSameState) && (timeInSameState < dotTime + unitTime) ) {
        // this is *dot* (0)
        morseWord[morseCount++] = 0;
      }

    } else {
      // button is off
      if (spaceInLetters + unitTime < timeInSameState && timeInSameState <= spaceInWords + unitTime) {
        // space between words
        Serial.print(letters);
        sentence[++letters] = ' ';
        Serial.println(letters);
        printSentence(sentence, letters);

      } else if (spaceInPartsInLetter + unitTime < timeInSameState && timeInSameState <= spaceInLetters + unitTime) {
        // space between letters
        Serial.print(letters);
        sentence[++letters] = parseWord(morseWord, morseCount);
        Serial.println(letters);
        morseCount = 0;
        printSentence(sentence, letters);
      }
    }
    // reset time count
    timeInSameState = 0;
  }

  timeInSameState += timeCheck;

  if ( (timeInSameState >= spaceInLetters + unitTime) && !sentencePrinted) {
    sentencePrinted = true;
    printSentence(sentence, letters);
  }

  // change lastButtonState
  lastButtonState = buttonState;

  // print out the state of the button:
  digitalWrite(boardLedPin, buttonState);
  delay(timeCheck);        // delay in between reads for stability
}

// This is not worry good choice for this name
// can't think of better rn though
char parseWord(const bool word[], const int size) {
  Trie *crawl = words;
  for(int i = 0; i < size; i++) {
    if (crawl->t[word[i]] == nullptr)
      return ' ';
    crawl = crawl->t[word[i]];
  }

  Serial.println(crawl->val);
  return crawl->val;
}

void printSentence(const char * sentence, const int letters) {
  Serial.println(letters);
  for (int i = 0; i < letters+1; i++)
  {
    Serial.print(sentence[i]);
  }
  Serial.println();
}
