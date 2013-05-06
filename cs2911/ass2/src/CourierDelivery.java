import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Collections;
import java.util.LinkedList;
import java.util.Scanner;


public class CourierDelivery {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try
		{
			Scanner sc = new Scanner(new FileReader(args[0]));  //args[0] is the first command line argument
			LinkedList<Job> jobs = new LinkedList<Job>();
			while(sc.hasNext() == true) {
				sc.next();
				int fromX = new Integer(sc.next());
				int fromY = new Integer(sc.next());
				sc.next();
				int toX = new Integer(sc.next());
				int toY = new Integer(sc.next());
				jobs.add(new Job(new Point(fromX, fromY), new Point(toX, toY)));
				sc.nextLine();
			}
			sc.close();
			Point home = new Point(0, 0);
			Astar(home, jobs);
			
		}
		catch (FileNotFoundException e) {}
	}

	private static void Astar(Point home, LinkedList<Job> jobs) {
		LinkedList<Delivery> deliveries = new LinkedList<Delivery>();
		deliveries.add(new Delivery(home, null, new LinkedList<Job>()));
		int explored = 0;
		Delivery current = deliveries.peek();
		boolean found = false;
		while (deliveries.size() > 0 && found == false) {
			current = deliveries.poll();
			
			if (!current.getPreviousJobs().contains(current.getJob())) {
				explored++;
				if(current.getPreviousJobs().size() == jobs.size()) {
					found = true;
					current.getPreviousJobs().remove(0);
					current.getPreviousJobs().add(current.getJob());
					printPath(explored, current.getCost(), current.getPreviousJobs());
				}
				for (Job job : jobs) {
					Point start;
					if(current.getJob() == null) {
						start = home; // special case for first job
					} else {
						start = current.getJob().getTo();
					}
					Delivery next = new Delivery(start, job, current.getPreviousJobs());
					next.getPreviousJobs().add(current.getJob());
					next.setCost(current.getCost() + start.distance(job.getFrom()) + job.getFrom().distance(job.getTo()));
					next.setHeuristic(next.getCost() + heursitic(current, jobs));
					
					deliveries.add(next);
				}
				Collections.sort(deliveries, new CompareDeliveries());
			}
		}
		return;
	}
	
	private static void printPath(int explored, int cost, LinkedList<Job> path) {
		System.out.println(explored);
		System.out.println(cost);
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

	private static int heursitic(Delivery current, LinkedList<Job> jobs) {
		int h = 0;
		int i = 0;
		Integer minDistance = null;
		while (i < jobs.size()) {
			if(!current.getPreviousJobs().contains(jobs.get(i))) {
				h += jobs.get(i).getFrom().distance(jobs.get(i).getTo());
				if(minDistance == null) {
					minDistance = current.getStart().distance(jobs.get(i).getFrom());
				} else if (minDistance > current.getStart().distance(jobs.get(i).getFrom())) {
					minDistance = current.getStart().distance(jobs.get(i).getFrom());
				}
			}
			i++;
		}
		return h + minDistance;
	}
}