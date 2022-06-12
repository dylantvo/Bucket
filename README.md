# Bucket

A repository for Bucket, a robot designed to compete in the 2020 IEEE R5 Robotics Competition through the UNO Robotics Club.

# Competition Rules

The game manual for the 2020 IEEE R5 Robotics Competition can be found here: https://r5conferences.org/wp-content/uploads/sites/126/Robotics_Game_Manual.pdf

# Our Approach

From the game manual, the scoring is calculated as follows:

- +1 point for each item of litter loaded in the Litterbug or placed on the field at the
start of each match.

- -1 point for each item of litter on the field at the end of the match. This includes
any parts that have come off the competition robot.

- +2 points for each item of litter in the correct bin (Cans in the can bin, bottles in
the plastic bin, snack bags and paper trays in the trash bin)

- +1 point for each can or bottle placed in the trash bin. No points are awarded for
putting incorrect items in a particular recycle bin.

- -1 point for each piece of litter from the match that is outside the field perimeter
and not in a bin or touching the robot

- +2 points for each type of litter that the litterbug has been persuaded to no longer
dispense, as indicated by the litterbug lights going out. An additional +2 point
bonus will be awarded if the litterbug has been persuaded to no longer dispense
any of the types of litter, as indicated by all four lights going out.


Our plan for achieving the most points was to gather all trash and deliver it to the trash bin, regardless of the type of trash. By doing this, we would maximize our points while minimizing the amount of time retrieving the trash. This strategy would also allow us to focus on deactivating the litterbug. Since points are not deducted for delivering trash to the incorrect bin and that we would be rewarded significantly more points for prioritizing deactivating the litterbug, we felt this would be the most efficient method.


# Design

![IMG_7803](https://user-images.githubusercontent.com/106710808/173253231-d3029c83-24fb-4798-b3fd-fa7bb197962f.jpg)

**Chassis**

Bucket consists of a large acryllic chassis with a sweeping mechanism for retrieving and storing trash. Using four mecanum wheels driven by NEMA stepper motors, bucket is capable of moving precisely with relatively low power. 

**Electrical System**

Bucket is powered by a custom Li-Po battery pack in a 4S2P configuration (~14.8V nominal). Two buck/boost converters use this battery pack to create a 30V and 5V distribution. The stepper motors and the sweeping mechanism motor is powered by the 30V distribution. The ultrasonic sensors are powered by the 5V distribution. All devices are controlled by an Arduino Mega microcontroller.

**Program**

Bucket will prioritize deactivating the litterbug robot at the beginning of the round to maximize points. Once this operation has been completed, it will begin to retrieve the trash. In order to retrieve all trash on the field, Bucket will run through a sequence that will allow it to cover each block of the 12'x12' playing field. It then proceeds to the trash bin and operates the sweeping mechanism in reverse, pushing the trash out.


Video of Bucket: https://www.youtube.com/watch?v=kkj2QAQp_2s


# Contributors

**Mechanical**


- Tim Ash

- Jaime McCarroll


**Electrical**


- Michael Ceraso

- Dylan Vo

- Sean Demers

- John McGraw

- Anne Kroll

- Floyd Andrus

- Angela Ellis

**Programming**


- Michael Ceraso

- Dylan Vo

- Sean Demers
