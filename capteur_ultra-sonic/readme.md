# Schéma

       [ CAPTEUR HC-SR04 ]                    [ ESP32 ]               [ ECRAN OLED I2C ]
          VCC (5V) <--------------------------> VIN / 5V
          Trig     <--------------------------> D5 (GPIO 5)
          GND      <--------------------------> GND <--------------------> GND

          Echo -------> [ Résistance 1k ] ----> D18 (GPIO 18)
                                       |
                                [ Résistance 2k ]
                                       |
                                      GND

                                                3V3 <--------------------> VCC (3.3V)
                                                D21 (SDA) <--------------> SDA
                                                D22 (SCL) <--------------> SCL
