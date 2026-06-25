# AERA Test Framework

TODO:  
The `test` framework was developed as part of the thesis...  
For a detailed test documentation, see `report.md`.

## Setup

### Prerequisites
* **Google Mock (gmock) v1.11.0**: This is managed via NuGet.  
To install it, right-click the Solution in Visual Studio and select **Restore NuGet Packages**.   
*Note: The specific dependency configuration can be found in `packages.config`.*

### Building & Running Tests
* **Manual Build Required:** By default, the `test` project is excluded from the global solution build to avoid forcing dependencies on other workflows. To compile it, right-click the `test` project explicitly and select **Build**.
* **Target Configuration:** The test framework is configured to build using **Release** + **Win32**. If you need to use a different configuration, you must manually update the file paths defined at the top of `test_all.cpp`.
* **Discovering & Running Tests:** Once the build is complete, all tests will be automatically discovered inside the Visual Studio *Test Explorer* window, where they can be run or debugged. 