# Debian

---

[TOC]

---

## Installation

### Install the required packages

```
sudo apt update
sudo apt install -y build-essential cmake git pkg-config libusb-1.0-0-dev libtecla-dev
```
> Returns
```powershell
Hit:1 http://deb.debian.org/debian bookworm InRelease
Hit:2 http://deb.debian.org/debian-security bookworm-security InRelease
Hit:3 http://deb.debian.org/debian bookworm-updates InRelease
Hit:4 http://archive.raspberrypi.com/debian bookworm InRelease
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
All packages are up to date.
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
build-essential is already the newest version (12.9).
cmake is already the newest version (3.25.1-1).
git is already the newest version (1:2.39.5-0+deb12u1).
pkg-config is already the newest version (1.8.1-1).
The following package was automatically installed and is no longer required:
  libwlroots12
Use 'sudo apt autoremove' to remove it.
The following additional packages will be installed:
  libtecla1 libusb-1.0-doc
The following NEW packages will be installed:
  libtecla-dev libtecla1 libusb-1.0-0-dev libusb-1.0-doc
0 upgraded, 4 newly installed, 0 to remove and 0 not upgraded.
Need to get 573 kB of archives.
After this operation, 2,790 kB of additional disk space will be used.
Get:1 http://deb.debian.org/debian bookworm/main arm64 libtecla1 arm64 1.6.3-3+b1 [102 kB]
Get:2 http://deb.debian.org/debian bookworm/main arm64 libtecla-dev arm64 1.6.3-3+b1 [195 kB]
Get:3 http://deb.debian.org/debian bookworm/main arm64 libusb-1.0-0-dev arm64 2:1.0.26-1 [83.0 kB]
Get:4 http://deb.debian.org/debian bookworm/main arm64 libusb-1.0-doc all 2:1.0.26-1 [193 kB]
Fetched 573 kB in 0s (1,252 kB/s)     
Selecting previously unselected package libtecla1:arm64.
(Reading database ... 153222 files and directories currently installed.)
Preparing to unpack .../libtecla1_1.6.3-3+b1_arm64.deb ...
Unpacking libtecla1:arm64 (1.6.3-3+b1) ...
Selecting previously unselected package libtecla-dev:arm64.
Preparing to unpack .../libtecla-dev_1.6.3-3+b1_arm64.deb ...
Unpacking libtecla-dev:arm64 (1.6.3-3+b1) ...
Selecting previously unselected package libusb-1.0-0-dev:arm64.
Preparing to unpack .../libusb-1.0-0-dev_2%3a1.0.26-1_arm64.deb ...
Unpacking libusb-1.0-0-dev:arm64 (2:1.0.26-1) ...
Selecting previously unselected package libusb-1.0-doc.
Preparing to unpack .../libusb-1.0-doc_2%3a1.0.26-1_all.deb ...
Unpacking libusb-1.0-doc (2:1.0.26-1) ...
Setting up libtecla1:arm64 (1.6.3-3+b1) ...
Setting up libusb-1.0-doc (2:1.0.26-1) ...
Setting up libusb-1.0-0-dev:arm64 (2:1.0.26-1) ...
Setting up libtecla-dev:arm64 (1.6.3-3+b1) ...
Processing triggers for man-db (2.11.2-2) ...
Processing triggers for libc-bin (2.36-9+rpt2+deb12u9) ...
```

### [Building bladeRF libraries and tools from source](https://github.com/Nuand/bladeRF/wiki/Getting-Started%3A-Linux#building-bladerf-libraries-and-tools-from-source)

#### a). Clone the **bladeRF** source code

```
git clone https://github.com/Nuand/bladeRF.git
```
> Returns
```powershell
cd bladeRF
Cloning into 'bladeRF'...
remote: Enumerating objects: 38356, done.
remote: Counting objects: 100% (1889/1889), done.
remote: Compressing objects: 100% (308/308), done.
remote: Total 38356 (delta 1682), reused 1634 (delta 1577), pack-reused 36467 (from 3)
Receiving objects: 100% (38356/38356), 13.24 MiB | 4.90 MiB/s, done.
Resolving deltas: 100% (24111/24111), done.
```

#### b). Compile and Install the binaries

