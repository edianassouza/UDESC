import java.io.*;
import java.nio.channels.FileChannel;
import java.net.*;
import java.util.ArrayList;
import java.util.Scanner;
import java.lang.Integer;

class UDPClient implements Runnable {
	static ArrayList<String> mensagens = new ArrayList<String>();
	static ArrayList<String> portas = new ArrayList<String>();
    static Scanner scan = new Scanner(System.in);
    //variavel que incrementa cada vez que recebe uma mensagem
    static int contadorRecebidos = 0;
    //variavel que vai armazenar a porta escolhida pelo usuario
    static int port2;
    static String host;
    //variavel que armaezena a porta de quem solicitou a ultima mensagem
    static String ultimaPortaRecebida = "-2";
    

	    public static void main(String args[]) throws Exception {
            //Variavel que vai guardar o nome do arquivo que o cliente quer enviar
            File file;

            //Verificando se passou todos os elementos na execução do codigo
            
            if(args.length<=2){
                host = args[0];
                port2 = Integer.valueOf(args[1]).intValue();
            }
            else{
                System.out.println("Argumentos incorretos");
                System.exit(1);
            }

            if (port2==9920){
            	System.out.println("Esta porta ja esta em uso. Por favor, compile novamente e escolha outra porta");
            	System.exit(1);
            }

            BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in));
            DatagramSocket clientSocket = new DatagramSocket();
            InetAddress IPAddress = InetAddress.getByName(host);

            byte[] sendData = new byte[1024];
            byte[] receiveData = new byte[1024];
            ArrayList<Integer> indexMensagens = new ArrayList<Integer>();
            int op = 0;
            Scanner read = new Scanner(System.in);
            int numMsg, indice;
            String mensagem, msgId;
            
            //Porta do servidor
            int port = 9920;
            
            //cria a Thread para escutar o servidor
            new Thread(new UDPClient()).start();
            
           
            while(op != -1){
                System.out.println("Digite 1 para enviar mensagem, 2 para receber ou -1 para sair");
                op = read.nextInt();

                //Opção 1 = vai enviar uma mensagem
                if(op==1){
                    System.out.println("Digite o nome do arquivo que deseja enviar (somente o nome, por exemplo: file1K.txt) :");
                    
                    //Usuario digita o nome do arquivo que quer enviar
                    String n_arq = inFromUser.readLine();
                    file = new File(n_arq);

                    //Verificando se o arquivo na pasta
                    if(file.exists()){

                        //Concatenando com o numero da porta de entrada
                        String sentence=n_arq+":"+port2;
                        sendData = sentence.getBytes();
                        
                        //enviando para o server uma mensagem dizendo que tem uma nova mensagem
                        DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, port);
                        clientSocket.send(sendPacket);

                        //Recebe indice do server dizendo qual numero a mensagem corresponde
                        DatagramPacket receivePacket = new DatagramPacket(receiveData,receiveData.length);
                        clientSocket.receive(receivePacket);
                        String messageFromServer = new String(receivePacket.getData());

                        //Se a ultima porta que recebeu uma mensagem for diferente de -2, vai entrar nesse if. 
                        if (!(ultimaPortaRecebida.equals("-2"))){
                            //Pega a ultimaPortaRecebida
                            String auxiliar = ultimaPortaRecebida.substring(1, ultimaPortaRecebida.length());
                            //Verifica se na mensagem que recebeu do servidor contem a string da ultima porta
                            if (messageFromServer.contains(auxiliar)){
                                String msgCerta = messageFromServer.replace(auxiliar,"");
                                String[] sIndice = msgCerta.split("\0");
                                indice = Integer.parseInt(sIndice[0]);
                                
                                System.out.println("O último arquivo enviado corresponde a mensagem #" + indice);

                                msgId=indice+":"+sentence;
                                
                                //Adiciona no ArrayList como: 0:file1K.txt:4001
                                mensagens.add(msgId);
                                messageFromServer="";
                            }
                        //Se a ultima porta que recebeu uma mensagem for igual a -2 (significa que ainda nao tentou receber nenhuma mensagem)
                        }else{

                            String[] aux = messageFromServer.split("\0");
                            String numeroIndice = aux[0];
                            System.out.println("O último arquivo enviado corresponde a mensagem #" + numeroIndice);

                            String[] sIndice = messageFromServer.split("\0");
                            indice = Integer.parseInt(sIndice[0]);
                        
                            msgId=indice+":"+sentence;
                            
                        
                            //Adiciona no ArrayList como: 0:file1K.txt:4001
                            mensagens.add(msgId);
                            messageFromServer="";

                        }

                        
                    //Entra nesse else se o usuario digita um arquivo que nao existe na pasta
                    }else{
                        System.out.println("Não existe nenhum arquivo com esse nome. Digite novamente, por favor!");
                    }
                    
                //Se entrar aqui é porque o usuário deseja receber um arquivo
                }if(op==2){
                    System.out.println("Digite o número de mensagem que deseja receber (somente o numero, SEM #): ");
                    numMsg=read.nextInt();
                    mensagem="#"+numMsg;
                    sendData=mensagem.getBytes();
                    System.out.println("Consultando o servidor para localização dessa mensagem...");

                    //envia mensagem para o servidor, avisando qual o número da mensagem que deseja
                    DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, port);
                    clientSocket.send(sendPacket);

                    //recebe mensagem do servidor, onde o servidor avisa a porta de entrada de quem contem o arquivo
                    DatagramPacket receivePacket2 = new DatagramPacket(receiveData,receiveData.length);
                    clientSocket.receive(receivePacket2);
                    String messageFromServer = new String(receivePacket2.getData());

                    //Se a mensagem do servidor vir como '-333', significa que nenhum usuário possui essa mensagem
                    if(messageFromServer.contains("-333")){
                        System.out.println("Nenhum usuário enviou essa mensagem ainda");

                    //Se a mensagem do servidor não vir como '-333', significa que ele mandou a porta de quem possui a mensagem
                    //Vai entrar nesse else para isso
                    }else{
                        //Lidando com a mensagem que o servidor
                        String[] requisitado = messageFromServer.split("\0");
                        ultimaPortaRecebida = requisitado[0];
                        int portaMsg = Integer.parseInt(requisitado[0]);
                        
                        String strSendData = Integer.toString(numMsg);

                        sendData = strSendData.getBytes();

                        //Enviou uma mensagem para a porta de entrada de quem possui a mensagem, bem como o numero da mensagem que deseja
                        DatagramPacket sendListen = new DatagramPacket(sendData, sendData.length, IPAddress, portaMsg);
                        clientSocket.send(sendListen);
                        
                        //Vai receber a mensagem com a resposta do nome do arquivo que corresponde aquela mensagem. Com isso, irá fazer uma copia da mensagem, porém, alterando o nome do arquivo;
                        DatagramPacket receivePacket = new DatagramPacket(new byte[1024], 1024);
                        clientSocket.receive(receivePacket);
                        String sentence = new String(receivePacket.getData());

                        //Precisa fazer isso porque a resposta do servidor vem com \0 no final. Quando pedir para abrir esse arquivo, se não tirar o \0, acaba dandoe erro
                        String[] sentenceAuxiliar = sentence.split("\0");
                        String aux = sentenceAuxiliar[0];

                        //Criando o nome do arquivo de saida
                        String nomeArquivo = port2+"-"+contadorRecebidos+"."+portaMsg;
                        
                        //Fazendo uma copia do arquivo. Para isso, primeiro lê tudo e depois escreve
                        String dadosLeitura = read(aux);
                        if(write(nomeArquivo, dadosLeitura)){
                            System.out.println("O arquivo recebido possui o nome " + nomeArquivo + ". Essa mensagem foi transmitida pela porta " + portaMsg);
                        }

                        //Aumenta o numero de mensagens recebidas por usuário
                        contadorRecebidos++;

                        //Adiciona no array list
                        mensagens.add(numMsg+":"+sentence);

                        //Envia index para o servidor pedindo para atualizar os indices, pois agora tambem tem essa mensagem
                        String atualizaIndice = "&:" + numMsg;
                        sendData = atualizaIndice.getBytes();
                        DatagramPacket sendPacket2 = new DatagramPacket(sendData, sendData.length, IPAddress, port);
                        clientSocket.send(sendPacket2);
                    }
                }
                if (op==-1){
                	System.exit(1);
                }
            }
            
        }


        //Thread que vai ficar escutando, isso é, se alguem requisitar uma mensagem, vai enviar por aqui
        public void run(){
         	
            try{
         	  int msgN;
         	  DatagramSocket listener = new DatagramSocket(port2);
         	  byte[] receiveData = new byte[1024];
              byte[] sendData = new byte[1024];
              String mensagem_solicitada;

                while(true){
                    //Recebendo uma mensagem
                    DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
                    listener.receive(receivePacket);
                    String sentence = new String(receivePacket.getData());
                    //System.out.println("Algum cliente está solicando a mensagem #" + sentence + " que voce possui");

                    InetAddress IPAddress = receivePacket.getAddress();

                    //Pegando qual porta de quem mandou a mensagem
                    int port = receivePacket.getPort();
                    
                    //Pegando o dado da mensagem, isso é, qual arquivo o outro usuário deseja
                    String[] msgCliente = sentence.split("\0");
                    //Transformando esse usuário em um numero inteiro
                    msgN = Integer.parseInt(msgCliente[0]);
                    
                    //Percorre o arrayList de mensagens
                    for (int i = 0; i < mensagens.size(); ++i) {
                        String mensagem = (String) mensagens.get(i);
                        
            			String[] index_mensagem = mensagem.split(":");
                        
                        //Retira o valor de index das mensagens que possui no ArrayList
            			int index = Integer.parseInt(index_mensagem[0]);
                        
                        //Compara para ver se, o index da mensagem que tem no array é o mesmo que o usuário deseja
            			if(index == msgN){
                            
                            //Vai mandar o nome do arquivo da mensagem
            				mensagem_solicitada = index_mensagem[1];
                            
            				//Preparando o envio
                    		sendData=mensagem_solicitada.getBytes();
        					DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, port);
                    		listener.send(sendPacket);
            				break;
            			}

    			    }
                }
            }catch (Exception ex) {
            	System.out.println("Esta porta ja esta em uso. Por favor, compile novamente e escolha outra porta");
            	System.exit(1);
              }   
        }


        //Função para escrever no arquivo
        public static boolean write(String n_arq, String mensagem) {
            BufferedWriter bw = null;
            FileWriter fw = null;
            try {
                fw = new FileWriter(n_arq);
                bw = new BufferedWriter(fw);
                bw.write(mensagem);
                return true;
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
            return false;
        }

        //Função para ler do arquivo
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

