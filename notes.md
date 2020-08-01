# Notes
This is a scratchpad for notes as this project develops.

---

## High-level BLE
### Remote
**GAP Role**: BLE central

**GATT Role**: Server

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
**GAP Role**: BLE peripheral

**GATT Role**: Client

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

---

## BLE Modules
### Remote (`src/ble/ble_remote.cpp,hpp`)
This is the top-level BLE module for the remote. Application ↔ BLE interactions
should all take place through this module.
#### Application ↔ Remote Interactions:
- App → BLE: BLE initialization
- BLE → App: BLE initialization complete
- App → BLE: new sensor data

### Receiver (`src/ble/ble_receiver.cpp,hpp`)
This is the top-level BLE module for the receiver. Application ↔ BLE interactions
should all take place through this module.
#### Application ↔ Remote Interactions:
- App → BLE: BLE initialization
- BLE → App: BLE initialization complete
- App ← BLE: new sensor data

## High-level Application

---

## Coding Style
For general style, adherence to Google's style guide is made using cpplint (run
from the static_analysis directory using `run_cpplint`). There currently seems
to be some inexplicable "legal/copyright" warnings that are being ignored.

For module design, the following considerations are made:
- If a module shall exist only once, it should be a namespace
    - E.g. modules that interact with the soft device; there is only one
    soft device, with only one instance of its own state.
- If a module might exist multiple times, it should be a class
    - E.g. modules that interact with external hardware; there could be
    several pieces of this external hardware connected to different pins

For lines that exceed 100 columns, the line should be split at an "appropriate"
break point. The new line should be aligned "appropriately" with the previous
line. What's "appropriate" might vary case-to-case.

Modules typically consist of several pieces of private and public data
and logic. For both namespaces and classes, the following headers are used to
ease looking over/organizing code.
```
////////////////////////////////////////////////////////////////////////////////
// <heading name>
////////////////////////////////////////////////////////////////////////////////
```
Header headings might include:
- Types/aliases/definitions(/etc.)
- Constants
- Constructors and destructors (for class modules)
- Public functions

Source code headings might include:
- Internal prototypes (for namespace'd modules)
- Internal data (for namespace'd modules)
- Internal implementations
- Public implementations

Headers should be organized (in order):
- Includes
- Private data (if a class)
- Types/aliases/definitions
- Constants
- Constructors/destructors (if a class, if implemented)
- Public functions
- Private functions
- Constructors/destructors (if a class, if deleted)

Source code should be organized (in order):
- Includes
- Private types
- Private prototypes (if a namespace)
- Private data (if a namespace)
- Constructors/destructors (if a class, if necessary)
- Public functions
- Private functions

---

# TODOs
- 07/29/20 (both): Go through and add consistent formatting for comments/documentation
    especially in headers
- 07/29/20 (both): Implement BLE ES server/client ready alerts to the client
- 07/29/20 (both): Test BLE ES server/client
- 07/29/20 (both): Implement secure BLE comms (using peer manager?)
- 07/03/20 (both): Implement a "delete_bonds" type function
- 06/24/20 (both): Implement power management
