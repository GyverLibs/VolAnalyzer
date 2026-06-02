This is an automatic translation and may be incorrect in some places. See the source README and examples for authoritative information.

[![latest](https://img.shields.io/github/v/release/GyverLibs/VolAnalyzer.svg?color=brightgreen)](https://github.com/GyverLibs/VolAnalyzer/releases/latest/download/VolAnalyzer.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/VolAnalyzer.svg)](https://registry.platformio.org/libraries/gyverlibs/VolAnalyzer)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/VolAnalyzer?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# VolAnalyzer
A library for amplitude sound analysis on Arduino
- A clever adaptive algorithm
- Built-in filters for smooth value flow
- Receiving volume in the specified range regardless of the range of the input signal
- Receiving a signal for sudden changes in sound
- Work in virtual analyzer mode (without reference to ADC MK)

### Compatibility
Compatible with all Arduino platforms (Arduino features are used)

## Contents
- [Installation](#install)
- [Initialization](#init)
- [Use of use](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- The library can be found under the name **VolAnalyzer** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download the library](https://github.com/GyverLibs/VolAnalyzer/archive/refs/heads/main.zip).zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/ *
    - (Arduino IDE) Automatic installation from .zip: *Sketch/Connect library/Add .ZIP library...* and specify downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features.
- Through the library manager IDE: find the library as when installing and click "Update"
- Manually: **Delete the folder with the old version** and then put the new one in its place. “Replacement” can not be done: sometimes new versions delete files that will remain when replaced and can lead to errors!

<a id="init"></a>
## Initialization
```cpp
VolAnalyzer analyzer(A0);   // pin
VolAnalyzer analyzer;       // virtual analyzer
```

<a id="usage"></a>
## Use of use
```cpp
// =========================== TICK ===========================
// interview as often as possible. It can make a difference if it is a virtual analyzer.
// return true at the end of the sample analysis
bool tick(int read = -1);

// ========================== SETTINGS ==========================
void setPin(int8_t pin);        // pin
void setDt(uint16_t dt);        // set the time between surveys ADC (mx) (by default 500)
void setWindow(uint8_t window); // setting the width of the sample window (in silence). 20)
void setTrsh(uint16_t trsh);    // set the volume threshold in ADC units (silent 40)

// ========================== VOLUME ==========================
void setVolDt(uint8_t dt);      // set the volume filtration period (silent 20)
void setVolK(uint8_t vk);       // set the volume smoothness coefficient 0-31 (silent 25)
uint16_t getVol();              // Get the volume within setVolMin. setVolMax
void setVolMin(uint8_t vol);    // set the minimum volume (silent 0)
void setVolMax(uint8_t vol);    // set the maximum volume (silent 100)

// ========================= AMPLITUDE =========================
void setAmpliDt(uint8_t dt);    // determine the filtering period of amplitude envelopes
void setAmpliK(uint8_t rk);     // set the amplitude smoothness coefficient 0-31 (silent 31)
uint16_t getMin();              // obtain the current value of the envelope minimums (from v1.5 - 0)
uint16_t getMax();              // get the current value of the envelope maximums

// =========================== PULSE ===========================
void setPulseMax(uint8_t maxV); // upper threshold of pulse response (volume scale)
void setPulseMin(uint8_t minV); // lower threshold of pulse reset (volume scale)
void setPulseTimeout(uint16_t tout);    // pulse timeout, Ms.
bool pulse();                   // loudness jump (true)

// ========================== RAW DATA ===========================
uint16_t getRaw();              // get the raw signal value for the sample
uint16_t getTrsh();             // Get the volume threshold in ADC units
```

<a id="example"></a>
## Example
```cpp
// sound analysis

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
## Versions
- v1.0
- v1.1 = sharper drop when sound is lost
- v1.2 - +compatibility. Returned the smooth drop of sound
- v1.3 - new pulse handler
- v1.4 - Improvement of the algorithm
- v1.5 - Strong relief and improvement of the algorithm
- v1.6 - more severe response to sound
- v1.7 - no dividing by 0 in map
- v1.8 - now works with 12 bit ADC
- v1.9 - facilitated SRAM

<a id="feedback"></a>
## Bugs and feedback
If you find bugs, create **Issue**, or better write to the mail immediately.[alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
The library is open for revision and your **Pull Requests*!

When reporting bugs or incorrect work of the library, it is necessary to specify:
- Library version
- What is used by the IC
- SDK version (for ESP)
- Arduino IDE version
- Are embedded examples that use features and designs that cause bugs in your code working correctly?
- What code was downloaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimum code.
