# Quantum-Key-Distribution-ELC

# Low-Cost Quantum Key Distribution (QKD) Simulator using Photonic Communication

**Project 19**

Supervised by: Dr. G.V. Eswara Rao & Dr. Subhrendu Chattopadhyay

Repository: [mittalsukhansh/Quantum-Key-Distribution-ELC](https://github.com/mittalsukhansh/Quantum-Key-Distribution-ELC)

---

## Overview

This project implements a low-cost, hardware-based simulator for **Quantum Key Distribution (QKD)** using photonic (optical) communication. QKD is a method for securely exchanging cryptographic keys using quantum states of light (photons). Its security is guaranteed by the laws of quantum mechanics rather than computational hardness — any attempt at measurement/eavesdropping disturbs the quantum state and introduces detectable errors. This makes QKD **information-theoretically secure**, as opposed to the computationally-secure schemes used in classical cryptography.

## How It Works

**Security Principle:** Any interception changes photon polarization/phase and introduces detectable errors.

**Goal:** Generate symmetric secret keys for encryption with provable detection of eavesdropping.

**QKD Workflow:**
1. Prepare quantum states
2. Transmit photons
3. Measure in chosen bases
4. Sift key
5. Estimate error rate
6. Apply error correction & privacy amplification

**Channel:** Implemented over free-space optical links (photonic communication); sensitive to loss and noise.

## Protocol: BB84

The system implements the **BB84 protocol** (Bennett & Brassard, 1984) — the canonical QKD protocol, using two conjugate polarization bases (rectilinear & diagonal).

1. **State Preparation** – Alice prepares photons in one of four polarization states {0°, 45°, 90°, 135°}, representing bits and bases.
2. **Measurement & Sifting** – Bob measures each photon in a randomly chosen basis; Alice and Bob then publicly compare bases and discard mismatched measurements.
3. **Error Estimation** – A subset of the sifted key is sampled to estimate the Quantum Bit Error Rate (QBER). A high QBER indicates eavesdropping or channel/hardware issues.
4. **Post-Processing** – Error correction (e.g., CASCADE/LDPC) and privacy amplification are applied to produce the final secure key.

**Real-world applications:** Banking & Finance, Government & Defence, Telecom & Backbone Links, Cloud & Data Centres.

## Hardware Architecture

### Transmitter (Alice)
- Laser diode
- Arduino UNO
- LED
- 3D-printed polarizer assembly + stepper motor
- Firmware: [`transmitter.txt`](./transmitter.txt)

### Receiver (Bob)
- Photodiode (BPW32/BPW34)
- 100 V potentiometer
- Resistors
- Op-amp (LM358)
- ESP32 (v1)
- Firmware: [`reciever.c`](./reciever.c)

### Stepper Motors & Polarizers
- Stepper motors rotate 3D-printed polarizer mounts to switch between the four BB84 polarization bases programmatically.
- A custom gear train (motor gear + driven gear on the polarizer holder) provides a precise, repeatable angular step for each basis change.
- Firmware synchronizes motor rotation on the TX side with analyzer rotation on the RX side for every transmitted bit.
- Gear reduction improved angular accuracy beyond the stepper motor's native step resolution, reducing polarization-alignment error.

### Wiring
See [`connection_setup_wire.jpeg`](./connection_setup_wire.jpeg) for the full TX/RX wiring diagram.

## Mechanical Design (3D-Printed Parts)

To reduce cost and improve repeatability, all mechanical parts were custom-designed and 3D-printed using a CAD-driven, iterative design process. Designs are in [`cad/`](./cad).

| Part | Purpose |
|---|---|
| Gears | Transfer precise rotational motion from the stepper motor to the polarizer disc |
| Gear Holders | Keep gears aligned and meshed correctly so rotation stays smooth and slip-free |
| Enclosures (laser & receiver) | Shield the laser diode and photodiode from ambient light and dust, keeping the optical path clean and consistent |
| Base plate & height elevator | Provide a stable, aligned mounting platform for TX/RX components |

Material: **PLA**, chosen for dimensional stability and ease of printing.

## Challenges & Solutions

### Transmitter / Receiver
| Challenge | Solution |
|---|---|
| Unstable timing at low pulse rates causing missed detections | Slowed and fine-tuned code timing so each bit had sufficient read time |
| Laser beam missing the BPW32 detector (alignment drift) | Built a shoebox enclosure with a precisely measured hole to align the beam onto the sensor |
| Receiver unreliable at lower bit rates (timing drift/misreads) | Optimized firmware timing between samples |
| Difficulty identifying a reliable ON/OFF detection threshold under varying light | Repeatedly tested the circuit and manually tuned the threshold using live sensor readings — see [`01_BPW32_ADC_Test.ino`](./01_BPW32_ADC_Test.ino) and [`02_OOK_Detection.ino`](./02_OOK_Detection.ino) |

### 3D Printing
| Challenge | Solution / Notes |
|---|---|
| Lab 3D printer became unreliable mid-build | Reprinted affected parts; adjusted build schedule |
| Achieving tight tolerances | Careful measurement using vernier calipers and screw gauges |
| Material selection | Chose PLA for the best balance of stiffness and printability |

### Stepper Motors & Polarizers
| Challenge | Solution / Result |
|---|---|
| Friction & wear causing step loss | Mechanical adjustments to the gear train |
| Flexible mounts causing misalignment | Improved mounting surface rigidity |
| Steps-per-degree varying under load | Calibration tuning |

**Result:** After mechanical and firmware adjustments, repeatability improved to within acceptable alignment tolerances for experimental QKD demonstration at reduced bit rates.

## Key Learnings

1. **Quantum Cryptography Fundamentals** – How qubits, polarization encoding, and the BB84 protocol combine to enable provably secure key exchange.
2. **Photonics & Hardware Precision** – The importance of exact alignment and timing when working with lasers, polarizers, and photodiodes in a real optical setup.
3. **Mechanical Design & Prototyping** – Practical skills in 3D printing, precise measurement, and iterative design to solve real hardware integration problems.
4. **Embedded Systems Debugging** – Hands-on experience tuning code timing and detection thresholds to make a real-time system work reliably under changing conditions.

## References

- B. A. Forouzan, *Data Communications and Networking*, 5th ed. New York, NY: McGraw-Hill Education, 2013.
- BB84 Protocol
- Photodiode (BPW32/BPW34) datasheet
- Stepper motor datasheet
- Full build documentation: [`QKD-Kit-Development.pdf`](./QKD-Kit-Development.pdf)

---

## Repository Structure
```
.
├── Daily loger/                 # Build/testing logs kept during development
├── cad/                         # 3D-printable part designs (gears, holders, enclosures, base plate)
├── codes/                       # Additional firmware / support code
├── 01_BPW32_ADC_Test.ino        # Arduino sketch: raw ADC test on the BPW32 photodiode
├── 02_OOK_Detection.ino         # Arduino sketch: On-Off Keying (OOK) detection logic
├── reciever.c                   # Receiver (Bob / ESP32) firmware
├── transmitter.txt              # Transmitter (Alice / Arduino UNO) firmware
├── connection_setup_wire.jpeg   # TX/RX wiring diagram
├── Inventory                    # Bill of materials / component inventory
├── QKD-Kit-Development.pdf      # Full project report / build documentation
└── README.md
```

## Getting Started

1. **Bill of materials** – check [`Inventory`](./Inventory) for the full component list.
2. **Wiring** – follow [`connection_setup_wire.jpeg`](./connection_setup_wire.jpeg) to connect the laser/Arduino UNO (TX) and photodiode/ESP32 (RX).
3. **Mechanical build** – 3D-print the parts in [`cad/`](./cad) (PLA recommended) and assemble the gear train, polarizer mounts, and enclosures.
4. **Firmware**
   - Flash [`transmitter.txt`](./transmitter.txt) (rename to `.ino` as needed) to the Arduino UNO for the transmitter.
   - Flash [`reciever.c`](./reciever.c) to the ESP32 for the receiver.
   - Use [`01_BPW32_ADC_Test.ino`](./01_BPW32_ADC_Test.ino) to verify the photodiode is reading correctly before running full detection.
   - Use [`02_OOK_Detection.ino`](./02_OOK_Detection.ino) to test On-Off Keying detection and threshold tuning.
5. **Reference** – see [`QKD-Kit-Development.pdf`](./QKD-Kit-Development.pdf) for the complete build walkthrough.
