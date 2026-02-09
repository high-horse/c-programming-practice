#### install `Video4Linux2(v4l2) util`
`v4l2` is primary linux kernel api for real time video 
capture and read

command reference is in [V4L2Loopback](https://wiki.archlinux.org/title/V4l2loopback)
```bash
sudo apt install v4l-utils
# test if installed
v4l2-ctl --list-devices

# test subsquent video this way
sudo ffplay /dev/video0
```
