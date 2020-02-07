import java.io.*;
import java.net.*;

/*
 * A chat server that delivers public and private messages.
 */
public class Server {

  private static DataInputStream is = null;
  private static PrintStream os = null;
  // The server socket.
  private static ServerSocket serverSocket = null;
  // The client socket.
  private static Socket clientSocket = null;

  // This chat server can accept up to maxClientsCount clients' connections.
  private static final int maxClientsCount = 5;
  public static final serverThread[] threads = new serverThread[maxClientsCount];
  private static final String[] nome_clientes = new String[maxClientsCount];

  private static Server instancia = null;

  private Server(){}

  public static void main(String args[]) {

    // The default port number.
    int portNumber = 2772, cont = 0;

    /*
     * Open a server socket on the portNumber (default 2222). Note that we can
     * not choose a port less than 1023 if we are not privileged users (root).
     */
    try {
      serverSocket = new ServerSocket(portNumber);
    } catch (IOException e) {
      System.out.println(e);
    }

    /*
     * Create a client socket for each connection and pass it to a new client
     * thread.
     */

    while (true) {
      try {
        int i=0;
          clientSocket = serverSocket.accept();
          for(i=0; i < maxClientsCount;i++) {
            if (threads[i] == null) {
              (threads[i] = new serverThread(clientSocket, threads, i+1)).start();
              //threads[i].chat();
              break;
            }
          }
        
          if (i == maxClientsCount) {
            PrintStream os = new PrintStream(clientSocket.getOutputStream());
            os.println("Server too busy. Try later.");
            os.close();
            clientSocket.close();

        }
      } catch (IOException e) {
        System.out.println(e);
      }
    }
  }

  public static synchronized Server getInstance() {
      //testa para ver se ninguem instanciou ainda
   if(instancia == null ) {
      instancia = new Server();
    }
    //retorna a instancia ja criada
    return instancia;
  }

  public boolean verifica_nome (String name){
    for (int i=0; i<maxClientsCount; i++){
      if(nome_clientes[i]==null)
        break;
      if (nome_clientes[i].equalsIgnoreCase(name)){
        return false;
      }
    }
    for (int i=0; i<maxClientsCount; i++){
      if (nome_clientes[i]==null){
        nome_clientes[i]=name;
        return true;
      }
    }
    return true;
  }

}
