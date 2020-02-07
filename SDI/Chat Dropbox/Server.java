package com.wsudesc.app;
import com.dropbox.core.DbxException;
import com.dropbox.core.DbxRequestConfig;
import com.dropbox.core.v2.DbxClientV2;
import com.dropbox.core.v2.files.FileMetadata;
import com.dropbox.core.v2.files.ListFolderResult;
import com.dropbox.core.v2.files.Metadata;
import com.dropbox.core.v2.users.FullAccount;
import java.util.List;
import java.io.*;
import java.util.*;
import java.lang.*;

public class Server {    
    private static final String ACCESS_TOKEN = "DJafwq24y2AAAAAAAAADDKUv-6M85BNErlOg166hpYbZC3IYiXdxQBzaKYnDhuPT";
    private static ListFolderResult result;
    private static final DbxRequestConfig config = new DbxRequestConfig("dropbox/java-tutorial", "en_US");
    private static final DbxClientV2 client = new DbxClientV2(config, ACCESS_TOKEN);

    public static void main(String args[]) throws DbxException, IOException, Exception {
    
    	String nomeArquivo, caminho;
        
        // Get current account info
        FullAccount account = client.users().getCurrentAccount();
        System.out.println(account.getName().getDisplayName());

        // Get files and folder metadata from Dropbox root directory
        while (true) {

        	result = client.files().listFolder("/SDI_Aluno8/");

        	//Percorre as pastas gerais
            for (Metadata metadata : result.getEntries()) {
            	nomeArquivo = metadata.getPathLower();
                if(nomeArquivo.toLowerCase().indexOf("saida")!=-1)
                {
                    result = client.files().listFolder(nomeArquivo);

                    //Percore os arquivos das pastas
                    for (Metadata metadata1 : result.getEntries()) {
                    	caminho = metadata1.getPathLower();
                        System.out.println(caminho);
                        //mandar para todas as outras pasta
                        envia_entradas(caminho, nomeArquivo);
                    }
                }
                
            }

            result = client.files().listFolderContinue(result.getCursor());
        }

    }

    public static void envia_entradas(String caminho, String localInicial) throws Exception{
    	//Variaveis auxiliares
    	String auxiliar, teste, teste1, guardarAuxiliar;
    	String nomeArquivo2, nomeArquivo3;

        int index_incio = caminho.indexOf("mensagem");
        int index_fim = caminho.indexOf(".txt");

        //Isso é pro role de pegar o nome de quem esta enviando a mensagem
    	int index_nome = caminho.indexOf("f_saida");
    	index_nome = index_nome + 7;
        
        OutputStream downloadFile;

        //Primeiro o servidor faz o download e coloca em tmp
        try{
        	//Entra aqui se nao for a mensagem de entrou, pois precisa utilizar a variavel indexNome
        	if (caminho.indexOf("entrou")==-1){
        		downloadFile = new FileOutputStream("/tmp/"+caminho.substring(index_nome, index_incio-1)+"-"+caminho.substring(index_incio, index_fim)+".serv");
         		FileMetadata metadata2 = client.files().downloadBuilder(caminho).download(downloadFile);
            	System.out.println("Mensagem salva pelo servidor em /tmp/"+caminho.substring(index_nome, index_incio-1)+"-"+caminho.substring(index_incio, index_fim)+".serv");
            	downloadFile.close();	
            	//Quando é a mensagem de entrou, nao precisa utilizar, pois ela é salva no formado /sdi_aluno8/f_saidamaria/mensagemmariaentrou.txt
        	}else{
        		downloadFile = new FileOutputStream("/tmp/"+caminho.substring(index_incio, index_fim)+".serv");
         		FileMetadata metadata2 = client.files().downloadBuilder(caminho).download(downloadFile);
            	System.out.println("Mensagem salva pelo servidor em /tmp/"+caminho.substring(index_incio, index_fim)+".serv");
            	downloadFile.close();	
        	}
        	
        }catch (Exception e){
        	System.out.println(e);
        }

        //percorre as pastas para enviar
        result = client.files().listFolder("/SDI_Aluno8/");
        for (Metadata metadata3 : result.getEntries()) {
        	nomeArquivo2 = metadata3.getPathLower();
        	System.out.println("Pasta analisa " + nomeArquivo2);

        	//Verifica se nao vai enviar a mensagem pro usuário que cedeu a mensagem, nem se vai analisar uma pasta de entrada
        	if ((!(nomeArquivo2.equals(localInicial))) && ((nomeArquivo2.indexOf("saida"))!=-1)){
        		
        		//Pega o nome de quem vai receber para colocar na extensão
        		String aux = nomeArquivo2;
        		int nome = aux.indexOf("f_saida");
        		nome = nome +7;

        		//Roles para o nome da pasta ficar certinha no dropbox, pois, mesmo que nao se user lower case, o caminho fica tudo minusculo. Como o dropbox é case sensitive, tem que alterar tudo para o mesmo padrao (maior rolo isso aqui)
      			auxiliar = nomeArquivo2.replace("sdi", "SDI");
      			auxiliar = auxiliar.replace("aluno8", "Aluno8");
      			//Aqui cria a pasta F_Entrada
      			auxiliar = auxiliar.replace("f_saida", "F_Entrada");
      			guardarAuxiliar = auxiliar;
      			teste = auxiliar.substring(21, 22).toUpperCase().concat(auxiliar.substring(22));

      			//teste1 é a variavel que deixa ela certinha oficialmente
      			teste1 = guardarAuxiliar.substring(0, 21).concat(teste);

      			//Servidor faz o upload e coloca em F_Entrada
           		InputStream in;
            	try{
            		//Entra aqui se nao for a mensagem de entrou, pois precisa utilizar a variavel indexNome
            		if (caminho.indexOf("entrou")==-1){
  
               			in = new FileInputStream("/tmp/"+caminho.substring(index_nome, index_incio-1)+"-"+caminho.substring(index_incio, index_fim)+".serv");
                		//faz o upload do arquivo na pasta F_Entrada(teste1 = //SDI_Aluno8/F_EntradaNome/NomeDeQuemEnviou-mensagem0.nomeDeQuemVaiReceber)

                		System.out.println("Mensagem salva no caminho " + teste1+"/"+caminho.substring(index_nome, index_incio-1)+"-"+caminho.substring(index_incio, index_fim)+"."+aux.substring(nome, aux.length()));
                		FileMetadata fmetadata = client.files().uploadBuilder(teste1+"/"+caminho.substring(index_nome, index_incio-1)+"-"+caminho.substring(index_incio, index_fim)+"."+aux.substring(nome, aux.length())).uploadAndFinish(in);
                		//FileMetadata fmetadata = client.files().uploadBuilder(teste1+"/"+caminho.substring(index_incio, index_fim)+".txt").uploadAndFinish(in);
                	}else{
                		in = new FileInputStream("/tmp/"+caminho.substring(index_incio, index_fim)+".serv");
                		//faz o upload do arquivo na pasta F_Entrada
                		System.out.println("Mensagem salva no caminho " + teste1+"/"+caminho.substring(index_incio, index_fim)+".txt");
                		FileMetadata fmetadata = client.files().uploadBuilder(teste1+"/"+caminho.substring(index_incio, index_fim)+".txt").uploadAndFinish(in);
                	}
            	}catch (Exception e){
                   	System.out.println(e);
            	}   	
        	}else{
        		
        	}		
        }
    }
}
