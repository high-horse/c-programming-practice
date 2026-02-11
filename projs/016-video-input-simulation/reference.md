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

#### [linux kernel module reference](https://man7.org/linux/man-pages/man8/kmod.8.html)
```bash
#include <libkmod.h>
#include <stdio.h>

int main() {
    struct kmod_ctx *ctx = kmod_new(NULL, NULL);
    struct kmod_module *mod;

    if (!ctx) return 1;

    if (kmod_module_new_from_name(ctx, "v4l2loopback", &mod) < 0)
        return 1;

    const char *opts =
        "devices=1 video_nr=10 card_label=VirtualCam exclusive_caps=1";

    if (kmod_module_insert_module(mod, 0, opts) < 0) {
        perror("kmod insert");
        return 1;
    }

    kmod_module_unref(mod);
    kmod_unref(ctx);
    
    // on exit
    // kmod_module_remove_module(mod, 0); or system("modprobe -r v4l2loopback");
    // or run := sudo v4l2loopback-ctl delete /dev/video7
    return 0;
}

```
