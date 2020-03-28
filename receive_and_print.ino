int curr = 0;
int prev = 0;
int n = 0;
bool record = 0;
   
volatile int _prev = 0;
volatile int _curr = 0;
volatile int _now = 0;
volatile bool _state = 0;

unsigned int output = 0;

#define isr_pin 10
#define led_pin LED_BUILTIN

#define SHORTPULS     300
#define LONGPULS      850
#define SYNCPULS      10300
#define HYST          100

#define HIGH_       1
#define LOW_        0
#define START_      2
#define STOP_       3
#define UNDEF_      4

//volatile int complete = 0;
//volatile bool copysuccess = false;
//volatile int i = 0;

//volatile int previous = 0;
//volatile bool pinState = false;
//volatile bool write = 0;
//int buffer[8];
//int receivedBytes[8];

void ICACHE_RAM_ATTR pinChange()
{
  noInterrupts();
  _state = 1;
  _now = micros();
  _curr = _now - _prev;
  _prev = _now;
  interrupts();
}

void setup() {
  Serial.begin(115200);
  pinMode(isr_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(isr_pin), pinChange, CHANGE);
}

int getLogic(int curr, int prev)
{
  if(((LONGPULS - HYST) <= prev) && (prev <= (LONGPULS + HYST)) 
  && ((SHORTPULS - HYST) <= curr) && (curr <= (SHORTPULS + HYST)))
  {
    return HIGH_;
  }
  else if(((SHORTPULS - HYST) <= prev) && (prev <= (SHORTPULS + HYST)) 
       && ((LONGPULS - HYST) <= curr) && (curr <= (LONGPULS + HYST)))
  {
    return LOW_;
  }
  else if(((LONGPULS - HYST) <= curr) && (curr <= (LONGPULS + HYST)) 
       && ((SYNCPULS - HYST*2) <= prev) && (prev <= (SYNCPULS + HYST*2)))
  {
    return START_;
  }
  else if(((SHORTPULS - HYST) <= prev) && (prev <= (SHORTPULS + HYST)) 
       && ((SYNCPULS - HYST*2) <= curr) && (curr <= (SYNCPULS + HYST*2)))
  {
    return STOP_;
  }
  else
  {
    return UNDEF_;
  }
}

void loop() 
{
  if(_state)
  {
    //Serial.print(record); Serial.print("\t"); Serial.print(curr); Serial.print("\t"); Serial.print(prev); Serial.print("\n");
    _state = 0;
    curr = _curr;
    if(getLogic(curr, prev) == START_ && !record)
    {
      record = 1;
      output = 0;
      Serial.print("START:: ");
    }
    else if((getLogic(curr, prev) == STOP_ && record) || n >= 32)
    {
      record = 0;
      n = 0;
      Serial.print(":: STOP "); Serial.print(output); Serial.print("\n");
    }
    if(record == 1)
    {
      bool valid = digitalRead(isr_pin);
      // LOGIC 1
      if(getLogic(curr, prev) == HIGH_ && valid)
      {
        n++;
        output |= 1;
        Serial.print("1"); //Serial.print(curr); Serial.print(" "); Serial.print(prev); Serial.print("\n");
        if(n < 32)
          output = output << 1;
      }
      // LOGIC 0
      else if(getLogic(curr, prev) == LOW_ && valid)
      {
        n++;
        Serial.print("0"); //Serial.print(curr); Serial.print(" "); Serial.print(prev); Serial.print("\n");
        if(n < 32)
          output = output << 1;
      }
      //Serial.print("\t"); Serial.print(curr); Serial.print("\t"); Serial.print(prev); Serial.print("\t"); Serial.print(valid); Serial.print("\n");  
    }
    prev = curr;
  }
}
