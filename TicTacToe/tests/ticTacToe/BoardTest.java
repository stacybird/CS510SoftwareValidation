package ticTacToe;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by stacy on 5/7/15.
 */
public class BoardTest {
    TicTacToe.Board myBoard;

    @Before
    public void setUp() throws Exception {
        myBoard = new TicTacToe.Board();
    }

    @Test
    public void testHasWinEmptyBoard() throws Exception {
        boolean actual = myBoard.hasWin();
        boolean expected = false;
        assertEquals("Expected empty board to be no-win", expected, actual);
    }

    @Test
    public void testIsOver() throws Exception {
        boolean actual = myBoard.isOver();

        assertTrue(false);

    }

    @Test
    public void testDrawBoard() throws Exception {
        assertTrue(false);

    }

    @Test
    public void testSampleBoard() throws Exception {
        assertTrue(false);

    }

    @Test
    public void testCurrentBoard() throws Exception {
        assertTrue(false);

    }

    @After
    public void tearDown() throws Exception {
    }
}