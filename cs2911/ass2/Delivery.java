import java.util.LinkedList;

/**
 * @author vtra143
 * A delivery consists of a start point, which is where you ended up from the last job or (0,0) for the first delivery,
 * the next job to do and a LinkedList of all the previous jobs done up to that point. It also contains the cost to 
 * have done the previous jobs and a heuristic to estimate how long it should take to finish the rest of the jobs. 
 */
public class Delivery {

	private Point start;
	private Job job;
	private int cost;
	private int heuristic;
	private LinkedList<Job> previousJobs;
	
	/**
	 * Constructs a Delivery object.
	 * @param start the starting point of a Delivery, which is either where you ended up after the last job, or (0, 0)
	 * @param job the next job to perform
	 * @param previousJobs a LinkedList of previous jobs performed up to that point
	 */
	
	Delivery(Point start, Job job, LinkedList<Job> previousJobs) {
		this.start = start;
		this.job = job;
		this.cost = 0;
		this.heuristic = 0;
		this.previousJobs = new LinkedList<Job>();
		this.previousJobs.addAll(previousJobs);
	}
	
	/**
	 * Returns the start point of a delivery.
	 * @return the start point of a delivery
	 */
	public Point getStart() {
		return this.start;
	}
	/**
	 * Returns the current job to be performed.
	 * @return the current job to be performed
	 */
	public Job getJob() {
		return this.job;
	}
	
	/**
	 * Returns the cost to do all of the previous jobs up until this point.
	 * @return the cost to do all of the previous jobs up until this point
	 */
	public int getCost() {
		return this.cost;
	}
	
	/**
	 * Sets the cost of doing all of the previous jobs up until this point.
	 * @param cost the cost of doing all of the previous jobs up until this point
	 */
	public void setCost(int cost) {
		this.cost = cost;
	}
	
	/**
	 * Returns the estimate of how much distance is to be travelled to finish the rest of the jobs.
	 * @return the estimate of how much distance is to be travelled to finish the rest of the jobs
	 */
	public int getHeuristic() {
		return this.heuristic;
	}
	
	/**
	 * Sets the estimate of how much distance is to be travelled to finish the rest of the jobs.
	 * @param hueristic the estimate of how much distance is to be travelled to finish the rest of the jobs
	 */
	public void setHeuristic(int hueristic) {
		this.heuristic = hueristic;
	}
	
	/**
	 * Returns a list of the previous jobs done up until this point.
	 * @return a list of the previous jobs done up until this point
	 */
	public LinkedList<Job> getPreviousJobs() {
		return this.previousJobs;
	}
	
	/**
	 * Compares an object with a delivery. Two deliveries are equal if the list of previous jobs
	 * has the same elements, but not necessarily in the same order, and if the next job to do
	 * is the same job.
	 * @param o takes in an object which must be a Delivery
	 * @return true if the object and delivery are equal, false otherwise 
	 */
	public boolean equals(Object o) {
		if(o instanceof Delivery) {
			Delivery d = (Delivery) o;
			if(this.job == d.getJob()) {
				for (Job j : this.getPreviousJobs()) {
					if(!d.getPreviousJobs().contains(j)) {
						return false;
					}
				}
				return true;
			}
		}
		return false;
	}
}