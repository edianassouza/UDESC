import java.io.*;
import java.net.*;
/**
 *
 * @author lycog
 */
public class MulticastReceiver extends Thread{
  
  private static String name;
  private static int cont;//número de mensagens recebidas
  private String id;//id da Thread
  private Client client;

  public MulticastReceiver(Client client){
    this.client = client;
  }

  public void setNome(String name){
    this.name = name;
    this.cont = 1;
  }

  public void run() {
    MulticastSocket socket = null;
    DatagramPacket inPacket = null;
    byte[] inBuf = new byte[64000];
    try {
      //Prepare to join multicast group
      socket = new MulticastSocket(8888);
      InetAddress address = InetAddress.getByName("224.2.2.3");
      socket.joinGroup(address);
 
      while (true) {
        inPacket = new DatagramPacket(inBuf, inBuf.length);
        socket.receive(inPacket);
        String msg = new String(inBuf, 0, inPacket.getLength());
        String aux[];
        if(msg.indexOf("$")==-1){
          aux = msg.split("-", 2);
          if(msg.indexOf(name)==-1)
            write(aux[1], this.id);
        }
        else{
          if(msg.indexOf("$#")!=-1){//Se a mensagem tiver S#, significa que o servidor está passando o id para o cliente.
            if(msg.indexOf(name)!=-1){//Verificar se o seu nome esta na mensagem, para ter certeza que o id é seu.
              aux = msg.split(name, 2);
              id = aux[1];
            }
          }
          else{
            if(msg.indexOf("$ *** The user "+name)!=-1){
              System.out.println("$ Bye!");
              client.closed = true;
              Thread.currentThread().stop();
            }
            System.out.println(msg);
          }
        }
      }
    } catch (IOException ioe) {
      System.out.println(ioe);
    }
  }

  //Escrevo o arquivo .clientid
  public static void write(String content, String id) {
    //System.out.println("ID: "+id);
    BufferedWriter bw = null;
    FileWriter fw = null;
    String FILENAME = null;
    String msg;

    if(cont<10)
      FILENAME = name+"-0"+cont+"."+"client"+id;//adicionar o seu id no .clientid
    else
      FILENAME = name+"-"+cont+"."+"client"+id;

    try {
      fw = new FileWriter(FILENAME);
      bw = new BufferedWriter(fw);
      msg = content.replace("%", "\n");//Subistituir os % do arquivo por \n, para corrigir as quebras de linha
      msg = msg.replace("&", "\t");
      bw.write(msg);
      cont++;
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