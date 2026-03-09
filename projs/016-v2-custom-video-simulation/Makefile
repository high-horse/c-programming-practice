

build:
	gcc -std=c2x -Wall -Wextra main.c -o a.out  -lavformat -lavcodec -lswscale -lavutil


run:
	./a.out

mount-camera:
	sudo modprobe v4l2loopback video_nr=9 card_label="my dummy videocam" 

unmount-camera:
	sudo sudo modprobe -r v4l2loopback

list-devices:
	v4l2-ctl --list-devices

test-video:
	ffplay /dev/video9

test-camera-formats:
	v4l2-ctl --device /dev/video9 --list-formats-ext
