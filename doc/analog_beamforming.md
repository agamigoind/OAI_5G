This document explains the implementation of analog beamforming in OAI codebase.

[[_TOC_]]

# Introduction to analog beamforming

Beamforming is a technique applied to antenna arrays to create a directional radiation pattern. This often consists in providing a different phase shift to each element of the array such that signals with a different angle of arrival/departure experience a change in radiation pattern because of constructive or destructive interference.

There are three main beamforming techinques: analog, digital and hybrid. The names refer to the phase shift application before or after the digital to analog conversion (or analog to digital in reception). When we speak about analog beamforming we generally refer to a techinique where the phase shifts that produce the beam stearing are applied by the radio unit (RU) choosing from a finite set of steering directions. The advantage of analog beamforming is a simplified analog circuitry and therefore reduced costs.

The presence of a limited number of predefined beams at RU poses constraints to the scheduler at gNB. As a matter of fact, the scheduler can serve only a limited number of beams, depending on the RU characteristics (possibly only 1), in a given time scale, that also depends on the RU characteristics (e.g. 1 slot or 1 symbol). This limitation doesn't exist for digital beamforming.

# Configuration file fields for analog beamforming

A set of parameters in configuration files controls the implementation of analog beamforming and instructs the scheduler on how to behave in such scenarios. Since most notably this technique in 5G is employed in FR2, the configuration file example currently available is a RFsim one for band 261. [Config file example](../ci-scripts/conf_files/gnb.sa.band261.u3.32prb.rfsim.conf)

In the `MACRLC` section of configuration files, there are three new parameters: `set_analog_beamforming`, `beam_duration` and `beams_per_period`. The explanation of these parameters is here provided:
- `set_analog_beamforming` can be set to 1 or 0 to activate or desactivate analog beamforming (default value is 0)
- `beam_duration` is the number of slots (currently minimum duration of a beam) the scheduler is tied to a beam (default value is 1)
- `beams_per_period` is the number of concurrent beams the RU can handle in the beam duration (default value is 1)

In the `gNBs` section of the configuration file, under the phyisical parameters, a vector field containing the set of beam indices to be provided by the OAI L1 to the RU is also required. This field is called `beam_weights`. In current implementation, the number of beam indices should be equal to the number of SSBs transmitted.

# Implementation in OAI scheduler

A new MAC structure `NR_beam_info_t` controls the behavior of the scheduler in presence of analog beamforming. Besides the already mentioned parameters `beam_duration` and `beams_per_period`, the structure also holds a matrix `beam_allocation[i][j]`, whose indices `i` and `j` stands respectively for the number of beams in the period and the slot index (the size of the latter depends on the frame characteristics).
This matrix contains the beams already allocated in a given slot, to flag the scheduler to use one of these to schedule a UE in one of these beams. If the matrix is full (all the beams in the given period, e.g. slot) are already allocated, the scheduler can't allocate a UE in a new beam.
To this goal, we extended the virtual resource block (VRB) map by one dimension to also contain information per allocated beam. As said, the scheduler can independently schedule users in a number of beams up to `beams_per_period` concurrently.

It is important to note that in current implementation, there are several periodical channels, e.g. PRACH or PUCCH for CSI et cetera, that have the precendence in being assigned a beam, that is because the scheduling is automatic, set in RRC configuration, and not up to the scheduler. For these instances, we assume the beam is available (if not there are assertions to stop the process). For data channels, the currently implemented PF scheduler is used. The only modification is that a UE can be served only if there is a free beam available or the one of the beams already in use correspond to that UE beam.
