extern "C"
{
  #include <Project.h>
}
#include <Arduino.h>
#include <Servo.h>
#include <Arduino_Pins.h>
#include <Arduino_Extended_IO.h>


volatile uint8_t pinStatus = LOW;
volatile uint8_t pinStatus2 = LOW;
volatile uint16_t pulseLength = DEFAULT_PULSE_WIDTH;
static servo_t servoList[MAX_SERVOS];
static int8_t servoCount = 0;
static int8_t activeServo = 0;
uint32_t pinToPointer[NUM_BASE_PINS]= 
  { D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10,
    D11, D12, D13, D14, D15, D16, D17, D18, D19};

CY_ISR(ServoInterrupt_ISR)
{
  // Reading the status register clears the interrupt, so we *have* to do it.
  uint8_t status = ServoCounter_STATUS;

  // However, if we've no servos attached, we don't need to do anything else.
  if (servoCount == 0)
  {
    return;
  }

  /* If a zero status interrupt occurred, we need to reset the active servo
   * index to zero. */
  if ( ( status & ServoCounter_STATUS_ZERO ) !=0 )
  {
    digitalWrite(D4, HIGH);
    digitalWrite(D4, LOW);
  }

  if (activeServo == servoCount)
  {
    ServoCounter_WriteCompare(0);
    activeServo = 0;
    return;
  }

  /* If a compare interrupt occurred, we want to 
     - toggle the current active PWM pin
     - set the compare register to trip at the next event
  */
  uint32_t pinName = servoList[activeServo].Pin.nbr;
  if ( ( status & ServoCounter_STATUS_CMP ) != 0 )
  {
    if (CyPins_ReadPin(pinName) == LOW) 
    {
      CyPins_SetPin(pinName);
      ServoCounter_WriteCompare((activeServo * CHANNEL_SWITCH_TIME)+servoList[activeServo].ticks);
    }
    else 
    {
      CyPins_ClearPin(pinName);
      ++activeServo;
      ServoCounter_WriteCompare(activeServo * CHANNEL_SWITCH_TIME);
    }
  }
}


Servo::Servo()             
{

  if (servoCount < MAX_SERVOS)
  {
    this->servoIndex = servoCount++;
    servoList[this->servoIndex].ticks = DEFAULT_PULSE_WIDTH;
  }
  else
  {
    this->servoIndex = INVALID_SERVO;
  }
}


uint8_t Servo::attach(int pin)             
{
  return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max)
{
  if ( (ServoCounter_ReadControlRegister() & 0x80) == 0 )
  {
    ServoCounter_Start();
    ServoCounter_WritePeriod(25000);
    ServoCounter_WriteCompare(0);
    ServoCounter_SetInterruptMode(ServoCounter_STATUS_CMP_INT_EN_MASK | ServoCounter_STATUS_ZERO_INT_EN_MASK);
    ServoInterrupt_StartEx(ServoInterrupt_ISR);
  }
  if (this->servoIndex != 255)
  {
    if ( (pin > 0 ) && (pin < 20) )
    {
      pin = pinToPointer[pin];
    }
    pinMode(pin, OUTPUT);
    servoList[this->servoIndex].Pin.nbr = pin;
    this->min = min;
    this->max = max;
    servoList[this->servoIndex].Pin.isActive = 1;
  }
  return this->servoIndex;
}

void Servo::detach()
{
  pinMode(servoList[this->servoIndex].Pin.nbr, INPUT);
  servoList[this->servoIndex].Pin.isActive = 0;
}

void Servo::write(int value)
{
  if (value <= 180)
  {
    value = map(value, 0, 180, this->min, this->max);
    writeMicroseconds(value);
  }
  if ( (value <= this->max) && (value >= this->min))
  {
    writeMicroseconds(value);
  }
}

void Servo::writeMicroseconds(int value)
{
  servoList[this->servoIndex].ticks = value;
}

int Servo::read()
{
  return map(servoList[this->servoIndex].ticks, this->min, this->max, 0, 180);
}
          
int Servo::readMicroseconds()
{
  return servoList[this->servoIndex].ticks;
}
     
bool Servo::attached()             
{
  return (bool)servoList[this->servoIndex].Pin.isActive;
}

