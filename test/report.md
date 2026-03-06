# AERA Test Documentation

## 1. Embodiment

### 1.1 Sensing
Inputs coming from the environment (technically, data produced by the I/O devices) come in the form of facts for which the time interval is reduced to a single point in time (the time the input was produced) and for which the confidence value is one (since the fact represents an observation in the environment). 
Goals and Predictions are both pointers to facts.

| Test name | Description | Remarks |
| :--- | :--- | :--- |
| fact | tests basic fact syntax | |
| anti-fact | tests basic anti fact syntax | |
| goal | tests basic anti fact syntax | |
| prediction | tests basic anti fact syntax | |
| bathtub-sensing | tests the use of commands to achieve a simple goal | No Remarks :white_check_mark: |

### 1.2 Acting
AERA acts upon the physical world by calling commands on devices. There are two types of devices: internal (the executive, e.g.: inj, eje, mod, set, ...) and external (using custom commands).

#### 1.2.1 Tests for internal commands (icmd) of the executive
| Test name | Description | Remarks |
| :--- | :--- | :--- |
| inj | tests the icmd _inj | (R1) In decompiled_objects, the name of the injected object does not match what is provided.<br>(R2) If the object is injected to group "stdin", the object has no view in the decompilation file. |
| eje | tests the icmd _eje | (R1) This has never been used in existing Replicode files, and I wasn't able to reproduce all that the icmd should be able to do. |
| mod | tests the icmd _mod | No Remarks :white_check_mark: |
| set | tests the icmd _set | (R1) Setting and averaging works, but there is a caveat.<br>(R2) The documentation states that -1 represents an infinite resilience, but this was changed to the keyword "forever". |
| new-class | tests the icmd _new_class | (R1) This has never been used in existing Replicode files, and I wasn't able to reproduce all that the icmd should be able to do. |
| del-class | tests the icmd _del_class | (R1) This has never been used in existing Replicode files, and I wasn't able to reproduce all that the icmd should be able to do. |
| ldc | tests the icmd _ldc | (R1) This has never been used in existing Replicode files, and I wasn't able to reproduce all that the icmd should be able to do. |
| swp | tests the icmd _swp | (R1) This has never been used in existing Replicode files, and I wasn't able to reproduce all that the icmd should be able to do.<br>(R2) Documentation states that only values in {0, 1} should be allowed as a parameter, but this is not the case. |
| prb | tests the icmd _prb | (R1) This only ever been used in combination with "print"; which works as expected. |
| stop | tests the icmd _stop | No Remarks :white_check_mark: |

#### 1.2.2 Tests for external commands

| Test name | Description | Remarks |
| :--- | :--- | :--- |
| cmd | tests basic command syntax | (R1) The parameter device:did (as specified in the documentation) does not exist anymore. |
| bathtub-acting | tests the use of commands to achieve a simple goal | No Remarks :white_check_mark: |
