import org.junit.Test;

import static org.junit.Assert.assertEquals;

/**
 * Created by stacy on 5/30/15.
 *
 */
public class FormatPhoneNumberTest extends PhoneNumberFieldTest {

    String testNumber;

    @Test
    public void sevenDigitPhoneNumberWithoutExtension() throws Exception {
        testNumber = "4729501";
        newPhoneField(testNumber);
        assertEquals(testNumber, pnf.contents);
    }

    @Test
    public void tenDigitPhoneNumberWithoutExtension() throws Exception {
        testNumber = "5034729501";
        newPhoneField(testNumber);
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
