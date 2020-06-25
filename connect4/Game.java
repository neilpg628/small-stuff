import java.util.*;

public class Game {
	
	public static void main(String[] args) {
		Scanner input = new Scanner(System.in);
		Game.play(input);
	}
	
	public static int play(Scanner input) {
		
		Board b = new Board();
		int columnPlayed = 3; int player;
		

		b.addDisk(firstMovePlayer1(), 1);
		int nbTurn = 1;
		
		while (nbTurn < 42){ 
			player = nbTurn %2 + 1;
			if (player == 2){
				columnPlayed = getNextMove(input, b, 2);
			}
			if (player == 1){
				columnPlayed = movePlayer1(columnPlayed, b);
			}

			b.addDisk(columnPlayed, player);
			if (b.isWinning(columnPlayed, player)){
				b.print();
				System.out.println("Congrats to Player " + player + "!");
				return(player);
			}
			nbTurn++;
		}
		return -1;
	}
	
	public static int getNextMove(Scanner input, Board b, int player) {
		
		b.print();
		System.out.println("Enter a column:");
		int col = input.nextInt();
		
		while(col < 0 || col > 6 || b.available[col] > 5 ) {
			System.out.println("That value is out of range or its corresponding column is full! Enter another:");
			col = input.nextInt();
		}
		
		return col;
	}
	
	public static int firstMovePlayer1 (){
		return 3;
	}
	
	public static int movePlayer1 (int columnPlayed2, Board b){
		
		int col = b.canWinNextRound(1);
		if(col != -1) return col;
			
		col = b.canWinTwoTurns(1);
		if(col != -1) return col;
		
		col = b.canWinNextRound(2);
		if(col != -1) return col;
		
		if(b.available[columnPlayed2] < 6) return columnPlayed2;
			
		for(int i = 1; i < 7; i++) {
			if(columnPlayed2 >= i && b.available[columnPlayed2 - i] < 6) return columnPlayed2 - i;
			if(columnPlayed2 + i < 7 && b.available[columnPlayed2 + i] < 6) return columnPlayed2 + i;
		}
		
		return -1;
	}
	
}