```
mkdir build
cd build
cmake ..
make -j$(nproc)
sudo make install
sudo ldconfig
```
> Returns
```powershell
-- The CXX compiler identification is GNU 12.2.0
-- The C compiler identification is GNU 12.2.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Build type not specified: defaulting to a release build.
-- Found PkgConfig: /usr/bin/pkg-config (found version "1.8.1") 
-- Checking libusb version...
-- libusb version: 1.0.26

-- Looking for clock_gettime in c
-- Looking for clock_gettime in c - found
-- libad936x: Configuring build...
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE  
-- libad936x: Attempting to initialize missing submodule
-- Found Git: /usr/bin/git (found version "2.39.5") 
Submodule 'thirdparty/analogdevicesinc/no-OS' (https://github.com/analogdevicesinc/no-OS/) registered for path 'thirdparty/analogdevicesinc/no-OS'
Cloning into '/home/pi/Developer/bladeRF/thirdparty/analogdevicesinc/no-OS'...
Submodule path 'thirdparty/analogdevicesinc/no-OS': checked out '0bba46e6f6f75785a65d425ece37d0a04daf6157'
-- FindPatcher: found /usr/bin/patch
-- libad936x: Applying patch: 0001-0bba46e-nuand-modifications.patch
-- libad936x: Applying patch: 0002-0bba46e-pr-561.patch
-- libad936x: Applying patch: 0003-0bba46e-pr-573.patch
-- libad936x: Applying patch: 0004-0bba46e-pr-598.patch
-- libad936x: Applying patch: 0005-0bba46e-rm-ad9361_parse_fir.patch
-- libad936x: Applying patch: 0006-0bba46e-compilefix.patch
-- libad936x: Build configured
-- Checking libc version...
-- libc version: 2.36

--
-- libbladeRF version: 2.5.1-git-fe3304d7
-- libbladeRF_test: enabled
-- nuand bladeRF udev rules will be installed to '/etc/udev/rules.d' upon running 'make install'
--
-- Found libtecla: /usr/include, /usr/lib/aarch64-linux-gnu/libtecla.so
-- libedit not found.
-- Reusing existing interactive help for bladeRF-cli
-- libtecla OFF TRUE
-- libedit OFF FALSE
-- libtecla support enabled
-- Not building man page for bladeRF-cli. (BUILD_BLADERF_CLI_DOCUMENTATION is OFF)
-- Configured to build bladeRF-cli version: 1.9.0-git-fe3304d7
-- Looking for wsyncup in /usr/lib/aarch64-linux-gnu/libcurses.so
-- Looking for wsyncup in /usr/lib/aarch64-linux-gnu/libcurses.so - found
-- Looking for cbreak in /usr/lib/aarch64-linux-gnu/libncursesw.so
-- Looking for cbreak in /usr/lib/aarch64-linux-gnu/libncursesw.so - found
-- Looking for nodelay in /usr/lib/aarch64-linux-gnu/libncursesw.so
-- Looking for nodelay in /usr/lib/aarch64-linux-gnu/libncursesw.so - found
-- Found Curses: /usr/lib/aarch64-linux-gnu/libncursesw.so  
-- Skipping FX3 firmware build
-- Configuring done
-- Generating done
-- Build files have been written to: /home/pi/Developer/bladeRF/build
[  0%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_fir_filter.dir/__/__/__/common/src/conversions.c.o
[  0%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_crc32.dir/src/crc32.c.o
[  0%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_prng.dir/src/prng.c.o
[  0%] Building C object host/common/thirdparty/ad936x/CMakeFiles/ad936x.dir/ad9361.c.o
[  1%] Linking C executable ../../../output/bladeRF-fsk_test_crc32
[  2%] Linking C executable ../../../output/bladeRF-fsk_test_prng
[  2%] Built target bladeRF-fsk_test_crc32
[  2%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_correlator.dir/src/correlator.c.o
[  2%] Built target bladeRF-fsk_test_prng
[  2%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_pnorm.dir/src/pnorm.c.o
[  2%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_fir_filter.dir/src/fir_filter.c.o
[  2%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_correlator.dir/src/utils.c.o
[  3%] Linking C executable ../../../output/bladeRF-fsk_test_pnorm
[  3%] Built target bladeRF-fsk_test_pnorm
[  4%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_fir_filter.dir/src/utils.c.o
[  4%] Building C object host/common/thirdparty/ad936x/CMakeFiles/ad936x.dir/ad9361_api.c.o
[  5%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_correlator.dir/src/fsk.c.o
[  5%] Linking C executable ../../../output/bladeRF-fsk_test_fir_filter
[  5%] Built target bladeRF-fsk_test_fir_filter
[  6%] Building C object host/common/thirdparty/ad936x/CMakeFiles/ad936x.dir/ad9361_conv.c.o
[  6%] Linking C executable ../../../output/bladeRF-fsk_test_correlator
[  6%] Built target bladeRF-fsk_test_correlator
[  6%] Building C object host/common/thirdparty/ad936x/CMakeFiles/ad936x.dir/util.c.o
[  7%] Building C object host/common/thirdparty/ad936x/CMakeFiles/ad936x.dir/__/__/__/__/thirdparty/analogdevicesinc/no-OS_local/platform_bladerf2/platform.c.o
[  7%] Building C object host/common/thirdparty/ad936x/CMakeFiles/ad936x.dir/__/__/__/__/thirdparty/analogdevicesinc/no-OS_local/platform_bladerf2/adc_core.c.o
[  7%] Building C object host/common/thirdparty/ad936x/CMakeFiles/ad936x.dir/__/__/__/__/thirdparty/analogdevicesinc/no-OS_local/platform_bladerf2/dac_core.c.o
[  7%] Building C object host/common/thirdparty/ad936x/CMakeFiles/ad936x.dir/__/__/__/__/fpga_common/src/ad936x_params.c.o
[  8%] Linking C static library ../../../output/libad936x.a
[  8%] Built target ad936x
[  9%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/driver/spi_flash.c.o
[  9%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/backend/backend.c.o
[  9%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/driver/fpga_trigger.c.o
[  9%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/driver/fx3_fw.c.o
[  9%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/driver/si5338.c.o
[ 10%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/driver/ina219.c.o
[ 10%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/driver/dac161s055.c.o
[ 10%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/driver/smb_clock.c.o
[ 10%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf1/capabilities.c.o
[ 11%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf1/bladerf1.c.o
[ 11%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf1/compatibility.c.o
[ 12%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf1/calibration.c.o
[ 12%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf1/flash.c.o
[ 13%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf1/image.c.o
[ 13%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/board.c.o
[ 13%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/expansion/xb100.c.o
[ 14%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/expansion/xb200.c.o
[ 14%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/expansion/xb300.c.o
[ 14%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/streaming/async.c.o
[ 15%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/streaming/sync.c.o
[ 15%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/streaming/sync_worker.c.o
[ 15%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/init_fini.c.o
[ 16%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/helpers/timeout.c.o
[ 16%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/helpers/file.c.o
[ 16%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/helpers/version.c.o
[ 17%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/helpers/wallclock.c.o
[ 17%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/helpers/interleave.c.o
[ 17%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/helpers/configfile.c.o
[ 18%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/devinfo.c.o
[ 18%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/device_calibration.c.o
[ 18%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/bladerf.c.o
[ 19%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/__/__/common/src/sha256.c.o
[ 19%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/__/__/common/src/conversions.c.o
[ 19%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/__/__/common/src/log.c.o
[ 20%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/__/__/__/fpga_common/src/lms.c.o
[ 20%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/__/__/__/fpga_common/src/band_select.c.o
[ 20%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/__/__/common/src/parse.c.o
[ 21%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/__/__/common/src/range.c.o
[ 21%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/__/__/__/fpga_common/src/ad936x_helpers.c.o
[ 21%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/__/__/__/fpga_common/src/bladerf2_common.c.o
[ 22%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf2/bladerf2.c.o
[ 22%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf2/capabilities.c.o
[ 22%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf2/common.c.o
[ 23%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf2/compatibility.c.o
[ 23%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf2/rfic_fpga.c.o
[ 23%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf2/rfic_host.c.o
[ 24%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/backend/usb/nios_access.c.o
[ 24%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/backend/usb/nios_legacy_access.c.o
[ 24%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/backend/usb/usb.c.o
[ 25%] Building C object host/libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/backend/usb/libusb.c.o
[ 25%] Linking C shared library ../../output/libbladeRF.so
[ 25%] Built target libbladerf_shared
[ 25%] Building C object host/libraries/libbladeRF_test/test_clock_select/CMakeFiles/libbladeRF_test_clock_select.dir/src/main.c.o
[ 25%] Building C object host/libraries/libbladeRF_test/test_c/CMakeFiles/libbladeRF_test_c.dir/main.c.o
[ 26%] Building C object host/libraries/libbladeRF_test/test_async/CMakeFiles/libbladeRF_test_async.dir/main.c.o
[ 25%] Building C object host/libraries/libbladeRF_test/test_bootloader_recovery/CMakeFiles/libbladeRF_test_bootloader_recovery.dir/main.c.o
[ 27%] Linking C executable ../../../output/libbladeRF_test_c
[ 27%] Linking C executable ../../../output/libbladeRF_test_clock_select
[ 28%] Building C object host/libraries/libbladeRF_test/test_bootloader_recovery/CMakeFiles/libbladeRF_test_bootloader_recovery.dir/__/__/__/common/src/conversions.c.o
[ 28%] Built target libbladeRF_test_c
[ 29%] Building CXX object host/libraries/libbladeRF_test/test_cpp/CMakeFiles/libbladeRF_test_cpp.dir/main.cpp.o
[ 29%] Linking C executable ../../../output/libbladeRF_test_async
[ 29%] Built target libbladeRF_test_clock_select
[ 29%] Building C object host/libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/main.c.o
[ 29%] Built target libbladeRF_test_async
[ 30%] Building C object host/libraries/libbladeRF_test/test_freq_hop/CMakeFiles/libbladeRF_test_freq_hop.dir/main.c.o
[ 31%] Building C object host/libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_bandwidth.c.o
[ 31%] Building C object host/libraries/libbladeRF_test/test_freq_hop/CMakeFiles/libbladeRF_test_freq_hop.dir/__/common/src/test_common.c.o
[ 31%] Linking C executable ../../../output/libbladeRF_test_bootloader_recovery
[ 31%] Building C object host/libraries/libbladeRF_test/test_freq_hop/CMakeFiles/libbladeRF_test_freq_hop.dir/__/__/__/common/src/conversions.c.o
[ 31%] Built target libbladeRF_test_bootloader_recovery
[ 32%] Building C object host/libraries/libbladeRF_test/test_freq_hop/CMakeFiles/libbladeRF_test_freq_hop.dir/__/__/__/common/src/devcfg.c.o
[ 32%] Building C object host/libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_correction.c.o
[ 32%] Linking CXX executable ../../../output/libbladeRF_test_cpp
[ 32%] Building C object host/libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_enable_module.c.o
[ 33%] Building C object host/libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_frequency.c.o
[ 33%] Building C object host/libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_gain.c.o
[ 33%] Linking C executable ../../../output/libbladeRF_test_freq_hop
[ 33%] Built target libbladeRF_test_cpp
[ 33%] Building C object host/libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_loopback.c.o
[ 33%] Built target libbladeRF_test_freq_hop
[ 33%] Building C object host/libraries/libbladeRF_test/test_fw_check/CMakeFiles/libbladeRF_test_fw_check.dir/src/main.c.o
[ 34%] Building C object host/libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_rx_mux.c.o
[ 35%] Building C object host/libraries/libbladeRF_test/test_fw_check/CMakeFiles/libbladeRF_test_fw_check.dir/__/__/libbladeRF/src/driver/fx3_fw.c.o
[ 35%] Building C object host/libraries/libbladeRF_test/test_fw_check/CMakeFiles/libbladeRF_test_fw_check.dir/__/__/libbladeRF/src/helpers/file.c.o
[ 35%] Building C object host/libraries/libbladeRF_test/test_fw_check/CMakeFiles/libbladeRF_test_fw_check.dir/__/__/__/common/src/log.c.o
[ 35%] Building C object host/libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_sampling.c.o
[ 36%] Building C object host/libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_lpf_mode.c.o
[ 36%] Building C object host/libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_samplerate.c.o
[ 37%] Linking C executable ../../../output/libbladeRF_test_fw_check
[ 37%] Building C object host/libraries/libbladeRF_test/test_open/CMakeFiles/libbladeRF_test_open.dir/src/main.c.o
[ 37%] Built target libbladeRF_test_fw_check
[ 37%] Building C object host/libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_threads.c.o
[ 37%] Building C object host/libraries/libbladeRF_test/test_parse/CMakeFiles/libbladeRF_test_parse.dir/src/main.c.o
[ 38%] Building C object host/libraries/libbladeRF_test/test_parse/CMakeFiles/libbladeRF_test_parse.dir/__/__/__/common/src/conversions.c.o
[ 39%] Building C object host/libraries/libbladeRF_test/test_open/CMakeFiles/libbladeRF_test_open.dir/__/__/__/common/src/conversions.c.o
[ 40%] Building C object host/libraries/libbladeRF_test/test_peripheral_timing/CMakeFiles/libbladeRF_test_peripheral_timing.dir/main.c.o
[ 41%] Building C object host/libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_xb200.c.o
[ 41%] Building C object host/libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/__/__/__/common/src/conversions.c.o
[ 41%] Building C object host/libraries/libbladeRF_test/test_peripheral_timing/CMakeFiles/libbladeRF_test_peripheral_timing.dir/__/common/src/test_common.c.o
[ 41%] Building C object host/libraries/libbladeRF_test/test_parse/CMakeFiles/libbladeRF_test_parse.dir/__/__/__/common/src/parse.c.o
[ 41%] Linking C executable ../../../output/libbladeRF_test_open
[ 42%] Building C object host/libraries/libbladeRF_test/test_peripheral_timing/CMakeFiles/libbladeRF_test_peripheral_timing.dir/__/__/__/common/src/conversions.c.o
[ 42%] Built target libbladeRF_test_open
[ 43%] Building C object host/libraries/libbladeRF_test/test_gain_compare/CMakeFiles/libbladeRF_test_gain_compare.dir/src/main.c.o
[ 43%] Linking C executable ../../../output/libbladeRF_test_ctrl
[ 43%] Linking C executable ../../../output/libbladeRF_test_parse
[ 43%] Building C object host/libraries/libbladeRF_test/test_gain_compare/CMakeFiles/libbladeRF_test_gain_compare.dir/__/__/__/common/src/conversions.c.o
[ 43%] Built target libbladeRF_test_ctrl
[ 43%] Built target libbladeRF_test_parse
[ 43%] Building C object host/libraries/libbladeRF_test/test_gain_calibration/CMakeFiles/libbladeRF_test_gain_calibration.dir/src/main.c.o
[ 44%] Building C object host/libraries/libbladeRF_test/test_repeater/CMakeFiles/libbladeRF_test_repeater.dir/src/main.c.o
[ 44%] Linking C executable ../../../output/libbladeRF_test_peripheral_timing
[ 44%] Building C object host/libraries/libbladeRF_test/test_gain_calibration/CMakeFiles/libbladeRF_test_gain_calibration.dir/src/file_operations.c.o
[ 44%] Building C object host/libraries/libbladeRF_test/test_repeater/CMakeFiles/libbladeRF_test_repeater.dir/src/repeater.c.o
[ 44%] Built target libbladeRF_test_peripheral_timing
[ 44%] Building C object host/libraries/libbladeRF_test/test_quick_retune/CMakeFiles/libbladeRF_test_quick_retune.dir/main.c.o
[ 45%] Building C object host/libraries/libbladeRF_test/test_gain_calibration/CMakeFiles/libbladeRF_test_gain_calibration.dir/__/__/libbladeRF/src/helpers/file.c.o
[ 45%] Building C object host/libraries/libbladeRF_test/test_gain_compare/CMakeFiles/libbladeRF_test_gain_compare.dir/__/__/__/common/src/log.c.o
[ 46%] Linking C executable ../../../output/libbladeRF_test_gain_compare
[ 47%] Building C object host/libraries/libbladeRF_test/test_quick_retune/CMakeFiles/libbladeRF_test_quick_retune.dir/__/__/__/common/src/conversions.c.o
[ 47%] Building C object host/libraries/libbladeRF_test/test_gain_calibration/CMakeFiles/libbladeRF_test_gain_calibration.dir/__/__/__/common/src/log.c.o
[ 47%] Building C object host/libraries/libbladeRF_test/test_repeater/CMakeFiles/libbladeRF_test_repeater.dir/__/__/__/common/src/conversions.c.o
[ 47%] Built target libbladeRF_test_gain_compare
[ 47%] Linking C executable ../../../output/libbladeRF_test_gain_calibration
[ 47%] Building C object host/libraries/libbladeRF_test/test_repeated_stream/CMakeFiles/libbladeRF_test_repeated_stream.dir/src/main.c.o
[ 47%] Built target libbladeRF_test_gain_calibration
[ 47%] Building C object host/libraries/libbladeRF_test/test_rx_discont/CMakeFiles/libbladeRF_test_rx_discont.dir/src/main.c.o
[ 48%] Building C object host/libraries/libbladeRF_test/test_repeated_stream/CMakeFiles/libbladeRF_test_repeated_stream.dir/__/__/__/common/src/conversions.c.o
[ 48%] Linking C executable ../../../output/libbladeRF_test_quick_retune
[ 48%] Built target libbladeRF_test_quick_retune
[ 48%] Building C object host/libraries/libbladeRF_test/test_rx_discont/CMakeFiles/libbladeRF_test_rx_discont.dir/__/__/__/common/src/conversions.c.o
[ 49%] Building C object host/libraries/libbladeRF_test/test_scheduled_retune/CMakeFiles/libbladeRF_test_scheduled_retune.dir/src/main.c.o
[ 50%] Linking C executable ../../../output/libbladeRF_test_repeater
[ 50%] Built target libbladeRF_test_repeater
[ 50%] Building C object host/libraries/libbladeRF_test/test_repeated_stream/CMakeFiles/libbladeRF_test_repeated_stream.dir/__/__/__/common/src/devcfg.c.o
[ 50%] Building C object host/libraries/libbladeRF_test/test_scheduled_retune/CMakeFiles/libbladeRF_test_scheduled_retune.dir/src/hop_set.c.o
[ 51%] Building C object host/libraries/libbladeRF_test/test_sync/CMakeFiles/libbladeRF_test_sync.dir/src/main.c.o
[ 51%] Building C object host/libraries/libbladeRF_test/test_scheduled_retune/CMakeFiles/libbladeRF_test_scheduled_retune.dir/__/common/src/test_common.c.o
[ 52%] Linking C executable ../../../output/libbladeRF_test_rx_discont
[ 52%] Linking C executable ../../../output/libbladeRF_test_repeated_stream
[ 53%] Building C object host/libraries/libbladeRF_test/test_scheduled_retune/CMakeFiles/libbladeRF_test_scheduled_retune.dir/__/__/__/common/src/devcfg.c.o
[ 53%] Built target libbladeRF_test_repeated_stream
[ 53%] Built target libbladeRF_test_rx_discont
[ 53%] Building C object host/libraries/libbladeRF_test/test_sync/CMakeFiles/libbladeRF_test_sync.dir/src/test.c.o
[ 53%] Building C object host/libraries/libbladeRF_test/test_scheduled_retune/CMakeFiles/libbladeRF_test_scheduled_retune.dir/__/__/__/common/src/conversions.c.o
[ 53%] Building C object host/libraries/libbladeRF_test/test_sync/CMakeFiles/libbladeRF_test_sync.dir/__/__/__/common/src/conversions.c.o
[ 53%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/main.c.o
[ 54%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_tx_pulse.dir/src/test_tx_pulse.c.o
[ 54%] Linking C executable ../../../output/libbladeRF_test_scheduled_retune
[ 55%] Building C object host/libraries/libbladeRF_test/test_sync/CMakeFiles/libbladeRF_test_sync.dir/__/__/__/common/src/log.c.o
[ 56%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_timestamps.c.o
[ 56%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_tx_pulse.dir/src/test_timestamps.c.o
[ 56%] Built target libbladeRF_test_scheduled_retune
[ 56%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_tx_pulse.dir/src/loopback.c.o
[ 56%] Linking C executable ../../../output/libbladeRF_test_sync
[ 56%] Built target libbladeRF_test_sync
[ 56%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_rx_gaps.c.o
[ 57%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_txrx_hwloop.dir/src/test_txrx_hwloop.c.o
[ 57%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_rx_scheduled.c.o
[ 58%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_tx_onoff.c.o
[ 59%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_tx_pulse.dir/__/common/src/test_common.c.o
[ 59%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_tx_pulse.dir/__/__/__/common/src/conversions.c.o
[ 59%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_tx_onoff_nowsched.c.o
[ 59%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_tx_gmsk_bursts.c.o
[ 59%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_txrx_hwloop.dir/src/test_timestamps.c.o
[ 59%] Building C object host/libraries/libbladeRF_test/test_tune_timing/CMakeFiles/libbladeRF_test_tune_timing.dir/main.c.o
[ 60%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_loopback_onoff.c.o
[ 60%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_txrx_hwloop.dir/src/loopback.c.o
[ 61%] Building C object host/libraries/libbladeRF_test/test_tune_timing/CMakeFiles/libbladeRF_test_tune_timing.dir/__/common/src/test_common.c.o
[ 61%] Linking C executable ../../../output/libbladeRF_test_tx_pulse
[ 61%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_loopback_onoff_zp.c.o
[ 61%] Building C object host/libraries/libbladeRF_test/test_tune_timing/CMakeFiles/libbladeRF_test_tune_timing.dir/__/__/__/common/src/conversions.c.o
[ 61%] Built target libbladeRF_test_tx_pulse
[ 62%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_loopback_onoff_rf.c.o
[ 62%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/loopback.c.o
[ 62%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_format_mismatch.c.o
[ 63%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_txrx_hwloop.dir/__/common/src/test_common.c.o
[ 63%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_txrx_hwloop.dir/__/__/__/common/src/conversions.c.o
[ 63%] Linking C executable ../../../output/libbladeRF_test_tune_timing
[ 64%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_readback.c.o
[ 65%] Building C object host/libraries/libbladeRF_test/test_unused_sync/CMakeFiles/libbladeRF_test_unused_sync.dir/main.c.o
[ 65%] Built target libbladeRF_test_tune_timing
[ 65%] Linking C executable ../../../output/libbladeRF_test_unused_sync
[ 65%] Building C object host/libraries/libbladeRF_test/test_version/CMakeFiles/libbladeRF_test_version.dir/src/main.c.o
[ 65%] Built target libbladeRF_test_unused_sync
[ 66%] Linking C executable ../../../output/libbladeRF_test_version
[ 66%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_print_timestamps.c.o
[ 66%] Linking C executable ../../../output/libbladeRF_test_txrx_hwloop
[ 67%] Building C object host/libraries/libbladeRF_test/test_digital_loopback/CMakeFiles/libbladeRF_test_digital_loopback.dir/src/main.c.o
[ 67%] Built target libbladeRF_test_version
[ 67%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/__/common/src/test_common.c.o
[ 67%] Built target libbladeRF_test_txrx_hwloop
[ 67%] Building C object host/libraries/libbladeRF_test/test_interleaver/CMakeFiles/libbladeRF_test_interleaver.dir/src/main.c.o
[ 68%] Building C object host/libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/__/__/__/common/src/conversions.c.o
[ 68%] Building C object host/libraries/libbladeRF_test/test_interleaver/CMakeFiles/libbladeRF_test_interleaver.dir/__/__/libbladeRF/src/helpers/interleave.c.o
[ 69%] Building C object host/libraries/libbladeRF_test/test_interleaver/CMakeFiles/libbladeRF_test_interleaver.dir/__/__/__/common/src/log.c.o
[ 69%] Building C object host/libraries/libbladeRF_test/test_digital_loopback/CMakeFiles/libbladeRF_test_digital_loopback.dir/__/__/__/common/src/conversions.c.o
[ 69%] Linking C executable ../../../output/libbladeRF_test_interleaver
[ 69%] Building C object host/libraries/libbladeRF_test/test_rx_meta/CMakeFiles/libbladeRF_test_rx_meta.dir/src/main.c.o
[ 69%] Built target libbladeRF_test_interleaver
[ 69%] Building C object host/libraries/libbladeRF_test/test_fpga_load/CMakeFiles/libbladeRF_test_fpga_load.dir/src/main.c.o
[ 69%] Linking C executable ../../../output/libbladeRF_test_timestamps
[ 69%] Linking C executable ../../../output/libbladeRF_test_fpga_load
[ 69%] Built target libbladeRF_test_timestamps
[ 69%] Building C object host/libraries/libbladeRF_test/test_digital_loopback/CMakeFiles/libbladeRF_test_digital_loopback.dir/__/__/__/common/src/log.c.o
[ 69%] Built target libbladeRF_test_fpga_load
[ 69%] Building C object host/libraries/libbladeRF_test/test_rx_meta/CMakeFiles/libbladeRF_test_rx_meta.dir/src/include.c.o
[ 70%] Building C object host/libraries/libbladeRF_test/test_rx_meta/CMakeFiles/libbladeRF_test_rx_meta.dir/__/__/__/common/src/log.c.o
[ 70%] Generating src/cmd/doc/cmd_help.h
[ 70%] Building C object host/libraries/libbladeRF_test/test_rx_meta/CMakeFiles/libbladeRF_test_rx_meta.dir/__/__/__/common/src/conversions.c.o
[ 70%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/main.c.o
[ 71%] Linking C executable ../../../output/libbladeRF_test_digital_loopback
[ 72%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/common.c.o
[ 72%] Built target libbladeRF_test_digital_loopback
[ 72%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/calibrate.c.o
[ 72%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/cmd.c.o
[ 72%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/bladeRF-fsk.c.o
[ 72%] Linking C executable ../../../output/libbladeRF_test_rx_meta
[ 72%] Built target libbladeRF_test_rx_meta
[ 73%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/config.c.o
[ 73%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/__/__/__/common/src/conversions.c.o
[ 73%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/radio_config.c.o
[ 74%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/erase.c.o
[ 74%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/flash_backup.c.o
[ 74%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/fir_filter.c.o
[ 74%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/flash_image.c.o
[ 74%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/radio_config.c.o
[ 75%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/fsk.c.o
[ 76%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/fir_filter.c.o
[ 77%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/flash_init_cal.c.o
[ 77%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/flash_restore.c.o
[ 77%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/prng.c.o
[ 77%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/fsk.c.o
[ 77%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/fw_log.c.o
[ 78%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/generate.c.o
[ 78%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/crc32.c.o
[ 78%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_config.dir/src/config.c.o
[ 79%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/phy.c.o
[ 79%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/prng.c.o
[ 80%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/phy.c.o
[ 80%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_config.dir/__/__/__/common/src/conversions.c.o
[ 80%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/info.c.o
[ 80%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/jump_boot.c.o
[ 81%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/lms_reg_info.c.o
[ 81%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/link.c.o
[ 81%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/crc32.c.o
[ 82%] Linking C executable ../../../output/bladeRF-fsk_test_config
[ 82%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/link.c.o
[ 82%] Built target bladeRF-fsk_test_config
[ 82%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/test_suite.c.o
[ 83%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/utils.c.o
[ 83%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/pnorm.c.o
[ 84%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/utils.c.o
[ 84%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/correlator.c.o
[ 84%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/load.c.o
[ 84%] Building C object host/utilities/bladeRF-power/CMakeFiles/bladeRF-power.dir/__/__/common/src/conversions.c.o
[ 85%] Linking C executable ../../../output/bladeRF-fsk_test_suite
[ 85%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/pnorm.c.o
[ 85%] Built target bladeRF-fsk_test_suite
[ 85%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/mimo.c.o
[ 85%] Building C object host/utilities/bladeRF-power/CMakeFiles/bladeRF-power.dir/src/init.c.o
[ 85%] Building C object host/utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/correlator.c.o
[ 86%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/open.c.o
[ 87%] Building C object host/utilities/bladeRF-power/CMakeFiles/bladeRF-power.dir/src/helpers.c.o
[ 88%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/peek.c.o
[ 88%] Linking C executable ../../../output/bladeRF-fsk
[ 88%] Building C object host/utilities/bladeRF-power/CMakeFiles/bladeRF-power.dir/src/window.c.o
[ 88%] Built target bladeRF-fsk
[ 88%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/peekpoke.c.o
[ 88%] Building C object host/utilities/bladeRF-power/CMakeFiles/bladeRF-power.dir/src/filter.c.o
[ 88%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/printset.c.o
[ 89%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/poke.c.o
[ 90%] Building C object host/utilities/bladeRF-power/CMakeFiles/bladeRF-power.dir/src/text.c.o
[ 90%] Building C object host/common/test/dc_calibration/CMakeFiles/test_dc_calibration.dir/src/main.c.o
[ 90%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/printset_hardware.c.o
[ 90%] Building C object host/utilities/bladeRF-power/CMakeFiles/bladeRF-power.dir/src/main.c.o
[ 91%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/printset_impl.c.o
[ 92%] Building C object host/common/test/dc_calibration/CMakeFiles/test_dc_calibration.dir/__/__/src/conversions.c.o
[ 92%] Linking C executable ../../output/bladeRF-power
[ 92%] Building C object host/common/test/dc_calibration/CMakeFiles/test_dc_calibration.dir/__/__/src/dc_calibration.c.o
[ 92%] Built target bladeRF-power
[ 92%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/printset_xb.c.o
[ 92%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/probe.c.o
[ 93%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/recover.c.o
[ 93%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/rx.c.o
[ 93%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/rxtx.c.o
[ 94%] Linking C executable ../../../output/test_dc_calibration
[ 95%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/trigger.c.o
[ 95%] Built target test_dc_calibration
[ 95%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/tx.c.o
[ 95%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/version.c.o
[ 96%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/xb.c.o
[ 96%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/xb100.c.o
[ 97%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/xb200.c.o
[ 97%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/xb300.c.o
[ 97%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/input/input.c.o
[ 98%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/input/script.c.o
[ 98%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/__/__/common/src/conversions.c.o
[ 98%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/__/__/common/src/dc_calibration.c.o
[ 99%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/__/__/common/src/log.c.o
[ 99%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/__/__/common/src/str_queue.c.o
[ 99%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/__/__/common/src/parse.c.o
[100%] Building C object host/utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/input/tecla.c.o
[100%] Linking C executable ../../output/bladeRF-cli
[100%] Built target bladeRF-cli
[  3%] Built target ad936x
[ 20%] Built target libbladerf_shared
[ 21%] Built target libbladeRF_test_async
[ 22%] Built target libbladeRF_test_bootloader_recovery
[ 23%] Built target libbladeRF_test_c
[ 23%] Built target libbladeRF_test_clock_select
[ 24%] Built target libbladeRF_test_cpp
[ 29%] Built target libbladeRF_test_ctrl
[ 31%] Built target libbladeRF_test_freq_hop
[ 33%] Built target libbladeRF_test_fw_check
[ 34%] Built target libbladeRF_test_open
[ 35%] Built target libbladeRF_test_parse
[ 37%] Built target libbladeRF_test_peripheral_timing
[ 39%] Built target libbladeRF_test_gain_compare
[ 40%] Built target libbladeRF_test_gain_calibration
[ 42%] Built target libbladeRF_test_repeater
[ 43%] Built target libbladeRF_test_quick_retune
[ 44%] Built target libbladeRF_test_repeated_stream
[ 45%] Built target libbladeRF_test_rx_discont
[ 47%] Built target libbladeRF_test_scheduled_retune
[ 49%] Built target libbladeRF_test_sync
[ 55%] Built target libbladeRF_test_timestamps
[ 57%] Built target libbladeRF_test_tx_pulse
[ 59%] Built target libbladeRF_test_txrx_hwloop
[ 60%] Built target libbladeRF_test_tune_timing
[ 61%] Built target libbladeRF_test_unused_sync
[ 62%] Built target libbladeRF_test_version
[ 64%] Built target libbladeRF_test_digital_loopback
[ 65%] Built target libbladeRF_test_interleaver
[ 66%] Built target libbladeRF_test_rx_meta
[ 66%] Built target libbladeRF_test_fpga_load
[ 81%] Built target bladeRF-cli
[ 86%] Built target bladeRF-fsk
[ 90%] Built target bladeRF-fsk_test_suite
[ 91%] Built target bladeRF-fsk_test_config
[ 92%] Built target bladeRF-fsk_test_crc32
[ 93%] Built target bladeRF-fsk_test_prng
[ 94%] Built target bladeRF-fsk_test_fir_filter
[ 95%] Built target bladeRF-fsk_test_correlator
[ 96%] Built target bladeRF-fsk_test_pnorm
[ 98%] Built target bladeRF-power
[100%] Built target test_dc_calibration
Install the project...
-- Install configuration: ""
-- Installing: /usr/local/lib/pkgconfig/libbladeRF.pc
-- Installing: /usr/local/lib/libbladeRF.so.2
-- Installing: /usr/local/lib/libbladeRF.so
-- Installing: /usr/local/share/cmake/bladeRF/bladeRFConfig.cmake
-- Installing: /usr/local/share/cmake/bladeRF/bladeRFConfigVersion.cmake
-- Installing: /usr/local/include/libbladeRF.h
-- Installing: /usr/local/include/bladeRF1.h
-- Installing: /usr/local/include/bladeRF2.h
-- Installing: /etc/udev/rules.d/88-nuand-bladerf1.rules
-- Installing: /etc/udev/rules.d/88-nuand-bladerf2.rules
-- Installing: /etc/udev/rules.d/88-nuand-bootloader.rules
-- Installing: /usr/local/bin/bladeRF-cli
-- Set runtime path of "/usr/local/bin/bladeRF-cli" to ""
-- Installing: /usr/local/bin/bladeRF-fsk
-- Set runtime path of "/usr/local/bin/bladeRF-fsk" to ""
-- Installing: /usr/local/bin/bladeRF-power
-- Set runtime path of "/usr/local/bin/bladeRF-power" to ""
```

