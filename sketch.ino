#include<LiquidCrystal_I2C.h>
#define buz 13
#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2
#define Right_val 4
#define Left_val 2
#define INCR_val 5
#define DECR_val 3
#define SET_BUTT 6
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);
struct Time{
    int HH;
    int MM;
    int SS;
    int temp;
}TIME;
void SerialFlash(){
    char temp; 
    while(Serial.available()){
        temp=Serial.read();
    }
}
void Read(struct Time &t){
    Serial.println("Enter Hour");
    while(!Serial.available());
    if(Serial.available()){
        t.HH=Serial.parseInt();
        SerialFlash(); 
    }
    Serial.println("Enter Minute");
    while(!Serial.available());
    if(Serial.available()){
        t.MM=Serial.parseInt();
        SerialFlash();
    }
    Serial.println("Enter Second");
    while(!Serial.available());
    if(Serial.available()){
        t.SS=Serial.parseInt();
        SerialFlash(); 
    }
}
void lcdUpdate(struct Time &t){
  if(t.HH<10){
    lcd.setCursor(8,0);
    lcd.print("0");
    lcd.print(t.HH);
    lcd.print(":"); 
  }
  else{
    lcd.setCursor(8,0);
    lcd.print(t.HH);
    lcd.print(":");
  }
   if(t.MM<10){
    lcd.setCursor(11,0);
    lcd.print("0");
    lcd.print(t.MM);
    lcd.print(":"); 
  }
  else{
    lcd.setCursor(11,0);
    lcd.print(t.MM);
    lcd.print(":");
  }
   if(t.SS<10){
    lcd.setCursor(14,0);
    lcd.print("0");
    lcd.print(t.SS);
  }
  else{
    lcd.setCursor(14,0);
    lcd.print(t.SS);
  } 
}
int data=0;
int alarm=0;
void setup() {
    pinMode(buz,OUTPUT);
    pinMode(Right_val, INPUT_PULLUP);
    pinMode(Left_val, INPUT_PULLUP);
    pinMode(INCR_val, INPUT_PULLUP);
    pinMode(DECR_val, INPUT_PULLUP);
    pinMode(SET_BUTT, INPUT_PULLUP);

    lcd.begin(16,2);
    // lcd.print("PROJECT TIMER");
    Serial.begin(9600); 
    // lcd.setCursor(0,1);
    // lcd.print("TIME");
    TIME.temp=0;
}
void butRead(struct Time &t,int &data){
  data=!data;
  int pos=0;
  int HZ=0,HT=0,MZ=0,MT=0,SZ=0,ST=0;
  lcd.clear();
  lcd.print("TIME");
  lcd.setCursor(8,0);
  lcd.print("  :  :  ");
  t.SS=0;
  t.MM=0;
  t.HH=0;
  lcdUpdate(t);
  lcd.setCursor(8+pos,1);
      lcd.print("#");
  lcd.setCursor(0,1);
  lcd.print("<> UD O");
  boolean ar[2]={0,0};
  while(digitalRead(SET_BUTT)==HIGH){
    // RIGHT BUTTON
    if(digitalRead(Right_val)==LOW && ar[0]==0){
      ar[0]=1;
      lcd.setCursor(8 + pos, 1);
            lcd.print(" ");
      pos++;
      if(pos==2||pos==5){
        pos++;
      }
      if(pos==8){
        pos=0;
        }
        lcd.setCursor(8 + pos, 1);
            lcd.print("#");
            delay(200);

            
    }
      else if(digitalRead(Right_val)==HIGH && ar[0]==1){
      ar[0]=0;
      
    }
    //LEFT BUTTON
    if(digitalRead(Left_val)==LOW && ar[1]==0){
      ar[1] = 1;
            lcd.setCursor(8 + pos, 1);
            lcd.print(" ");  // Clear current position
            if (pos == 0) {
                pos = 8;
            }
            pos--;
            if (pos == 2 || pos == 5) {
                pos--;
            }
            lcd.setCursor(8 + pos, 1);
            lcd.print("#");
            delay(200);
    }else if (digitalRead(Left_val) == HIGH && ar[1] == 1) {
            ar[1] = 0;
        }
    //UP ARROW
    if(digitalRead(INCR_val)==LOW){
      switch(pos){
          case 0:if(HT<9)
                    HT++;
                  else
                    HT=0;
                  break;
          case 1:if(HZ<9)
                    HZ++;
                  else
                    HZ=0;
                  break;
          case 3:if(MT<5)
                    MT++;
                  else
                    MT=0;
                  break;
          case 4:if(MZ<9)
                    MZ++;
                  else
                    MZ=0;
                  break;
          case 6:if(ST<5)
                    ST++;
                  else
                    ST=0;
                  break;
          case 7:if(SZ<9)
                    SZ++;
                  else
                    SZ=0;
                  break;
      }
      delay(250);

    }
    //Down Arrow
    if(digitalRead(DECR_val)==LOW){
      switch(pos){
          case 0:if(HT>0)
                    HT--;
                  else
                    HT=9;
                  break;
          case 1:if(HZ>0)
                    HT--;
                  else
                    HZ=9;
                  break;
          case 3:if(MT>0)
                    MT--;
                  else
                    MT=5;
                  break;
          case 4:if(MZ>0)
                    MZ--;
                  else
                    MZ=9;
                  break;
          case 6:if(ST>0)
                    ST--;
                  else
                    ST=9;
                  break;
          case 7:if(SZ>0)
                    SZ--;
                  else
                    SZ=9;
                  break;
      }
      delay(250);

    }
    t.SS=ST*10+SZ;
    t.HH=HT*10+HZ;
    t.MM=MT*10+MZ;
    lcdUpdate(t);
  }



}
void timeUpdate(struct Time &t){
  if(TIME.temp==1000)
      t.temp=0;
    t.temp=t.temp+200;

  if(t.temp==1000&&t.SS>0){
    t.SS--;
  }else if(t.temp==1000&&t.SS==0&&t.MM>0){
    t.SS=59;
    t.MM--;
  }else if(t.temp==1000&&t.SS==0&&t.MM==0&&t.HH>0){
    t.SS=59;
    t.MM=59;
    t.HH--;

  }
}
void loop(){  
  if(data==0){
      //Read(TIME);
      butRead(TIME,data);
      // data=1;
      lcd.clear();
      lcd.print("TIME");
      lcd.setCursor(8,0);
      lcd.print("  :  :  ");

  }else{
    timeUpdate(TIME);
    lcdUpdate(TIME);
  }
    if(data==1&&TIME.HH==0&&TIME.MM==0&&TIME.SS==0){
    tone(buz, 31);
    // digitalWrite(buz,HIGH);
    alarm=alarm+200;
    }
    if(alarm==3000){
      data=0;
      alarm=0;
      noTone(buz);
      // digitalWrite(buz,LOW);
    }
    delay(200);
}