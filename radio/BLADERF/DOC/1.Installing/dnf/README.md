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
Last metadata expiration check: 2:28:56 ago on Sat 09 Nov 2024 12:33:13 PM CET.
Package gcc-11.3.1-4.3.el9.x86_64 is already installed.
Package gcc-c++-11.3.1-4.3.el9.x86_64 is already installed.
Package make-1:4.3-7.el9.x86_64 is already installed.
Package cmake-3.20.2-8.el9.x86_64 is already installed.
Package libusbx-devel-1.0.26-1.el9.x86_64 is already installed.
Package ncurses-devel-6.2-8.20210508.el9.x86_64 is already installed.
Dependencies resolved.
======================================================================================================================================================================
 Package                                Architecture               Version                                 Repository                                            Size
======================================================================================================================================================================
Upgrading:
 cmake                                  x86_64                     3.26.5-2.el9                            rhel-9-for-x86_64-appstream-rpms                     8.7 M
 cmake-data                             noarch                     3.26.5-2.el9                            rhel-9-for-x86_64-appstream-rpms                     2.4 M
 cmake-filesystem                       x86_64                     3.26.5-2.el9                            rhel-9-for-x86_64-appstream-rpms                      23 k
 cmake-rpm-macros                       noarch                     3.26.5-2.el9                            rhel-9-for-x86_64-appstream-rpms                      12 k
 cpp                                    x86_64                     11.4.1-3.el9                            rhel-9-for-x86_64-appstream-rpms                      11 M
 gcc                                    x86_64                     11.4.1-3.el9                            rhel-9-for-x86_64-appstream-rpms                      32 M
 gcc-c++                                x86_64                     11.4.1-3.el9                            rhel-9-for-x86_64-appstream-rpms                      13 M
 gcc-gfortran                           x86_64                     11.4.1-3.el9                            rhel-9-for-x86_64-appstream-rpms                      13 M
 gcc-plugin-annobin                     x86_64                     11.4.1-3.el9                            rhel-9-for-x86_64-appstream-rpms                      49 k
 libgcc                                 x86_64                     11.4.1-3.el9                            rhel-9-for-x86_64-baseos-rpms                         95 k
 libgfortran                            x86_64                     11.4.1-3.el9                            rhel-9-for-x86_64-baseos-rpms                        804 k
 libgomp                                x86_64                     11.4.1-3.el9                            rhel-9-for-x86_64-baseos-rpms                        270 k
 libquadmath                            x86_64                     11.4.1-3.el9                            rhel-9-for-x86_64-baseos-rpms                        197 k
 libquadmath-devel                      x86_64                     11.4.1-3.el9                            rhel-9-for-x86_64-appstream-rpms                      38 k
 libstdc++                              x86_64                     11.4.1-3.el9                            rhel-9-for-x86_64-baseos-rpms                        747 k
 libstdc++-devel                        x86_64                     11.4.1-3.el9                            rhel-9-for-x86_64-appstream-rpms                     2.4 M
 make                                   x86_64                     1:4.3-8.el9                             rhel-9-for-x86_64-baseos-rpms                        541 k
 ncurses                                x86_64                     6.2-10.20210508.el9                     rhel-9-for-x86_64-baseos-rpms                        410 k
 ncurses-base                           noarch                     6.2-10.20210508.el9                     rhel-9-for-x86_64-baseos-rpms                         99 k
 ncurses-c++-libs                       x86_64                     6.2-10.20210508.el9                     rhel-9-for-x86_64-appstream-rpms                      39 k
 ncurses-devel                          x86_64                     6.2-10.20210508.el9                     rhel-9-for-x86_64-appstream-rpms                     732 k
 ncurses-libs                           x86_64                     6.2-10.20210508.el9                     rhel-9-for-x86_64-baseos-rpms                        332 k

Transaction Summary
======================================================================================================================================================================
Upgrade  22 Packages

