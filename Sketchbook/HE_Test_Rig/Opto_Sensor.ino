#define OPTONCDT_BAUD 921600

char optoncdt_flag = 0;
uint32_t optoncdt_value = 0;

void optoncdt_init()
{
  pinMode(9, INPUT);
  pinMode(10, OUTPUT);
  Serial2.begin(OPTONCDT_BAUD);
}

void optoncdt_startReadings()
{
  Serial2.print("LASERPOW FULL\r\n");
  delay(100);
  Serial2.print("OUTPUT RS422\r\n");
}

uint32_t optoncdt_convertRaw(uint8_t* buff)
{
  uint32_t result = buff[0] & 0x3F;
  uint32_t mid = buff[1] & 0x3F;
  uint32_t hi = buff[2] & 0x0F;
  result += mid << 6;
  result += hi << 12;
  return result;
}

void optoncdt_task()
{
  static uint8_t buff[3];
  while (Serial2.available() > 0)
  {
    uint8_t c = Serial2.read();
    if ((c & (1 << 7)) != 0) // higher bits, sent last
    {
      buff[2] = c;
      optoncdt_flag = 1;
      optoncdt_value = optoncdt_convertRaw(buff);
    }
    else if ((c & 0xC0) != 0) // middle bits
    {
      buff[1] = c;
    }
    else if ((c & 0xC0) == 0) // lower bits
    {
      buff[0] = c;
    }
  }
  // new line sequences in the data stream should be automatically ignored due to this algorithm
  // some junk data might exist but only at the very start
}

