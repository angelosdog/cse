public class GreeterTester
{
   public static void main(String[] args)
   {
      Greeter worldGreeter = new Greeter("World");
      String greeting = worldGreeter.sayHello();
      System.out.println(greeting);
      Greeter daveGreeter = new Greeter("Dave");
      System.out.println(daveGreeter.sayHello());
      daveGreeter = worldGreeter;
      System.out.println(daveGreeter.sayHello());
      daveGreeter.setName("not Dave");
      System.out.println(daveGreeter.sayHello());
   }
}

