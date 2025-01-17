### WIT
WIT is a software tool for optimal and near-optimal allocation of scarce resources, when the resources are utilized in pre-determined sets of material parts (as in a bill of material) and/or sets of capacities (such as machine processing capacity).

WIT is intended to be a more actively maintained version of IBM/CMMPPT, from which this repository is forked.

This repository contains the WIT source code, testing code and data, documentation, and some application programs.

#### Note:
Before IBM/CMMPPT became open-source, it was called "WIT". Consequently, "wit" appears in the names of many of its components. So in this version, it's called WIT again, for simplicity. "WIT" used to be an acronym, but the acronym no longer applies.

#### Directories:

* **doc:**
  * Documentation
  * Status: Mixed: Some documents maintained; some not
* **wit:**
  * WIT source code, build facilities, and some test code
  * Status: Maintained
* **mcl:**
  * Message Class Library
  * Used by WIT and SCE for issuing messages (textual output)
  * Source code, build facilities, and test code
  * Status: Maintained
* **config:**
  * Generic Makefiles used by WIT, MCL, and other programs in this project
  * Status: Maintained
* **wit-sa-test:**
  * Data and scripts for testing WIT in Stand-Alone Executable mode.
  * Status: Maintained
* **samples:**
  * Small example WIT application programs.
  * Status: Mixed
* **sce:**
  * Supply Capability Engine: A major WIT application program
  * Source code, build facilities, and test code
  * Status: Maintained
* **scenario & witutil:**
  * Two class libraries used by SCE
  * Source code, build facilities, and test code
  * Status: Maintained
* **witj:**
  * An object-oriented Java interface to WIT
  * Source code, build facilities, and test code
  * Status: Out Of Date
    * Could be brought up-to-date if there is interest
* **witviz:**
  * A visualization tool for WIT models
  * Source code, build facilities, and test code
  * Uses witj
  * Status: Out Of Date
    * It might be possible to bring it up-to-date if there is interest, but that is not certain.

