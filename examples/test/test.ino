// амплитудный анализ звука

#include "VolAnalyzer.h"
VolAnalyzer analyzer(A0);

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (analyzer.tick()) {
    Serial.print(analyzer.getVol());
    Serial.print(',');
    Serial.print(analyzer.getRaw());
    Serial.print(',');
    Serial.print(analyzer.getMin());
    Serial.print(',');
    Serial.println(analyzer.getMax());
  }
}
