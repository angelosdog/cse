import java.io.FileNotFoundException;
import java.io.FileReader;
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
			while(sc.hasNextInt() == true) {
				jobs.add(new Job(new Point(sc.nextInt(), sc.nextInt()), new Point(sc.nextInt(), sc.nextInt())));
			}
			sc.close();
			
			Point home = new Point(0, 0);
			Point current = home;
			Job nextJob;
			LinkedList<Job> path = new LinkedList<Job>();
			int cost = 0;
			int explored = 0;
			while (jobs.size() > 0) {
				int tempFScore = cost + hueristic(current, jobs.get(0));
				nextJob = jobs.get(0);
				explored++;
				for (Job job : jobs) {
					int fScore = cost + hueristic(current, job);
					if (fScore < tempFScore) {
						nextJob = job;
						explored++;
					}
				}
				jobs.remove(nextJob);
				path.add(nextJob);
			}
		}
		catch (FileNotFoundException e) {}
	}

	private static int hueristic(Point current, Job job) {
		int distance = current.distance(job.getFrom());
		return distance;
	}
}
