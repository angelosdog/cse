import java.util.LinkedList;


public class Delivery {

	private Point start;
	private Job job;
	private int cost;
	private int heuristic;
	private LinkedList<Job> previousJobs;
	
	Delivery(Point start, Job job, LinkedList<Job> previousJobs) {
		this.start = start;
		this.job = job;
		this.cost = 0;
		this.heuristic = 0;
		this.previousJobs = new LinkedList<Job>();
		this.previousJobs.addAll(previousJobs);
	}
	public Point getStart() {
		return this.start;
	}
	
	public Job getJob() {
		return this.job;
	}
	
	public int getCost() {
		return this.cost;
	}
	public void setCost(int cost) {
		this.cost = cost;
	}
	
	public int getHeuristic() {
		return this.heuristic;
	}
	
	public void setHeuristic(int hueristic) {
		this.heuristic = hueristic;
	}
	
	public LinkedList<Job> getPreviousJobs() {
		return this.previousJobs;
	}
}