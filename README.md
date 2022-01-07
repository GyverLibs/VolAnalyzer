![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![author](https://img.shields.io/badge/author-AlexGyver-informational.svg)
# VolAnalyzer
Библиотека для амплитудного анализа звука на Arduino
- Хитрый адаптивный алгоритм
- Встроенные фильтры для плавного потока значений
- Получение громкости в указанном диапазоне независимо от диапазона входного сигнала
- Получение сигнала на резкие изменения звука
- Работа в режиме виртуального анализатора (без привязки к АЦП МК)

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **VolAnalyzer** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/VolAnalyzer/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Инициализация
```cpp
VolAnalyzer analyzer(A0);   // указать пин
VolAnalyzer analyzer;       // "виртуальный" анализатор
```

<a id="usage"></a>
## Использование
```cpp
// тикер анализатора. Вернёт true при завершении текущего анализа. Вызывать почаще
bool tick();                    // опрашивает указанный в setPin пин
bool tick(int thisRead);        // берёт указанное значение

// настройки анализа
void setPin(int pin);           // указать пин АЦП
void setDt(int dt);             // установить время между опросами АЦП, мкс (по умолч. 500)
void setWindow(int window);     // установка ширины окна выборки (по умолч. 20)
void setTrsh(int trsh);         // установить порог громкости в единицах raw АЦП (умолч 40)

// амплитуда
void setAmpliDt(int ampliDt);   // установить период фильтрации амплитудных огибающих, мс (умолч 150)
void setAmpliK(byte k);         // установить коэффициент фильтрации амплитудных огибающих 0-31 (умолч 30)

// громкость
void setVolDt(int volDt);       // установить период фильтрации громкости (умолч 20)
void setVolK(byte k);           // установить коэффициент фильтрации громкости 0-31 (умолч 25)
void setVolMin(int scale);      // установить минимальную величину громкости (умолч 0)
void setVolMax(int scale);      // установить максимальную величину громкости (умолч 100)

// пульс
void setPulseMax(int maxV);     // верхний порог срабатывания пульса (по шкале громкости)
void setPulseMin(int minV);     // нижний порог перезагрузки пульса (по шкале громкости)
void setPulseTimeout(int tout); // таймаут пульса, мс

// получаем значения
int getVol();                   // громкость в пределах setVolMin.. setVolMax
bool pulse();                   // резкий скачок громкости
int getMin();                   // текущее значение огибающей минимумов
int getMax();                   // текущее значение огибающей максимумов
int getRaw();                   // значение сырого сигнала
```

<a id="example"></a>
## Пример
```cpp
// амплитудный анализ звука

#include "VolAnalyzer.h"
VolAnalyzer analyzer(A0);


void setup() {
  Serial.begin(115200);
  analyzer.setVolK(20);
  analyzer.setTrsh(10);
  analyzer.setVolMin(10);
  analyzer.setVolMax(100);
}

void loop() {
  if (analyzer.tick()) {
    //Serial.print(analyzer.getVol());
    //Serial.print(',');
    Serial.print(analyzer.getRaw());
    Serial.print(',');
    Serial.print(analyzer.getMin());
    Serial.print(',');
    Serial.println(analyzer.getMax());
  }
}
```

<a id="versions"></a>
## Версии
- v1.0
- v1.1 - более резкое падение при пропадании звука
- v1.2 - +совместимость. Вернул плавное падение звука
- v1.3 - новый обработчик импульсов
- v1.4 - улучшение алгоритма
- v1.5 - сильное облегчение и улучшение алгоритма
- v1.6 - более резкая реакция на звук
- v1.7 - исключено деление на 0 в map
- v1.8 - теперь работает с 12 бит АЦП

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!