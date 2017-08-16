/**
 * CS 2110 Spring 2017 HW2
 * Part 2 - Coding with bases
 *
 * @author Michael Goodrum
 *
 * Global rules for this file:
 * - You may not use more than 2 conditionals per method. Conditionals are
 *   if-statements, if-else statements, or ternary expressions. The else block
 *   associated with an if-statement does not count toward this sum.
 * - You may not use more than 2 looping constructs per method. Looping
 *   constructs include for loops, while loops and do-while loops.
 * - You may not use nested loops.
 * - You may not declare any file-level variables.
 * - You may not declare any objects, other than String in select methods.
 * - You may not use switch statements.
 * - You may not use the unsigned right shift operator (>>>)
 * - You may not write any helper methods, or call any other method from this or
 *   another file to implement any method.
 * - The only Java API methods you are allowed to invoke are:
 *     String.length()
 *     String.charAt()
 * - You may not invoke the above methods from string literals.
 *     Example: "12345".length()
 * - When concatenating numbers with Strings, you may only do so if the number
 *   is a single digit.
 *
 * Method-specific rules for this file:
 * - You may not use multiplication, division or modulus in any method, EXCEPT
 *   decimalStringToInt.
 * - You may declare exactly one String variable each in intToBinaryString and
 *   and intToHexString.
 */
public class Bases
{
    /**
     * Convert a string containing ASCII characters (in binary) to an int.
     * You do not need to handle negative numbers. The Strings we will pass in will be
     * valid binary numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: binaryStringToInt("111"); // => 7
     */
    public static int binaryStringToInt(String binary)
    {
        int intValue = 0;
        int base = 2;
        int currentBase = 0;
        for (int x = binary.length() - 2; x >= 0; x--) {
            currentBase = currentBase + base;
            base = currentBase;
            if (binary.charAt(x) - 48 == 1) {
                intValue += currentBase;
            }
        }
        intValue += binary.charAt(binary.length() - 1) - 48;
        return intValue;
    }

    /**
     * Convert a string containing ASCII characters (in decimal) to an int.
     * You do not need to handle negative numbers. The Strings we will pass in will be
     * valid decimal numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: decimalStringToInt("123"); // => 123
     */
    public static int decimalStringToInt(String decimal)
    {
        if (decimal.length() == 0) {
            return 0;
        }
        if (decimal.length() == 1) {
            return decimal.charAt(0) - 48;
        }
        int decimalInt = 0;
        int power = 10;
        final int DEC = 10;
        for (int x = decimal.length() - 3; x >= 0; x--) {
            power = power * DEC;
            decimalInt += (decimal.charAt(x) - 48) * power;
        }
        decimalInt += decimal.charAt(decimal.length() - 1) - 48;
        decimalInt += (decimal.charAt(decimal.length() - 2) - 48) * DEC;
        return decimalInt;
    }

    /**
     * Convert a string containing ASCII characters (in hex) to an int.
     * The input string will only contain numbers and uppercase letters A-F.
     * You do not need to handle negative numbers. The Strings we will pass in will be
     * valid hexadecimal numbers, and able to fit in a 32-bit signed integer.
     *
     * Example: hexStringToInt("A6"); // => 166
     */
    public static int hexStringToInt(String hex)
    {
        int val = 0;
        for (int x = 0; x < hex.length(); ++x) {
            int dig = hex.charAt(x);
            if (dig > 57) {
                dig -= 55;
            } else {
                dig -= 48;
            }
            val += dig << ((hex.length() - 1 - x) << 2);
        }
        return val;
    }

    /**
     * Convert a int into a String containing ASCII characters (in binary).
     * You do not need to handle negative numbers.
     * The String returned should contain the minimum number of characters necessary to
     * represent the number that was passed in.
     *
     * Example: intToBinaryString(7); // => "111"
     */
    public static String intToBinaryString(int binary)
    {
        String binaryString = "";
        int counter = 0;
        int mask = 1 << 31;
        for (int x = 31; x >= 0; x--) {
            if (((1 << x) & binary) != 0) {
                binaryString += "1";
                counter++;
            } else if (counter > 0 | x == 0) {
                binaryString += "0";
            }
        }
        return binaryString;
    }

    /**
     * Convert a int into a String containing ASCII characters (in hexadecimal).
     * The output string should only contain numbers and uppercase letters A-F.
     * You do not need to handle negative numbers.
     * The String returned should contain the minimum number of characters necessary to
     * represent the number that was passed in.
     *
     * Example: intToHexString(166); // => "A6"
     */
    public static String intToHexString(int hex)
    {
        String str = "";
        int counter = 0;
        for (int x = 7; x >= 0; --x) {
            int nibble = (hex >> (x << 2)) & 0xF;
            if (nibble > 9) {
                str += (char)(nibble + 55);
                counter++;
            } else if (counter > 0 | nibble != 0 | x == 0) {
               str += (char)(nibble + 48);
               counter++;
            }
        }
        return str;
    }
}
