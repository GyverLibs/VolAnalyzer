// быстрый целочисленный экспоненциальный фильтр (только int)

#ifndef FastFilterVA_h
#define FastFilterVA_h
#include <Arduino.h>

#define FF_PASS_MAX 1
#define FF_PASS_MIN 2

class FastFilterVA {
public:
    // коэффициент 0-31
    FastFilterVA(byte k = 20, int dt = 0) {
        setK(k);
        setDt(dt);
    }
    
    // коэффициент 0-31
    void setK(byte k) {
        _k1 = k;
        _k2 = k / 2;
    }
    
    // установить период фильтрации
    void setDt(int dt) {
        _dt = dt;
    }
    
    // установить режим пропуска (FF_PASS_MAX / FF_PASS_MIN)
    void setPass(byte pass) {
        _pass = pass;
    }
    
    // установить исходное значение для фильтрации
    void setRaw(int raw) {
        _raw = raw;
    }
    
    // установить фильтрованное значение
    void setFil(int fil) {
        _raw_f = fil;
    }
        
    // расчёт по таймеру
    void compute() {
        if (_dt == 0 || millis() - _tmr >= _dt) {
            _tmr = millis();
            computeNow();
        }
    }
    
    // произвести расчёт сейчас
    void computeNow() {
        int k = _k1;
        if ((_pass == FF_PASS_MAX && _raw > _raw_f) 
            || (_pass == FF_PASS_MIN && _raw < _raw_f)) k = _k2;               
        _raw_f = (k * _raw_f + (32 - k) * _raw) >> 5;
    }
    
    // получить фильтрованное значение
    long getFil() {
        return _raw_f;
    }
    
    // получить последнее сырое значение
    long getRaw() {
        return _raw;
    }
    
private:
    uint32_t _tmr = 0;
    int _dt = 0;
    byte _k1 = 20, _k2 = 12;
    byte _pass = 0;
    int _raw_f = 0, _raw = 0;
};
#endif