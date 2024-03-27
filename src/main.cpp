#include "navigazione/navigazione.h"
#include "memoria/memoria.h"
#include "timer/timer.h"
#include "var_globali.h"
#include "pin_defs.h"

volatile uint8_t    scenaAttuale = menu;
uint8_t             pompaSelezionata = errore;
uint8_t             velocitaDefault = 0;
uint8_t             rotazione = 0;
String              ultimoProfiloCaricato = "Sinusoide";

/* ===================================== SETUP ====================================== */
/* ================================================================================== */


void setup() {

  Serial.begin(115200);

  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(pushButton1, INPUT);
  pinMode(pushButton2, INPUT);
  pinMode(ALARM_INPUT, INPUT);
  pinMode(TACHO_INPUT, INPUT);
  pinMode(ROTATION_OUT, OUTPUT);
  pinMode(DAC, OUTPUT);
  pinMode(STOP_OUT, OUTPUT); // 0 start, 1 stop

  uiTimerEnable();
  attachInterrupt(digitalPinToInterrupt(encoderPinA), readEncoderA, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), readEncoderB, FALLING);

  pompaSelezionata = chiediPompa();
  velocitaDefault = chiediVelocitaDefault();
  ultimoProfiloCaricato = chiediProfiloCaricato();

  // Wifi.begin(ssid, password);
  // da definire ssid e password in secrets.h
  
  schermataIniziale();

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
          scenaAttuale = menuWiFi();
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
