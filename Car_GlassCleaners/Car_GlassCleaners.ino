#define CHECK_HAND 3
#define CHECK_POS 4
#define CHECK_WASH 5
#define CONTROL_RELAY 6
#define ControlRelayActive digitalWrite(CONTROL_RELAY, 1)
#define ControlRelayInactive digitalWrite(CONTROL_RELAY, 0)
uint8_t count = 0;

void setup() {
  pinMode(CHECK_HAND, INPUT);
  pinMode(CHECK_POS, INPUT);
  pinMode(CHECK_WASH, INPUT);
  pinMode(CONTROL_RELAY, OUTPUT);
  if (digitalRead(CHECK_POS) != 0)
  {
    while (digitalRead(CHECK_POS) != 0)
    {
      ControlRelayActive;
    }
    ControlRelayInactive;
  }
}

void loop() {
  if (digitalRead(CHECK_HAND) == 1)
  {
    int mill = millis();
    while (digitalRead(CHECK_HAND) == 1)
    {
      if (millis() - mill >= 50)
      {
        ControlRelayActive;
        while (digitalRead(CHECK_POS) == 0);
        while (digitalRead(CHECK_POS) != 0);
        ControlRelayInactive;
        delay(5000);

        while (digitalRead(CHECK_POS) != 0)
        {
          ControlRelayActive;
        }
        ControlRelayInactive;
      }
    }
  }
  if (digitalRead(CHECK_WASH) == 1)
  {
    int mill = millis();
    while (digitalRead(CHECK_WASH) == 1)
    {
      if (millis() - mill >= 50)
      {
        count = 3;
        for (; count > 0; count--)
        {
          ControlRelayActive;
          while (digitalRead(CHECK_POS) == 0);
          while (digitalRead(CHECK_POS) != 0);
          ControlRelayInactive;
        }
      }
    }
  }
}

void yield()
{
  if (digitalRead(CHECK_WASH) == 1 && digitalRead(CHECK_HAND) == 1)
  {
    int mill = millis();
    while (digitalRead(CHECK_WASH) == 1)
    {
      if (millis() - mill >= 50)
      {
        ControlRelayActive;
        while (digitalRead(CHECK_POS) == 0);
        while (digitalRead(CHECK_POS) != 0);
        ControlRelayInactive;
      }
    }
  }
  if (digitalRead(CHECK_WASH) == 1 && digitalRead(CHECK_HAND) == 0)
  {
    int mill = millis();
    while (digitalRead(CHECK_WASH) == 1)
    {
      if (millis() - mill >= 50)
      {
        count = 3;
        for (; count > 0; count--)
        {
          ControlRelayActive;
          while (digitalRead(CHECK_POS) == 0);
          while (digitalRead(CHECK_POS) != 0);
          ControlRelayInactive;
        }
      }
    }
  }
}
