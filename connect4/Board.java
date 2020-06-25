public class Board {
	
	public int[][] board;
	public int[] available;
	boolean spaceLeft;
	
	public Board(){
		board = new int[7][6];
		available = new int[7];
		spaceLeft = true;
	}
	
	public void print(){
		System.out.println("| 0 | 1 | 2 | 3 | 4 | 5 | 6 |");
		System.out.println("+---+---+---+---+---+---+---+");
		for (int i = 0; i < 6; i++){
			System.out.print("|");
			for (int j = 0; j < 7; j++){
				if (board[j][5-i] == 0){
					System.out.print("   |");
				}
				else{
					System.out.print(" "+ board[j][5-i]+" |");
				}
			}
			System.out.println();
		}
	}
	
	public void addDisk (int index, int player){
		board[index][available[index]] = player;
		if(available[index] < 7) available[index]++;
		int s = 0;
		for(int i : available) s++;
		spaceLeft = (s != 42);
	}
	
	public boolean isWinning (int lastColumnPlayed, int player){
		
		int height = available[lastColumnPlayed] - 1;
		// VERTICAL CASE
		if(height > 2) {
			boolean vert = true;
			for(int i = 1; i < 4; i++) {
				if(board[lastColumnPlayed][height - i] != player) {
					vert = false;
					break;
				}
			}
			if(vert) return true;
		}
		
		int numFound = 1;
		
		for(int i = 1; true; i++) {
			if(!isValid(lastColumnPlayed - i, height - i)) break;
			if(board[lastColumnPlayed - i][height - i] == player)
				numFound++;
			else break;
		}
		
		for(int i = 1; true; i++) {
			if(!isValid(lastColumnPlayed + i, height + i)) break;
			if(board[lastColumnPlayed + i][height + i] == player)
				numFound++;
			else break;
		}
		
		if(numFound >= 4) return true;
		else numFound = 1;
		
		for(int i = 1; true; i++) {
			if(!isValid(lastColumnPlayed - i, height + i)) break;
			if(board[lastColumnPlayed - i][height + i] == player)
				numFound++;
			else break;
		}
		
		for(int i = 1; true; i++) {
			if(!isValid(lastColumnPlayed + i, height - i)) break;
			if(board[lastColumnPlayed + i][height - i] == player)
				numFound++;
			else break;
		}
		
		if(numFound >= 4) return true;
		else numFound = 1;
		
		for(int i = 1; true; i++) {
			if(!isValid(lastColumnPlayed - i, height)) break;
			if(board[lastColumnPlayed - i][height] == player)
				numFound++;
			else break;
		}
		
		for(int i = 1; true; i++) {
			if(!isValid(lastColumnPlayed + i, height)) break;
			if(board[lastColumnPlayed + i][height] == player)
				numFound++;
			else break;
		}
		
		return (numFound >= 4);
		
	}
	
	public int canWinNextRound (int player){
		
		for(int i = 0; i < 7; i++) {
			
			if(available[i] > 5) continue;
			
			board[i][available[i]] = player;
			available[i]++;
			
			boolean win = isWinning(i, player);
			
			available[i]--;
			board[i][available[i]] = 0;
			
			
			if(win) return i;
		}

		return -1; 
	}
	
	public int canWinTwoTurns (int player){
		
		int otherPlayer = player % 2 + 1;
		
		for(int i = 0; i < 7; i++) {
			if(available[i] > 5) continue;
			board[i][available[i]] = player;
			available[i]++;
			boolean good = true;
			if(canWinNextRound(otherPlayer) != -1) {
				available[i]--;
				board[i][available[i]] = 0;
				continue;
			} 
			
			else {
				
				for(int j = 0; j < 7; j++) {
					if(available[j] > 5) continue;
					board[j][available[j]] = otherPlayer;
					available[j]++;
					
					good = good && (canWinNextRound(player) != -1);
					
					available[j]--;
					board[j][available[j]] = 0;
					if(!good) break;
				}
			}
				
			available[i]--;
			board[i][available[i]] = 0;

			if(good) return i;
		}
		
		return -1; 
	}
	
	private boolean isValid(int i, int j) {return i >= 0 && i < 7 && j >= 0 && j < 6;}
	
}
