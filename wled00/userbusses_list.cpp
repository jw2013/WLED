
#include "wled.h"

#define TYPE_AT8870_I2C_PSPWM       112

#define USERBUS_JSON1                ""

#ifdef USERBUS_AT8870_I2C_PSPWM
  #include "../userbusses/AT8870_I2C_PSPWM/AT8870_I2C_PSPWM.h"
#endif

void addUserbusTypesJSONString(String &json) {
#ifdef USERBUS_AT8870_I2C_PSPWM
  json += "{i:" + String(TYPE_AT8870_I2C_PSPWM) + ",c:3,t:\"V\",n:\"AT8870 I²C PSPWM\"},";
#endif
}

Bus* createUserbus(BusConfig &bc) {
/*
   * Add your userbus class name here
   * || || ||
   * \/ \/ \/
   */
  switch ( bc.type ) {
    #ifdef USERBUS_AT8870_I2C_PSPWM
      case TYPE_AT8870_I2C_PSPWM: return new BusAT8870_I2C_PSPWM(bc);
    #endif
  }
  return nullptr;
}

