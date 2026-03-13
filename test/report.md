# AERA Test Documentation

## 1. Embodiment

### 1.1 Sensing
Inputs coming from the environment (technically, data produced by the I/O devices) come in the form of facts for which the time interval is reduced to a single point in time (the time the input was produced) and for which the confidence value is one (since the fact represents an observation in the environment). 
This section includes tests for Goals and Predictions, which are both pointers to facts.

| Test name | Description | Remarks |
| :--- | :--- | :--- |
| fact | tests basic fact syntax | (R1) The parameters `after` and `before` use timestamps, not microseconds. |
| anti-fact | tests basic anti fact syntax | (R1) The parameters `after` and `before` use timestamps, not microseconds. |
| goal | tests basic goal syntax | (R1) The structure of the goal class has changed. |
| prediction | tests basic prediction syntax | (R1) The structure of the prediction class has changed. |
| ball-external | tests if inputs coming from the (mocked) environment are handled correctly |  (R1) Since we are in a unit test environment, we use the existing mocked I/O device "ball". |

### 1.2 Acting
AERA acts upon the physical world by calling commands on devices. There are two types of devices: internal (the executive, e.g.: inj, eje, mod, set, ...) and external (using custom commands).

#### 1.2.1 Tests for internal commands (icmd) of the executive
| Test name | Description | Remarks |
| :--- | :--- | :--- |
| inj | tests the icmd _inj | (R1) In decompiled_objects, the name of the injected object does not match what is provided.<br>(R2) If the object is injected to group "stdin", the object has no view in the decompilation file. |
| eje | tests the icmd _eje | (R1) This has never been used in existing Replicode files, and I wasn't able to reproduce all that the icmd should be able to do. |
| mod | tests the icmd _mod | - |
| set | tests the icmd _set | (R1) The documentation states that -1 represents an infinite resilience, but this was changed to the keyword "forever". |
| new-class | tests the icmd _new_class | (R1) This has never been used in existing Replicode files, and I wasn't able to reproduce all that the icmd should be able to do. |
| del-class | tests the icmd _del_class | (R1) This has never been used in existing Replicode files, and I wasn't able to reproduce all that the icmd should be able to do. |
| ldc | tests the icmd _ldc | (R1) This has never been used in existing Replicode files, and I wasn't able to reproduce all that the icmd should be able to do. |
| swp | tests the icmd _swp | (R1) This has never been used in existing Replicode files, and I wasn't able to reproduce all that the icmd should be able to do.<br>(R2) Documentation states that only values in {0, 1} should be allowed as a parameter, but this is not the case. |
| prb | tests the icmd _prb | (R1) This only ever been used in combination with "print"; which works as expected. |
| stop | tests the icmd _stop | - |

#### 1.2.2 Tests for external commands

| Test name | Description | Remarks |
| :--- | :--- | :--- |
| cmd | tests basic command syntax | (R1) The parameter device:did (as specified in the documentation) does not exist anymore. |
| bathtub | tests the use of commands to achieve a simple goal | - |

## 2. Constructivism

TODO

## 3. Cumulative learning

This section could be expanded to include more detailed tests on the Activation, Reaction and Attention Control mechanisms.

### 3.1 Multitask learning
The ability to learn more than one task, either at once or in sequence.
AERA achieves this through reactive programs and models, which are triggered whenever there is a left-side match and related saliency/activation values are high enough.

| Test name | Description | Remarks |
| :--- | :--- | :--- |
| programs | tests basic program syntax | -|
| models | tests basic model syntax | -|
| mediation-mod | tests the mediation of `mod` commands | -|
| mediation-set | tests the mediation of `set` commands | (R1) Setting and averaging works, but illegal values are rounded to 0 or 1 (see test for details)|
| bathtub-hgs-sequence | runs the bathtub and hand-grab-sphere tasks in sequence | - |
| bathtub-hgs-parallel | runs the bathtub and hand-grab-sphere tasks in parallel | (R1) Commenting in the 2nd drive in causes AERA to run indefinitely (see test for details)|

### 3.2 Online learning
The ability to learn continuously, uninterrupted, and in real-time from experience as it comes, and without specifically iterating over it many times.  
This section also covers few-shot learning, which means to learn something from very few examples or very little data.

AERA can use new knowledge whenever it comes along.

| Test name | Description | Remarks |
| :--- | :--- | :--- |
| bathtub-new-knowledge | tests if the system can use new knowledge whenever it comes along | - |


### 3.3 Lifelong learning
The ability of an AI system to keep learning and integrating knowledge throughout its operational lifetime: learning is “always on”.
AERA can use new knowledge (or react to changes in saliency/activation) even while/after achieving a more complex goal like hand-grab-sphere.

| Test name | Description | Remarks |
| :--- | :--- | :--- |
| bathtub-act-change | tests the system's reaction to a control value change | - |
| test-ball-external-new-models | tests model creation from facts observed by an external device during a task | - |


### 3.4 Robust knowledge acquisition
The antithesis of which is brittle learning, where new knowledge results in catastrophic perturbations of prior knowledge (and behavior).
AERA achieves this through control values (saliency, activation, resilience, model success rate) that control which knowledge is more reliable than others.

| Test name | Description | Remarks |
| :--- | :--- | :--- |
| bathtub-success-rate-increase | tests model success rate update after correct predictions | - |
| bathtub-success-rate-decrease | tests model success rate update after failed predictions | - |
| bathtub-success-rate-reliable | tests model success rate update on reliable models | - |


### 3.5 Transfer learning
The ability to build new knowledge on top of old in a way that the old knowledge facilitates learning the new. While interference/forgetting should not occur, knowledge should still be defeasible.
In AERA, models exist separately from tasks, so any learned model can be used for any task. 

| Test name | Description | Remarks |
| :--- | :--- | :--- |
| sink | tests the models of the bathtub task on a different but similar task | - |

## 4. Knowledge of causes

Required to produce predictions and plans. AERA achieves this through concatened deduction and abduction (see sections 5.3 and 5.4)

### 4.1 Forward chaining
| Test name | Description | Remarks |
| :--- | :--- | :--- |
| bathtub-forward-chaining | tests the first part of planning: predictions | - |


### 4.2 Backward chaining
| Test name | Description | Remarks |
| :--- | :--- | :--- |
| bathtub-backward-chaining | tests the second part of planning: sub-goals | - |

## 5. Non-axiomatic reasoning

Required to operate on knowledge in worlds where the axioms are unknown, not guaranteed, and/or fallible. 

### 5.1 Defeasible reasoning

Reasoning where the system-produced output may be found to be incorrect, at any time, given additional data, reconsideration of background assumptions or discovery of logic errors. 

| Test name | Description | Remarks |
| :--- | :--- | :--- |
| bathtub-model-defeated | tests the system's behavior after a model's prediction is defeated  | (R1) This test is similar to bathtub-success-rate-decrease in 3.4 |
| bathtub-sim-defeated | checks output after a promoted simulation prediction is defeated  | (R1) This task is incomplete, but it suffices for the purpose here  |

### 5.2 Pattern matching

Pattern matching is central in AERA, where all existing object classes can be matched.

| Test name | Description | Remarks |
| :--- | :--- | :--- |
| ptn | tests basic ptn syntax  | (R1) Patterns cannot exist on their own and are therefore tested as part of other objects. |
| bathtub-pattern-matching | tests various instances of pattern matching ||

