

volatile int lastTime = 0;
volatile int duration = 0;

#define interruptPin 10
#define led LED_BUILTIN

#define shortPuls 200
#define longPuls 850
#define hysteresis 100

volatile int complete = 0;
volatile bool copysuccess = false;
volatile int i = 0;

volatile int previous = 0;
volatile bool pinState = false;
volatile bool write = 0;
int buffer[8];
int receivedBytes[8];

void ICACHE_RAM_ATTR pinChanged()
{
  //noInterrupts();
  //pinState = digitalRead(interruptPin);
  duration = micros() - lastTime;
  lastTime = micros();
  //interrupts();
}

void setup()
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(10), pinChanged, CHANGE);
}

void printSignal()
{
  Serial.print("\n==========================PRINT_SIGNAL==========================\n");

  Serial.print("receivedBytes[] = { ");
  for (int b = 0; b < 8; b++)
  {
    Serial.print(receivedBytes[b]);
    if (b == 7)
      Serial.print(" }\n");
    else
      Serial.print(", ");
  }

  memset(receivedBytes, 0, sizeof(receivedBytes));
  Serial.print(": mem cleared\n");
  Serial.print("==================================================================\n");
}

void bitAppend(bool state )
{
  buffer[i / 8] = buffer[i / 8] << 1;
  buffer[i / 8] = buffer[i / 8] ^ state;
  Serial.print(state);
  /*
    Serial.print("[byte][bit] : ");
    Serial.print("[");
    Serial.print(i/8);
    Serial.print("]");
    Serial.print("[");
    Serial.print(i%8);
    Serial.print("] - ");
    Serial.print(state);
    if((i%8) == 0)  {
    Serial.print(" > ");
    Serial.print(buffer[i/8]);  }
    Serial.print("\n");
  */
  i++;
  if (i == 64)  {
    memcpy( &receivedBytes, &buffer, sizeof(buffer) );
    copysuccess = 1;
  }
}

void loop()
{
  if (complete == 1 && previous != duration) // && (digitalRead(interruptPin) == 1))
  {
    /// START
    if ((abs(shortPuls - previous) < hysteresis) && (duration > 2000 && duration < 3000) && write == false)
    {
      digitalWrite(led, HIGH);
      Serial.print("\n::");
      i = 0;
      memset(buffer, 0, sizeof(buffer));
      write = true;
    }
    /// END
    else if ((abs(shortPuls - previous) < hysteresis) && (duration > 3000))
    {
      digitalWrite(led, LOW);
      Serial.print(":\\\n");
      i = 0;
      memset(buffer, 0, sizeof(buffer));
      write = false;
    }
    /// LOGIC 1
    else if ((abs(shortPuls - previous) < hysteresis) && (abs(shortPuls - duration) < hysteresis) && write == true)
    {
      Serial.print("1");
      bitAppend(1);
    }
    /// LOGIC 0
    else if ((abs(shortPuls - previous) < hysteresis) && (abs(longPuls - duration) < hysteresis) && write == true)
    {
      Serial.print("0");
      bitAppend(0);
    }
    else
    {
      digitalWrite(led, LOW);
      //Serial.print("x");
      //Serial.print("old_");
      //Serial.print(previous);
      //Serial.print(" - ");
      //Serial.print("new_");
      //Serial.print(duration);
      //Serial.print("\n");
      i = 0;
      memset(buffer, 0, sizeof(buffer));
      write = false;
    }
    previous = duration;
    complete = 0;
  }
  else if ( previous != duration) {
    complete = 1;
    previous = duration;
  }

  if (copysuccess == 1) {
    printSignal();
    copysuccess = 0;
  }
}
