/*
    Библиотека для амплитудного анализа звука на Arduino
    Документация: 
    GitHub: https://github.com/GyverLibs/VolAnalyzer
    Возможности:
    - Хитрый адаптивный алгоритм
    - Встроенные фильтры для плавного потока значений
    - Получение громкости в указанном диапазоне независимо от диапазона входного сигнала
    - Получение сигнала на резкие изменения звука
    - Работа в режиме виртуального анализатора (без привязки к АЦП МК)
    
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
    v1.1 - более резкое падение при пропадании звука
    v1.2 - +совместимость. Вернул плавное падение звука
    v1.3 - упрощение алгоритма. Новый обработчик импульсов
    v1.4 - улучшение алгоритма
*/

#ifndef VolAnalyzer_h
#define VolAnalyzer_h
#include <Arduino.h>
#include "FastFilterVA.h"

class VolAnalyzer {
public:
    // создать с указанием пина. Если не указывать - будет виртуальный анализатор
    VolAnalyzer (int pin = -1) {
        volF.setDt(20);
        volF.setPass(FF_PASS_MAX);
        maxF.setPass(FF_PASS_MAX);
        minF.setPass(FF_PASS_MIN);
        setVolK(25);
        setAmpliK(30);
        if (pin != -1) setPin(pin);
    }
    
    // указать пин АЦП
    void setPin(int pin) {
        _pin = pin;
        pinMode(_pin, INPUT);
    }
    
    // установить время между опросами АЦП (мкс) (по умолч. 500) 
    void setDt(int dt) {
        _dt = dt;
    }
    
    // УСТАРЕЛО! установить период между выборками
    void setPeriod(int period) {
        //_period = period;
    }
    
    // установка ширины окна выборки (по умолч. 20)
    void setWindow(int window) {
        _window = window;
    }
    
    // установить период фильтрации громкости (умолч 20)
    void setVolDt(int volDt) {
        volF.setDt(volDt);
    }
    
    // получить громкость в пределах setVolMin.. setVolMax
    int getVol() {
        return volF.getFil();
    }
    
    // установить минимальную величину громкости (умолч 0)
    void setVolMin(int scale) {
        _volMin = scale;
    }
    
    // установить максимальную величину громкости (умолч 100)
    void setVolMax(int scale) {
        _volMax = scale;
    }
    
    // установить порог громкости в единицах АЦП (умолч 30)
    void setTrsh(int trsh) {
        _trsh = trsh;
    }
    
    // установить период фильтрации амплитудных огибающих
    void setAmpliDt(int ampliDt) {
        _ampliDt = ampliDt;
    }	
    
    // установить коэффициент фильтрации громкости 0-31 (умолч 25)
    void setVolK(byte k) {
        volF.setK(k);
    }
    
    // установить коэффициент фильтрации амплитудных огибающих 0-31 (умолч 31)
    void setAmpliK(byte k) {
        maxF.setK(k);
        minF.setK(k);
    }    
    
    // верхний порог срабатывания пульса (по шкале громкости)
    void setPulseTrsh(int trsh) {
        _pulseTrsh = trsh;
    }
    
    // нижний порог перезагрузки пульса (по шкале громкости)
    void setPulseMin(int minV) {
        _pulseMin = minV;
    }
    
    // таймаут пульса, мс
    void setPulseTimeout(int tout) {
        _pulseTout = tout;
    }
    
    // резкий скачок громкости (true)
    bool getPulse() {
        if (_pulse) {
            _pulse = false;
            return true;
        }
        return false;
    }
    
    // получить текущее значение огибающей минимумов
    int getMin() {
        return minF.getFil();
    }
    
    // получить текущее значение огибающей максимумов
    int getMax() {
        return maxF.getFil();
    }
    
    // получить значение сырого сигнала
    int getRaw() {
        return _raw;
    }
    
    // получить максимальное значение сырого сигнала за выборку
    int getRawMax() {
        return _rawMax;
    }
    
    // опрашивать как можно чаще. Может принимать значение, если это виртуальный анализатор
    // вернёт true при окончании анализа выборки
    bool tick(int thisRead = -1) {
        volF.compute();
        if (millis() - _tmrAmpli >= _ampliDt) {         // таймер сглаживания амплитуды
            _tmrAmpli = millis();
            maxF.setRaw(_maxs);
            minF.setRaw(_mins);
            maxF.compute();
            minF.compute();
            _maxs = 0;
            _mins = 1023;
        }
        if (_dt == 0 || micros() - _tmrDt >= _dt) {     // таймер выборки
            _tmrDt = micros();
            if (thisRead == -1) thisRead = analogRead(_pin);
            if (thisRead > _max) _max = thisRead;       // ищем максимум
            if (!_first) {                              // первый запуск
                _first = 1;
                maxF.setFil(thisRead);
                minF.setFil(thisRead);
            }

            if (++count >= _window) {               // выборка завершена
                _raw = _max;                        // запомнили
                if (_max > _maxs) _maxs = _max;     // максимумы среди максимумов
                if (_max < _mins) _mins = _max;     // минимумы реди максимумов
                _rawMax = _maxs;                    // запомнили
                if (getMax() - getMin() < _trsh) _max = 0; // если окно громкости меньше порога, то 0
                else _max = constrain(map(_max, getMin(), getMax(), _volMin, _volMax), _volMin, _volMax); // перевод в громкость
                volF.setRaw(_max);                  // фильтр столбика громкости
                
                // обработка пульса
                if (!_pulseState) {
                    if (_max <= _pulseMin && millis() - _tmrPulse >= _pulseTout) _pulseState = 1;
                } else {
                    if (_max > _pulseTrsh) {
                        _pulseState = 0;
                        _pulse = 1;
                        _tmrPulse = millis();
                    }
                }
                _max = count = 0;
                return true;                              // выборка завершена
            }
        }
        return false;
    }    

private:
    // дефолты
    int _dt = 500;      // 500 мкс между сэмплами достаточно для музыки
    int _ampliDt = 150; // сглаживание амплитудных огибающих
    int _window = 20;   // при таком размере окна получаем длительность оцифровки 10 мс
    
    int _pin;
    uint32_t _tmrPulse = 0, _tmrDt = 0, _tmrAmpli = 0;
    int _raw = 0, _rawMax = 0;
    int _max = 0, _maxs = 0, _mins = 1023;
    int count = 0;
    int _volMin = 0, _volMax = 100, _trsh = 0;
    int _pulseTrsh = 80, _pulseMin = 0, _pulseTout = 100;
    bool _pulse = 0, _pulseState = 0, _first = 0;

    FastFilterVA minF, maxF, volF;
};
#endif