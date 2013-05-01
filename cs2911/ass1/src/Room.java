import java.util.LinkedList;

public class Room {
	private int capacity;
	private String roomNumber;
	private LinkedList<Booking> bookings;
	/**
	 * Constructs a room with the that fits capacity people and has the room number roomNumber.
	 * @param capacity The number of people the room can accommodate
	 * @param roomNumber The room number of the room
	 */
	public Room(int capacity, String roomNumber)
	{
		this.capacity = capacity;
		this.roomNumber = roomNumber;
		bookings = new LinkedList<Booking>();

	}
	/**
	 * Prints out all the bookings from the list of bookings for a Room instance.
	 */
	public void printRoom()
	{
		System.out.println(this.roomNumber);
		int i;
		Booking roomBooking;
		for(i = 0; i < bookings.size(); i++)
		{
			roomBooking = this.bookings.get(i);
			System.out.println(roomBooking.getUser() + " "
					+ roomBooking.getDateString() + " "
					+ roomBooking.getDuration() + " "
					+ roomBooking.getTitle());
		}
	}
	/**
	 * Returns the capacity of a room.
	 * @return the capacity of a room.
	 */
	public int getCapacity()
	{
		return this.capacity;
	}
	/**
	 * Returns the room number of a room.
	 * @return the room number of a room.
	 */
	public String getRoomNumber ()
	{
		return this.roomNumber;
	}
	/**
	 * Returns all of the bookings of a room.
	 * @return all of the bookings of a room.
	 */
	public LinkedList<Booking> getBookings()
	{
		return this.bookings;
	}
	/**
	 * Adds a booking to the list of bookings for a room. 
	 * @param booking a booking that is to be added to the list of bookings the room has.
	 */
	public void addBooking(Booking booking)
	{
		this.bookings.add(booking);
	}
	/**
	 * Removes a booking from the list of bookings for a room.
	 * @param booking A booking that is to be removed from the list of bookings the room has.
	 */
	public void removeBooking(Booking booking)
	{
		this.bookings.remove(booking);
	}
}
