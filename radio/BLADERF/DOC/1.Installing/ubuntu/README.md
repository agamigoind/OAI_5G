# Ubuntu

---

[TOC]

---

To install the bladeRF-cli on Ubuntu, follow these steps:

1. Install Prerequisites

Ensure that your system is up-to-date and has the necessary dependencies:

```
sudo apt update
sudo apt upgrade
sudo apt install build-essential cmake libusb-1.0-0-dev libglib2.0-dev
```
> Returns
```powershell
Hit:1 http://ports.ubuntu.com/ubuntu-ports noble InRelease
Get:2 http://ports.ubuntu.com/ubuntu-ports noble-updates InRelease [126 kB]
Hit:3 https://ppa.launchpadcontent.net/ettusresearch/uhd/ubuntu noble InRelease           
Get:4 http://ports.ubuntu.com/ubuntu-ports noble-backports InRelease [126 kB]
Get:5 http://ports.ubuntu.com/ubuntu-ports noble-security InRelease [126 kB]
Get:6 http://ports.ubuntu.com/ubuntu-ports noble-updates/main arm64 Components [149 kB]
Get:7 http://ports.ubuntu.com/ubuntu-ports noble-updates/universe arm64 Components [308 kB]
Get:8 http://ports.ubuntu.com/ubuntu-ports noble-updates/restricted arm64 Components [212 B]
Get:9 http://ports.ubuntu.com/ubuntu-ports noble-updates/multiverse arm64 Components [212 B]
Get:10 http://ports.ubuntu.com/ubuntu-ports noble-backports/main arm64 Components [208 B]
Get:11 http://ports.ubuntu.com/ubuntu-ports noble-backports/universe arm64 Components [11.7 kB]
Get:12 http://ports.ubuntu.com/ubuntu-ports noble-backports/restricted arm64 Components [216 B]
Get:13 http://ports.ubuntu.com/ubuntu-ports noble-backports/multiverse arm64 Components [212 B]
Get:14 http://ports.ubuntu.com/ubuntu-ports noble-security/main arm64 Components [5692 B]
Get:15 http://ports.ubuntu.com/ubuntu-ports noble-security/universe arm64 Components [52.0 kB]
Get:16 http://ports.ubuntu.com/ubuntu-ports noble-security/restricted arm64 Components [212 B]
Get:17 http://ports.ubuntu.com/ubuntu-ports noble-security/multiverse arm64 Components [212 B]
Fetched 906 kB in 2s (393 kB/s)                                        
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
3 packages can be upgraded. Run 'apt list --upgradable' to see them.
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
Calculating upgrade... Done
The following upgrades have been deferred due to phasing:
  python3-distupgrade ubuntu-release-upgrader-core ubuntu-release-upgrader-gtk
0 upgraded, 0 newly installed, 0 to remove and 3 not upgraded.
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
build-essential is already the newest version (12.10ubuntu1).
cmake is already the newest version (3.28.3-1build7).
The following additional packages will be installed:
  libblkid-dev libffi-dev libgirepository-2.0-0 libglib2.0-dev-bin libmount-dev libpcre2-16-0 libpcre2-dev libpcre2-posix3
  libselinux1-dev libsepol-dev libusb-1.0-doc python3-packaging uuid-dev
Suggested packages:
  gir1.2-glib-2.0-dev libglib2.0-doc libxml2-utils
The following NEW packages will be installed:
  libblkid-dev libffi-dev libgirepository-2.0-0 libglib2.0-dev libglib2.0-dev-bin libmount-dev libpcre2-16-0 libpcre2-dev
  libpcre2-posix3 libselinux1-dev libsepol-dev libusb-1.0-0-dev libusb-1.0-doc python3-packaging uuid-dev
0 upgraded, 15 newly installed, 0 to remove and 3 not upgraded.
Need to get 4272 kB of archives.
After this operation, 22.9 MB of additional disk space will be used.
Do you want to continue? [Y/n] y
Get:1 http://ports.ubuntu.com/ubuntu-ports noble-updates/main arm64 libgirepository-2.0-0 arm64 2.80.0-6ubuntu3.2 [71.5 kB]
Get:2 http://ports.ubuntu.com/ubuntu-ports noble/main arm64 libffi-dev arm64 3.4.6-1build1 [59.5 kB]
Get:3 http://ports.ubuntu.com/ubuntu-ports noble/main arm64 python3-packaging all 24.0-1 [41.1 kB]
Get:4 http://ports.ubuntu.com/ubuntu-ports noble-updates/main arm64 libglib2.0-dev-bin arm64 2.80.0-6ubuntu3.2 [138 kB]
Get:5 http://ports.ubuntu.com/ubuntu-ports noble-updates/main arm64 uuid-dev arm64 2.39.3-9ubuntu6.1 [34.8 kB]
Get:6 http://ports.ubuntu.com/ubuntu-ports noble-updates/main arm64 libblkid-dev arm64 2.39.3-9ubuntu6.1 [219 kB]
Get:7 http://ports.ubuntu.com/ubuntu-ports noble/main arm64 libsepol-dev arm64 3.5-2build1 [395 kB]
Get:8 http://ports.ubuntu.com/ubuntu-ports noble/main arm64 libpcre2-16-0 arm64 10.42-4ubuntu2 [196 kB]
Get:9 http://ports.ubuntu.com/ubuntu-ports noble/main arm64 libpcre2-posix3 arm64 10.42-4ubuntu2 [6642 B]
Get:10 http://ports.ubuntu.com/ubuntu-ports noble/main arm64 libpcre2-dev arm64 10.42-4ubuntu2 [680 kB]
Get:11 http://ports.ubuntu.com/ubuntu-ports noble/main arm64 libselinux1-dev arm64 3.5-2ubuntu2 [172 kB]
Get:12 http://ports.ubuntu.com/ubuntu-ports noble-updates/main arm64 libmount-dev arm64 2.39.3-9ubuntu6.1 [14.9 kB]
Get:13 http://ports.ubuntu.com/ubuntu-ports noble-updates/main arm64 libglib2.0-dev arm64 2.80.0-6ubuntu3.2 [1964 kB]
Get:14 http://ports.ubuntu.com/ubuntu-ports noble/main arm64 libusb-1.0-0-dev arm64 2:1.0.27-1 [80.5 kB]
Get:15 http://ports.ubuntu.com/ubuntu-ports noble/main arm64 libusb-1.0-doc all 2:1.0.27-1 [199 kB]
Fetched 4272 kB in 6s (766 kB/s)     
Selecting previously unselected package libgirepository-2.0-0:arm64.
(Reading database ... 138836 files and directories currently installed.)
Preparing to unpack .../00-libgirepository-2.0-0_2.80.0-6ubuntu3.2_arm64.deb ...
Unpacking libgirepository-2.0-0:arm64 (2.80.0-6ubuntu3.2) ...
Selecting previously unselected package libffi-dev:arm64.
Preparing to unpack .../01-libffi-dev_3.4.6-1build1_arm64.deb ...
Unpacking libffi-dev:arm64 (3.4.6-1build1) ...
Selecting previously unselected package python3-packaging.
Preparing to unpack .../02-python3-packaging_24.0-1_all.deb ...
Unpacking python3-packaging (24.0-1) ...
Selecting previously unselected package libglib2.0-dev-bin.
Preparing to unpack .../03-libglib2.0-dev-bin_2.80.0-6ubuntu3.2_arm64.deb ...
Unpacking libglib2.0-dev-bin (2.80.0-6ubuntu3.2) ...
Selecting previously unselected package uuid-dev:arm64.
Preparing to unpack .../04-uuid-dev_2.39.3-9ubuntu6.1_arm64.deb ...
Unpacking uuid-dev:arm64 (2.39.3-9ubuntu6.1) ...
Selecting previously unselected package libblkid-dev:arm64.
Preparing to unpack .../05-libblkid-dev_2.39.3-9ubuntu6.1_arm64.deb ...
Unpacking libblkid-dev:arm64 (2.39.3-9ubuntu6.1) ...
Selecting previously unselected package libsepol-dev:arm64.
Preparing to unpack .../06-libsepol-dev_3.5-2build1_arm64.deb ...
Unpacking libsepol-dev:arm64 (3.5-2build1) ...
Selecting previously unselected package libpcre2-16-0:arm64.
Preparing to unpack .../07-libpcre2-16-0_10.42-4ubuntu2_arm64.deb ...
Unpacking libpcre2-16-0:arm64 (10.42-4ubuntu2) ...
Selecting previously unselected package libpcre2-posix3:arm64.
Preparing to unpack .../08-libpcre2-posix3_10.42-4ubuntu2_arm64.deb ...
Unpacking libpcre2-posix3:arm64 (10.42-4ubuntu2) ...
Selecting previously unselected package libpcre2-dev:arm64.
Preparing to unpack .../09-libpcre2-dev_10.42-4ubuntu2_arm64.deb ...
Unpacking libpcre2-dev:arm64 (10.42-4ubuntu2) ...
Selecting previously unselected package libselinux1-dev:arm64.
Preparing to unpack .../10-libselinux1-dev_3.5-2ubuntu2_arm64.deb ...
Unpacking libselinux1-dev:arm64 (3.5-2ubuntu2) ...
Selecting previously unselected package libmount-dev:arm64.
Preparing to unpack .../11-libmount-dev_2.39.3-9ubuntu6.1_arm64.deb ...
Unpacking libmount-dev:arm64 (2.39.3-9ubuntu6.1) ...
Selecting previously unselected package libglib2.0-dev:arm64.
Preparing to unpack .../12-libglib2.0-dev_2.80.0-6ubuntu3.2_arm64.deb ...
Unpacking libglib2.0-dev:arm64 (2.80.0-6ubuntu3.2) ...
Selecting previously unselected package libusb-1.0-0-dev:arm64.
Preparing to unpack .../13-libusb-1.0-0-dev_2%3a1.0.27-1_arm64.deb ...
Unpacking libusb-1.0-0-dev:arm64 (2:1.0.27-1) ...
Selecting previously unselected package libusb-1.0-doc.
Preparing to unpack .../14-libusb-1.0-doc_2%3a1.0.27-1_all.deb ...
Unpacking libusb-1.0-doc (2:1.0.27-1) ...
Setting up libgirepository-2.0-0:arm64 (2.80.0-6ubuntu3.2) ...
Setting up libusb-1.0-doc (2:1.0.27-1) ...
Setting up libusb-1.0-0-dev:arm64 (2:1.0.27-1) ...
Setting up libffi-dev:arm64 (3.4.6-1build1) ...
Setting up libpcre2-16-0:arm64 (10.42-4ubuntu2) ...
Setting up python3-packaging (24.0-1) ...
Setting up uuid-dev:arm64 (2.39.3-9ubuntu6.1) ...
Setting up libsepol-dev:arm64 (3.5-2build1) ...
Setting up libpcre2-posix3:arm64 (10.42-4ubuntu2) ...
Setting up libglib2.0-dev-bin (2.80.0-6ubuntu3.2) ...
Setting up libblkid-dev:arm64 (2.39.3-9ubuntu6.1) ...
Setting up libpcre2-dev:arm64 (10.42-4ubuntu2) ...
Setting up libselinux1-dev:arm64 (3.5-2ubuntu2) ...
Setting up libmount-dev:arm64 (2.39.3-9ubuntu6.1) ...
Setting up libglib2.0-dev:arm64 (2.80.0-6ubuntu3.2) ...
Processing triggers for libc-bin (2.39-0ubuntu8.3) ...
Processing triggers for man-db (2.12.0-4build2) ...
Processing triggers for libglib2.0-0t64:arm64 (2.80.0-6ubuntu3.2) ...
Processing triggers for install-info (7.1-3build2) ...
```

