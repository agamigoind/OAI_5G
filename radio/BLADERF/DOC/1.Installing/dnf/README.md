# dnf (Dandified YUM)

---

[TOC]

---

## Installation

### Install the required packages

```
sudo dnf install gcc gcc-c++ make cmake libusb1-devel ncurses-devel
```
> Returns
```powershell
Updating Subscription Management repositories.
<very long ...>
  ncurses-libs-6.2-10.20210508.el9.x86_64             

Complete!
```

### [Building bladeRF libraries and tools from source](https://github.com/Nuand/bladeRF/wiki/Getting-Started%3A-Linux#building-bladerf-libraries-and-tools-from-source)

```
git clone https://github.com/Nuand/bladeRF.git ./bladeRF
```
> Returns
```powershell
Cloning into './bladeRF'...
remote: Enumerating objects: 38305, done.
remote: Counting objects: 100% (4712/4712), done.
remote: Compressing objects: 100% (1576/1576), done.
remote: Total 38305 (delta 3186), reused 4436 (delta 3016), pack-reused 33593 (from 1)
Receiving objects: 100% (38305/38305), 13.02 MiB | 21.58 MiB/s, done.
Resolving deltas: 100% (24168/24168), done.
```

```
cd host && mkdir build && cd build
```

```
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DINSTALL_UDEV_RULES=ON ../
```
>Returns
```powershell
-- The C compiler identification is GNU 11.4.1
-- Detecting C compiler ABI info
<long ...>
-- Found Curses: /usr/lib64/libncursesw.so  
-- Configuring done (40.0s)
-- Generating done (0.1s)
-- Build files have been written to: /home/bricer/Developer/bladeRF/host/build
```

### Groups


```
$ groups
```
> bricer

```
sudo groupadd bladerf
```

```
sudo usermod -a -G bladerf ${USER}
```

### Now log out and log back in...

```
$ groups
```
> bricer bladerf

### Compile && install

