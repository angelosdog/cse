import java.util.Comparator;


public class CompareCost implements Comparator<ASVertex> {
	public int compare(ASVertex v1, ASVertex v2){
		return v1.getHeuristic() - v2.getHeuristic();
	}

}