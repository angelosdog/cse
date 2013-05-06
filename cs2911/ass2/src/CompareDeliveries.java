import java.util.Comparator;


public class CompareDeliveries implements Comparator<Delivery>{
	public int compare (Delivery d1, Delivery d2) {
		return d1.getHeuristic() - d2.getHeuristic();
	}

}
