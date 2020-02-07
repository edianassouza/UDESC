package com.wsudesc.app;
import com.dropbox.core.DbxException;
import com.dropbox.core.DbxRequestConfig;
import com.dropbox.core.v2.DbxClientV2;
import com.dropbox.core.v2.files.FileMetadata;
import com.dropbox.core.v2.files.ListFolderResult;
import com.dropbox.core.v2.files.Metadata;
import com.dropbox.core.v2.users.FullAccount;
import java.util.List;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.IOException;
import java.io.*;
import java.util.*;

public class Client implements Runnable{

	private static final String ACCESS_TOKEN = "DJafwq24y2AAAAAAAAADDKUv-6M85BNErlOg166hpYbZC3IYiXdxQBzaKYnDhuPT";
	static String nome = "";
	static int cont_enviados = 0;
    static int cont_recebidos = 0;
    static DbxRequestConfig config = new DbxRequestConfig("dropbox/java-tutorial", "en_US");
    static DbxClientV2 client = new DbxClientV2(config, ACCESS_TOKEN);
    private static ListFolderResult result;

	public static void main(String args[]) throws DbxException, IOException, Exception {
		
        File file;
        String n_arquivo = "";
        Scanner scan = new Scanner(System.in);

        FullAccount account = client.users().getCurrentAccount();
        System.out.println(account.getName().getDisplayName());

        //Get files and folder metadata from Dropbox root directory
        //ListFolderResult result = client.files().listFolder("");
	
        System.out.println("Digite o seu nome:");
        nome = scan.nextLine();
        String arquivo_entrou = "/tmp/"+nome+".entrou";
        FileWriter arquivoInicial= new FileWriter(arquivo_entrou, true);
        arquivoInicial.write("O usuário "+nome+" entrou no chat");
        arquivoInicial.close();
        try (InputStream in = new FileInputStream(arquivo_entrou)) {
             FileMetadata metadata = client.files().uploadBuilder("/SDI_Aluno8/F_Saida"+nome+"/mensagem"+nome+"Entrou.txt").uploadAndFinish(in);
        }catch(IOException ex){
            throw new RuntimeException(ex);
        }

        new Thread(new Client()).start();

		while(true){
            System.out.println("\nPara enviar uma mensagem digite o nome do arquivo:");
            n_arquivo = scan.nextLine();
			n_arquivo = "/tmp/"+n_arquivo;
            System.out.println(n_arquivo);
            file = new File(n_arquivo);
			if(file.exists()){
				try (InputStream in = new FileInputStream(n_arquivo)) {
            		FileMetadata metadata = client.files().uploadBuilder("/SDI_Aluno8/F_Saida"+nome+"/"+"mensagem"+Integer.toString(cont_enviados)+".txt").uploadAndFinish(in);
        			cont_enviados ++;
        		}	
			}
            else{
                System.out.println("Não existe arquivo com esse nome.");
            }
		}
	}

    //Thread que faz download das mensage
	public void run(){
        String caminho1, caminho2, nomeaux;
        try{
            Thread.sleep(5000);
        }
        catch (Exception e){}

		while(true){
            try{
                result = client.files().listFolder("/SDI_Aluno8/");
            }catch (Exception e){}
            
            //Percorre as pastas gerais
            for (Metadata metadata : result.getEntries()) {
                caminho1 = metadata.getPathLower();
                //Verifica a pasta de entrada
                if (caminho1.indexOf("/F_Entrada"+nome)!=-1){
                    for (Metadata metadata2 : result.getEntries()) {
                        caminho2 = metadata2.getPathLower();
                        //Verifica se o arquivo esta com a extensao: .nome (exemplo: .debora)
                        nomeaux = nome.toLowerCase();
                        if (caminho2.indexOf("."+nomeaux)!=-1){
                            try{
                                int index_inicio = caminho2.indexOf("F_Entrada");
                                index_inicio = index_inicio+12;
                                System.out.println(caminho2.substring(index_inicio, caminho2.length()));
                                OutputStream downloadFile = new FileOutputStream("/tmp/"+caminho2.substring(index_inicio, caminho2.length()));
                                try{
                                    //usar o ListFolder
                                    FileMetadata metadata3 = client.files().downloadBuilder(caminho2).download(downloadFile);
                                    System.out.println("Mensagem recebida. Salva em /tmp/"+caminho2.substring(index_inicio, caminho2.length()));
                                    //cont_recebidos++;
                                }catch (Exception e){                                }
                                finally{
                                    downloadFile.close();
                                }
                            }
                            catch (Exception e){}        
                        }   
                    }
                }
            }
	   }
    }
}