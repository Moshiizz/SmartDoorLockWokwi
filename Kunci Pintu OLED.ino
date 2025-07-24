//SMART DOOR LOCK DENGAN ESP32 DAN OLED

//HEADER----------------------------------
#include <Keypad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define LEBAR_LAYAR 128 // Lebar layar OLED yang digunakan
#define TINGGI_LAYAR 64 // Tinggi layar OLED yang digunakan
Adafruit_SSD1306 oled(LEBAR_LAYAR, TINGGI_LAYAR, &Wire, -1);
#include <ESP32Servo.h>
//-----------------------------------------

//DEFINISI VARIABEL------------------------
Servo myservo; 

#define LED 26
#define buzzer 25

const int BARIS = 4;
const int KOLOM = 4;
char keys[BARIS][KOLOM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[BARIS] = {14,12,19,18};//connect to the row pinouts of the keypad
byte colPins[KOLOM] =  {5,4,2,15};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, BARIS, KOLOM);

char customKey;         //Variabel penampung input keypad
int number = 0;         //Variabel penampung nilai angka
int password = 1270;    //Password
//--------------------------------------

//VOID SETUP----------------------------
void setup() {
  Serial.begin(9600);
  // initialize OLED display with I2C address 0x3C
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1);
  }     
  oled.clearDisplay();              // clear display

  oled.setTextSize(2);              // Atur ukuran text
  oled.setTextColor(WHITE);         // Atur warna text
  oled.setCursor(20, 20);           // Atur posisi text pada display
  oled.println("SELAMAT DATANG");   // Text yang dicetak
  delay(8000);
  oled.display();                   // menampilkan display OLED
  oled.clearDisplay(); 

  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  myservo.attach(13);
  myservo.write(90); // setting posisi awal servo
}
//-----------------------------------------

//VOID LOOP---------------------------------
void loop() {
  oled.setTextSize(1);              // Atur ukuran text
  oled.setTextColor(WHITE);         // Atur warna text
  oled.setCursor(0,1);              // Atur posisi text pada display
  oled.println("Input Password:");  // Text yang dicetak
  oled.display();                   // menampilkan display OLED

  customKey = keypad.getKey();


  //------------Prosedur jika input berupa angka------------//
  switch(customKey){
    case '0' ... '9':
      number = number * 10 + (customKey - '0');
      oled.setTextSize(1);              // Atur ukuran text
      oled.setTextColor(WHITE);         // Atur warna text
      oled.setCursor(0,25);
      oled.print(number);
      oled.display();                   // menampilkan display OLED
      
      Serial.print(customKey);
      Serial.print(",");
      Serial.println(number);
    break;

    //------------Jika input '#' maka cek password------------//
    case '#':
      //Jika password benar, maka
      if(number == password){           
        digitalWrite(26, HIGH); // LED menyala
        oled.setTextSize(1);              // Atur ukuran text
        oled.setTextColor(WHITE);         // Atur warna text
        oled.setCursor(0,25);
        oled.print("Password Benar");  //Tampilan LCD
        oled.display();                   // menampilkan display OLED
        oled.clearDisplay();
        tone(buzzer, 500, 100); //PIN, FREKUENSI, WAKTU (durasi suara)
        delay(100);
        tone(buzzer, 500, 100); //PIN, FREKUENSI, WAKTU (durasi suara)
        
        // myservo.write(0); // posisi servo turun
        int right = 0; 
        for (right = 90; right >= 0; right -= 1){
          myservo.write(right);
        }

        //Reset variabel number 
        number = 0;
        delay(5000);
      }

      //Jika password salah, maka
      else{   
        oled.setTextSize(1);              // Atur ukuran text
        oled.setTextColor(WHITE);         // Atur warna text                          
        oled.setCursor(0,25);
        oled.print("Password Salah");  //Tampilan LCD
        oled.display();                   // menampilkan display OLED
        oled.clearDisplay();
        digitalWrite(26, LOW); // LED mati
        tone(buzzer, 150, 700); //PIN, FREKUENSI, WAKTU (durasi suara)
        
        // myservo.write(90); // posisi servo menyamping
        int wrong = 0; 
        for (wrong = 0; wrong <= 90; wrong += 1){
          myservo.write(wrong);
        }

        //Reset variabel number 
        number = 0;
        delay(5000);
      }
    break;

    //------------Jika input '*' maka hapus tampilan------------//
    case '*':
      number = 0; //Reset variabel number 
      oled.clearDisplay(); // clear display
    break;
  }
}
//---------------------------------------------