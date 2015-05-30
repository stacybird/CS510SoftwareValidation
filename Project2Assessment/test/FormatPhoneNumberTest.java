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
    public void sevenDigitPhoneNumberWithoutExtension() throws Exception {
        phone = new int[] {4,7,2,9,5,0,1};
        extension = new int[] {};
        expected = "472-9501";
        result = pnf.formatPhoneNumber(phone, extension);
        assertEquals(expected, result);
    }

    @Test
    public void tenDigitPhoneNumberWithoutExtension() throws Exception {
        phone = new int[] {5,0,3,4,7,2,9,5,0,1};
        extension = new int[] {};
        expected = "(503) 472-9501";
        result = pnf.formatPhoneNumber(phone, extension);
        assertEquals(expected, result);
    }

    @Test
    public void elevenDigitPhoneNumberWithoutExtension() throws Exception {
        phone = new int[] {1,5,0,3,4,7,2,9,5,0,1};
        extension = new int[] {};
        expected = "+1 (503) 472-9501";
        result = pnf.formatPhoneNumber(phone, extension);
        assertEquals(expected, result);
    }

    @Test
    public void elevenDigitPhoneNumberWithExtension() throws Exception {
        phone = new int[] {1,5,0,3,4,7,2,9,5,0,1};
        extension = new int[] {5,0,0,0};
        expected = "+1 (503) 472-9501 ext. 5000";
        result = pnf.formatPhoneNumber(phone, extension);
        assertEquals(expected, result);
    }

    @Test(expected = IllegalStateException.class)
    public void invalidLength() throws Exception {
        phone = new int[] {1,5,0,3,4,7,2,9,5};
        extension = new int[] {5,0,0,0};
        expected = "+1 (503) 472-9501 ext. 5000";
        result = pnf.formatPhoneNumber(phone, extension);

        assertEquals(expected, result);
    }

}