Total download size: 86 M
Is this ok [y/N]: y
Downloading Packages:
(1/22): ncurses-c++-libs-6.2-10.20210508.el9.x86_64.rpm                                                                                79 kB/s |  39 kB     00:00    
(2/22): ncurses-devel-6.2-10.20210508.el9.x86_64.rpm                                                                                  851 kB/s | 732 kB     00:00    
(3/22): cmake-filesystem-3.26.5-2.el9.x86_64.rpm                                                                                       50 kB/s |  23 kB     00:00    
(4/22): cmake-3.26.5-2.el9.x86_64.rpm                                                                                                 8.4 MB/s | 8.7 MB     00:01    
(5/22): gcc-plugin-annobin-11.4.1-3.el9.x86_64.rpm                                                                                    139 kB/s |  49 kB     00:00    
(6/22): libstdc++-devel-11.4.1-3.el9.x86_64.rpm                                                                                       5.9 MB/s | 2.4 MB     00:00    
(7/22): cmake-data-3.26.5-2.el9.noarch.rpm                                                                                            4.6 MB/s | 2.4 MB     00:00    
(8/22): cmake-rpm-macros-3.26.5-2.el9.noarch.rpm                                                                                       23 kB/s |  12 kB     00:00    
(9/22): cpp-11.4.1-3.el9.x86_64.rpm                                                                                                    18 MB/s |  11 MB     00:00    
(10/22): gcc-c++-11.4.1-3.el9.x86_64.rpm                                                                                               27 MB/s |  13 MB     00:00    
(11/22): libquadmath-devel-11.4.1-3.el9.x86_64.rpm                                                                                    211 kB/s |  38 kB     00:00    
(12/22): gcc-gfortran-11.4.1-3.el9.x86_64.rpm                                                                                          27 MB/s |  13 MB     00:00    
(13/22): gcc-11.4.1-3.el9.x86_64.rpm                                                                                                   53 MB/s |  32 MB     00:00    
(14/22): ncurses-6.2-10.20210508.el9.x86_64.rpm                                                                                       793 kB/s | 410 kB     00:00    
(15/22): ncurses-base-6.2-10.20210508.el9.noarch.rpm                                                                                  189 kB/s |  99 kB     00:00    
(16/22): libgcc-11.4.1-3.el9.x86_64.rpm                                                                                               513 kB/s |  95 kB     00:00    
(17/22): ncurses-libs-6.2-10.20210508.el9.x86_64.rpm                                                                                  1.6 MB/s | 332 kB     00:00    
(18/22): libgomp-11.4.1-3.el9.x86_64.rpm                                                                                              1.4 MB/s | 270 kB     00:00    
(19/22): libstdc++-11.4.1-3.el9.x86_64.rpm                                                                                            3.7 MB/s | 747 kB     00:00    
(20/22): libgfortran-11.4.1-3.el9.x86_64.rpm                                                                                          4.1 MB/s | 804 kB     00:00    
(21/22): libquadmath-11.4.1-3.el9.x86_64.rpm                                                                                          344 kB/s | 197 kB     00:00    
(22/22): make-4.3-8.el9.x86_64.rpm                                                                                                    1.1 MB/s | 541 kB     00:00    
----------------------------------------------------------------------------------------------------------------------------------------------------------------------
Total                                                                                                                                  25 MB/s |  86 MB     00:03     
Running transaction check
Transaction check succeeded.
Running transaction test
Transaction test succeeded.
Running transaction
  Preparing        :                                                                                                                                              1/1 
  Upgrading        : libgcc-11.4.1-3.el9.x86_64                                                                                                                  1/44 
  Running scriptlet: libgcc-11.4.1-3.el9.x86_64                                                                                                                  1/44 
  Upgrading        : libstdc++-11.4.1-3.el9.x86_64                                                                                                               2/44 
  Upgrading        : libquadmath-11.4.1-3.el9.x86_64                                                                                                             3/44 
  Upgrading        : make-1:4.3-8.el9.x86_64                                                                                                                     4/44 
  Upgrading        : cmake-rpm-macros-3.26.5-2.el9.noarch                                                                                                        5/44 
  Upgrading        : cmake-filesystem-3.26.5-2.el9.x86_64                                                                                                        6/44 
  Upgrading        : libgfortran-11.4.1-3.el9.x86_64                                                                                                             7/44 
  Upgrading        : libstdc++-devel-11.4.1-3.el9.x86_64                                                                                                         8/44 
  Upgrading        : libgomp-11.4.1-3.el9.x86_64                                                                                                                 9/44 
  Upgrading        : ncurses-base-6.2-10.20210508.el9.noarch                                                                                                    10/44 
  Upgrading        : ncurses-libs-6.2-10.20210508.el9.x86_64                                                                                                    11/44 
  Upgrading        : ncurses-c++-libs-6.2-10.20210508.el9.x86_64                                                                                                12/44 
  Upgrading        : cmake-data-3.26.5-2.el9.noarch                                                                                                             13/44 
  Upgrading        : cmake-3.26.5-2.el9.x86_64                                                                                                                  14/44 
  Upgrading        : cpp-11.4.1-3.el9.x86_64                                                                                                                    15/44 
  Upgrading        : gcc-11.4.1-3.el9.x86_64                                                                                                                    16/44 
  Running scriptlet: gcc-11.4.1-3.el9.x86_64                                                                                                                    16/44 
  Upgrading        : libquadmath-devel-11.4.1-3.el9.x86_64                                                                                                      17/44 
  Upgrading        : gcc-gfortran-11.4.1-3.el9.x86_64                                                                                                           18/44 
  Upgrading        : gcc-plugin-annobin-11.4.1-3.el9.x86_64                                                                                                     19/44 
  Running scriptlet: gcc-plugin-annobin-11.4.1-3.el9.x86_64                                                                                                     19/44 
  Upgrading        : gcc-c++-11.4.1-3.el9.x86_64                                                                                                                20/44 
  Upgrading        : ncurses-devel-6.2-10.20210508.el9.x86_64                                                                                                   21/44 
  Upgrading        : ncurses-6.2-10.20210508.el9.x86_64                                                                                                         22/44 
  Cleanup          : ncurses-devel-6.2-8.20210508.el9.x86_64                                                                                                    23/44 
  Cleanup          : cmake-3.20.2-8.el9.x86_64                                                                                                                  24/44 
  Cleanup          : ncurses-c++-libs-6.2-8.20210508.el9.x86_64                                                                                                 25/44 
  Cleanup          : gcc-gfortran-11.3.1-4.3.el9.x86_64                                                                                                         26/44 
  Cleanup          : libgfortran-11.3.1-4.3.el9.x86_64                                                                                                          27/44 
  Cleanup          : gcc-c++-11.3.1-4.3.el9.x86_64                                                                                                              28/44 
  Cleanup          : cmake-data-3.20.2-8.el9.noarch                                                                                                             29/44 
  Cleanup          : ncurses-6.2-8.20210508.el9.x86_64                                                                                                          30/44 
  Cleanup          : gcc-plugin-annobin-11.3.1-4.3.el9.x86_64                                                                                                   31/44 
  Running scriptlet: gcc-plugin-annobin-11.3.1-4.3.el9.x86_64                                                                                                   31/44 
  Cleanup          : libquadmath-devel-11.3.1-4.3.el9.x86_64                                                                                                    32/44 
  Cleanup          : libstdc++-devel-11.3.1-4.3.el9.x86_64                                                                                                      33/44 
  Cleanup          : cmake-filesystem-3.20.2-8.el9.x86_64                                                                                                       34/44 
  Cleanup          : cmake-rpm-macros-3.20.2-8.el9.noarch                                                                                                       35/44 
  Cleanup          : gcc-11.3.1-4.3.el9.x86_64                                                                                                                  36/44 
  Cleanup          : libstdc++-11.3.1-4.3.el9.x86_64                                                                                                            37/44 
  Cleanup          : libquadmath-11.3.1-4.3.el9.x86_64                                                                                                          38/44 
  Cleanup          : ncurses-libs-6.2-8.20210508.el9.x86_64                                                                                                     39/44 
  Cleanup          : ncurses-base-6.2-8.20210508.el9.noarch                                                                                                     40/44 
  Cleanup          : libgcc-11.3.1-4.3.el9.x86_64                                                                                                               41/44 
  Running scriptlet: libgcc-11.3.1-4.3.el9.x86_64                                                                                                               41/44 
  Cleanup          : cpp-11.3.1-4.3.el9.x86_64                                                                                                                  42/44 
  Cleanup          : libgomp-11.3.1-4.3.el9.x86_64                                                                                                              43/44 
  Cleanup          : make-1:4.3-7.el9.x86_64                                                                                                                    44/44 
  Running scriptlet: make-1:4.3-7.el9.x86_64                                                                                                                    44/44 
  Verifying        : ncurses-devel-6.2-10.20210508.el9.x86_64                                                                                                    1/44 
  Verifying        : ncurses-devel-6.2-8.20210508.el9.x86_64                                                                                                     2/44 
  Verifying        : ncurses-c++-libs-6.2-10.20210508.el9.x86_64                                                                                                 3/44 
  Verifying        : ncurses-c++-libs-6.2-8.20210508.el9.x86_64                                                                                                  4/44 
  Verifying        : cmake-3.26.5-2.el9.x86_64                                                                                                                   5/44 
  Verifying        : cmake-3.20.2-8.el9.x86_64                                                                                                                   6/44 
  Verifying        : cmake-filesystem-3.26.5-2.el9.x86_64                                                                                                        7/44 
  Verifying        : cmake-filesystem-3.20.2-8.el9.x86_64                                                                                                        8/44 
  Verifying        : gcc-plugin-annobin-11.4.1-3.el9.x86_64                                                                                                      9/44 
  Verifying        : gcc-plugin-annobin-11.3.1-4.3.el9.x86_64                                                                                                   10/44 
  Verifying        : libstdc++-devel-11.4.1-3.el9.x86_64                                                                                                        11/44 
  Verifying        : libstdc++-devel-11.3.1-4.3.el9.x86_64                                                                                                      12/44 
  Verifying        : cmake-data-3.26.5-2.el9.noarch                                                                                                             13/44 
  Verifying        : cmake-data-3.20.2-8.el9.noarch                                                                                                             14/44 
  Verifying        : cmake-rpm-macros-3.26.5-2.el9.noarch                                                                                                       15/44 
  Verifying        : cmake-rpm-macros-3.20.2-8.el9.noarch                                                                                                       16/44 
  Verifying        : cpp-11.4.1-3.el9.x86_64                                                                                                                    17/44 
  Verifying        : cpp-11.3.1-4.3.el9.x86_64                                                                                                                  18/44 
  Verifying        : gcc-c++-11.4.1-3.el9.x86_64                                                                                                                19/44 
  Verifying        : gcc-c++-11.3.1-4.3.el9.x86_64                                                                                                              20/44 
  Verifying        : gcc-gfortran-11.4.1-3.el9.x86_64                                                                                                           21/44 
  Verifying        : gcc-gfortran-11.3.1-4.3.el9.x86_64                                                                                                         22/44 
  Verifying        : libquadmath-devel-11.4.1-3.el9.x86_64                                                                                                      23/44 
  Verifying        : libquadmath-devel-11.3.1-4.3.el9.x86_64                                                                                                    24/44 
  Verifying        : gcc-11.4.1-3.el9.x86_64                                                                                                                    25/44 
  Verifying        : gcc-11.3.1-4.3.el9.x86_64                                                                                                                  26/44 
  Verifying        : ncurses-6.2-10.20210508.el9.x86_64                                                                                                         27/44 
  Verifying        : ncurses-6.2-8.20210508.el9.x86_64                                                                                                          28/44 
  Verifying        : ncurses-base-6.2-10.20210508.el9.noarch                                                                                                    29/44 
  Verifying        : ncurses-base-6.2-8.20210508.el9.noarch                                                                                                     30/44 
  Verifying        : ncurses-libs-6.2-10.20210508.el9.x86_64                                                                                                    31/44 
  Verifying        : ncurses-libs-6.2-8.20210508.el9.x86_64                                                                                                     32/44 
  Verifying        : libgcc-11.4.1-3.el9.x86_64                                                                                                                 33/44 
  Verifying        : libgcc-11.3.1-4.3.el9.x86_64                                                                                                               34/44 
  Verifying        : libgomp-11.4.1-3.el9.x86_64                                                                                                                35/44 
  Verifying        : libgomp-11.3.1-4.3.el9.x86_64                                                                                                              36/44 
  Verifying        : libquadmath-11.4.1-3.el9.x86_64                                                                                                            37/44 
  Verifying        : libquadmath-11.3.1-4.3.el9.x86_64                                                                                                          38/44 
  Verifying        : libstdc++-11.4.1-3.el9.x86_64                                                                                                              39/44 
  Verifying        : libstdc++-11.3.1-4.3.el9.x86_64                                                                                                            40/44 
  Verifying        : make-1:4.3-8.el9.x86_64                                                                                                                    41/44 
  Verifying        : make-1:4.3-7.el9.x86_64                                                                                                                    42/44 
  Verifying        : libgfortran-11.4.1-3.el9.x86_64                                                                                                            43/44 
  Verifying        : libgfortran-11.3.1-4.3.el9.x86_64                                                                                                          44/44 
