# hotspot_controller
A set of tools to enable and disable a specific NetworkManager connection/device
at certain hours through nmcli and systemctl.

## How to use
  The installed tools act as a wrapper of nmcli, they allow to manipulate a
  specific connection and device through systemctl services and timers.
  
  * hotspot.up@.service enable the provided NetworkManager connection.
  * hotspot.down@.service disable the provided NetworkManager device.
  * The associated timers launch triggers the above services at a given hour 
every day (up at 0700 down at 0000).

  The script `hotspot.sh` ease the handling of the services.
  Commands are:
  * `start` execute the service `hotspot.up` and start the timers `hotspot.up` and `hotspot.down`.
  * `stop` execute the service `hotspot.down`.
  * `disable` execute the service `hotspot.down` and stop the timers `hotspot.up` and `hotspot.down`.

## Building
The two binaries are compiled as follow:

``` bash
gcc src/hotspot-button.c -o build/hotspot-button -lwiringPi
gcc src/hotspot-led.c -o build/hotspot-led -lwiringPi
```

## Installing
To install the tools run the following commands: 
 ``` bash
  # Build the two binaries.
  gcc src/hotspot-button.c -o build/hotspot-button -lwiringPi
  gcc src/hotspot-led.c -o build/hotspot-led -lwiringPi

  # Copy the content of the build and dist directories to /usr/local/bin.
  sudo cp build/* dist/* /usr/local/bin/

  # Copy the content of the service direcotry to /etc/systemctl/system.
  sudo cp service/* /etc/systemd/system/

  # Reload the systemctl daemon
  sudo systemctl daemon-reload
 ```

