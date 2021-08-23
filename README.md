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
void setPin(int pin);       // указать пин АЦП
void setDt(int dt);         // установить время между опросами АЦП (мкс) (по умолч. 500)
void setPeriod(int period); // установить период между выборками (мс) (по умолч. 4)
void setWindow(int window); // установка ширины окна выборки (по умолч. 20)
void setVolDt(int volDt);   // установить период фильтрации громкости (умолч 20)
int getVol();               // получить громкость в пределах setVolMin.. setVolMax
void setVolMin(int scale);  // установить минимальную величину громкости (умолч 0)
void setVolMax(int scale);  // установить максимальную величину громкости (умолч 100)
void setTrsh(int trsh);     // установить порог громкости в единицах АЦП (умолч 30)
void setAmpliDt(int ampliDt);   // установить период фильтрации амплитудных огибающих
void setVolK(byte k);       // установить коэффициент фильтрации громкости 0-31 (умолч 25)
void setAmpliK(byte k);     // установить коэффициент фильтрации амплитудных огибающих 0-31 (умолч 31)
int getMin();               // получить текущее значение огибающей минимумов
int getMax();               // получить текущее значение огибающей максимумов
int getRaw();               // получить значение сырого сигнала
int getRawMax();            // получить максимальное значение сырого сигнала за выборку
bool getPulse();            // true - резкий скачок громкости

// опрашивать как можно чаще. Может принимать значение, если это виртуальный анализатор
// вернёт true при окончании анализа выборки
bool tick(int thisRead);
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

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!