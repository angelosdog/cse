import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.LinkedList;
import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.Scanner;

/**
 * Reads from args a list of jobs and finds the most efficient order to perform the jobs in to minimise the distance travelled.
 * @author vtra143
 */
public class CourierDelivery {

	/**
	 * Read from args a list of jobs.
	 * @param args command line arguments
	 */
	public static void main(String[] args) {
		try
		{
			Scanner sc = new Scanner(new FileReader(args[0]));  //args[0] is the first command line argument
			ArrayList<Job> jobs = new ArrayList<Job>();
			while(sc.hasNext() == true) {
				sc.next();
				int fromX = new Integer(sc.next());
				int fromY = new Integer(sc.next());
				sc.next();
				int toX = new Integer(sc.next());
				int toY = new Integer(sc.next());
				jobs.add(new Job(new Point(fromX, fromY), new Point(toX, toY)));
				if(sc.hasNextLine()) {
					sc.nextLine();
				}
			}
			sc.close();
			Point home = new Point(0, 0);
			Astar(home, jobs);
			
		}
		catch (FileNotFoundException e) {}
	}
	
	/**
	 * Performs an A* search on a list of jobs starting from a Point home.
	 * @param home the starting point of the courier
	 * @param jobs a list of jobs to complete
	 */
	private static void Astar(Point home, ArrayList<Job> jobs) {
		PriorityQueue<Delivery >deliveries = new PriorityQueue<Delivery>(1, new CompareDeliveries());
		deliveries.add(new Delivery(home, null, new LinkedList<Job>()));
		int explored = 0;
		Delivery current = deliveries.peek();
		ArrayList<Delivery> exploredStates = new ArrayList<Delivery>();
		boolean found = false;
		while (deliveries.size() > 0 && found == false) {
			current = deliveries.poll();
			if (!exploredStates.contains(current)) {
				exploredStates.add(current);
				explored++;
				if(current.getPreviousJobs().size() == jobs.size() && !current.getPreviousJobs().contains(current.getJob())) { // goal
					found = true;
					current.getPreviousJobs().remove(0);
					current.getPreviousJobs().add(current.getJob());
					printPath(explored, current.getCost(), current.getPreviousJobs());
				}
				for (Job job : jobs) {
					if(!current.getPreviousJobs().contains(job) && !current.getPreviousJobs().contains(current.getJob())) {
						Point start;
						if(current.getJob() == null) {
							start = home; // special case for first job
						} else {
							start = current.getJob().getTo();
						}
						Delivery next = new Delivery(start, job, current.getPreviousJobs());
						next.getPreviousJobs().add(current.getJob());
						next.setCost(current.getCost() + start.distance(job.getFrom()) + job.getFrom().distance(job.getTo()));
						next.setHeuristic(next.getCost() + heursitic(next, jobs));
						
						deliveries.add(next);
					}
				}
			}
		}
		return;
	}
	
	/**
	 * Prints out the commands to complete the job as specified by the output format.
	 * @param explored number of nodes explored to find the solution
	 * @param cost the total Manhattan distance travelled to complete all the jobs 
	 * @param path the order of the jobs that were taken
	 */
	private static void printPath(int explored, int cost, LinkedList<Job> path) {
		System.out.println(explored + " nodes explored");
		System.out.println("cost = " + cost);
		if(path.size() > 0) {
			Point current = new Point(0,0);
			Job nextJob = path.poll();
			String action;
			if(nextJob.getFrom().getX() == 0 && nextJob.getFrom().getY() == 0) {
				action = "Carry";
			} else {
				action = "Move";
			}
			while(nextJob != null) {
				if(action.equals("Move")) {
					System.out.println(action + " from " + current.getX() + " " + current.getY() + 
							" to " + nextJob.getFrom().getX() + " " + nextJob.getFrom().getY());
					current = nextJob.getFrom();
					action = "Carry";
				} else {
					System.out.println(action + " from " + current.getX() + " " + current.getY() + 
							" to " + nextJob.getTo().getX() + " " + nextJob.getTo().getY());
					current = nextJob.getTo();
					nextJob = path.poll();

					if(nextJob != null && !(current.getX() == nextJob.getFrom().getX() && current.getY() == nextJob.getFrom().getY())){
						action = "Move";
					}
				}
			}
		} 
		
	}
	
	/**
	 * Estimate how much distance is left to travel to complete the rest of the jobs. This current heuristic sums up all of the
	 * distances that the remaining jobs cover as well as adds the distance to the closest job from where the current Delivery finishes.
	 * @param current the current Delivery that is about to finish
	 * @param jobs a list of all the jobs
	 * @return an estimate how much distance is left to travel to complete the rest of the jobs
	 */
	private static int heursitic(Delivery current, ArrayList<Job> jobs) {
		int h = 0;
		int i = 0;
		Integer minDistance = null;
		while (i < jobs.size()) {
			if(!current.getPreviousJobs().contains(jobs.get(i)) && !(current.getJob() == jobs.get(i))) {
				h += jobs.get(i).getFrom().distance(jobs.get(i).getTo());
				if(minDistance == null) {
					minDistance = current.getJob().getTo().distance(jobs.get(i).getFrom());
				} else if (current.getJob().getTo().distance(jobs.get(i).getFrom()) < minDistance) {
					minDistance = current.getJob().getTo().distance(jobs.get(i).getFrom());
				}
			}
			i++;
		}
		if(minDistance == null) {
			return h;
		}
		return h + minDistance;
	}
	
}