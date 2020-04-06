## Abstract

ESPHome configuration to drive LC-Technology relay modules. 
Tested on a 12V 4 channel version with nuvoton MCU.
See detailed writeup of this project in my blog:  

https://ncrmnt.org/?p=3333


## Running & flashing

```
esphome xxx.yaml run
```

## Configs

This repository contains 2 sample configs

- gatekeeper.yaml

A config I used to drive Nice Flor-S remote control buttons. 

- relay-simple.yaml

A config that just exposes all relays for your fun. 

- relay.yaml

Same as the above, but with LEDs and buttons. Needs alternative nuvoton firmware

## nuvoton firmware. 

Stock firmware only supports relay control. I've also created an alternative 
firmware for nuvoton MCU that exposes three LEDs with PWM (RGB) and 
reports button states. You'll need nuvoton's ISP and a Windows box (or a VM) to flash it.
I used linux to develop the firmware and a windows box to flash it.

## Legal stuff

Copyright © 2019 Andrew 'Necromant' Andrianov <spam@ncrmnt.org>
This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the COPYING file for more details.