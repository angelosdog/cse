import java.util.LinkedList;

/**
 * Represents a node in the graph and stores all outgoing edges from that node
 * @author Timothy Wiley
 */
public class Vertex {
	private final String value;
	private LinkedList<Edge> edges;
	private int costFromRoot;

	/**
	 * Construct a vertex given the String value of the vertex
	 * @param item String value
	 */
	public Vertex(String item) {
		this.value = item;
		edges = new LinkedList<Edge>();
		this.costFromRoot = 0;
	}
	
	public String getValue() {
		return value;
	}

	public LinkedList<Edge> getEdges() {
		return edges;
	}

	@Override
	/**
	 * This object is equal to obj if
	 * 	- obj is a Vertex and this.value == other.value
	 *  - obj is a String and this.value == (String) obj
	 */
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;

		if (obj.getClass() == String.class) {
			return value.equals(obj);
		} else if (getClass() == obj.getClass()) {
			Vertex other = (Vertex) obj;
			return value.equals(other.value);
		}
		return false;
	}
	
	@Override
	public String toString() {
		StringBuffer string = new StringBuffer();
		string.append(value + ":\n");
		for(Edge edge : edges) {
			string.append("\t-> " + edge.getTo().getValue() + "\n");
		}
		return string.toString();
	}
	
	public void setCost (int cost) {
		this.costFromRoot = cost;
	}
	
	public int getCost(){
		return this.costFromRoot;
	}
}