import java.net.*;
import java.io.*;

public class Client implements Runnable {

    // The client socket
    private static Socket clientSocket = null;
    // The output stream
    private static PrintStream os = null;
    // The input stream
    private static DataInputStream is = null;

    private static BufferedReader inputLine = null;
    public static boolean closed = false;
    private static String name;
    private static int cont;//numero de mensagens
    private static Server server = Server.getInstance();
    public static Client instance = null;

    private Client(){instance=this;};


    public static void main(String[] args) {

        int portNumber = 2772;
        String host = "localhost";

        if(args.length<=2){
            host = args[0];
            portNumber = Integer.valueOf(args[1]).intValue();
        }
        else{
            System.out.println("Argumentos incorretos");
            System.exit(1);
        }

        cont = 1;

        try {
            clientSocket = new Socket(host, portNumber);
            inputLine = new BufferedReader(new InputStreamReader(System.in));
            os = new PrintStream(clientSocket.getOutputStream());
            is = new DataInputStream(clientSocket.getInputStream());
        } catch (UnknownHostException e) {
            System.err.println("Don't know about host " + host);
        } catch (IOException e) {
            System.err.println("Couldn't get I/O for the connection to the host "
                    + host);
        }

        /*
         * If everything has been initialized then we want to write some data to the
         * socket we have opened a connection to on the port portNumber.
         */
        if (clientSocket != null && os != null && is != null) {
            try {

                MulticastReceiver c = new MulticastReceiver(instance);
                /* Create a thread to read from the server. */
                new Thread(new Client()).start();
                new Thread(c).start();
                boolean le_nome = true;
                String texto;

                if(le_nome){//entra nesse if apenas uma vez para pegar o nome do cliente
                    texto = inputLine.readLine().trim();
                    name = texto;
                    le_nome = false;
                    os.println(texto);
                    c.setNome(name);
                }

                while (!closed) {
                    // Verifica se existe um arquivo com o nome do cliente e o próximo cont.
                    // Se existe, lê o conteúdo do arquivo e manda para o servidor.
                    String arquivo = null;
                    if(cont<10)
                        arquivo = "chat"+name+"-0"+cont+".chat";
                    else
                        arquivo = "chat"+name+"-"+cont+".chat";
                    File file = new File(arquivo);
                    if(file.exists()){
                        texto = reader(arquivo);
                        cont++;
                        os.println(texto);
                    }
                }
                /*
                 * Close the output stream, close the input stream, close the socket.
                 */
                os.close();
                is.close();
                clientSocket.close();
                System.exit(1);
            } catch (IOException e) {
                System.err.println("IOException:  " + e);
            }
        }
    }

    //Thread extra para a inicialização
    public void run() {
        /*
         * Keep on reading from the socket till we receive "Bye" from the
         * server. Once we received that then we want to break.
         */
        String responseLine;
        try {
            while ((responseLine = is.readLine()) != null) {
                if(responseLine.indexOf("$ *** The user "+name)!=-1){
                	closed = true;
                	Thread.currentThread().stop();
                }
                else{
	                System.out.println(responseLine);
	                if(responseLine.indexOf("This name is already")!=-1){
	                    closed = true;
	                    Thread.currentThread().stop();
	                }
            	}
            }
        } catch (IOException e) {
            System.err.println("IOException:  " + e);
        }
    }

    //Leio o arquivo .chat
    public static String reader(String arquivo) {
        BufferedReader br = null;
        FileReader fr = null;
        String FILENAME = arquivo;
        String resposta = "";
        int i=0;
        if(cont<10)
            resposta = "0"+cont+"-";
        else
            resposta = cont+"-";
        try {
            fr = new FileReader(FILENAME);
            br = new BufferedReader(fr);
            String sCurrentLine;
            br = new BufferedReader(new FileReader(FILENAME));
            while ((sCurrentLine = br.readLine()) != null) {
                sCurrentLine =  sCurrentLine.replace("\t", "&");
                if(i==0){
                    resposta = resposta+sCurrentLine;
                    i++;
                }
                else
                    resposta = resposta+"%"+sCurrentLine;
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (br != null) {
                   br.close();
                }
                if (fr != null) {
                    fr.close();
                }
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
        return resposta;
    }
}
