Whole project description is in Diplomski (3).pdf file

📡 ESP32 DSP Signal Filtering Project

This project demonstrates real-time digital signal processing (DSP) on the ESP32 microcontroller. An analog signal is read via the ADC, filtered using an IIR Butterworth low-pass filter, and the result is sent over the serial port for visualization or further processing.

The system mimics a typical DSP pipeline:
	1.	Analog signal acquisition.
	2.	Filter design and simulation (offline, in MATLAB or Python).
	3.	Coefficient extraction and firmware implementation on ESP32.
	4.	Real-time signal monitoring.

⸻

🧠 Project Overview
	•	Microcontroller: ESP32-WROOM
	•	Signal Source: Analog input (e.g., signal generator or sensor)
	•	Filter Type: IIR Low-Pass (Butterworth, order = 3)
	•	Sampling Rate: 100 Hz (configurable)
	•	Output: Filtered signal via serial port (printf), viewable via Python or serial terminal
