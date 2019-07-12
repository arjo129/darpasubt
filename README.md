# net/batman-doc
Document all things related to batman.

## Network Bridges
A network bridge is a virtual cable, that connects two network interfaces.

By default, OpenWrt puts `eth0` and `wlan0` on the same bridge. This allows
connections on `wlan0` to connect over `eth0`. If `eth0` is connected to the
internet, then `wlan0` is connected to the internet.
```
root@OpenWrt:~# brctl show
bridge name     bridge id               STP enabled     interfaces
br-lan          7fff.b827eb551e8a       no              eth0
                                                        wlan0
```
