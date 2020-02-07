import java.io.*;
import java.net.*;
import java.util.ArrayList;

class UDPServer {

    public static void main(String args[]) throws Exception {
        DatagramSocket serverSocket = new DatagramSocket(9920);
        byte[] receiveData = new byte[1024];
        byte[] sendData = new byte[1024];
        ArrayList<String> mensagens = new ArrayList<String>();
        int index, contador = 0;
        String contmensagem;
        String mensagem_solicitada;
        boolean existeMsg = false;

        while (true) {
            System.out.println("Servidor ativo. Esperando os clientes...");

            //Recebendo alguma mensagem
            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
            serverSocket.receive(receivePacket);
            String sentence = new String(receivePacket.getData());

            //Pegando o IP de quem mandou a mensagem
            InetAddress IPAddress = receivePacket.getAddress();
            //Pegando a porta de quem mandou a mensagem
            int port = receivePacket.getPort();

            //Se entrar aqui, significa que a mensagem estava solicitando um arquivo
            if((sentence.substring(0, 1)).equals("#")){
                System.out.println("O servidor sofreu uma solicitação de localização da mensagem");

                //Verificando o index da mensagem que o usuário deseja
                index = Integer.parseInt(sentence.substring(1, 2));
                
                //Percorrendo o array para ver qual porta possui a mensagem que o usuário solicitou
                for (int i = 0; i < mensagens.size(); ++i) {
                    String mensagem = (String) mensagens.get(i);
                    String[] index_mensagem = mensagem.split(":");

                    //Ids que contem no array
                    int ind = Integer.parseInt(index_mensagem[0]);

                    //Comparando para ver se o id que possui no array é o mesmo que o usuário deseja
                    if(index == ind){

                        //Vai mandar para o usuário a porta de quem deve solicitar
                        mensagem_solicitada = index_mensagem[1];
                        
                        //Preparando o envio da porta que possui a mensagem solicitada
                        sendData=mensagem_solicitada.getBytes();
                        DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, port);
                        serverSocket.send(sendPacket);
                        existeMsg =  true;
                        break;
                    }

                }
                
                //Se não exister a mensagem no array do servidor, significa que a mesma ainda não foi enviada. Para isso, envia uma mensagem
                //Como -333 para o cliente
                if (existeMsg==false){

                    //Quando manda -333 é porque não achou aquela mensagem
                    mensagem_solicitada = "-333";
                    
                    //Preparando envio para o cliente
                    sendData=mensagem_solicitada.getBytes();
                    DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, port);
                    serverSocket.send(sendPacket);    
                }


            //Se entrar aqui, significa que é uma atualização ao servidor. Ou seja, mais um cliente possui uma determinada mensagem
            }else if((sentence.substring(0, 1)).equals("&")){
                String[] armazenamento = sentence.split(":");
                String numMsg = armazenamento[1].substring(0,1);
                String dados = numMsg+":"+port;     
                System.out.println("O servidor sofreu uma atualizacao de seus indices");
                mensagens.add(dados);
                

            //Se entrar aqui, significa que alguem enviou uma nova mensagem e está notificando o servidor
            }else{
                String ip = IPAddress.toString();
                String[] sMensagemPorta = sentence.split("\0");
                String[] porta = sMensagemPorta[0].split(":");
                //Porta de quem recebeu
                int port2 = Integer.parseInt(porta[1]);
                mensagens.add(contador + ":" + port2);
                contmensagem=null;
                contmensagem = Integer.toString(contador);
                System.out.println("O cliente com a porta " + port2+ " acabou de publicar o arquivo #"+contmensagem);
                sendData=null;
                sendData = (contmensagem.getBytes());

                //Manda uma mensagem para o cliente indicando qual o numero de mensagem aquele arquivo corresponde
                DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, port);
                serverSocket.send(sendPacket);

                //Contador de mensagens que há na rede
                contador++;

            }
        }
    }
}