Installed products updated.

Upgraded:
  cmake-3.26.5-2.el9.x86_64                            cmake-data-3.26.5-2.el9.noarch                           cmake-filesystem-3.26.5-2.el9.x86_64                 
  cmake-rpm-macros-3.26.5-2.el9.noarch                 cpp-11.4.1-3.el9.x86_64                                  gcc-11.4.1-3.el9.x86_64                              
  gcc-c++-11.4.1-3.el9.x86_64                          gcc-gfortran-11.4.1-3.el9.x86_64                         gcc-plugin-annobin-11.4.1-3.el9.x86_64               
  libgcc-11.4.1-3.el9.x86_64                           libgfortran-11.4.1-3.el9.x86_64                          libgomp-11.4.1-3.el9.x86_64                          
  libquadmath-11.4.1-3.el9.x86_64                      libquadmath-devel-11.4.1-3.el9.x86_64                    libstdc++-11.4.1-3.el9.x86_64                        
  libstdc++-devel-11.4.1-3.el9.x86_64                  make-1:4.3-8.el9.x86_64                                  ncurses-6.2-10.20210508.el9.x86_64                   
  ncurses-base-6.2-10.20210508.el9.noarch              ncurses-c++-libs-6.2-10.20210508.el9.x86_64              ncurses-devel-6.2-10.20210508.el9.x86_64             
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
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Found PkgConfig: /usr/bin/pkg-config (found version "1.7.3") 
-- Checking libusb version...
-- libusb version: 1.0.26

