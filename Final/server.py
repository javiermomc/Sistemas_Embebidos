# Authors:
#   Javier Mondragon Martin del Campo
#   Luis Alejandro Enriquez Solis
#   Marisa Diaz

from flask import Flask, render_template, url_for, jsonify, request
from jinja2.utils import consume
import time
from slaves import *

rtc = RTC()
lcd = LCD()
tc = TC74()
led = Arduino()

print(tc.get())

lcd.clear()
lcd.display_string('Waiting for', 1)
lcd.display_string('Client...', 2)

app = Flask(__name__)

state_bulb = False
state_lcd = False

@app.route('/')
def index():
  return render_template('index.html')

@app.route('/manager', methods=['GET', 'POST'])
def manager():
  global state_bulb
  global state_lcd
  data = request.get_json()
  if data['request'] == 'light':
    state_bulb = data['body']
    led.write(state_bulb)
    return jsonify(state_bulb)
  if data['request'] == 'text':
    if state_lcd == False:
      state_lcd = True
      lcd.clear()
      disp = data['body'].split('\n')
      lcd.display_string(disp[0][0:16], 1)
      lcd.display_string(disp[1][0:16], 2)
      state_lcd = False
      return jsonify(True)
    else:
      return jsonify(False)
    
  if data['request'] == 'time':
    try:
      data = rtc.get()
      print(data)
      h = hex(data[2])[2:]
      m = hex(data[1])[2:]
      s = hex(data[0])[2:]
      if len(h) == 1:
        h = '0'+h
      if len(m) == 1:
        m = '0'+m
      if len(s) == 1:
        s = '0'+s
      current_time = h+':'+m+':'+s
      return jsonify(current_time)
    except:
      print('No time')
      return jsonify('error')
  if data['request'] == 'temp':
    try:
      data = tc.get()
      print(data)
      return jsonify(data)
    except:
      print('No temp')
      return jsonify('error')
  return('')


if __name__ == "__main__":
  app.run(host='0.0.0.0', debug=True)
