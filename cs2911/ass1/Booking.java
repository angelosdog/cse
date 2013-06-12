import java.util.Calendar;
import java.util.LinkedList;

public class Booking {
	private String user;
	private int capacity;
	private int numWeeks;
	private Calendar date;
	private int duration;
	private String title;

	/**
	 * Constructs a booking called title under the user for capacity people for numWeeks weeks starting from date for duration hours
	 * @param user the user who is making a booking
	 * @param capacity the minimum capacity for a room needed
	 * @param numWeeks how many weeks the booking is made
	 * @param date the date of the first booking
	 * @param duration the duration of the booking in hours
	 * @param title the name of the booking
	 */
	public Booking(String user, int capacity, int numWeeks, Calendar date, int duration, String title)
	{
		this.user = user;
		this.capacity = capacity;
		this.numWeeks = numWeeks;
		this.date = date;
		this.duration = duration;
		this.title = title;
	}
	/**
	 * Checks the validity of adding a booking, and then proceeds to add that booking to the system if it is possible.
	 * @param rooms a list of rooms that currently exist in the room booking system.
	 * @return the room that was booked, or null if no booking can be made.
	 */
	public Room newBooking(LinkedList<Room> rooms)
	{
		int i, j, k;
		boolean found = false;
		boolean booked = false;
		Room room = null;
		for(i = 0; i < rooms.size() && found == false; i++)
		{
			room = rooms.get(i);
			if (room.getCapacity() >= this.capacity)
			{
				LinkedList<Booking> bookings = room.getBookings();
				Booking existingBooking = null;
				Booking newBooking = (Booking) this.clone();
				Booking newBookingEnd = (Booking) this.clone();
				newBookingEnd.date.add(Calendar.HOUR_OF_DAY, newBooking.duration);
				booked = false;
				for(j = 0; j < this.numWeeks && booked == false; j++)
				{
					for(k = 0; k < bookings.size() && newBooking.date.get(Calendar.YEAR) == 2013; k++)
					{
						existingBooking = bookings.get(k);
						Booking existingBookingEnd = (Booking) existingBooking.clone();
						existingBookingEnd.date.add(Calendar.HOUR_OF_DAY, existingBookingEnd.duration);
						if(!((newBooking.date.getTimeInMillis() < existingBooking.date.getTimeInMillis() 
								&& newBookingEnd.date.getTimeInMillis() <= existingBooking.date.getTimeInMillis())
								|| (existingBookingEnd.date.getTimeInMillis() <= newBooking.date.getTimeInMillis()
								&& existingBookingEnd.date.getTimeInMillis() < newBookingEnd.date.getTimeInMillis())))
						{
							booked = true;
						}
					}
					newBooking.date.add(Calendar.DAY_OF_MONTH, 7);
					newBookingEnd.date.add(Calendar.DAY_OF_MONTH, 7);
				}
				if(booked == false)
				{
					found = true;
				}

			}

		}
		if(found == false)
		{
			return null;
		}
		else
		{
			int numWeeks = this.numWeeks;
			room.addBooking(this);

			i = 1;
			while(i < numWeeks)
			{	
				Booking addBooking = (Booking) this.clone();
				addBooking.date.add(Calendar.DAY_OF_MONTH, 7*i);
				addBooking.numWeeks = addBooking.numWeeks - i;
				room.addBooking(addBooking);
				i++;
			}
			return room;
		}
	}
	/**
	 * Checks if the bookings that need to be removed exist, and then removes them from the system if they do exist.
	 * @param room the room which has the bookings that the user wants to delete.
	 * @return a list of all the bookings that were deleted.
	 */
	public LinkedList<Booking> removeBooking(Room room)
	{
		Booking existingBooking = null;
		LinkedList<Booking> foundBookings = new LinkedList<Booking>();
		Booking currentBooking = (Booking) this.clone();
		for(int i = 0; i < this.numWeeks; i++)
		{
			for(int j = 0; j < room.getBookings().size(); j++)
			{
				existingBooking = room.getBookings().get(j);
				if(existingBooking.user.equals(currentBooking.user)
					&& existingBooking.date.get(Calendar.MONTH) == currentBooking.date.get(Calendar.MONTH)
					&& existingBooking.date.get(Calendar.DAY_OF_MONTH) == currentBooking.date.get(Calendar.DAY_OF_MONTH)
					&& existingBooking.date.get(Calendar.HOUR_OF_DAY) == currentBooking.date.get(Calendar.HOUR_OF_DAY))
				{
					foundBookings.add(existingBooking);
				}
			}
			currentBooking.date.add(Calendar.DAY_OF_MONTH, 7);
		}
		if(foundBookings.size() == this.numWeeks)
		{
			room.getBookings().removeAll(foundBookings);
			return foundBookings;
		}
		else
		{
			return null;
		}
	}
	/**
	 * Returns the user field in the booking object.
	 * @return the user field in the Booking object
	 */
	public String getUser()
	{
		return this.user;
	}
	/**
	 * Returns the duration of a booking.
	 * @return the duration of a booking
	 */
	public int getDuration()
	{
		return this.duration;
	}
	/**
	 * Returns the title of a booking object.
	 * @return the title of a booking.
	 */
	public String getTitle()
	{
		return this.title;
	}
	/**
	 * Returns the date of the booking as an Calendar object.
	 * @return the date of the booking as an Calendar object.
	 */
	public Calendar getDate()
	{
		return this.date;
	}
	/**
	 * Return the date as a string in MMM DD HH format.
	 * @return the date as a string in MMM DD HH format.
	 */
	public String getDateString()
	{
		String date = new String();
		date = monthToStr(this.date.get(Calendar.MONTH)) + " " + this.date.get(Calendar.DAY_OF_MONTH) + " " + this.date.get(Calendar.HOUR_OF_DAY);
		return date;
	}
	/**
	 * Returns a deep copy of a booking this booking (the elements are also cloned).
	 * @return a deep copy of this booking instance.
	 */
	public Object clone()
	{
		Booking newBooking = null;
		if(this instanceof Booking)
		{
			Calendar newDate = (Calendar) this.date.clone();
			newBooking = new Booking(this.user, this.capacity, this.numWeeks, newDate, this.duration, this.title);
			
		}
		return newBooking;
	}
	/**
	 * Returns a Calendar.MONTH field into a readable string.
	 * @param MONTH a month value that is equivalent to the field value in the Calendar class.
	 * @return the month in the form MMM.
	 */
	private String monthToStr(int MONTH)
	{
		String month = new String();
		if(MONTH == Calendar.JANUARY)
		{
			month = "Jan";
		}
		else if (MONTH == Calendar.FEBRUARY)
		{
			month = "Feb";
		}
		else if (MONTH == Calendar.MARCH)
		{
			month = "Mar";
		}
		else if (MONTH == Calendar.APRIL)
		{
			month = "Apr";
		}
		else if (MONTH == Calendar.MAY)
		{
			month = "May";
		}
		else if (MONTH == Calendar.JUNE)
		{
			month = "Jun";
		}
		else if (MONTH == Calendar.JULY)
		{
			month = "Jul";
		}
		else if (MONTH == Calendar.AUGUST)
		{
			month = "Aug";
		}
		else if (MONTH == Calendar.SEPTEMBER)
		{
			month = "Sep";
		}
		else if (MONTH == Calendar.OCTOBER)
		{
			month = "Oct";
		}
		else if (MONTH == Calendar.NOVEMBER)
		{
			month = "Nov";
		}
		else if (MONTH == Calendar.DECEMBER)
		{
			month = "Dec";
		}
		return month;
	}
}