2. Install bladeRF from Ubuntu Repositories

You can use the default Ubuntu repositories to install bladeRF-cli (though the version might not be the latest):

```
sudo apt install bladerf
```
> Returns
```powershell
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
The following additional packages will be installed:
  libbladerf2 libtecla1t64
Suggested packages:
  bladerf-firmware bladerf-fpga
The following NEW packages will be installed:
  bladerf libbladerf2 libtecla1t64
0 upgraded, 3 newly installed, 0 to remove and 3 not upgraded.
Need to get 383 kB of archives.
After this operation, 1260 kB of additional disk space will be used.
Do you want to continue? [Y/n] y
Get:1 http://ports.ubuntu.com/ubuntu-ports noble/universe arm64 libbladerf2 arm64 0.2023.02-4build1 [184 kB]
Get:2 http://ports.ubuntu.com/ubuntu-ports noble/universe arm64 libtecla1t64 arm64 1.6.3-3.1build1 [68.8 kB]
Get:3 http://ports.ubuntu.com/ubuntu-ports noble/universe arm64 bladerf arm64 0.2023.02-4build1 [130 kB]
Fetched 383 kB in 2s (226 kB/s)
Selecting previously unselected package libbladerf2:arm64.
(Reading database ... 139813 files and directories currently installed.)
Preparing to unpack .../libbladerf2_0.2023.02-4build1_arm64.deb ...
Unpacking libbladerf2:arm64 (0.2023.02-4build1) ...
Selecting previously unselected package libtecla1t64:arm64.
Preparing to unpack .../libtecla1t64_1.6.3-3.1build1_arm64.deb ...
Unpacking libtecla1t64:arm64 (1.6.3-3.1build1) ...
Selecting previously unselected package bladerf.
Preparing to unpack .../bladerf_0.2023.02-4build1_arm64.deb ...
Unpacking bladerf (0.2023.02-4build1) ...
Setting up libbladerf2:arm64 (0.2023.02-4build1) ...
No diversion 'diversion of /lib/udev/rules.d/88-nuand-bladerf1.rules to /lib/udev/rules.d/88-nuand-bladerf1.rules.usr-is-merged
 by usr-is-merged', none removed.
No diversion 'diversion of /lib/udev/rules.d/88-nuand-bladerf2.rules to /lib/udev/rules.d/88-nuand-bladerf2.rules.usr-is-merged
 by usr-is-merged', none removed.
No diversion 'diversion of /lib/udev/rules.d/88-nuand-bootloader.rules to /lib/udev/rules.d/88-nuand-bootloader.rules.usr-is-me
rged by usr-is-merged', none removed.
Setting up libtecla1t64:arm64 (1.6.3-3.1build1) ...
Setting up bladerf (0.2023.02-4build1) ...
Processing triggers for man-db (2.12.0-4build2) ...
Processing triggers for libc-bin (2.39-0ubuntu8.3) ...
```

