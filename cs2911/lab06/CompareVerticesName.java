import java.util.Comparator;


public class CompareVerticesName implements Comparator<Vertex> {
	public int compare(Vertex v1, Vertex v2){
		return v1.getValue().compareTo(v2.getValue());
	}

}