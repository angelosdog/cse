/**
 * A Job consists of two Points and is defined as a from Point and a to Point.
 * @author vtra143
 */
public class Job {

	private Point from;
	private Point to;
	
	/**
	 * Constructs a Job taking in a from Point and a to Point.
	 * @param from the Point that a Job is from
	 * @param to the Point that a Job is to
	 */
	Job(Point from, Point to) {
		this.from = from;
		this.to = to;
	}
	
	/**
	 * Returns the from Point of a Job.
	 * @return the from Point of a Job
	 */
	public Point getFrom() {
		return this.from;
	}
	
	/**
	 * Returns the to Point of a Job.
	 * @return the to Point of a Job
	 */
	public Point getTo() {
		return this.to;
	}
	
}