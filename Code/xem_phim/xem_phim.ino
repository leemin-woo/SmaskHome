/*
  Web: (không có)
   - Đèn chiêu sáng: 0,1
   - Điều hòa: 2,3
   - Chế độ xem phim: 4,5
*/

#include <Servo.h>

#define CBHN A0
#define button_bat_auto 2// công tắc bật chế độ xem phim
#define button_tat_auto 3//cong tac tat che do xem phim
#define button_den 4// cong tac bật đèn chiếu sáng
#define button_tivi 5// cong tac bật tivi
#define button_rem 6// công tắc kéo rèm
#define button_dieuhoa 7// cong tac bat tat dieu hoa
#define led_cauthang 8
#define led 9//den chieu sang
#define fan 10 // dieu hoa
#define tivi 11//den tivi
#define pinServo 12 // mo 0, dong 179

boolean state_led = 0, state_tivi = 0, state_fan = 0, auto_mode = 0, state_rem = 0;
int pos = 90, data_send = 0, data_receive = 0;
// pos = 179 la rem mo
Servo rem;

void setup()
{
  pinMode(CBHN, INPUT);

  pinMode(button_den, INPUT_PULLUP);
  pinMode(button_tivi, INPUT_PULLUP);
  pinMode(button_dieuhoa, INPUT_PULLUP);
  pinMode(button_rem, INPUT_PULLUP);
  pinMode(button_bat_auto, INPUT_PULLUP);
  pinMode(button_tat_auto, INPUT_PULLUP);

  pinMode(led_cauthang, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(tivi, OUTPUT);
  pinMode(fan, OUTPUT);

  digitalWrite(led, 0); // relay BAT o muc thap, TAT o muc cao
  digitalWrite(fan, 0);
  digitalWrite(tivi, 1);
  digitalWrite(led_cauthang, 1);
  delay(3000);
  digitalWrite(led, 1); // relay BAT o muc thap, TAT o muc cao
  digitalWrite(fan, 1);
  digitalWrite(tivi, 0);
  digitalWrite(led_cauthang, 0);

  rem.attach(pinServo);
  while(pos != 1) {
    pos --;
    rem.write(pos);
    delay(20);
  }
  rem.write(pos);
  Serial.begin(9600);
}

void loop() {

  //  if(Serial.available()) { // Nhan tin hieu tu web
  //    while(Serial.available() < 2);
  //    byte b1 = Serial.read();
  //    byte b2 = Serial.read();
  //    data_receive = b1*256 + b2;
  //    switch(data_receive) {
  //      case 0: // tat den
  //        digitalWrite(led, 1);
  //        state_led = 0;
  //        break;
  //       case 1: // bat den
  //        digitalWrite(led, 0);
  //        state_led = 1;
  //        break;
  //       case 2: // tat quat
  //        digitalWrite(fan, 1); // kich relay muc cao la TAT
  //        state_fan = 0;
  //        break;
  //       case 3: // bat quat
  //        digitalWrite(fan, 0); // kich relay muc thao la BAT
  //        state_fan = 1;
  //        break;
  //       case 4: // bat che do xem phim
  //        auto_mode = 1;
  //        break;
  //       case 5: // tat che do xem phim
  //        auto_mode = 0;
  //    }
  //  }

  //-----------------------------------------------------------------
  //Serial.println(digitalRead(CBHN));
  //delay(200);
  
  // bật chế dọ xem phim: tat den, bat tivi, keo rem, bat dieu hoa
  if (digitalRead(button_bat_auto) == 0)
  {
    while (digitalRead(button_bat_auto) == 0);
    // tat den, bat tivi, keo rem, bat dieu hoa
    digitalWrite(tivi, HIGH); // bat tivi
    state_tivi = 1;

    digitalWrite(led, HIGH); // tat den
    state_led = 0;
    
    digitalWrite(fan, LOW); // bat dieu hoa
    state_fan = 1;
    
    if (pos != 1) {
      while (pos != 1) { //
        pos--;
        rem.write(pos);
        delay(20);
      }
    }
    state_rem = 0; // rem dong
  }
  if (digitalRead(button_tat_auto) == 0) // tat che do xem phim: bat den, tat tivi, tat dieu hoa, mo rem
  { while ((digitalRead(button_tat_auto) == 0));

    digitalWrite(tivi, LOW);
    state_tivi = 0;

    digitalWrite(led, LOW); // bat den
    state_led = 1;
    delay(60);

    digitalWrite(fan, HIGH); // tat dieu hoa
    state_fan = 0;

    if (pos != 179) { // mo rem
      while (pos != 179) { //
        pos++;
        rem.write(pos);
        delay(20);
      }
    }
    state_rem = 1;
  }

  //den cau thang
  if (digitalRead(CBHN) == 0) {
    digitalWrite(led_cauthang, HIGH);
    delay(2500);
  }
  else digitalWrite(led_cauthang, LOW);
  //-------------------------
  // bat den chieu sang
  if (digitalRead(button_den) == 0)
  {
    while (digitalRead(button_den) == 0);
    state_led = !state_led;
    if (state_led == 1) { // bat den
      digitalWrite(led, 0);
    }
    else { // tat den
      digitalWrite(led, 1);
    }
  }

  //----------------------------------------------------------
  // bat tivi
  if (digitalRead(button_tivi) == 0)
  {
    while (digitalRead(button_tivi) == 0);
    state_tivi = !state_tivi;
    digitalWrite(tivi, state_tivi);
  }
  //---------------------------------------------------------
  // bat dieu hoa
  if (digitalRead(button_dieuhoa) == 0)
  {
    while (digitalRead(button_dieuhoa) == 0);
    state_fan = !state_fan;
    if (state_fan == 1) { // bat dieu hoa
      digitalWrite(fan, 0);
    }
    else { // tat dieu hoa
      digitalWrite(fan, 1);
    }
  }

  //----------------------------------
  //kéo rèm
  if ((digitalRead(button_rem) == 0))
  {
    while (digitalRead(button_rem) == 0);
    state_rem = !state_rem;
    if(state_rem == 0) {
      while(pos != 1) {
        pos--;
        rem.write(pos);
        delay(20);
      }
    }
    else {
      while(pos != 179) {
        pos++;
        rem.write(pos);
        delay(20);
      }
    }
  }

  //---------------------------
}

