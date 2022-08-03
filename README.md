# ROB-550: BotLab, Team 5

## Description
This repository is the MobileBot section of the code base of the BotLab project in the ROB-550 course. At a high level, running this on a Beagle Bone Blue 
board allows low level control algorithms to be deployed and tested on a 2 DOF wheeled robot (MBot). 

The BotLab repository that makes up the other half of this code base can be found [here](https://github.com/Aaatresh/team-2-3-botlab).

## Getting Started

### Dependencies
* Python 2.7
* C++

### Installing
```
  git clone git@github.com:Aaatresh/team-2-3-mobilebot.git
```

### Executing the code
Run ```mobilebot/mobilebot.c``` to take forward and rotational velocity commands and perform low level control operations. Sensor data is read and odometry data is calculated, before both being published. 

## Contributors
* Anirudh Ashok Aatresh ([aaatresh@umich.edu](mailto:aaatresh@umich.edu))
* Devansh Agrawal ([devansh@umich.edu](mailto:devansh@umich.edu))
* Shreya Phirke ([sphirke@umich.edu](mailto:sphirke@umich.edu))
