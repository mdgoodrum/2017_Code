/**
 * CS 2110 Spring 2017 HW2
 * Part 3 - Coding with bitwise operators
 *
 * @author Michael Goodrum
 *
 * Global rules for this file:
 * - All of these functions must be completed in ONE line. That means they should be
 *   of the form "return [...];", with a single semicolon. No partial credit will be
 *   awarded for any Method that isn't completed in one line.
 *     Hint: If you are stuck trying to complete one of these functions in one line,
 *     try completing them in multiple lines first, given the constraints below. For
 *     example, if you wanted to calculate the distance between two points:
 *       int x1, x2, y1, y2;
 *       int dx = x1 - x2;
 *       int dx2 = dx * dx;
 *       int dy = y1 - y2;
 *       int dy2 = dy * dy;
 *       int zsum = dx2 + dy2;
 *       int ans = (int) sqrt(zsum);
 *     By inspection, you can substitute the expressions into different lines:
 *       int x1, x2, y1, y2;
 *       int dx2 = (x1 - x2) * (x1 - x2)
 *       int dy2 = (y1 - y2) * (y1 - y2)
 *       int ans = (int) sqrt(dx2 + dy2)
 *     And finally:
 *       int x1, x2, y1, y2;
 *       int ans = (int) sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
 *     If your multiline solution can't be reduced to 1 line, you're doing it wrong.
 *
 * - You may not use more than 2 conditionals per method, which for this file would
 *   just be ternary expressions.
 * - You may not declare any variables.
 * - You may not use casting.
 * - You may not use the unsigned right shift operator (>>>)
 * - You may not write any helper methods, or call any other method from this or
 *   another file to implement any method.
 * - In functions where addition or subtraction is allowed, you may only do so
 *   with the number 1.
 *
 * Method-specific rules for this file:
 * - You may not use multiplication, division or modulus in any method.
 * - You may not use addition, subtraction, or conditionals in setByte,
 *   getShort, pack, or xor.
 * - You noy not use bitshifting or the exclusive OR operator (^) in xor.
 *
 * Finally, your code must be robust and concise. If we asked you to print out the values 1 through 100
 * and you wrote 100 separate print statements, then sure, it works, but no one's gonna hire a coder who does
 * that. Likewise, you will NOT get credit for verbose answers for which there is a much more concise
 * solution. For instance, if you need to shift a value by n*4 times, you may not write x << n << n << n << n
 * or you will get no credit, because there is a much more concise way to do this in only two operations by
 * first shifting n. Keep this in mind, ESPECIALLY in the first 2 functions in this file.
 *
 * Remember that for this assignment, All bit masks must be written in hexadecimal.
 * This is the convention for masks and makes it much easier for the programmer to
 * understand the code. If you write a mask in any other base you will lose points.
 *
 * All of these functions accept ints as parameters because if you pass in a number
 * (which is of type int by default) into a Method accepting a byte, then the Java
 * compiler will complain even if the number would fit into that type.
 *
 * Now, keep in mind the return value is also an int. Please read the comments about how
 * many significant bits to return and make sure that the other bits are not set or else
 * you will not get any points for that test case.
 * i.e if I say to return 6 bits and you return 0xFFFFFFFF, you will lose points.
 *
 * Definitions of types:
 * nibble - 4 bits
 * byte   - 8 bits
 * short  - 16 bits
 * int    - 32 bits
 */
public class Operations
{
    /**
     * Get a 16-bit short from an int.
     *
     * Ints are made of 2 shorts, numbered like so:
     *   S1 | S0
     *
     * For a graphical representation of the bits:
     *   3322222222221111111111
     *   10987654321098765432109876543210 <= starting with bit 0
     *   ---------------+---------------+
     *       Short 1    |    Short 0    |
     *
     * Examples:
     *     getShort(0x56781234, 0); // => 0x1234
     *     getShort(0xFF254545, 1); // => 0xFF25
     *
     * Note: Remember, no multiplication allowed!
     *
     * @param num The int to get a short from.
     * @param which Determines which nibble gets returned - 0 for least-significant short.
     *
     * @return A short corresponding to the "which" parameter from num.
     */
    int getShort(int num, int which)
    {
        return num >> (which << 4) & 0xffff;
    }

