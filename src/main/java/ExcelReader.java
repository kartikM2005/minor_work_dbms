import org.apache.poi.ss.usermodel.*;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import java.util.Scanner;

public class ExcelReader {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter the path to the Excel file: ");
        String excelFilePath = scanner.nextLine();
        scanner.close();

        // Using try-with-resources to ensure the file stream and workbook are closed
        // automatically
        try (FileInputStream inputStream = new FileInputStream(new File(excelFilePath));
                Workbook workbook = new XSSFWorkbook(inputStream)) {

            // Get the first sheet from the workbook
            Sheet sheet = workbook.getSheetAt(0);

            // Iterate through each row in the sheet
            for (Row row : sheet) {
                // Iterate through each cell in the row
                for (Cell cell : row) {
                    // Check the cell type and print the value accordingly
                    switch (cell.getCellType()) {
                        case STRING:
                            System.out.print(cell.getStringCellValue() + "\t");
                            break;
                        case NUMERIC:
                            System.out.print(cell.getNumericCellValue() + "\t");
                            break;
                        case BOOLEAN:
                            System.out.print(cell.getBooleanCellValue() + "\t");
                            break;
                        case FORMULA:
                            System.out.print(cell.getCellFormula() + "\t");
                            break;
                        default:
                            System.out.print("UNKNOWN\t");
                    }
                }
                // Move to the next line after printing all cells in a row
                System.out.println();
            }

        } catch (IOException e) {
            System.err.println("Error reading Excel file: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
