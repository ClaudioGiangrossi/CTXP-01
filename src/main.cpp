#include "navigazione/navigazione.h"
#include "memoria/memoria.h"
#include "timer/timer.h"
#include "var_globali.h"
#include "pin_defs.h"
#include <WiFi.h>

volatile uint8_t    scenaAttuale = menu;
uint8_t             pompaSelezionata = errore;
uint8_t             velocitaDefault = 0;
uint8_t             rotazione = 0;
String              ultimoProfiloCaricato = "Sinusoide";


//-----------------------------------------------------------------------------
//                  WiFi
//-----------------------------------------------------------------------------
const char* ssid = "MSDLAB_2019a";    // ssid
const char* password = "MSDLAB2019";  // psw
WiFiServer server(80);                // Set web server port number num.


void setup() {
  Serial.begin(115200);
  pinMode(encoderPinA, INPUT_PULLUP);     // setup GPIO
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(pushButton1, INPUT);
  pinMode(pushButton2, INPUT);
  pinMode(ALARM_INPUT, INPUT);
  pinMode(TACHO_INPUT, INPUT);
  pinMode(ROTATION_OUT, OUTPUT);
  //pinMode(DAC, OUTPUT);
  pinMode(STOP_OUT, OUTPUT); // 0 start, 1 stop

  uiTimerEnable();      // init timer UI
//F  attachInterrupt(digitalPinToInterrupt(encoderPinA), readEncoderA, FALLING);
//F  attachInterrupt(digitalPinToInterrupt(encoderPinB), readEncoderB, FALLING);
  init_DAC();           // config DAC

  /* carico ultimo stato */
  pompaSelezionata = chiediPompa();
  velocitaDefault = chiediVelocitaDefault();
  ultimoProfiloCaricato = chiediProfiloCaricato();

  schermataIniziale();
  Serial.println("Ciao!"); 

  Serial.print("Connecting to ");     // Connect to Wi-Fi network with SSID and password
  Serial.print(ssid);
  Serial.print(" / ");
  Serial.println(password);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");         // Print local IP address and start web server
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}



/* ===================================== LOOP ===================================== */
/* ================================================================================ */


void loop() {

  switch (scenaAttuale)
  {

    case menu:
          scenaAttuale = selezioneMenu();
        break;

    case pompa:
          scenaAttuale = selezionePompa();
        break;

    case steady:
          scenaAttuale = menuSteady();
        break;

    case profilo:
          scenaAttuale = menuProfilo();
        break;

    case wifi:
          scenaAttuale = menuWiFiFra();
        break; 

    default:
          #ifndef NDEBUG
          Serial.println("ERRORE: scenaAttuale ha assunto un valore illegale");
          #endif

          esp_restart();
        break;
  }

  resetFlagsUI();
  delay(150);
  
}