-- Looking for clock_gettime in c
-- Looking for clock_gettime in c - found
-- libad936x: Configuring build...
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE  
-- libad936x: Attempting to initialize missing submodule
-- Found Git: /usr/bin/git (found version "2.39.3") 
Submodule 'thirdparty/analogdevicesinc/no-OS' (https://github.com/analogdevicesinc/no-OS/) registered for path 'thirdparty/analogdevicesinc/no-OS'
Cloning into '/home/bricer/Developer/bladeRF/thirdparty/analogdevicesinc/no-OS'...
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
-- libc version: 2.34

--
-- libbladeRF version: 2.5.1-git-fe3304d7
-- libbladeRF_test: enabled
-- The CXX compiler identification is GNU 11.4.1
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- nuand bladeRF udev rules will be installed to '/etc/udev/rules.d' upon running 'make install'
--
-- libtecla not found.
-- libedit not found.
-- Reusing existing interactive help for bladeRF-cli
-- libtecla OFF FALSE
-- libedit OFF FALSE
-- Not building man page for bladeRF-cli. (BUILD_BLADERF_CLI_DOCUMENTATION is OFF)
-- Configured to build bladeRF-cli version: 1.9.0-git-fe3304d7
-- Looking for wsyncup in /usr/lib64/libcurses.so
-- Looking for wsyncup in /usr/lib64/libcurses.so - found
-- Looking for cbreak in /usr/lib64/libncursesw.so
-- Looking for cbreak in /usr/lib64/libncursesw.so - found
-- Looking for nodelay in /usr/lib64/libncursesw.so
-- Looking for nodelay in /usr/lib64/libncursesw.so - found
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