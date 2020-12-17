# ESP Battery Monitor
A battery and power usage monitor based on the ESP8266 (Node V2) and the INA3221 current monitor.
The SW is based on the ESP8266 Non-OS SDK and uses the IoT-Demo as a basis.

Features:
- Measures up to 12 current and voltage values (3 per INA3221, common ground)
- Estimates state-of-charge and state-of-health of a battery based on coulomb counting and a voltage (OCV) to state-of-charge curve.
- Provide the values via a REST interface as JSON data (the interface is based on the ESP IoT-Demo included in the Non-OS-SDK)

# Current State
!!Untested - Work in progress!!

- INA3221 driver implemented
- PowerMeter component translates shunt voltages to currents
- First draft of fuel gauge component to estimate battery stats

To Do:
- Implement battery state calculation
    - End-Of-Charge detection
    - Correction of coulomb counter when voltage is relaxed (OCV)
    - Calculate State-Of-Health from relaxed voltages
- REST API
    - Use the IoT Demo as a basis and add the required fields to the JSON data

- Remove superfluous code from the IoT Demo