#include <Arduino.h>
#include <UnoCarV2.h>

void setup() {
  Serial.begin(115200);

  unoCarV2.begin();
}

// drive1 = driveA
// drive2 = driveB
// ...
// drive6 = driveF

// stop1 = stopA
// stop2 = stopB
// ...
// stop6 = stopF

void loop() {
  // 1 號馬達以一半速度前進
  unoCarV2.drive1(2048);
  Serial.println("M1 - 前進 half speed");
  delay(1000);

  // 停止 1 號馬達
  unoCarV2.stop1();
  Serial.println("M1 停止");
  delay(500);

  // 2 號馬達以全速後退
  unoCarV2.drive2(-4095);
  Serial.println("M2 - 後退 full speed");
  delay(1000);

  // 停止所有馬達
  unoCarV2.stopAll();
  Serial.println("停止所有馬達");
  delay(1000);

  // 同時啟動全部馬達
  Serial.println("全部馬達全速運轉");
  unoCarV2.drive1(4095);
  unoCarV2.drive2(4095);
  unoCarV2.drive3(4095);
  unoCarV2.drive4(4095);
  unoCarV2.drive5(4095);
  unoCarV2.drive6(4095);
  delay(2000);

  // 停止所有馬達
  unoCarV2.stopAll();
  // unoCarV2.stop(M1);
  // unoCarV2.stop(M2);
  // ...
  Serial.println("停止所有馬達");
  delay(1000);
}