- [ ] Check the installation:

```
bladeRF-cli --interactive
```
> Returns
```powershell
[WARNING @ host/libraries/libbladeRF/src/backend/usb/libusb.c:529] Found a bladeRF via VID/PID, but could not open it due to insufficient permissions.

No bladeRF device(s) available.

If one is attached, ensure it is not in use by another program
and that the current user has permission to access it.

bladeRF> version

  bladeRF-cli version:        1.9.0-0.2023.02-4build1
  libbladeRF version:         2.5.0-0.2023.02-4build1

  Device version information unavailable: No device attached.
```

```
sudo bladeRF-cli --interactive
```
> Returns
```powershell
[WARNING @ host/libraries/libbladeRF/src/board/bladerf2/bladerf2.c:461] FPGA bitstream file not found.
[WARNING @ host/libraries/libbladeRF/src/board/bladerf2/bladerf2.c:462] Skipping further initialization...
bladeRF> version

  bladeRF-cli version:        1.9.0-0.2023.02-4build1
  libbladeRF version:         2.5.0-0.2023.02-4build1

  Firmware version:           2.4.0-git-a3d5c55f
  FPGA version:               Unknown (FPGA not loaded)

bladeRF> 
```

4. Install via PPA (Optional)

Some Ubuntu versions may have a Personal Package Archive (PPA) available for BladeRF:

```
sudo add-apt-repository ppa:nuand/bladerf
sudo apt update
sudo apt install bladerf
```

5. Update Firmware and FPGA

Download the latest firmware and FPGA images from Nuand’s GitHub releases and flash them:

```
bladeRF-cli -l <path_to_firmware.img>
bladeRF-cli -L <path_to_fpga.img>
```

Summary
-	Use sudo apt install bladerf for the easiest installation.
-	Build from source if you need the latest version.
-	Verify the installation with bladeRF-cli -i.
