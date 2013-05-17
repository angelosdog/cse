import java.util.Comparator;

/**
 * @author vtra143
 * Implements Comparator interface Delivery class to compare heuristic value
 */
public class CompareDeliveries implements Comparator<Delivery>{
	public int compare (Delivery d1, Delivery d2) {
		return d1.getHeuristic() - d2.getHeuristic();
	}

}
