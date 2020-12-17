# ESP Battery Monitor
A battery and power usage monitor based on the ESP8266 (Node V2) and the INA3221 current monitor.
The SW is based on the ESP8266 Non-OS SDK and uses the IoT-Demo as a basis.
Features:
- Measures up to 12 current and voltage values (3 per INA3211, common ground)
- Estimates state-of-charge and state-of-health of a battery based on coulomb counting and an voltage (OCV) to state-of-charge curve.
- Provide the values via a REST interface as JSON data (the interface is based on the ESP IoT-Demo included in the Non-OS-SDK)
