# Environmental Data Viewer

## Purpose:
This was developed to vizualize sensor data from a raspberry pi. 

## Stack:
This application utilizes Java Spring, python, MySQL, Highcharts, Bootstrap, and jasperReports. 

## Folder Descriptions:
-The viewer folder is the Java Spring application that is deployed onto a Raspberry Pi using tomcat. 

-The Sensor folder contains the python code to read the sensor data, display it on a oled display, and insert it into the MySQL database. 

## Future Plans:
- Add in a dynamic way to add more sensors.

- Develop a user registration page.
- Develop admin functions 

  - Clear database for mutiple timeframes (day/week/month/year)
  
  - Add ability to stop/restart Raspberry pi
  
  - User managment
  
  - Device managment
  
 - Add more statistics to dashboard
 
 - Add more graphs with date filtering for the data
 
 - Link in local weather api and display in the dashboard
 
 - Add map and allow sensors to colorcode rooms based on readings. 
 
## Dashboard Screenshots:

### Dashboard:
<img src="https://github.com/bpawlin1/enivData/blob/main/Images/DashboardHome.JPG" alt="Dashboard" style="max-width: 100%;">

### Data Listing:
<img src="https://github.com/bpawlin1/enivData/blob/main/Images/TableView.JPG" alt="Dashboard" style="max-width: 100%;">

