package ticTacToe;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

import static org.junit.Assert.*;

/**
 * Created by stacy on 4/30/15.
 * This is an example on testing the public contract
 */
public class TicTacToeTest {
    private final ByteArrayOutputStream outContent = new ByteArrayOutputStream();
    private final ByteArrayOutputStream errContent = new ByteArrayOutputStream();

    @Before
    public void setUp() throws Exception {
        System.setOut(new PrintStream(outContent));
        System.setErr(new PrintStream(errContent));
    }

    @After
    public void tearDown() throws Exception {
        System.setOut(null);
        System.setErr(null);
    }

    @Test
    public void testMain() throws Exception {
        String expected = "Below is the board coordinate system:";
        String[] arguments = new String [0];
        TicTacToe.main(arguments);
        assertEquals(expected, outContent.toString());

    }



}