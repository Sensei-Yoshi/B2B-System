# What is B2B System?
The Bike to Bike System or in short, the B2B system all started when we were bored during the pandemic. As we were brainstorming ideas for our local science fair, we stumbled across research articles stating how the visually impaired’s health is deteriorating over time because of limited exercise options. We also thought of our own personal experiences working with them and saw their passion to exercise. We observed that they would love to ride a bike and they already experience a strong sense of balance. With a limited number of biking systems such as the tandem bike, they do not feel independent and thus make them feel like being on a leash. This is where we want to step in by designing, prototyping, and testing a semi-autonomous system to give the joy and freedom of riding a bike, independently to the visually impaired community.
# How does it work?

The system features 2 bikes, a sighted person riding on the pilot bike ahead of a visually impaired person riding on the guided bike.

The pilot bike has 1 module, a Data Transmitter. It consists of a Gyro sensor and a GPS, sending the yaw angle of the bike and GPS coordinates via a RF chip to the guided bike. The data is sent at each turn.

The guided bike has 3 modules, namely a Data Receiver, Mechanical Clicker, and Safely Module. 

The Data Receiver consists of an RF chip receiving data from the pilot bike, a GPS to provide the guided bike’s current location, and an MP3 player attached to a headphone providing voice instructions.  Arduino code calculates the distance, time, and turn angle between the two bikes and then produces voice instructions like “Turn Left 3 Clicks in 4..3.. 2..1”. 
The Mechanical Clicker is attached to the bike’s handlebar and produces a click sound and haptic sensation based on the degree of the turn. Each click is 15 degrees.

The Safety Module consists of 3 ultrasonic sensors attached in front of the bike with 2 vibrating motors attached to the bike handle grips. Ultrasonic sensors sense any object within 10 ft of distance and a 45-degree conical radius in front of the bike to vibrate the handle grips to alert of any sudden obstacles.

# How can you help us?
We are both amateurs at C programming and working with arduino, and so we wanted you reach out to all of you to take this project further. The B2B system has won many awards from a local science fair all the way to getting 1st place in the Technology catagory and the Engineering atagory in a national science fair called **Broadcom Masters**. We also collaborated with our main target group, the visually impaired and hope to implement their feedback in the future. Thank you so much to all of you who contribute to our project! If you need to contact us email us at ... (B2B google account)
