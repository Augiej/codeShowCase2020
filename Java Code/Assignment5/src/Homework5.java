
import java.io.File;
import java.io.FileNotFoundException;
import java.util.LinkedList;
import java.util.Scanner;
import java.util.Stack;

public class Homework5 {

	public static void main(String[] args) {
		File file = new File("C:\\Users\\Augie\\eclipse-workspace\\Assignment5\\src\\figure22_9");
		Graph a = getGraph(file);
		SCC(a);
	}
	//reads a text document for the data to make the graph
	public static Graph getGraph(File x) {
		try {
			Scanner a = new Scanner(x);
			String size = a.nextLine();
			int v = Integer.parseInt(size);
			Graph result = new Graph(v);
			String pair;
			while(a.hasNextLine()) {
				pair = a.nextLine();
				String[] split = pair.split(" ");
				int first = Integer.parseInt(split[0]);
				int second = Integer.parseInt(split[1]);
				result.addAdj(first, second);
			}
			a.close();
			return result;
		}
		catch(FileNotFoundException e) {
			System.out.println(e);
		}
		return null;
		
	}
	//Function returns the SCCs by printing them at the end through the DFS recur function
		public static void SCC(Graph a) {
			Stack<Integer> stack = new Stack<Integer>();
			boolean [] visit = new boolean[a.size];
			//set the boolean array to false because none have been visited
			for(int i = 0; i < a.size; i ++) {
				visit[i] = false;
			}
			//perform the BFS keeping track of the stack
			for(int i = 0; i < a.size; i++) {
				if(!visit[i]) {
					a.SCCStack(i, visit, stack);
				}
			}
			System.out.println(stack);
			Graph transposed = a.transpose();
			//reset the visit boolean array
			for(int i = 0; i < a.size; i ++) {
				visit[i] = false;
			}
			while(!stack.empty()){
				int index = stack.pop();
				if(!visit[index]) {
					transposed.DFSRecur(index, visit);
					System.out.println();
				}
			}
		}
	//inner class that has the functionality of a graph
	static class Graph{
		private LinkedList<Integer> adjacent[];
		int size;
		public Graph(int setSize) {
			size = setSize;
			adjacent = new LinkedList[size];
			for(int i = 0; i < size; i++) {
				adjacent[i] = new LinkedList<Integer>();
			}
		}
		void addAdj(int a, int b) {
			adjacent[a].add(b);
		}
		void print() {
			for(int i = 0; i < size; i++) {
				for(int j = 0; j < adjacent[i].size(); j++) {
					System.out.print(adjacent[i].get(j) + " ");
				}
				System.out.println();
			}
		}
		//function for testing the DFS function
		void DFS(int start) {
			boolean[] visit = new boolean[this.size];
			DFSRecur(start, visit);
		}
		//recursive part of the DFS function that prints the SCCs
		void DFSRecur(int index, boolean [] visit) {
			visit[index] = true;
			System.out.print(index + " ");
			for(int i = 0; i < adjacent[index].size(); i++) {
				if(!visit[adjacent[index].get(i)]) {
					DFSRecur(adjacent[index].get(i), visit);
				}
			}
		}
		//DFS function but adds values to stack depending on the finish times
		void SCCStack(int index, boolean [] visit, Stack<Integer> stack) {
			visit[index] = true;
			for(int i = 0; i < adjacent[index].size(); i++) {
				if(!visit[adjacent[index].get(i)]) {
					SCCStack(adjacent[index].get(i), visit, stack);
				}
			}
			stack.push(index);
		}
		
		Graph transpose() {
			Graph transposed = new Graph(this.size);
			for(int i = 0; i < size; i++) {
				for(int j = 0; j < this.adjacent[i].size(); j++) {
					transposed.addAdj(this.adjacent[i].get(j), i); 
				}
			}
			return transposed;
		}
	}
	

}
