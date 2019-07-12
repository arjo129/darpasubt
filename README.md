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

## SSH over IPv6
It is possible to SSH from device A into an IPv6 device B, if batman is running 
on device B. I suspect this is because a layer 2 protocol needs to be running 
in order for SSH over IPv6 to work.

The SSH command is:
```
ssh -6 <user>@<device B address>%<device A interface>
```
