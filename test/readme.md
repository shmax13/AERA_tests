# AERA Test Framework

The `test` project provides an extensible test framework for AERA. Tests consist of Replicode and TXT
input-output pairs. Replicode files provide the seed with which AERA is run, while TXT files provide the lines
which are expected to show up in AERA's decompiled output.

For detailed test documentation, see `report.md`.

## Setup

### Prerequisites
* **Google Mock (gmock) v1.11.0**: This is managed via NuGet.  
To install it, right-click the Solution in Visual Studio and select **Restore NuGet Packages**.   
*Note: The specific dependency configuration can be found in `packages.config`.*

### File & Folder Structure
* **Viewing the Folder Tree:** By default, Visual Studio maps files using virtual filters, which will cause all test files to appear in one flat root list. To see the actual, organized directory structure on disk, click the **Show All Files** icon at the top of the Solution Explorer window.

### Building & Running Tests
* **Manual Build Required:** By default, the `test` project is excluded from the global solution build to avoid forcing dependencies on other workflows. To compile it, right-click the `test` project explicitly and select **Build**.
* **Target Configuration:** The test framework is configured to build using **Release** + **Win32**. If you need to use a different configuration, you must manually update the file paths defined at the top of `test_all.cpp`.
* **Discovering & Running Tests:** Once the build is complete, all tests will be automatically discovered inside the Visual Studio *Test Explorer* window, where they can be run or debugged. 
