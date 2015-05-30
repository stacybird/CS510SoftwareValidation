import org.junit.*;

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
public class PhoneNumberFieldTest {

    protected PhoneNumberField pnf;

    @BeforeClass
    public static void runBeforeClass() {
        System.out.println("Start testing FormatPhoneNumber Class");
    }

    @AfterClass
    public static void runAfterClass() {
        System.out.println("End testing FormatPhoneNumber Class");
    }

    @Before
    public void setUp() throws Exception {
        System.out.print("\t\tStarting test.....\n");
    }

    @After
    public void tearDown() throws Exception {
        System.out.print("\t\t.....test has ended\n");
    }

    public void newPhoneField(String number) {
        pnf = new PhoneNumberField(number);
    }

/*
    @Test
    public void testGetPhoneNumber() throws Exception {

    }

    @Test
    public void testFormatPhoneNumber() throws Exception {

    }

    @Test
    public void testParseContentsForPhoneNumber() throws Exception {

    }
*/
}
