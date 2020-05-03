# ArduinoDDSGen
Arduino Function Generator

This is the software for an Arduino-based function generator. It is more of a learning experiment as it is not very useful because its frequency range is very limited (5-210 Hz). With the DAC that I'm curently using, it can reach a max frequency of around 480Hz, but precisely setting the frequency above 210Hz is impossible due to the low resolution of the timers (see the Excel spreadsheet in the repository). Waveforms are generated using an 8bit R-2R DAC, connected via SPI using an 74HC595 shift register. Square waves are generated using fast PWM and they can reach frequencies in excess of 100kHz.
