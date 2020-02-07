import java.io.*;
import java.net.*;
/*
 * A chat server that delivers public and private messages.
 */

public class serverThread extends Thread{


  private static DataInputStream is = null;
  private static PrintStream os = null;
  private Socket clientSocket = null;
  private final serverThread[] threads;
  private int maxClientsCount;
  private int id;
  private Server server = Server.getInstance();

 public serverThread(Socket clientSocket, serverThread[] threads, int id) {
   this.clientSocket = clientSocket;
   this.threads = threads;
   maxClientsCount = threads.length;
   this.id = id;
 }

public void run() { 

  int maxClientsCount = this.maxClientsCount;
  serverThread[] threads = this.threads;
  
  DatagramPacket outPacket = null;
  DatagramSocket socket = null;
  byte[] outBuf;
  
  String msg;
  int cont = 1;

  try {
      socket = new DatagramSocket();
  } catch (IOException ioe) {
      System.out.println(ioe);
  }

      try {
          is = new DataInputStream(clientSocket.getInputStream());
          os = new PrintStream(clientSocket.getOutputStream());
          os.println("$ Enter your name:");
          String name = is.readLine().trim();//lê pelo teclado o nome do cliente que acabou de entrar 
          if(!server.verifica_nome(name)){
            os.println("$ This name is already being use. Try again.");
            socket.close();
            server.threads[id-1]=null;
            Thread.currentThread().stop();
          }
          
          // Imprime a mensagem de boas vindas na tela
          os.println("$ Hello! "+name+", welcome to our chat room.To leave create any file with only the word quit.\n");

          //Manda por multicast a mensagem de que um novo cliente entrou na sala
          msg = "$ A new user "+name +" entered the chat room !!! ***\n";
          outBuf = msg.getBytes();
          InetAddress address = InetAddress.getByName("224.2.2.3");
          outPacket = new DatagramPacket(outBuf, outBuf.length, address, 8888);
          socket.send(outPacket);

          //Cria uma mensagem para mandar o id do cliente
          String id_client;
          if(id<10)
            id_client = "$#"+name+"0"+id;
          else
            id_client = "$#"+name+id;

          //Manda o id com o nome do cliente por Multicast
          outBuf = id_client.getBytes();
          outPacket = new DatagramPacket(outBuf, outBuf.length, address, 8888);
          socket.send(outPacket);
         
          String text;
          while(true){//While para receber mensagens do cliente
            is = new DataInputStream(clientSocket.getInputStream());
            text = is.readLine().trim();
            String aux[] = text.split("-", 2); //Tira o nome do cliente e o id da mensagem que recebeu

            msg = name +":" + text;
            cont = Integer.parseInt(aux[0]);
            write(name, cont, msg); //Escreve a mensagem que recebeu em um arquivo .serv
            outBuf = msg.getBytes();
             
            //Manda a mensagem que recebeu por multicast
            address = InetAddress.getByName("224.2.2.3");
            outPacket = new DatagramPacket(outBuf, outBuf.length, address, 8888);
            socket.send(outPacket);

            //verifica se a mensagem que mandou é quit
            if (aux[1].equalsIgnoreCase("quit")) {
            	os.println("$ *** The user "+name+ " is leaving the chat room !!! *** \n");
         		  msg = "$ *** The user " +name+ " is leaving the chat room !!! *** \n";
            	outBuf = msg.getBytes();
            	address = InetAddress.getByName("224.2.2.3");
            	outPacket = new DatagramPacket(outBuf, outBuf.length, address, 8888);
            	socket.send(outPacket);
            	server.threads[id-1]=null;
            	socket.close();
            	Thread.currentThread().stop();
             	break; //Sai do while
	        }
            
          }

            //Não sei o que está acontecendo aqui
            msg = name +": " + text;
            outBuf = msg.getBytes();
     
            //Send to multicast IP address and port
            address = InetAddress.getByName("224.2.2.3");
            outPacket = new DatagramPacket(outBuf, outBuf.length, address, 8888);
            socket.send(outPacket);

            //Se saiu do while, fecho o socket do cliente pois ele me enviou /quit.
            is.close();
            os.close();
            clientSocket.close();
      } catch (IOException e) {
        System.out.println(e);
      }

  }

  //Escrevo o arquivo .serv
  public static void write(String name, int cont, String content) {

    BufferedWriter bw = null;
    FileWriter fw = null;
    String FILENAME = null;
    String s[] = content.split("-", 2);
    String msg;

    if(cont<10)
      FILENAME = name+"-0"+cont+".serv";
    else
      FILENAME = name+"-"+cont+".serv";

    try {
      fw = new FileWriter(FILENAME);
      bw = new BufferedWriter(fw);
      msg = s[1].replace("%", "\n");
      msg = msg.replace("&", "\t");   
      bw.write(msg);
    } catch (IOException e) {
      e.printStackTrace();
    } finally {
      try {
        if (bw != null)
          bw.close();
        if (fw != null)
          fw.close();
      } catch (IOException ex) {
        ex.printStackTrace();
      }
    } 
  }

}
