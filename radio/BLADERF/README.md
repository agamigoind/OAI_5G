# BladeRF 2.0 Micro Documentation (Updated)

---

[TOC]

---

As of **2025**, this documentation provides instructions for setting up and using the **BladeRF 2.0 Micro** with OpenAirInterface (OAI). If you encounter any discrepancies, contact the support team or check the latest resources on Nuand's GitHub.

---

### **1. Install BladeRF 2.0 Micro Libraries**

   - Install the latest **bladeRF 2.0 Micro** libraries and utilities.  
     It's recommended to build from source for the most recent updates.

      Follow this link :point_right: [1.Installing](DOC/1.Installing) to find your Linux package Manager
   

---

### **2. Flashing the FPGA Image**

   Download the latest FPGA images from Nuand's GitHub.

    Follow this link :point_right: [2.Flashing](DOC/2.Flashing) to flash the board.

---

### **3. Automatic Calibration (AD9361)**

   With the **AD9361** RF transceiver in the **BladeRF 2.0 Micro**, calibration is now **automatic**. This includes calibration of:
   - LMS parameters (IQ imbalance correction).
   - DC offset correction for both RX and TX paths.
   
   There is no longer a need to manually run multiple calibration commands (`cal lms`, `cal dc rxtx`), as was required with previous models. The transceiver performs dynamic calibration automatically during operation.

   However, you can still manually verify calibration using:

   ```bash
   sudo bladeRF-cli -i
   cal lms
   cal dc rxtx
   ```

---

   **Note:** Do not run OAI's `./build_oai -I -w BLADERF`. Instead, manually install the bladeRF dependencies.

---

### **5. Tune the RX Gain Using the gNB (NR)**

   Run the softmodem and monitor the **gNB (NR)** logs for input signal strength and performance.  
   Instructions are available on the OAI wiki:  
   [OAI NR Documentation](https://gitlab.eurecom.fr/oai/openairinterface5g/wikis/T/basic).

   In the logs, check the **input signal** values for time-domain signal power. This signal level should be around **30**. If the power level deviates, modify the `max_rxgain` parameter in your configuration file.

   **Configuration file:**  
   `targets/PROJECTS/GENERIC-NR-5GC/CONF/gnb.sa.band78.fr1.51PRB.bladerf20xa0.conf`

   Update the following parameters in the file:
   - `tracking_area_code`
   - `plmn_list` (MCC, MNC, and MNC length)
   - `mme_ip_address` (IP address of the EPC or 5GC core)
   - `NETWORK_INTERFACES` (Set gNB interface addresses to match your EPC or 5GC setup)

---

### **6. Test the Setup**

   Connect a UE and run traffic tests (e.g., **iperf**).  
   Expected throughput:
   - **Downlink:** Over **16 Mbps**  
   - **Uplink:** Over **8 Mbps**

   In the OAI logs, you should observe entries with **PHR 40** and **CQI 15** if the setup is correctly optimized.

---

### **7. Troubleshooting**

   Although calibration is automatic, you may still manually recalibrate if issues arise:

   ```bash
   sudo bladeRF-cli -i
   cal lms
   cal dc rxtx
   ```

   Ensure you are using appropriate radio equipment, including:
   - **Duplexers**
   - **Proper antennas**
   - **A low-interference environment**

   Check USB connections for stability and ensure the device is not overheating.

---

### **8. Additional Notes**

   - For high sample rates, ensure your system has sufficient USB 3.1 bandwidth and CPU resources.
   - Monitor **RX/TX overruns** using logs and adjust parameters (e.g., sample rate, buffer size, and gain) as needed.
   - Stay updated with the latest firmware and driver releases from Nuand's GitHub repository.

---

This updated documentation ensures compatibility with the **BladeRF 2.0 Micro**, focusing on its automatic calibration capabilities and usage in modern **gNB (NR)** setups with OpenAirInterface.
