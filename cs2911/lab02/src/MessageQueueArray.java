/**
   A first-in, first-out collection of messages. This
   implementation is not very efficient. We will consider
   a more efficient implementation in chapter 3.
*/
public class MessageQueueArray
{
   /**
      Constructs an empty message queue.
   */
   public MessageQueueArray()
   {
	   size = 10;
	   queue = new Message[size];
	   items = 0;
   }

   /**
      Remove message at head.
      @return message that has been removed from the queue
   */
   public Message remove()
   {
	   if(items == 0)
	   {
		   return null;
	   }
	   else
	   {
		   Message returnMessage = queue[0];
		   int i;
		   for(i=0; i < size; i++)
		   {
			   queue[i] = queue[i++];
		   }
		   items--;
		   return returnMessage;
	   }
   }

   /**
      Append message at tail.
      @param newMessage the message to be appended
   */
   public void add(Message newMessage)
   {
	   int i, previous;
	   if(items == size-1)
	   {
		   Message[] newQueue = new Message[size*2];
		   for(i=0; i<size;i++)
		   {
			   newQueue[i] = queue[i];
		   }
		   size = size*2;
		   queue = newQueue;
	   }
	   for(i=size-1;i >= 0; i--)
	   {
		   previous = i-1;
		   queue[i] = queue[previous];
	   }
	   queue[0] = newMessage;
	   items++;
   }

   /**
      Get the total number of messages in the queue.
      @return the total number of messages in the queue
   */
   public int size()
   {
      return items;
   }

   /**
      Get message at head.
      @return message that is at the head of the queue, or null
      if the queue is empty
   */
   public Message peek()
   {
      if(items == 0)
      {
    	  return null;
      }
      else
      {
    	  return queue[0];
      }
   }

   /**
    * Deletes a given Message from the Message Queue
    * @param message Inputed message
    */
   
   public void delete (Message message)
   {
	   int index = 0;
	   while(index < size && queue[index].equals(message))
	   {
		   index++;
	   }
	   int i, next;
	   for(i = index; i < size; i++)
	   {
		   next = i++;
		   queue[i] = queue[next];
	   }
   }
   
   private Message[] queue;
   private int size;
   private int items;
}
