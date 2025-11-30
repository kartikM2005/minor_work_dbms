# Java Excel Reader Demo

This is a simple Java program that demonstrates how to read an Excel file (`.xlsx`) using the Apache POI library.

## Prerequisites

- Java Development Kit (JDK) 8 or higher
- Maven

## Setup

1.  Ensure you have an Excel file (e.g., `sample.xlsx`) ready.
2.  The `pom.xml` file contains the necessary dependencies for Apache POI.

## How to Run

You can run the program using Maven:

```bash
mvn clean compile exec:java
```

When prompted, enter the path to your Excel file (e.g., `sample.xlsx`).

## Code Description

The `ExcelReader.java` file contains the logic:
1.  Prompts the user for the Excel file path.
2.  Opens the specified file using `FileInputStream`.
2.  Creates an `XSSFWorkbook` instance.
3.  Gets the first sheet.
4.  Iterates through rows and cells.
5.  Prints the cell values to the console based on their type (String, Numeric, Boolean, etc.).
