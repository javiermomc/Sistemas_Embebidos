from datetime import datetime
import time
import smbus

bus = smbus.SMBus(1)

class ARDUINO:
  __address = 0x08
  def led_on(self):
    bus.write_byte(self.__address, 0x1)
  
  def led_off(self):
    bus.write_byte(self.__address, 0x0)

class RTC:
  __address = 0x68

  def __init__(self):
    now = datetime.now()
    year = now.year%100
    setup = [
      int((int(now.second/10))<<4|(now.second%10)),
      int((int(now.minute/10))<<4|(now.minute%10)),
      int((int(now.hour/10))<<4|(now.hour%10)),
      int(now.weekday()),
      int((int(now.day/10))<<4|(now.day%10)),
      int((int(now.month/10))<<4|(now.month%10)),
      int((int(year/10))<<4|(year%10))
    ]
    bus.write_i2c_block_data(self.__address, 0, setup)
  def get(self):
    return bus.read_i2c_block_data(self.__address, 0, 3)

class TC74:
  __address = 0x4d

  def get(self):
    return bus.read_byte(self.__address)

class LCD:

  # commands
  __LCDCLEARDISPLAY = 0x01
  __LCDRETURNHOME = 0x02
  __LCDENTRYMODESET = 0x04
  __LCDDISPLAYCONTROL = 0x08
  __LCDCURSORSHIFT = 0x10
  __LCDFUNCTIONSET = 0x20
  __LCDSETCGRAMADDR = 0x40
  __LCDSETDDRAMADDR = 0x80

  # flags for display entry mode
  __LCDENTRYRIGHT = 0x00
  __LCDENTRYLEFT = 0x02
  __LCDENTRYSHIFTINCREMENT = 0x01
  __LCDENTRYSHIFTDECREMENT = 0x00

  # flags for display on/off control
  __LCDDISPLAYON = 0x04
  __LCDDISPLAYOFF = 0x00
  __LCDCURSORON = 0x02
  __LCDCURSOROFF = 0x00
  __LCDBLINKON = 0x01
  __LCDBLINKOFF = 0x00

  # flags for display/cursor shift
  __LCDDISPLAYMOVE = 0x08
  __LCDCURSORMOVE = 0x00
  __LCDMOVERIGHT = 0x04
  __LCDMOVELEFT = 0x00

  # flags for function set
  __LCD8BITMODE = 0x10
  __LCD4BITMODE = 0x00
  __LCD2LINE = 0x08
  __LCD1LINE = 0x00
  __LCD5x10DOTS = 0x04
  __LCD5x8DOTS = 0x00

  # flags for backlight control
  __LCDBACKLIGHT = 0x08
  __LCDNOBACKLIGHT = 0x00

  __address = 0x27

  __En = 0b00000100 # self.__Enable bit
  __Rw = 0b00000010 # Read/Write bit
  __Rs = 0b00000001 # Register select bit

  def __init__(self):
    self.write(0x03)
    self.write(0x03)
    self.write(0x03)
    self.write(0x02)

    self.write(self.__LCDFUNCTIONSET | self.__LCD2LINE | self.__LCD5x8DOTS | self.__LCD4BITMODE)
    self.write(self.__LCDDISPLAYCONTROL | self.__LCDDISPLAYON)
    self.write(self.__LCDCLEARDISPLAY)
    self.write(self.__LCDENTRYMODESET | self.__LCDENTRYLEFT)
    time.sleep(0.2)


  # clocks EN to latch command
  def strobe(self, data):
    bus.write_byte(self.__address, data | self.__En | self.__LCDBACKLIGHT)
    time.sleep(.0005)
    bus.write_byte(self.__address, ((data & ~self.__En) | self.__LCDBACKLIGHT))
    time.sleep(.0001)

  def write_four_bits(self, data):
    bus.write_byte(self.__address, data | self.__LCDBACKLIGHT)
    time.sleep(.0001)
    self.strobe(data)

  # write a command to lcd
  def write(self, cmd, mode=0):
    self.write_four_bits(mode | (cmd & 0xF0))
    self.write_four_bits(mode | ((cmd << 4) & 0xF0))

  # write a character to lcd (or character rom) 0x09: backlight | RS=DR<
  # works!
  def write_char(self, charvalue, mode=1):
    self.write_four_bits(mode | (charvalue & 0xF0))
    self.write_four_bits(mode | ((charvalue << 4) & 0xF0))

  # put string function with optional char positioning
  def display_string(self, string, line=1, pos=0):
    if line == 1:
      pos_new = pos
    elif line == 2:
      pos_new = 0x40 + pos
    elif line == 3:
      pos_new = 0x14 + pos
    elif line == 4:
      pos_new = 0x54 + pos
    
    self.write(0x80 + pos_new)

    for char in string:
      self.write(ord(char), self.__Rs)

  # clear lcd and set to home
  def clear(self):
    self.write(self.__LCDCLEARDISPLAY)
    self.write(self.__LCDRETURNHOME)

  # define backlight on/off (lcd.backlight(1); off= lcd.backlight(0)
  def backlight(self, state): # for state, 1 = on, 0 = off
    if state == 1:
        bus.write_byte(self.__address, self.__LCDBACKLIGHT)
    elif state == 0:
        bus.write_byte(self.__address, self.__LCDNOBACKLIGHT)

  # add custom characters (0 - 7)
  def load_custom_chars(self, fontdata):
    self.write(0x40);
    for char in fontdata:
        for line in char:
          self.write_char(line)
