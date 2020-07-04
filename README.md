About nullpo-mod
================
Nullpo-mod is a linux kernel module that implements a character device file
that causes SEGV when it is about be either read or written.

Install
=======

 1. Build the kernel module.

    ```
    $ make
    ```

 2. Load the built module.

    ```
    $ sudo insmod nullpo.ko
    ```


Usage
=====

Reading more than or equal to 1 byte from /dev/nullpo will cause segmentation
fault.

```sh
$ cat /den/nullpo
Segmentation fault (core dumped)
```

Writing more than or equal to 1 byte to /dev/nullpo will also cause
segmentation fault.

```sh
$ echo foo | tee /dev/nullpo
Segmentation fault (core dumped)
```

read()/write() call for /dev/nullpo with 0 bytes reading/writing will be
success with return value zero.

```sh
$ echo -n | tee /dev/nullpo
$ echo $?
0
```

*Warning*: redirecing to/from /dev/nullpo may cause a shell crash.

Supplement
==========

To install the module to the system
------------------------------------------

You can install the module to the system and load from there.

```
$ sudo make install
$ sudo depmod
$ modprobe nullpo
```

I cannot open /dev/nullpo due to permission error
-------------------------------------------------

Depending to the system settings, /dev/nullpo cannot be opend except by root
user. To grant permission for other user to open the device file, you have
to create appropriate udev rules. There is a sample udev configuration file
`99-nullpo.rules` in the source directory. Putting that file into /etc/udev/rules.d
grant permission for all users to read/write /dev/nullpo from next module loading.
