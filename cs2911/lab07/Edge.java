/**
 * Represents an Edge in the graph as going between two vertices
 * @author Timothy Wiley
 */
public class Edge {
	private final Vertex from;
	private final Vertex to;
	private final int weight;

	/**
	 * Construct an edge given two vertices
	 * @param from Vertex the edge is from
	 * @param to Vertex the edge is to
	 */
	public Edge(Vertex from, Vertex to, int weight) {
		this.from = from;
		this.to = to;
		this.weight = weight;
	}

	public Vertex getFrom() {
		return from;
	}

	public Vertex getTo() {
		return to;
	}
	
	@Override
	/**
	 * Two edges are equal if their from.equal() and to.equal() return true
	 */
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;

		Edge other = (Edge) obj;
		return from.equals(other.from) && to.equals(other.to);
	}
	
	@Override
	public String toString() {
		return from.getValue() + " -> " + to.getValue();
	}
	public int getWeight(){
		return this.weight;
	}
}