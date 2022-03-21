#include <FreeRTOS_AVR.h>

#define DO_STAIR  (A0)
#define RE_STAIR  (A1)
#define MI_STAIR  (A2)
#define FA_STAIR  (A3)
#define SO_STAIR  (A4)
#define LA_STAIR  (A5)
#define TI_STAIR  (A6)
#define HDO_STAIR (A7)

int stairs_pin[8] = {DO_STAIR, RE_STAIR, MI_STAIR, FA_STAIR, SO_STAIR, LA_STAIR, TI_STAIR, HDO_STAIR};
int stairs[8] = {0,};


SemaphoreHandle_t xMutex;

static void sensor_measurement(int arg) {
  
  int state = 0; 
  for(int i = 0; i < 30; i++)
  {
    state += analogRead(stairs_pin[arg]);   
    Serial.println(state);
  }
  state = state/30;
  Serial.println(state);
  while (1) {
    int Val = analogRead(stairs_pin[arg]);
    if (Val < state-100) {
      stairs[arg] = 1;
    } else {
      stairs[arg] = 0;
    }
  }
}

static void send_text(int arg) {
  while (1) {
    xSemaphoreTake(xMutex, portMAX_DELAY);
    Serial.print("@");
    Serial.print(stairs[0]);
    Serial.print(", ");
    Serial.print(stairs[1]);
    Serial.print(", ");
    Serial.print(stairs[2]);
    Serial.print(", ");
    Serial.print(stairs[3]);
    Serial.print(", ");
    Serial.print(stairs[4]);
    Serial.print(", ");
    Serial.print(stairs[5]);
    Serial.print(", ");
    Serial.print(stairs[6]);
    Serial.print(", ");
    Serial.print(stairs[7]);
    Serial.print(", ");
    Serial.println("&");
    
    xSemaphoreGive(xMutex);
  }
}


void setup() {
  Serial.begin(9600);
  portBASE_TYPE s1;
  xMutex = xSemaphoreCreateMutex();
  for (int i = 0; i < 3; i++) {
    s1 = xTaskCreate(sensor_measurement, NULL, 500, i, 1, NULL);
    if (s1 != pdPASS) {
      Serial.println(F("Creation problem"));
      while (1);
    }
  }
  

  portBASE_TYPE s2 = xTaskCreate(send_text, "Thread2", 500, NULL, 1, NULL);
  if (s2 != pdPASS) {
    Serial.println(F("Creation problem"));
    while (1);
  }

  vTaskStartScheduler();
  Serial.println(F("lnsufficient RAM"));
  while (1);
}

void loop() {

}
