# gateBluetooth

To setup the EEPROM of the Arduino, execute `./gate/init/gen_init.sh` and upload sketch on Arduino board.
Run sketch once and upload gate.ino

##Usage:

/!\\ End of command is : `.`

`IDcommand.`: play command <command> authentifacted with id ID

ex: for ID 2 open gate: 2o.

##Commands:

+ `o`: open gate
+ `rID`: remove ID from db (/!\\ Need root id)
+ `aID`: add ID from db (/!\\ Need root id)
+ `c`: clear db (/!\\ Need root id)
+ `d`: show db (/!\\ Need root id)
+ `e`: dump interesing part of eeprom (/!\\ Need root id)