```
make && sudo make install && sudo ldconfig
```
> Returns
```powershell
[  0%] Building C object common/thirdparty/ad936x/CMakeFiles/ad936x.dir/ad9361.c.o
[  0%] Building C object common/thirdparty/ad936x/CMakeFiles/ad936x.dir/ad9361_api.c.o
[  1%] Building C object common/thirdparty/ad936x/CMakeFiles/ad936x.dir/ad9361_conv.c.o
[  1%] Building C object common/thirdparty/ad936x/CMakeFiles/ad936x.dir/util.c.o
[  1%] Building C object common/thirdparty/ad936x/CMakeFiles/ad936x.dir/home/bricer/Developer/bladeRF/thirdparty/analogdevicesinc/no-OS_local/platform_bladerf2/platform.c.o
[  2%] Building C object common/thirdparty/ad936x/CMakeFiles/ad936x.dir/home/bricer/Developer/bladeRF/thirdparty/analogdevicesinc/no-OS_local/platform_bladerf2/adc_core.c.o
[  2%] Building C object common/thirdparty/ad936x/CMakeFiles/ad936x.dir/home/bricer/Developer/bladeRF/thirdparty/analogdevicesinc/no-OS_local/platform_bladerf2/dac_core.c.o
[  2%] Building C object common/thirdparty/ad936x/CMakeFiles/ad936x.dir/home/bricer/Developer/bladeRF/fpga_common/src/ad936x_params.c.o
[  3%] Linking C static library ../../../output/libad936x.a
[  3%] Built target ad936x
[  3%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/backend/backend.c.o
[  3%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/driver/spi_flash.c.o
[  4%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/driver/fx3_fw.c.o
[  4%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/driver/fpga_trigger.c.o
[  4%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/driver/si5338.c.o
[  5%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/driver/ina219.c.o
[  5%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/driver/dac161s055.c.o
[  5%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/driver/smb_clock.c.o
[  6%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf1/bladerf1.c.o
[  6%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf1/capabilities.c.o
[  6%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf1/compatibility.c.o
[  7%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf1/calibration.c.o
[  7%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf1/flash.c.o
[  8%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf1/image.c.o
[  8%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/board.c.o
[  8%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/expansion/xb100.c.o
[  9%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/expansion/xb200.c.o
[  9%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/expansion/xb300.c.o
[  9%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/streaming/async.c.o
[ 10%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/streaming/sync.c.o
[ 10%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/streaming/sync_worker.c.o
[ 10%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/init_fini.c.o
[ 11%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/helpers/timeout.c.o
[ 11%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/helpers/file.c.o
[ 11%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/helpers/version.c.o
[ 12%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/helpers/wallclock.c.o
[ 12%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/helpers/interleave.c.o
[ 12%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/helpers/configfile.c.o
[ 13%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/devinfo.c.o
[ 13%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/device_calibration.c.o
[ 13%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/bladerf.c.o
[ 14%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/__/__/common/src/sha256.c.o
[ 14%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/__/__/common/src/conversions.c.o
[ 14%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/__/__/common/src/log.c.o
[ 15%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/home/bricer/Developer/bladeRF/fpga_common/src/lms.c.o
[ 15%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/home/bricer/Developer/bladeRF/fpga_common/src/band_select.c.o
[ 15%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/__/__/common/src/parse.c.o
[ 16%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/__/__/common/src/range.c.o
[ 16%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/home/bricer/Developer/bladeRF/fpga_common/src/ad936x_helpers.c.o
[ 16%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/home/bricer/Developer/bladeRF/fpga_common/src/bladerf2_common.c.o
[ 17%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf2/bladerf2.c.o
[ 17%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf2/capabilities.c.o
[ 17%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf2/common.c.o
[ 18%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf2/compatibility.c.o
[ 18%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf2/rfic_fpga.c.o
[ 18%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/board/bladerf2/rfic_host.c.o
[ 19%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/backend/usb/nios_access.c.o
[ 19%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/backend/usb/nios_legacy_access.c.o
[ 19%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/backend/usb/usb.c.o
[ 20%] Building C object libraries/libbladeRF/CMakeFiles/libbladerf_shared.dir/src/backend/usb/libusb.c.o
[ 20%] Linking C shared library ../../output/libbladeRF.so
[ 20%] Built target libbladerf_shared
[ 21%] Building C object libraries/libbladeRF_test/test_async/CMakeFiles/libbladeRF_test_async.dir/main.c.o
[ 21%] Linking C executable ../../../output/libbladeRF_test_async
[ 21%] Built target libbladeRF_test_async
[ 21%] Building C object libraries/libbladeRF_test/test_bootloader_recovery/CMakeFiles/libbladeRF_test_bootloader_recovery.dir/main.c.o
[ 22%] Building C object libraries/libbladeRF_test/test_bootloader_recovery/CMakeFiles/libbladeRF_test_bootloader_recovery.dir/__/__/__/common/src/conversions.c.o
[ 22%] Linking C executable ../../../output/libbladeRF_test_bootloader_recovery
[ 22%] Built target libbladeRF_test_bootloader_recovery
[ 22%] Building C object libraries/libbladeRF_test/test_c/CMakeFiles/libbladeRF_test_c.dir/main.c.o
[ 23%] Linking C executable ../../../output/libbladeRF_test_c
[ 23%] Built target libbladeRF_test_c
[ 23%] Building C object libraries/libbladeRF_test/test_clock_select/CMakeFiles/libbladeRF_test_clock_select.dir/src/main.c.o
[ 23%] Linking C executable ../../../output/libbladeRF_test_clock_select
[ 23%] Built target libbladeRF_test_clock_select
[ 24%] Building CXX object libraries/libbladeRF_test/test_cpp/CMakeFiles/libbladeRF_test_cpp.dir/main.cpp.o
[ 24%] Linking CXX executable ../../../output/libbladeRF_test_cpp
[ 24%] Built target libbladeRF_test_cpp
[ 24%] Building C object libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/main.c.o
[ 25%] Building C object libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_bandwidth.c.o
[ 25%] Building C object libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_correction.c.o
[ 25%] Building C object libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_enable_module.c.o
[ 26%] Building C object libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_frequency.c.o
[ 26%] Building C object libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_gain.c.o
[ 26%] Building C object libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_loopback.c.o
[ 27%] Building C object libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_rx_mux.c.o
[ 27%] Building C object libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_sampling.c.o
[ 28%] Building C object libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_lpf_mode.c.o
[ 28%] Building C object libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_samplerate.c.o
[ 28%] Building C object libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_threads.c.o
[ 29%] Building C object libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/src/test_xb200.c.o
[ 29%] Building C object libraries/libbladeRF_test/test_ctrl/CMakeFiles/libbladeRF_test_ctrl.dir/__/__/__/common/src/conversions.c.o
[ 29%] Linking C executable ../../../output/libbladeRF_test_ctrl
[ 29%] Built target libbladeRF_test_ctrl
[ 30%] Building C object libraries/libbladeRF_test/test_freq_hop/CMakeFiles/libbladeRF_test_freq_hop.dir/main.c.o
[ 30%] Building C object libraries/libbladeRF_test/test_freq_hop/CMakeFiles/libbladeRF_test_freq_hop.dir/__/common/src/test_common.c.o
[ 30%] Building C object libraries/libbladeRF_test/test_freq_hop/CMakeFiles/libbladeRF_test_freq_hop.dir/__/__/__/common/src/conversions.c.o
[ 31%] Building C object libraries/libbladeRF_test/test_freq_hop/CMakeFiles/libbladeRF_test_freq_hop.dir/__/__/__/common/src/devcfg.c.o
[ 31%] Linking C executable ../../../output/libbladeRF_test_freq_hop
[ 31%] Built target libbladeRF_test_freq_hop
[ 31%] Building C object libraries/libbladeRF_test/test_fw_check/CMakeFiles/libbladeRF_test_fw_check.dir/src/main.c.o
[ 32%] Building C object libraries/libbladeRF_test/test_fw_check/CMakeFiles/libbladeRF_test_fw_check.dir/__/__/libbladeRF/src/driver/fx3_fw.c.o
[ 32%] Building C object libraries/libbladeRF_test/test_fw_check/CMakeFiles/libbladeRF_test_fw_check.dir/__/__/libbladeRF/src/helpers/file.c.o
[ 32%] Building C object libraries/libbladeRF_test/test_fw_check/CMakeFiles/libbladeRF_test_fw_check.dir/__/__/__/common/src/log.c.o
[ 33%] Linking C executable ../../../output/libbladeRF_test_fw_check
[ 33%] Built target libbladeRF_test_fw_check
[ 33%] Building C object libraries/libbladeRF_test/test_open/CMakeFiles/libbladeRF_test_open.dir/src/main.c.o
[ 34%] Building C object libraries/libbladeRF_test/test_open/CMakeFiles/libbladeRF_test_open.dir/__/__/__/common/src/conversions.c.o
[ 34%] Linking C executable ../../../output/libbladeRF_test_open
[ 34%] Built target libbladeRF_test_open
[ 34%] Building C object libraries/libbladeRF_test/test_parse/CMakeFiles/libbladeRF_test_parse.dir/src/main.c.o
[ 35%] Building C object libraries/libbladeRF_test/test_parse/CMakeFiles/libbladeRF_test_parse.dir/__/__/__/common/src/conversions.c.o
[ 35%] Building C object libraries/libbladeRF_test/test_parse/CMakeFiles/libbladeRF_test_parse.dir/__/__/__/common/src/parse.c.o
[ 35%] Linking C executable ../../../output/libbladeRF_test_parse
[ 35%] Built target libbladeRF_test_parse
[ 36%] Building C object libraries/libbladeRF_test/test_peripheral_timing/CMakeFiles/libbladeRF_test_peripheral_timing.dir/main.c.o
[ 36%] Building C object libraries/libbladeRF_test/test_peripheral_timing/CMakeFiles/libbladeRF_test_peripheral_timing.dir/__/common/src/test_common.c.o
[ 37%] Building C object libraries/libbladeRF_test/test_peripheral_timing/CMakeFiles/libbladeRF_test_peripheral_timing.dir/__/__/__/common/src/conversions.c.o
[ 37%] Linking C executable ../../../output/libbladeRF_test_peripheral_timing
[ 37%] Built target libbladeRF_test_peripheral_timing
[ 38%] Building C object libraries/libbladeRF_test/test_gain_compare/CMakeFiles/libbladeRF_test_gain_compare.dir/src/main.c.o
[ 38%] Building C object libraries/libbladeRF_test/test_gain_compare/CMakeFiles/libbladeRF_test_gain_compare.dir/__/__/__/common/src/conversions.c.o
[ 38%] Building C object libraries/libbladeRF_test/test_gain_compare/CMakeFiles/libbladeRF_test_gain_compare.dir/__/__/__/common/src/log.c.o
[ 39%] Linking C executable ../../../output/libbladeRF_test_gain_compare
[ 39%] Built target libbladeRF_test_gain_compare
[ 39%] Building C object libraries/libbladeRF_test/test_gain_calibration/CMakeFiles/libbladeRF_test_gain_calibration.dir/src/main.c.o
[ 39%] Building C object libraries/libbladeRF_test/test_gain_calibration/CMakeFiles/libbladeRF_test_gain_calibration.dir/src/file_operations.c.o
[ 40%] Building C object libraries/libbladeRF_test/test_gain_calibration/CMakeFiles/libbladeRF_test_gain_calibration.dir/__/__/libbladeRF/src/helpers/file.c.o
[ 40%] Building C object libraries/libbladeRF_test/test_gain_calibration/CMakeFiles/libbladeRF_test_gain_calibration.dir/__/__/__/common/src/log.c.o
[ 40%] Linking C executable ../../../output/libbladeRF_test_gain_calibration
[ 40%] Built target libbladeRF_test_gain_calibration
[ 41%] Building C object libraries/libbladeRF_test/test_repeater/CMakeFiles/libbladeRF_test_repeater.dir/src/main.c.o
[ 41%] Building C object libraries/libbladeRF_test/test_repeater/CMakeFiles/libbladeRF_test_repeater.dir/src/repeater.c.o
[ 41%] Building C object libraries/libbladeRF_test/test_repeater/CMakeFiles/libbladeRF_test_repeater.dir/__/__/__/common/src/conversions.c.o
[ 42%] Linking C executable ../../../output/libbladeRF_test_repeater
[ 42%] Built target libbladeRF_test_repeater
[ 42%] Building C object libraries/libbladeRF_test/test_quick_retune/CMakeFiles/libbladeRF_test_quick_retune.dir/main.c.o
[ 43%] Building C object libraries/libbladeRF_test/test_quick_retune/CMakeFiles/libbladeRF_test_quick_retune.dir/__/__/__/common/src/conversions.c.o
[ 43%] Linking C executable ../../../output/libbladeRF_test_quick_retune
[ 43%] Built target libbladeRF_test_quick_retune
[ 43%] Building C object libraries/libbladeRF_test/test_repeated_stream/CMakeFiles/libbladeRF_test_repeated_stream.dir/src/main.c.o
[ 44%] Building C object libraries/libbladeRF_test/test_repeated_stream/CMakeFiles/libbladeRF_test_repeated_stream.dir/__/__/__/common/src/conversions.c.o
[ 44%] Building C object libraries/libbladeRF_test/test_repeated_stream/CMakeFiles/libbladeRF_test_repeated_stream.dir/__/__/__/common/src/devcfg.c.o
[ 44%] Linking C executable ../../../output/libbladeRF_test_repeated_stream
[ 44%] Built target libbladeRF_test_repeated_stream
[ 44%] Building C object libraries/libbladeRF_test/test_rx_discont/CMakeFiles/libbladeRF_test_rx_discont.dir/src/main.c.o
[ 44%] Building C object libraries/libbladeRF_test/test_rx_discont/CMakeFiles/libbladeRF_test_rx_discont.dir/__/__/__/common/src/conversions.c.o
[ 45%] Linking C executable ../../../output/libbladeRF_test_rx_discont
[ 45%] Built target libbladeRF_test_rx_discont
[ 46%] Building C object libraries/libbladeRF_test/test_scheduled_retune/CMakeFiles/libbladeRF_test_scheduled_retune.dir/src/main.c.o
[ 46%] Building C object libraries/libbladeRF_test/test_scheduled_retune/CMakeFiles/libbladeRF_test_scheduled_retune.dir/src/hop_set.c.o
[ 46%] Building C object libraries/libbladeRF_test/test_scheduled_retune/CMakeFiles/libbladeRF_test_scheduled_retune.dir/__/common/src/test_common.c.o
[ 47%] Building C object libraries/libbladeRF_test/test_scheduled_retune/CMakeFiles/libbladeRF_test_scheduled_retune.dir/__/__/__/common/src/devcfg.c.o
[ 47%] Building C object libraries/libbladeRF_test/test_scheduled_retune/CMakeFiles/libbladeRF_test_scheduled_retune.dir/__/__/__/common/src/conversions.c.o
[ 47%] Linking C executable ../../../output/libbladeRF_test_scheduled_retune
[ 47%] Built target libbladeRF_test_scheduled_retune
[ 48%] Building C object libraries/libbladeRF_test/test_sync/CMakeFiles/libbladeRF_test_sync.dir/src/main.c.o
[ 48%] Building C object libraries/libbladeRF_test/test_sync/CMakeFiles/libbladeRF_test_sync.dir/src/test.c.o
[ 48%] Building C object libraries/libbladeRF_test/test_sync/CMakeFiles/libbladeRF_test_sync.dir/__/__/__/common/src/conversions.c.o
[ 49%] Building C object libraries/libbladeRF_test/test_sync/CMakeFiles/libbladeRF_test_sync.dir/__/__/__/common/src/log.c.o
[ 49%] Linking C executable ../../../output/libbladeRF_test_sync
[ 49%] Built target libbladeRF_test_sync
[ 49%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/main.c.o
[ 50%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_timestamps.c.o
[ 50%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_rx_gaps.c.o
[ 50%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_rx_scheduled.c.o
[ 51%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_tx_onoff.c.o
[ 51%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_tx_onoff_nowsched.c.o
[ 51%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_tx_gmsk_bursts.c.o
[ 52%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_loopback_onoff.c.o
[ 52%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_loopback_onoff_zp.c.o
[ 53%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_loopback_onoff_rf.c.o
[ 53%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/loopback.c.o
[ 53%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_format_mismatch.c.o
[ 54%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_readback.c.o
[ 54%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/src/test_print_timestamps.c.o
[ 54%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/__/common/src/test_common.c.o
[ 55%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_timestamps.dir/__/__/__/common/src/conversions.c.o
[ 55%] Linking C executable ../../../output/libbladeRF_test_timestamps
[ 55%] Built target libbladeRF_test_timestamps
[ 56%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_tx_pulse.dir/src/test_tx_pulse.c.o
[ 56%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_tx_pulse.dir/src/test_timestamps.c.o
[ 56%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_tx_pulse.dir/src/loopback.c.o
[ 57%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_tx_pulse.dir/__/common/src/test_common.c.o
[ 57%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_tx_pulse.dir/__/__/__/common/src/conversions.c.o
[ 57%] Linking C executable ../../../output/libbladeRF_test_tx_pulse
[ 57%] Built target libbladeRF_test_tx_pulse
[ 58%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_txrx_hwloop.dir/src/test_txrx_hwloop.c.o
[ 58%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_txrx_hwloop.dir/src/test_timestamps.c.o
[ 58%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_txrx_hwloop.dir/src/loopback.c.o
[ 59%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_txrx_hwloop.dir/__/common/src/test_common.c.o
[ 59%] Building C object libraries/libbladeRF_test/test_timestamps/CMakeFiles/libbladeRF_test_txrx_hwloop.dir/__/__/__/common/src/conversions.c.o
[ 59%] Linking C executable ../../../output/libbladeRF_test_txrx_hwloop
[ 59%] Built target libbladeRF_test_txrx_hwloop
[ 59%] Building C object libraries/libbladeRF_test/test_tune_timing/CMakeFiles/libbladeRF_test_tune_timing.dir/main.c.o
[ 60%] Building C object libraries/libbladeRF_test/test_tune_timing/CMakeFiles/libbladeRF_test_tune_timing.dir/__/common/src/test_common.c.o
[ 60%] Building C object libraries/libbladeRF_test/test_tune_timing/CMakeFiles/libbladeRF_test_tune_timing.dir/__/__/__/common/src/conversions.c.o
[ 60%] Linking C executable ../../../output/libbladeRF_test_tune_timing
[ 60%] Built target libbladeRF_test_tune_timing
[ 61%] Building C object libraries/libbladeRF_test/test_unused_sync/CMakeFiles/libbladeRF_test_unused_sync.dir/main.c.o
[ 61%] Linking C executable ../../../output/libbladeRF_test_unused_sync
[ 61%] Built target libbladeRF_test_unused_sync
[ 61%] Building C object libraries/libbladeRF_test/test_version/CMakeFiles/libbladeRF_test_version.dir/src/main.c.o
[ 62%] Linking C executable ../../../output/libbladeRF_test_version
[ 62%] Built target libbladeRF_test_version
[ 63%] Building C object libraries/libbladeRF_test/test_digital_loopback/CMakeFiles/libbladeRF_test_digital_loopback.dir/src/main.c.o
[ 63%] Building C object libraries/libbladeRF_test/test_digital_loopback/CMakeFiles/libbladeRF_test_digital_loopback.dir/__/__/__/common/src/conversions.c.o
[ 63%] Building C object libraries/libbladeRF_test/test_digital_loopback/CMakeFiles/libbladeRF_test_digital_loopback.dir/__/__/__/common/src/log.c.o
[ 64%] Linking C executable ../../../output/libbladeRF_test_digital_loopback
[ 64%] Built target libbladeRF_test_digital_loopback
[ 64%] Building C object libraries/libbladeRF_test/test_interleaver/CMakeFiles/libbladeRF_test_interleaver.dir/src/main.c.o
[ 64%] Building C object libraries/libbladeRF_test/test_interleaver/CMakeFiles/libbladeRF_test_interleaver.dir/__/__/libbladeRF/src/helpers/interleave.c.o
[ 65%] Building C object libraries/libbladeRF_test/test_interleaver/CMakeFiles/libbladeRF_test_interleaver.dir/__/__/__/common/src/log.c.o
[ 65%] Linking C executable ../../../output/libbladeRF_test_interleaver
[ 65%] Built target libbladeRF_test_interleaver
[ 65%] Building C object libraries/libbladeRF_test/test_rx_meta/CMakeFiles/libbladeRF_test_rx_meta.dir/src/main.c.o
[ 65%] Building C object libraries/libbladeRF_test/test_rx_meta/CMakeFiles/libbladeRF_test_rx_meta.dir/src/include.c.o
[ 66%] Building C object libraries/libbladeRF_test/test_rx_meta/CMakeFiles/libbladeRF_test_rx_meta.dir/__/__/__/common/src/log.c.o
[ 66%] Building C object libraries/libbladeRF_test/test_rx_meta/CMakeFiles/libbladeRF_test_rx_meta.dir/__/__/__/common/src/conversions.c.o
[ 66%] Linking C executable ../../../output/libbladeRF_test_rx_meta
[ 66%] Built target libbladeRF_test_rx_meta
[ 66%] Building C object libraries/libbladeRF_test/test_fpga_load/CMakeFiles/libbladeRF_test_fpga_load.dir/src/main.c.o
[ 66%] Linking C executable ../../../output/libbladeRF_test_fpga_load
[ 66%] Built target libbladeRF_test_fpga_load
[ 66%] Generating src/cmd/doc/cmd_help.h
[ 66%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/main.c.o
[ 67%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/common.c.o
[ 67%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/calibrate.c.o
[ 67%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/cmd.c.o
[ 68%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/erase.c.o
[ 68%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/flash_backup.c.o
[ 68%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/flash_image.c.o
[ 69%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/flash_init_cal.c.o
[ 69%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/flash_restore.c.o
[ 69%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/fw_log.c.o
[ 70%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/generate.c.o
[ 70%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/info.c.o
[ 70%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/jump_boot.c.o
[ 71%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/lms_reg_info.c.o
[ 71%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/load.c.o
[ 71%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/mimo.c.o
[ 72%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/open.c.o
[ 72%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/peek.c.o
[ 72%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/peekpoke.c.o
[ 73%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/poke.c.o
[ 73%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/printset.c.o
[ 73%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/printset_hardware.c.o
[ 74%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/printset_impl.c.o
[ 74%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/printset_xb.c.o
[ 74%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/probe.c.o
[ 75%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/recover.c.o
[ 75%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/rx.c.o
[ 75%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/rxtx.c.o
[ 76%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/trigger.c.o
[ 76%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/tx.c.o
[ 76%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/version.c.o
[ 77%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/xb.c.o
[ 77%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/xb100.c.o
[ 78%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/xb200.c.o
[ 78%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/cmd/xb300.c.o
[ 78%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/input/input.c.o
[ 79%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/input/script.c.o
[ 79%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/__/__/common/src/conversions.c.o
[ 79%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/__/__/common/src/dc_calibration.c.o
[ 80%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/__/__/common/src/log.c.o
[ 80%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/__/__/common/src/str_queue.c.o
[ 80%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/__/__/common/src/parse.c.o
[ 81%] Building C object utilities/bladeRF-cli/CMakeFiles/bladeRF-cli.dir/src/input/fgets.c.o
[ 81%] Linking C executable ../../output/bladeRF-cli
[ 81%] Built target bladeRF-cli
[ 81%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/bladeRF-fsk.c.o
[ 82%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/config.c.o
[ 82%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/__/__/__/common/src/conversions.c.o
[ 82%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/radio_config.c.o
[ 83%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/fir_filter.c.o
[ 83%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/fsk.c.o
[ 83%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/prng.c.o
[ 84%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/phy.c.o
[ 84%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/crc32.c.o
[ 84%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/link.c.o
[ 85%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/utils.c.o
[ 85%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/pnorm.c.o
[ 85%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk.dir/src/correlator.c.o
[ 86%] Linking C executable ../../../output/bladeRF-fsk
[ 86%] Built target bladeRF-fsk
[ 86%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/radio_config.c.o
[ 86%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/fir_filter.c.o
[ 87%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/fsk.c.o
[ 87%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/prng.c.o
[ 87%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/crc32.c.o
[ 88%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/phy.c.o
[ 88%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/link.c.o
[ 88%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/test_suite.c.o
[ 89%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/utils.c.o
[ 89%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/pnorm.c.o
[ 89%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_suite.dir/src/correlator.c.o
[ 90%] Linking C executable ../../../output/bladeRF-fsk_test_suite
[ 90%] Built target bladeRF-fsk_test_suite
[ 90%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_config.dir/src/config.c.o
[ 90%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_config.dir/__/__/__/common/src/conversions.c.o
[ 91%] Linking C executable ../../../output/bladeRF-fsk_test_config
[ 91%] Built target bladeRF-fsk_test_config
[ 91%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_crc32.dir/src/crc32.c.o
[ 92%] Linking C executable ../../../output/bladeRF-fsk_test_crc32
[ 92%] Built target bladeRF-fsk_test_crc32
[ 92%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_prng.dir/src/prng.c.o
[ 93%] Linking C executable ../../../output/bladeRF-fsk_test_prng
[ 93%] Built target bladeRF-fsk_test_prng
[ 93%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_fir_filter.dir/__/__/__/common/src/conversions.c.o
[ 93%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_fir_filter.dir/src/fir_filter.c.o
[ 94%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_fir_filter.dir/src/utils.c.o
[ 94%] Linking C executable ../../../output/bladeRF-fsk_test_fir_filter
[ 94%] Built target bladeRF-fsk_test_fir_filter
[ 94%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_correlator.dir/src/correlator.c.o
[ 94%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_correlator.dir/src/utils.c.o
[ 95%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_correlator.dir/src/fsk.c.o
[ 95%] Linking C executable ../../../output/bladeRF-fsk_test_correlator
[ 95%] Built target bladeRF-fsk_test_correlator
[ 95%] Building C object utilities/bladeRF-fsk/c/CMakeFiles/bladeRF-fsk_test_pnorm.dir/src/pnorm.c.o
[ 96%] Linking C executable ../../../output/bladeRF-fsk_test_pnorm
[ 96%] Built target bladeRF-fsk_test_pnorm
[ 96%] Building C object utilities/bladeRF-power/CMakeFiles/bladeRF-power.dir/__/__/common/src/conversions.c.o
[ 96%] Building C object utilities/bladeRF-power/CMakeFiles/bladeRF-power.dir/src/init.c.o
[ 97%] Building C object utilities/bladeRF-power/CMakeFiles/bladeRF-power.dir/src/helpers.c.o
[ 97%] Building C object utilities/bladeRF-power/CMakeFiles/bladeRF-power.dir/src/window.c.o
[ 97%] Building C object utilities/bladeRF-power/CMakeFiles/bladeRF-power.dir/src/filter.c.o
[ 98%] Building C object utilities/bladeRF-power/CMakeFiles/bladeRF-power.dir/src/text.c.o
[ 98%] Building C object utilities/bladeRF-power/CMakeFiles/bladeRF-power.dir/src/main.c.o
[ 98%] Linking C executable ../../output/bladeRF-power
[ 98%] Built target bladeRF-power
[ 98%] Building C object common/test/dc_calibration/CMakeFiles/test_dc_calibration.dir/src/main.c.o
[ 99%] Building C object common/test/dc_calibration/CMakeFiles/test_dc_calibration.dir/__/__/src/conversions.c.o
[ 99%] Building C object common/test/dc_calibration/CMakeFiles/test_dc_calibration.dir/__/__/src/dc_calibration.c.o
[100%] Linking C executable ../../../output/test_dc_calibration
[100%] Built target test_dc_calibration
[sudo] password for bricer: 
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
-- Install configuration: "Release"
-- Installing: /usr/local/lib64/pkgconfig/libbladeRF.pc
-- Installing: /usr/local/lib64/libbladeRF.so.2
-- Installing: /usr/local/lib64/libbladeRF.so
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

# References

### sudoer (adding path)

If you want to run `bladeRF-cli` with `sudo` without specifying the full path, you’ll need to add `/usr/local/bin` to the `secure_path` in the `sudoers` file.

Here’s how:

1. **Edit the `sudoers` File**:
   - Use `visudo`, the safe way to edit the `sudoers` file:
     ```bash
     sudo visudo
     ```

2. **Locate the `secure_path` Setting**:
   - Find the line that starts with `Defaults secure_path`. It will look something like this:
     ```bash
     Defaults    secure_path = /sbin:/bin:/usr/sbin:/usr/bin
     ```

3. **Add `/usr/local/bin` to `secure_path`**:
   - Modify the line to include `/usr/local/bin`:
     ```bash
     Defaults    secure_path = /sbin:/bin:/usr/sbin:/usr/bin:/usr/local/bin
     ```

4. **Save and Exit**:
   - After editing, save and close `visudo` (usually with `Ctrl+X` and then `Y` to confirm).

Now, when you use `sudo bladeRF-cli -i`, it should work without needing the full path.

### epel-release

```
sudo yum install epel-release
```

:x: This didn't work