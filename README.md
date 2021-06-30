# INS GPS Integration
Some types of navigation systems that are generally used today are global positioning system (GPS) and inertial navigation system (INS). GPS relies on satellites to constantly determine the position of an object. Unlike GPS, INS work independently by utilizing a set of inertial sensors (accelerometer and gyroscopes) and processing devices to find the position, velocity, orientation, and other important navigation quantities. This project investigates the performance of the integration of GPS and INS simultaneously in tracking the position of moving object and searching for other important navigation quantities.

<img src="/img/track2.jpg" width=70%>

<img src="/img/plot.png">

## Specification
-  SBG Systems Ellipse-N INS
-  Intenal GNSS Tallysman TW2710
-  System processing using laptop with specs:
    -  CPU Intel Core i7-6700HQ
    -  GPU NVIDIA GeForce GTX 950M
    -  RAM 8 GB
## Tutorial
This program was built using Visual Studio 2019 and Spyder as IDE
1. Inside sbgECom folder, extract all files and folder of sbgECom.zip
2. Create Visual Studio 2019 project using sbgECom.sln from sbgECom/projects/visual folder. Then build solution.
3. Navigate to sbgECom/examples/ellipseMinimal/projects/visual folder then build project again using ellipseMinimal.vcxproj file.
4. Install Inertial SDK from [here](https://www.sbg-systems.com/ellipse-ellipse-micro-support/). This application needed to install driver for INS.
5. Connect GNSS to INS, then connect INS to laptop under USB port. INS driver will automatically installed.
6. Open ellipseMinimal.c from sbgECom/examples/ellipseMinimal/src. Compile and run (debug) the program.
7. While logging the data, run Python program from plot folder using Spyder IDE. The plot will form a track of INS position.
