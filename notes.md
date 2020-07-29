# Notes
This is a scratchpad for notes as this project develops.

## High-level BLE
### Remote
**Role**: BLE central

**Service Role**: Server

**Startup**:
- Initialize common BLE functionality
    - BLE stack/soft device
    - GATT module
- Check for paired address
- Initialize BLE central-specific functionality
    - Scanning module
        - If paired: scan for BLE addr
        - If not paired: scan for UUID + appearance
- Initialize BLE ES server

**Continuous**:
- Alert application when BLE is completely initialized
- Push notifications as the sensor module gives updates

### Receiver
**Role**: BLE peripheral

**Service Role**: Client

**Startup**:
- Initialize common BLE functionality
    - BLE stack/soft device
    - GATT module
- Check for paired address
- Initialize BLE peripheral-specific functionality
    - GAP module
    - Advertising module
        - If paired: advertise name
        - If not paired: advertise UUID + appearance
    - Connection parameters
    - DB discovery
- Initialize BLE ES client

**Continuous**:
- Alert application when BLE is completely initialized
- Alert application when notifications come in

## BLE Modules
### Remote (`src/ble/ble_remote.cpp,hpp`)
This is the top-level BLE module for the remote. Application <-> BLE interactions
should all take place through this module.
#### Application ↔ Remote Interactions:
- App → BLE: BLE initialization
- BLE → App: BLE initialization complete
- App → BLE: new sensor data

### Receiver (`src/ble/ble_receiver.cpp,hpp`)
This is the top-level BLE module for the receiver. Application <-> BLE interactions
should all take place through this module.
#### Application ↔ Remote Interactions:
- App → BLE: BLE initialization
- BLE → App: BLE initialization complete
- App ← BLE: new sensor data
