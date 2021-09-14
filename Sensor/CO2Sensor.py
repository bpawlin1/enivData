#
#
# This project will collect temperature,  humidity, and CO2  information using a SCD30 sensor
# and send this information to a MySQL database.
#
from __future__ import print_function
import qwiic_serlcd
import time
import sys
import time
import board
import busio
import adafruit_scd30
import datetime
import MySQLdb


i2c = busio.I2C(board.SCL, board.SDA)
scd = adafruit_scd30.SCD30(i2c)



# Settings for database connection
hostname = '127.0.0.1'
username = 'bpawling'
password = 'Bandit2015'
database = 'Environmental_Data'

device = 'dev-pi'




# Routine to insert temperature records into the Environmental_Data co2_data table:
def insert_record( device, datetime, temp, hum, c02, co2_rating ):
        query = "INSERT INTO co2_data (device,datetime,temp,humidity,co2,co2_rating) " \
                "VALUES (%s,%s,%s,%s,%s,%s)"
        args = (device, datetime,temp,humidity,co2,co2_rating)

        try:
            conn = MySQLdb.connect( host=hostname, user=username, passwd=password, db=database )
            cursor = conn.cursor()
            cursor.execute(query, args)
            conn.commit()

        except Exception as error:
            print(error)

        finally:
          cursor.close()
          conn.close()


# Main loop
myLCD = qwiic_serlcd.QwiicSerlcd()
myLCD.setBacklight(255, 255, 255) # Set backlight to bright white
myLCD.setContrast(5) 
myLCD.clearScreen() # clear the screen




while True:
        data = scd.data_available
        if data:
            print("Data Available!")
            temp = scd.temperature * 9/5.0 + 32
            now = datetime.datetime.now()
            humidity = scd.relative_humidity
            co2 = scd.CO2
            if scd.CO2 < 1000:
                co2_rating = "Good"
                myLCD.clearScreen()
                myLCD.setBacklight(0, 255, 0)# bright green
            if scd.CO2 > 1000 and  scd.CO2 < 2000:
                co2_rating = "Poor"
                myLCD.clearScreen()
                myLCD.setBacklight(255, 255, 0)# bright yellow
            if scd.CO2 > 2000 and  scd.CO2 < 5000:
                co2_rating = "Bad"
                myLCD.clearScreen()
                myLCD.setBacklight(0xFF, 0x8C, 0x00) # orange
            if scd.CO2 > 5000:
                co2_rating = "Very Bad"
                myLCD.clearScreen()
                myLCD.setBacklight(255, 0, 0)
            
            date = now.strftime('%Y-%m-%d %H:%M:%S')
            lcdDate = now.strftime('%m-%d %H:%M')

            insert_record(device,str(date),format(temp,'.2f'),format(humidity,'.2f'), format(co2,'.2f'), co2_rating)
            print (device,str(date),format(temp,'.2f'),format(humidity,'.2f'), format(co2,'.2f'), co2_rating)
            
            data = (co2_rating)
            temp = (format(temp,'.2f'))

            myLCD.print(lcdDate)
            myLCD.setCursor(0,1)
            myLCD.print("CO2:" + data + " ")
            myLCD.print(str(temp) + "f")
            time.sleep(360)