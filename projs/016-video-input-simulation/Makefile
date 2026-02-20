.PHONY: build

build:
	gcc -std=c2x  -D_POSIX_C_SOURCE=200809L -Wall -Wextra src/* -o bin/a.out

run:
	./bin/a.out


create-device:
	sudo modprobe v4l2loopback

create-device-args:
	sudo modprobe v4l2loopback video_nr=9 card_label="My Fake Webcam" exclusive_caps=1
# 	sudo modprobe v4l2loopback video_nr=2 card_label="VirtualCam" exclusive_caps=1

create-device-args-2:
	v4l2-ctl --device /dev/video9 --set-fmt-video=width=1920,height=1080,pixelformat=YUYV

list-device:
	v4l2-ctl --list-devices


list-device-format:
	v4l2-ctl --device /dev/video2 --list-formats-ext

delete-device:
	sudo modprobe -r v4l2loopback

