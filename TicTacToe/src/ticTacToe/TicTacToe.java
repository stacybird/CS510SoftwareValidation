package ticTacToe;
import java.util.Scanner;
/**
 * Created by jfortier
 */
public class TicTacToe {
    private static Scanner in = new Scanner(System.in);
    private static Board game;

    private static int winsX = 0;
    private static int winsO = 0;
    private static int ties = 0;

    public static void main(String[] args)
    {

        boolean continuePlaying = true;
        do {
            System.out.println("Welcome to TicTacToe!");
            game = new Board();

            //Tutorial
            System.out.println("Below is the board coordinate system:");
            System.out.println(game.sampleBoard());

            //Poll for player readiness
            String response;
            boolean validResponse = false;
            do {
                System.out.println("Are you ready to play? (y/n)");
                response = in.nextLine();
                if (response.equalsIgnoreCase("y") || response.equalsIgnoreCase("n")) validResponse = true;
                else System.out.println("That input wasn't recognized. Try \"y\" or \"n\".");
            } while (!validResponse);

            //Player is ready; play a game
            if (response.equalsIgnoreCase("y"))
            {
                char nextWinner = play();
                updateScoreBoard(nextWinner);
            }
            else if (response.equalsIgnoreCase("n"))
            {
                continuePlaying = false;
                System.out.println("Thanks for playing!");
            }
            System.out.println("Score...");
            System.out.println("X Won: "+winsX);
            System.out.println("O Won: "+winsO);
            System.out.println("Ties: "+ties);

        } while(continuePlaying);
    }

    private static void updateScoreBoard(char winner)
    {
        if (winner == 'X') winsX++;
        else if (winner == 'T') ties++;
    }

    /**
     * Play one game of TicTacToe.
     */
    private static char play()
    {
        System.out.println(game.currentBoard());
        while(true) {
            //Give X a turn and return if X wins or ties
            turn('X');
            if (game.isOver()) return isWinner('X');

            //Give O a turn and return if O wins or ties
            turn('O');
            if (game.isOver()) return isWinner('O');

            //If neither player wins or ties, continue playing
        }
    }

    /**
     * The specified player takes a turn
     * @param xo
     */
    private static void turn(char xo)
    {
        System.out.println("Player "+xo+", it's your turn!");
        System.out.print("Play position: ");
        String position = in.nextLine();
        int pos = Integer.parseInt(position);
        System.out.println();
        try {
            System.out.println(game.makeMove(xo,pos));
        } catch (IllegalArgumentException e)
        {
            System.out.println("Whoops! That's not a valid move!");
            turn(xo);
        }
    }

    /**
     * Determines whether the game has been won, and
     * attributes the win to the specified player
     * @param xo
     * @return
     */
    private static char isWinner(char xo)
    {
        if (game.hasWin())
        {
            System.out.println("Player "+xo+" wins!");
            return xo;
        } else {
            System.out.println("Tie!");
            return 'T';
        }

    }

    /**
     * Models a TicTacToe board
     * Changed to package local for testing, grr.  -Stacy
     */
    static class Board {

        /**
         * The default empty board
         */
        private char[][] board = {{'_','_','_'},{'_','_','_'},{'_','_','_'}};

        /**
         * The sample board showing the position map
         */
        private final char[][] SAMPLE_BOARD = {{'1','2','3'},{'4','5','6'},{'7','8','9'}};

        private int moves = 0;

        public Board() {}

        /**
         * Places a player piece on the board, if possible, at the indicated position
         * @param xo The player piece
         * @param pos The board position
         * @return A string representation of the current board
         * @throws IllegalArgumentException if the move is not valid
         */
        private String makeMove(char xo, int pos) throws IllegalArgumentException
        {
            int row = getRow(pos);
            int col = getCol(pos);

            if (board[row][col] == '_'){
                board[row][col] = xo;
                moves++;
            }
            else throw new IllegalArgumentException("That move is invalid.");

            return drawBoard(board);
        }

        /**
         * Returns the row index of this position in the board array
         * @param pos
         * @return
         */
        private int getRow(int pos)
        {
            return (pos-1)/3;
        }

        /**
         * Returns the column index of this position in the board array
         * @param pos
         * @return
         */
        private int getCol(int pos)
        {
            int row = getRow(pos);
            return (pos-(row*3))-1;
        }

        /**
         * Returns whether a win condition has been met on the board
         * @return True if the last move resulted in a win condition, false otherwise.
         */
        public boolean hasWin()
        {
            //check for a row win
            for (int r = 0; r < 3; r++)
                if (board[r][0] != '_' && //not empty
                        board[r][0] == board[r][1] &&
                        board[r][1] == board[r][2])
                    return true;

            //check for a col win
            for (int c = 0; c <3; c++)
                if (board[0][c] != '_' && //not empty
                        board[0][c] == board[1][c] &&
                        board[1][c] == board[2][c])
                    return true;

            //check for diag wins
            if (board[0][0] != '_' && //not empty
                    board[0][0] == board[1][1] &&
                    board[1][1] == board[2][2])
                return true;
            if (board[0][2] != '_' && //not empty
                    board[0][2] == board[1][1] &&
                    board[1][1] == board[2][0])
                return true;

            //no win found
            return false;
        }

        /**
         * Returns whether all moves have been exhausted on the board
         * @return
         */
        public boolean isOver()
        {
            if (moves >= 9) return true;
            else if (hasWin()) return true;
            return false;
        }

        /**
         * Returns a string representation of a TicTacToe board
         * @param someBoard
         * @return
         * made package local for testing, bah -Stacy
         */
        String drawBoard(char[][] someBoard)
        {
            String boardChars = " ___ ___ ___ \n";
            for(int row = 0; row < someBoard.length; row++)
            {
                boardChars += "|";
                char[] rowArray = someBoard[row];
                for (int col = 0; col < rowArray.length; col++)
                {
                    boardChars += "_"+rowArray[col]+"_|";
                    boardChars += "\n";
                }
            }
            return boardChars;
        }

        public String sampleBoard(){return drawBoard(SAMPLE_BOARD);}
        public String currentBoard(){return drawBoard(board);}
    }
}
