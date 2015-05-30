package Project2Assessment.test;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by stacy on 5/30/15.
 *
 * Notes from instructor:
 *
 * Write the bodies for the following test methods, plus any additional test
 * methods that would be appropriate.  Use helper methods and inheritance
 * where it makes sense (assertions should never go anywhere but a test
 * method body).  Regardless of the test result, an appropriate message should
 * print to the console when:
 *  -- Testing of this class begins,
 *  -- Any particular test begins,
 *  -- Any particular test ends, and
 *  -- Test of this class ends.
 */
public class FormatPhoneNumberTest {

    @Before
    public void setUp() throws Exception {
        System.out.print("\t\tStarting test.....\n");
    }

    @After
    public void tearDown() throws Exception {
        System.out.print("\t\t.....test has ended\n");
    }

    @Test
    public void sevenDigitPhoneNumberWithoutExtension() throws Exception {

    }

    @Test
    public void tenDigitPhoneNumberWithoutExtension() throws Exception {

    }
    @Test
    public void elevenDigitPhoneNumberWithoutExtension() throws Exception {

    }
    @Test
    public void elevenDigitPhoneNumberWithExtension() throws Exception {

    }
}