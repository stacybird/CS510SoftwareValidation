import org.junit.*;

import static org.junit.Assert.*;

/**
 * Created by stacy on 5/30/15.
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
        pnf = new PhoneNumberField();
    }

    @After
    public void tearDown() throws Exception {
        System.out.print("\t\t.....test has ended\n");
    }

    @Test
    public void testGetPhoneNumber() throws Exception {

    }

    @Test
    public void testFormatPhoneNumber() throws Exception {

    }

    @Test
    public void testParseContentsForPhoneNumber() throws Exception {

    }
}