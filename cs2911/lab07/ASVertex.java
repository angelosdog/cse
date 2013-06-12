
public class ASVertex {
	private Vertex v;
	private int c;
	private int h;
	private ASVertex previous;
	
	public ASVertex(Vertex v, int c, int h, ASVertex previous) {
		this.v = v;
		this.c = c;
		this.h = h;
		this.previous = previous;
	}
	
	public Vertex getVertex() {
		return this.v;
	}
	
	public int getCost() {
		return this.c;
	}
	public int getHeuristic() {
		return this.h;
	}
	public void setCost(int c) {
		this.c = c;
	}
	
	public void setHeuristic(int h) {
		this.h = h;
	}
	
	public ASVertex getPrevious() {
		return this.previous;
	}
}
