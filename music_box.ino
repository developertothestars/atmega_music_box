#include <Arduino.h>
#include <NonBlockingRtttl.h>
#include <U8x8lib.h>

U8X8_SSD1306_128X64_NONAME_4W_SW_SPI u8x8(/* clock=*/ 10, /* data=*/ 16, /* cs=*/ 0, /* dc=*/ 15, /* reset=*/ 14);
#define BUZZER_PIN 5

const char * songs[] = {
"GirlFromIpane:d=4,o=5,b=160:g.,8e,8e,d,g.,8e,e,8e,8d,g.,e,e,8d,g,8g,8e,e,8e,8d,f,d,d,8d,8c,e,c,c,8c,a#4,2c",
"Batman:d=8,o=5,b=160:16a,16g#,16g,16f#,16f,16f#,16g,16g#,4a.,p,d,d,c#,c#,c,c,c#,c#,d,d,c#,c#,c,c,c#,c#,d,d,c#,c#,c,c,c#,c#,g6,p,4g6",
"MoveYour:d=16,o=5,b=140:8a6,8p,4g6,4e6,8d6,8p,8f6,8e6,8f6,8e6,4d6,8a,8p,8a6,8p,4g6,4e6,8d6,8e6,8f6,8e6,8f6,8e6,4d6,8a",
"StarWars:d=4,o=5,b=120:8f,8p,8f,8p,8f,8p,a#,8p,f6,8p,8d#6,8d6,8c6,a#6,8p,f6,8p,8d#6,8d6,8c6,a#6,8p,f6,8p,8d#6,8d6,8d#6,c6",
"StarWars/Imp:d=4,o=5,b=112:8d.,16p,8d.,16p,8d.,16p,8a#4,16p,16f,8d.,16p,8a#4,16p,16f,d.,8p,8a.,16p,8a.,16p,8a.,16p,8a#,16p,16f,8c#.,16p,8a#4,16p,16f,d.,8p,8d.6,16p,8d,16p,16d,8d6,8p,8c#6,16p,16c6,16b,16a#,8b,8p,16d#,16p,8g#,8p,8g,16p,16f#,16f,16e,8f,8p,16a#4,16p,2c#",
"Star Trek:d=4,o=5,b=63:8f.,16a#,d#.6,8d6,16a#.,16g.,16c.6,f6",
"FunkyTow:d=4,o=4,b=125:8c6,8c6,8a#5,8c6,8p,8g5,8p,8g5,8c6,8f6,8e6,8c6,2p,8c6,8c6,8a#5,8c6,8p,8g5,8p,8g5,8c6,8f6,8e6,8c6",
"YouSpinM:d=4,o=5,b=180:g,g,g,a#,a#,8f,8f,8c6,8c6,8a#,8g,8g,8g,8c6,8c6,8a#,8a#,f,f,c6,a#,p",
"Zelda:d=4,o=6,b=200:2a_5,2f5,2p,8c,8d,2f,2p,f,8f_,8g_,2p,a_,8p,8g_,g_,8f_,2p,2f,d_,8f,2f_,2p,d_,c_,8d_,2f,2p,c_,c,8d,2e,2p,1f.",
"You'veGo:d=16,o=5,b=140:8a#,4g#,8g,8d#,4p,8a#,4g#,8g,8d#,4p,8c,4d#,8f,8g,4p,8c,4d#,8f,8d#",
"YMCA1:d=4,o=5,b=160:8c#6,8a#,2p,8a#,8g#,8f#,8g#,8a#,c#6,8a#,c#6,8d#6,8a#,2p,8a#,8g#,8f#,8g#,8a#,c#6,8a#,c#6,8d#6,8b,2p,8b,8a#,8g#,8a#,8b,d#6,8f#6,d#6,f.6,d#.6,c#.6,b.,a#,g#"
};

byte songIndex = 0; //which song to play when the previous one finishes

void setup() {
  u8x8.begin();
  u8x8.setPowerSave(0);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
}

int x;
int y;
int oldx;
int oldy;
unsigned long oldtime;
unsigned long nowtime;

void showsongs(void)
{
  int i;
  int j;
  char buf[8];
  char *p;
  for (i = 0; i < 8; i++)
  {
    memset(buf, 0, 8);
    p = (char *)songs[i];
    for (j = 0; j < sizeof(buf); j++)
    {
      if (*(p + j) != ':')
        buf[j] = *(p + j);
      else
        break;
    }
    u8x8.drawString(1, i, buf);
  }
}

void loop() {
  if ( !rtttl::isPlaying() )
  {
    #if 0
    if (songIndex == 0)
    {
      rtttl::begin(BUZZER_PIN, songs[4]);
      songIndex++; //ready for next song

    }
    #endif

  }
  else
  {
    rtttl::play();
  }
  
  nowtime = millis();
  if (nowtime - oldtime > 200)
  {
    oldtime = nowtime;
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(oldx, oldy, " ");
    showsongs();
    u8x8.drawString(x, y, ">");
    oldx = x;
    oldy = y;
    if (digitalRead(9))
        y += 1;
    if (digitalRead(8))
        y -= 1;
    if (digitalRead(7))
      if (!rtttl::isPlaying())
        rtttl::begin(BUZZER_PIN, songs[y]);

    if (digitalRead(6))
        x -= 1;
    if (x < 0) x = 0;
    if (x > 15) x = 15;
    if (y < 0) y = 0;
    if (y > 7) y = 7;
  }
}
