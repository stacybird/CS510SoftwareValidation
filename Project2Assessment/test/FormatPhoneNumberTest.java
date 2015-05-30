import org.junit.Test;

import static org.junit.Assert.assertEquals;

/**
 * Created by stacy on 5/30/15.
 *
 */
public class FormatPhoneNumberTest extends PhoneNumberFieldTest {
    String result;
    String expected;
    private int[] phone;
    private int[] extension;

    @Test
    public void testFormatPhoneNumber() throws Exception {
        testNumber = "4729501";
        newPhoneField(testNumber);

    }

    @Test
    public void sevenDigitPhoneNumberWithoutExtension() throws Exception {
        phone = new int[] {4,7,2,9,5,0,1};
        extension = new int[] {};
        expected = "472-9501";
        result = pnf.formatPhoneNumber(phone, extension);
        assertEquals(expected, result);
    }

    @Test
    public void tenDigitPhoneNumberWithoutExtension() throws Exception {
        testNumber = "5034729501";
        newPhoneField(testNumber);
        pnf.parseContentsForPhoneNumber();
        assertEquals(testNumber, pnf.contents);
    }

    @Test
    public void elevenDigitPhoneNumberWithoutExtension() throws Exception {
        testNumber = "15034729501";
        newPhoneField(testNumber);
        assertEquals(testNumber, pnf.contents);
    }

    @Test
    public void elevenDigitPhoneNumberWithExtension() throws Exception {
        testNumber = "150347295015000";
        newPhoneField(testNumber);
        assertEquals(testNumber, pnf.contents);
    }
}
