#include <FlexCAN.h>

#define RPMID 218099784
#define ECUSPEED 250000         //PE3 Baudrate

class canClass : public CANListener {
public:
  void printFrame(CAN_message_t &frame, int mailbox);
  void gotFrame(CAN_message_t &frame, int mailbox);
};

void canClass::printFrame(CAN_message_t &frame, int mailbox) {
  Serial.print("ID: ");
  Serial.print(frame.id, HEX);
  Serial.print(" Data: ");
  for (int c = 0; c < frame.len; c++) {
    Serial.print(frame.buf[c], HEX);
    Serial.write(' ');
  }
  Serial.write('\r');
  Serial.write('\n');
}

void canClass::gotFrame(CAN_message_t &frame, int mailbox)
{

    if (frame.id == RPMID){
      int lowByte = frame.buf[2];
      int highByte = frame.buf[3];
      int newRPM = ((highByte * 256) + lowByte);
      Serial.println(newRPM);
    }
}

void setup(void) {
  Serial.begin(57600);
  Serial.println("online");

  canClass canClass;

  Can0.begin(ECUSPEED); // PE3 ECU SPEED

  // Allow Extended CAN id's through
  CAN_filter_t allPassFilter;
  allPassFilter.ext = 1;
  for (uint8_t filterNum = 1; filterNum < 16; filterNum++) 
    Can0.setFilter(allPassFilter, filterNum);
}

void loop(void) {}
