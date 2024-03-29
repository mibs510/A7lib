# A7lib

![Alt text](https://www.makerfabs.com/image/cache/makerfabs/A7%20Minimum%20System%20GPRS%20GSM%20GPS/A7%20Minimum%20System%20GPRS%20GSM%20GPS_1-1000x750.JPG "Ai A7 GSM/GPRS/GPS")
A7lib is a trivial library for the arduino family used with an [AI A7 GPRS/GSM/GPS shield](https://www.ai-thinker.com/product/gprs).

The library allows you to send/receive SMS with simple OOP methods. In addition, it also allows you to receive or make phone calls, and grab current location.

Install:
========

	$ `cd ~/Arduino/libraries`
	$ `git clone https://github.com/mibs510/A7lib`

A7
========
If you're going to use the A7 with `SoftwareSerial.h` make sure to set the A7 with a baudrate of 57600.
Doing otherwise will result with modem replys containing nasty characters hindering some of `A7lib`'s function calls.
Take a look [here](https://mcmillan.website/dealing-arduino-nano-3-serial-ports/) for further reading.


Credits
========
A7lib is adopted by Connor McMillan (connor@mcmillan.website).
The orignial library was intially written as [`SerialSMS`](https://github.com/meirm/SerialGSM) by Meir Michanie (meirm@riunx.com) and released under the GPL3. So big thanks to Meir.
