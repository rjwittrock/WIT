### WIT
WIT is a software tool for optimal and near-optimal allocation of scarce resources, when the resources are utilized in pre-determined sets of material parts (as in a bill of material) and/or sets of capacities (such as machine processing capacity).

This repository is a modifiable version of the IBM/CMMPPT repository, from which it is forked: The IBM/CMMPPT repository was made read-only in 2024.

This repository contains the WIT source code, testing code and data, documentation, and some application programs.

#### Note:
Before IBM/CMMPPT became open-source, it was called "WIT". Consequently, "wit" appears in the names of many of its components. So in this version, it's called WIT again, for simplicity. "WIT" used to be an acronym, but the acronym no longer applies.

#### Directories:

* **doc:**
  * Documentation
* **wit:**
  * WIT source code, build facilities, and some test code
* **mcl:**
  * Message Class Library
  * Used by WIT and SCE for issuing messages (textual output)
  * Source code, build facilities, and test code
* **config:**
  * Generic Makefiles used by WIT, MCL, and other programs in this project
* **wit-sa-test:**
  * Data and scripts for testing WIT in Stand-Alone Executable mode.
* **samples:**
  * Small example WIT application programs.
* **sce:**
  * Supply Capability Engine: A major WIT application program
  * Source code, build facilities, and test code
* **scenario & witutil:**
  * Two class libraries used by SCE
  * Source code, build facilities, and test code
* **witj:**
  * An object-oriented Java interface to WIT
  * Source code, build facilities, and test code
* **witviz:**
  * A visualization tool for WIT models
  * Source code, build facilities, and test code
  * Uses witj

