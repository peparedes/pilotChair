# stanford_pilotchair
Arduino code for pilotchair developed at Stanford University in 2016 summer, supervised by Dr. Pablo Paredes and prof. James Landay.

Mind·ful
ˈmīn(d)fəl/ (adjective)
Conscious or aware of something.

 * Pilot Chair
 * 
 * supervisor | paredes@cs.stanford.edu 
 * developers | minkyuk@andrew.cmu.edu, ml27@princeton.edu, carriecai08@gmail.com
 *         
 * PilotChair API is written in Arduino. pilotChair.ino supports the activations and control of the 
 * motors in the back of the car seat, motivated and developed from an human-computer interaction perspective. 
 * Features with the motor range from activation of a single motor to a activation of a row. 
 * Signals received from the seat through arduino are total 14, representing activation of each 
 * row/col for a 7x7 matrix of motors. Note that not all entries have a corresponding motor; mat is 
 * specficed in constant below. Mixtures of interventions involve an apparent-tactile motion for which 
 * each signal sent is overlapped by a factor (e.g. 20ms) for an effective swipe. Apparent Tactile Motion 
 * requires no hard resets in between interventions or subsets of it; we manually packaged and implemented 
 * ovalaps for basic prototype/movements. More updates are to come in the code.


Apparent Tactile Motion

http://ieeexplore.ieee.org/document/7177720/?arnumber=7177720&tag=1



 
