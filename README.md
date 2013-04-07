Laser-Pointer
=============

Laser Pointer with PowerPoint controller

License
-------

[GPL version 3](./LICENSE.txt)

Copyright &copy; 2013 Yasunobu OKAMURA

Copyright &copy; 2012 Objective development ([USBaspLoader][usbasploader], [HID Keys][hidkeys] and [V-USB][vusb])

[vusb]: http://www.obdev.at/products/vusb/index.html
[hidkeys]: http://www.obdev.at/products/vusb/hidkeys.html
[usbasploader]: http://www.obdev.at/products/vusb/usbasploader.html

Make
----

### Schematics

* [Laser & IR Transmitter](laser-transmitter.pdf)
* [IR Receiver](receiver.pdf)

Original schematics were made by [Qt-BSch3V Modified](https://github.com/informationsea/Qt-BSch3V-Modified)

### Firmwares

* [Laser & IR Transmitter](./laser-transmitter-src)
* [IR Receiver](./receiver-src)
* [IR Receiver Bootloader](./bootloader-for-receiver) (I recommend to install bootloader if you want to modify IR Receiver)

No host software is required.
IR Receiver is recognized as keyboard.

### Photo

* [Photo 1](./photo/photo1.jpg)
* [Photo 2](./photo/photo2.jpg)



Parts List
----------

### IR Receiver

* LED (for debug)
    * Green LED x1
    * Blue LED x1
* Push switch x2 (for debug)
* Liner Regulator 3.3V x1
* USB Connector
    * A Connector or B Receptor x1
* IR Receiver x1
* AVR ATmega328P x1
* Resistor
    * 100 x2
    * 68 x2
* Crystal 12MHz
* Capacitor
    * 22pF x2
    * 0.1uF x3
* Pin header 4 pin x1 (for debug)

### Laser and IR Transmitter

* 2SC1815 x2
* LED x1
* IR LED x2
* Push switch x3
* ATtiny2313
* Pin header 2x3 pin x1 (for debug)
* Resistor
    * 1k x3
    * 100 x1
    * 10k x2
* Step-up DC/DC Converter 3.3V
* Battery Box
* Laser diode

  
