import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.LinkedList;
import java.util.Scanner;
import java.util.Calendar;


public class RoomBookingSystem {

	/**
	 * @param args the arguments from a command line call to the program.
	 */
	public static void main(String[] args) {
		try
		{
			Scanner sc = new Scanner(new FileReader(args[0]));  //args[0] is the first command line argument
			LinkedList<Room> rooms = new LinkedList<Room>();
			while(sc.hasNext())
			{
				String command = new String(sc.next());
				if(command.equals("Room"))
				{
					Integer capacity = new Integer(sc.next());
					String roomNumber = new String(sc.next());
					Room room = new Room(capacity, roomNumber);
					boolean existing = false;
					for(int i = 0; i < rooms.size(); i++){
						if(rooms.get(i).getRoomNumber().equals(roomNumber)){
								existing = true;
						}
					}
					if(existing == false) {
						rooms.add(room);
					}
				}
				else if (command.equals("Book"))
				{
					String user = new String(sc.next());
					Integer capacity = new Integer(sc.next());
					Integer numWeeks = new Integer(sc.next());
					Calendar date = Calendar.getInstance(); // Calendar requires instantiation
					date.set(Calendar.YEAR, 2013); // Change all the fields of the Calendar
					date.set(Calendar.MONTH, new Integer(monthToCalendarField(sc.next())));
					date.set(Calendar.DAY_OF_MONTH, new Integer(sc.next()));
					date.set(Calendar.HOUR_OF_DAY, new Integer(sc.next()));
                    date.set(Calendar.MINUTE, 0);
                    date.set(Calendar.SECOND, 0);
                    date.set(Calendar.MILLISECOND, 0);
					Integer duration = new Integer(sc.next());
					String title = new String(sc.next());
					Booking booking = new Booking(user, capacity, numWeeks, date, duration, title);
					Room room = booking.newBooking(rooms);
					if (room != null)
					{
						System.out.println("Room " + room.getRoomNumber() + " assigned");
					}
					else
					{
						System.out.println("Booking rejected");
					}

				} 
				else if (command.equals("Change"))
				{
					String user = sc.next();
					String roomNumber = sc.next();
					int numWeeks = new Integer(sc.next());
					Calendar date = Calendar.getInstance();
					date.set(Calendar.YEAR, 2013);
					date.set(Calendar.MONTH, new Integer(monthToCalendarField(sc.next())));
					date.set(Calendar.DAY_OF_MONTH, new Integer(sc.next()));
					date.set(Calendar.HOUR_OF_DAY, new Integer(sc.next()));
                    date.set(Calendar.MINUTE, 0);
                    date.set(Calendar.SECOND, 0);
                    date.set(Calendar.MILLISECOND, 0);
					Booking booking = new Booking(user, 0, numWeeks, date, 0, null);
					int i = 0;
					while(i < rooms.size() && roomNumber.equals(rooms.get(i).getRoomNumber()) == false)
					{
						i++;
					}
					if (i == rooms.size())
					{
						System.out.println("Change rejected");
					}
					else 
					{
						LinkedList<Booking> oldBooking = booking.removeBooking(rooms.get(i));
						if(oldBooking == null)
						{
							System.out.println("Change rejected");
						}
						else
						{
							Integer capacity = new Integer(sc.next());
							Calendar newDate = Calendar.getInstance(); 
							newDate.set(Calendar.YEAR, 2013);
							newDate.set(Calendar.MONTH, new Integer(monthToCalendarField(sc.next())));
							newDate.set(Calendar.DAY_OF_MONTH, new Integer(sc.next()));
							newDate.set(Calendar.HOUR_OF_DAY, new Integer(sc.next()));
                            date.set(Calendar.MINUTE, 0);
                            date.set(Calendar.SECOND, 0);
                            date.set(Calendar.MILLISECOND, 0);
                            Integer duration = new Integer(sc.next());
							String title = new String(sc.next());
							Booking newBooking = new Booking(user, capacity, numWeeks, newDate, duration, title);
							Room room = newBooking.newBooking(rooms);
							if (room != null)
							{
								System.out.println("Room " + room.getRoomNumber() + " assigned");
							}
							else
							{
								System.out.println("Change rejected");
								rooms.get(i).getBookings().addAll(oldBooking);
							}
						}
					}

				}
				else if (command.equals("Delete"))
				{
					String user = sc.next();
					String roomNumber = sc.next();
					int numWeeks = new Integer(sc.next());
					Calendar date = Calendar.getInstance();
					date.set(Calendar.YEAR, 2013);
					date.set(Calendar.MONTH, new Integer(monthToCalendarField(sc.next())));
					date.set(Calendar.DAY_OF_MONTH, new Integer(sc.next()));
					date.set(Calendar.HOUR_OF_DAY, new Integer(sc.next()));
                    date.set(Calendar.MINUTE, 0);
                    date.set(Calendar.SECOND, 0);
                    date.set(Calendar.MILLISECOND, 0);
					Booking booking = new Booking(user, 0, numWeeks, date, 0, null);
					int i = 0;
					while(i < rooms.size() && roomNumber.equals(rooms.get(i).getRoomNumber()) == false)
					{
						i++;
					}
					if (i == rooms.size())
					{
						System.out.println("Deletion rejected");
					}
					else
					{
						if(booking.removeBooking(rooms.get(i)) != null)
						{
							System.out.println("Reservations deleted");
						}
						else
						{
							System.out.println("Deletion rejected");
						}
					}
				}
				else if (command.equals("Print"))
				{
					String roomNumber = sc.next();
					Room room = null;
					boolean found = false;
					for(int i = 0; i < rooms.size() && found == false; i++)
					{
						room = rooms.get(i);
						if(room.getRoomNumber().equals(roomNumber))
						{
							found = true;
						}
					}
					if (found == true)
					{
						room.printRoom();
					}
				}

				sc.nextLine();
			}
			sc.close();
		}
		catch (FileNotFoundException e) {}
	}
	/**
	 * Converts a month from MMM format to a Calendar.MONTH field.
	 * @param month a month in the form of MMM
	 * @return the month as a Calendar.MONTH field from the Calendar class
	 */
	static int monthToCalendarField(String month)
	{
		int MONTH = -1;
		if(month.equals("Jan"))
		{
			MONTH = Calendar.JANUARY;
		}
		else if (month.equals("Feb"))
		{
			MONTH = Calendar.FEBRUARY;
		}
		else if (month.equals("Mar"))
		{
			MONTH = Calendar.MARCH;
		}
		else if (month.equals("Apr"))
		{
			MONTH = Calendar.APRIL;
		}
		else if (month.equals("May"))
		{
			MONTH = Calendar.MAY;
		}
		else if (month.equals("Jun"))
		{
			MONTH = Calendar.JUNE;
		}
		else if (month.equals("Jul"))
		{
			MONTH = Calendar.JULY;
		}
		else if (month.equals("Aug"))
		{
			MONTH = Calendar.AUGUST;
		}
		else if (month.equals("Sep"))
		{
			MONTH = Calendar.SEPTEMBER;
		}
		else if (month.equals("Oct"))
		{
			MONTH = Calendar.OCTOBER;
		}
		else if (month.equals("Nov"))
		{
			MONTH = Calendar.NOVEMBER;
		}
		else if (month.equals("Dec"))
		{
			MONTH = Calendar.DECEMBER;
		}
		
		return MONTH;
	}
}
