thermal-halt

Tiny daemon to monitor raspberry-pi thermal_zone0 and halt in the event it breaches a configurable threshold.

'make' will produce a binary which can be manually run if desired

'make install' configures a systemd service called 'thermal-halt' with sane defaults in the configuration environment variables.

configuration:
   TEMP_OFF    - environment variable for threshold
   CHECK_COUNT - number of violations of threshold before halting
   CHECK_WAIT  - seconds to wait between measurements

Tested on a rpi4b
