
class BusAT8870_I2C_PSPWM : public Bus {
  public:
    BusAT8870_I2C_PSPWM(BusConfig &bc) : Bus(bc.type, bc.start, bc.autoWhite, bc.count) {
      allocateData(bc.count * 4);
      _hasRgb = true;
      _hasWhite = true;
      _pins[0] = bc.pins[0];
      _pins[1] = bc.pins[1];
      _pins[2] = bc.pins[2];
      _pins[3] = bc.pins[3];
      _pins[4] = bc.pins[4];
      _valid = 1;
    }
    
    ~BusAT8870_I2C_PSPWM() {
      cleanup();
      _type = 0;
      _valid = false;
      freeData();      
    }

    void setPixelColor(uint16_t pix, uint32_t c) {
      if (!_valid || pix >= _len) return;
      uint16_t i = pix * 4;
      _data[i++] = R(c);
      _data[i++] = G(c);
      _data[i++] = B(c);
      _data[i++] = W(c);
    }

    uint32_t getPixelColor(uint16_t pix) const {
      if (!_valid || pix >= _len) return 0;
      uint16_t i = pix * 4;
      return RGBW32(_data[i], _data[i+1], _data[i+2], _data[i+3]);
    }
    
    uint8_t getPins(uint8_t* pinArray = nullptr) const {
      if (!_valid) return 0;
      unsigned numPins = 5;
      for (unsigned i = 0; i < numPins; i++) pinArray[i] = _pins[i];
      return numPins;
    }

    void show(void) {
      uint32_t c = getPixelColor(0);
      if ( hasWhite() ) {
        c = autoWhiteCalc(c);
      }

      uint8_t pwm_duty;
      uint16_t pwm_delay;

      Wire.beginTransmission(_pins[0]);

      Wire.write(16);                           // 16 == start of PWM addresses

      pwm_delay = 0;                            // PWM delay for red = 0
      pwm_duty = (R(c) * _bri) / 255;       // PWM duty for red, scaled by brightness
      Wire.write(pwm_duty);
      Wire.write(pwm_delay);

      pwm_delay += pwm_duty;                    // PWM delay for green, start after red
      if ( pwm_delay > 254 ) pwm_delay -= 255;
      pwm_duty = (G(c) * _bri) / 255;       // PWM duty for green, scaled by brightness
      Wire.write(pwm_duty);
      Wire.write(pwm_delay);

      pwm_delay += pwm_duty;                    // PWM delay for blue, start after green
      if ( pwm_delay > 254 ) pwm_delay -= 255;
      pwm_duty = (B(c) * _bri) / 255;       // PWM duty for blue, scaled by brightness
      Wire.write(pwm_duty);
      Wire.write(pwm_delay);

      pwm_delay += pwm_duty;                    // PWM delay for white, start after blue
      if ( pwm_delay > 254 ) pwm_delay -= 255;
      pwm_duty = (W(c) * _bri) / 255;   // PWM duty for white, scaled by brightness
      Wire.write(pwm_duty);
      Wire.write(pwm_delay);

      Wire.endTransmission();
    }

    void cleanup() {
      _type = 0;
      _valid = false;
      freeData();      
    }


    uint8            _pins[5];              // used as configuration hints for the Usermod

};


