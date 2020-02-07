import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;
//import com.rabbitmq.client.DeliverCallback;
import com.rabbitmq.client.*;
import java.util.*;
import java.io.*;
import java.nio.file.*;


public class Cliente implements Runnable{

	static String nome;

	public Cliente(){}
	public Cliente(String nome_){
		this.nome = nome_;
	}
	
	static int cont_recebido = 0, cont_enviado = 0;
	static Scanner scan = new Scanner(System.in);

	static String EXCHANGE_ENTRADA = "entrada";
	static ConnectionFactory factory; 
    static Connection connection;
    static Channel channel_entrada;

    static String queueName;

    static String EXCHANGE_SAIDA = "saida";
    static Channel channel_saida;

    static String n_arquivo = null;

	public static void main(String args[]){

		try{
			factory = new ConnectionFactory();
	    	factory.setHost("localhost");
	    	connection = factory.newConnection();
	    	channel_entrada = connection.createChannel();
	    	channel_entrada.exchangeDeclare(EXCHANGE_ENTRADA, "fanout");
	    	queueName = channel_entrada.queueDeclare().getQueue();
	    	channel_entrada.queueBind(queueName, EXCHANGE_ENTRADA, "");
		
	    	channel_saida = connection.createChannel();
    		channel_saida.exchangeDeclare(EXCHANGE_SAIDA, "fanout");
		} catch(Exception ee){}

		System.out.println("Digite o seu nome:");
		nome = scan.nextLine();
		System.out.println("\nPara enviar uma mensagem digite o nome do arquivo:\n");
		new Thread(new Cliente(nome)).start();
		File file;

		//DeliverCallback deliverCallback = (consumerTag, delivery) -> {
        Consumer consumer = new DefaultConsumer(channel_entrada){
        public void handleDelivery(String consumerTag, Envelope envelope, AMQP.BasicProperties properties, byte[] body){
        	//String message = new String(delivery.getBody(), "UTF-8");
        	String message = new String(body);
        	String aux[] = message.split("&", 3);
        	if(!aux[0].equals(nome)){
        		n_arquivo = nome+"-"+aux[0]+"-"+aux[1]+".client";
        		Cliente.write(n_arquivo, aux[2]);
        		System.out.println("Recebi uma mensagem de " +aux[0]+". Salvei no arquivo: "+n_arquivo);
				System.out.println("\nPara enviar uma mensagem digite o nome do arquivo:");
        	}
        }
    	};
		
		try{
			channel_entrada.basicConsume(queueName, true, consumer);
		} catch (Exception e){}
		

	}

	public void run(){

		String n_arq;
		File file;
		String message;
		while(true){
			n_arq = scan.nextLine();
			file = new File(n_arq);
			if(file.exists()){
				message = Cliente.read(n_arq);
				message = this.nome+"&"+cont_enviado+"&"+message;
				cont_enviado++;
				try{
					channel_saida.basicPublish(EXCHANGE_SAIDA, "", null, message.getBytes("UTF-8"));
				} catch (Exception e){}
				System.out.println("Enviei a mensagem do arquivo "+n_arq+".\n\nPara enviar uma mensagem digite o nome do arquivo:");
			}else{
				System.out.println("Nao existe arquivo com esse nome.\n\nPara enviar uma mensagem digite o nome do arquivo:");
			}
		}

	}

	public static void write(String n_arq, String mensagem) {
	    BufferedWriter bw = null;
	    FileWriter fw = null;
	    try {
	      fw = new FileWriter(n_arq);
	      bw = new BufferedWriter(fw);
	      bw.write(mensagem);
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

	public static String read(String arquivo) {
        BufferedReader br = null;
        FileReader fr = null;
        String FILENAME = arquivo;
        String resposta = "";
        int i=0;
        try {
            fr = new FileReader(FILENAME);
            br = new BufferedReader(fr);
            String sCurrentLine;
            br = new BufferedReader(new FileReader(FILENAME));
            while ((sCurrentLine = br.readLine()) != null) {
                if(i==0){
                    resposta = resposta+sCurrentLine;
                    i++;
                }
                else
                    resposta = resposta+"\n"+sCurrentLine;
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