#include <Wiimote.h>
#include <HardwareSerial.h>
HardwareSerial oSerial(1); // RX, TX

#define START_FRAME      0xAAAA     // serial command start-of-frame magic word

typedef struct{
  uint16_t start_of_frame;
  int16_t  steer;
  int16_t  speed;
  uint16_t checksum;
} Serialcommand;

Serialcommand oCmd;

int ny, nx;
boolean active = false;
boolean prev_c = false;
boolean connected = false;
int loops_since_last_command = 0;

void setup() {
  Serial.begin(115200);
  oSerial.begin(19200, SERIAL_8N1, 25, 26);
  Wiimote::init();
  Wiimote::register_callback(1, wiimote_callback);
}

void loop() {
  Wiimote::handle();
  if (!connected) Serial.printf("Fuck!\n");
  if (loops_since_last_command > 1000000/3) {
    Send(0,0);
  }
  loops_since_last_command++;
}

void hover(int raw_x, int raw_y, boolean raw_c, boolean raw_z) {
  if (raw_c){
    int min_x = 26;
    int max_x = 223;
    int dead_x = 124;
    int range_x = dead_x - min_x;
    
    int min_y = 32;
    int max_y = 226;
    int dead_y = 131;
    int range_y = dead_y - min_y;
    
    int deadzone = 10;
  
    int x = raw_x - dead_x;
    int y = raw_y - dead_y;
    boolean boost = raw_z;

    int16_t speed = 0;
    int16_t steer = 0;

    if (abs(x) < deadzone) {
      steer = 0;
    } else {
      steer = x;
      steer = (int16_t) (((float) steer) / range_x * 200);
    }
    if (abs(y) < deadzone){
      speed = 0;
    } else {
      speed = y;
      speed = (int16_t) (((float) speed) / range_y * 300);
    };

    if (raw_z) {
      speed = (int16_t) (((float) speed) / range_y * 1000);
    }

    // Clamping
    speed = max((int16_t) -1000, min(speed, (int16_t) 1000));
    steer = max((int16_t) -1000, min(steer, (int16_t) 1000));
    steer = steer * -1;

    Send(speed, steer);
  } else {
    Send(0,0);
  }
}

void Send(int16_t iSpeed, int16_t steer)
{
  Serial.printf("speed: %04d, steer: %04d\n", iSpeed, steer);
  oCmd.start_of_frame = START_FRAME;
  oCmd.steer = steer;
  oCmd.speed = iSpeed;

  oCmd.checksum = (uint16_t)(START_FRAME ^ oCmd.steer ^ oCmd.speed);
  oSerial.write((uint8_t *) &oCmd, sizeof(oCmd));
  delay(20);
  loops_since_last_command = 0;
}

void wiimote_callback(uint8_t number, uint8_t* data, size_t len) {
  connected = true;
  //Serial.printf("wiimote number=%d len=%d ", number, len);
  if(data[1]==0x32){
    for (int i = 0; i < 4; i++) {
      //Serial.printf("%02X ", data[i]);
    }

    // http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
    uint8_t* ext = data+4;
    /*Serial.printf(" ... Nunchuk: sx=%3d sy=%3d c=%d z=%d\n",
      ext[0],
      ext[1],
      0==(ext[5]&0x02),
      0==(ext[5]&0x01)
    );*/
    hover(ext[0], ext[1], 0==(ext[5]&0x02), 0==(ext[5]&0x01));
  }else{
    for (int i = 0; i < len; i++) {
      Serial.printf("%02X ", data[i]);
    }
    Serial.print("\n");
  }
}