    /**
     * Set an 8-bit byte in an int.
     *
     * Ints are made of 4 bytes, numbered like so:
     *   B3 | B2 | B1 | B0
     *
     * For a graphical representation of the bits:
     *   3322222222221111111111
     *   10987654321098765432109876543210 <= starting with bit 0
     *   -------+-------+-------+-------+
     *      B3  |   B2  |   B1  |   B0  |
     *
     * Examples:
     *     setByte(0xAAA5BBC6, 0x12, 1); // => 0xAAA512C6
     *     setByte(0x56B218F9, 0xFF, 3); // => 0xFF7818F9
     *
     * Note: Remember, no multiplication allowed!
     *
     * @param num The int that will be modified.
     * @param a_byte The byte to insert into the integer.
     * @param which Selects which byte to modify - 0 for least-significant short.
     *
     * @return The modified int.
     */
    int setByte(int num, int a_byte, int which)
    {
        return (a_byte << (which << 3)) | (num & ~(0xFF << (which << 3)));
    }

    /**
     * Pack 4 bytes into an int.
     *
     * The bytes should be placed consecutively in the 32-bit int in the order
     * specified by the parameters.
     *
     * Example:
     *     pack(0x12, 0x34, 0x56, 0x78); // => 0x12345678
     *     pack(0xDE, 0xAD, 0xBE, 0xEF); // => 0xDEADBEEF
     *
     * @param b3 Most significant byte (will always be an 8-bit number).
     * @param b2 2nd byte (will always be an 8-bit number).
     * @param b1 3rd byte (will always be an 8-bit number).
     * @param b0 Least significant byte (will always be an 8-bit number).
     *
     * @return a 32-bit value formatted like so: b3b2b1b0
     */
    int pack(int b3, int b2, int b1, int b0)
    {
        return (b3 << 24) | (b2 << 16) | (b1 << 8) | (b0);
    }

    /**
     * Extract a range of bits from a number.
     *
     * Examples:
     *     bitRange(0x00001234, 0, 4);  // => 0x00000004
     *     bitRange(0x00001234, 4, 8);  // => 0x00000023
     *     bitRange(0x12345678, 0, 28); // => 0x02345678
     *     bitRange(0x55555555, 5, 7);  // => 0x0000002A
     *
     * Note: We will only pass in values 1 to 32 for n.
     *
     * @param num An n-bit 2's complement number.
     * @param s The starting bit to grab
     * @param n The number of bits to return.
     * @return The n-bit number num[s:s+n-1].
     */
    int bitRange(int num, int s, int n)
    {
        return (num >> s) & ((1 << n) - 1);
    }

    /**
     * NOTE: For this method, you may only use &, |, and ~.
     *
     * Perform an exclusive-or on two 32-bit ints.
     *
     * Examples:
     *     xor(0xFF00FF00, 0x00FF00FF); // => 0xFFFFFFFF
     *     xor(0x12345678, 0x87654321); // => 0x95511559
     *
     * @param num1 An int
     * @param num2 Another int
     *
     * @return num1 ^ num2
     */
    int xor(int num1, int num2)
    {
        return (num1 | num2) & (~num1 | ~num2);
    }

    /**
     * Return true if the given number is a power of 2.
     *
     * Examples:
     *     powerOf2(1024); // => true
     *     powerOf2(23);   // => false
     *
     * Note: Make sure you handle ALL the cases!
     * Note2: Remember: Robust and concise! Do not just return an OR of all the powers of 2.
     *
     * @param num a 32-bit int. Since this is an int, it is SIGNED!
     * @return true if num is a power of 2.
     */

    boolean powerOf2(int num)
    {
        return ((num & (num - 1)) == 0) && num > 0;
    }
}
