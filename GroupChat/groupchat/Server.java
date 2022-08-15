package groupchat;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public class Server {
    static List<clts> clt;
    static int k;
    public static void main(String[] args) {
        try{

        ServerSocket ss = new ServerSocket(8000);
        k = 1; 
        clt = new ArrayList<>();
        clt.add(null);
        Socket s;
        while(k!=0){
            s = ss.accept();
            
            clt.add(new clts(s,new DataInputStream(s.getInputStream()),new DataOutputStream(s.getOutputStream()), k));
            clt.get(k).start();    
            
            k++;
        }
        ss.close();
        }catch(IOException ie){System.out.println("IOEXCEPTION MAIN SERVER");}
    }
    static void informAll(String msg){
        
    }
}

class clts extends Thread{
    Socket s;
    DataInputStream din;
    DataOutputStream dout;
    int id;
    String name;

    clts(Socket s,DataInputStream din,DataOutputStream dout,int id){
        this.s = s;
        this.din = din;
        this.dout = dout;
        this.id = id;
    }
    public void run(){
        try{

        this.name = din.readUTF();
        String msg = this.name+" has joined the chat";
        writeMsg(msg);
        while(true){
            msg = this.name+" : "+din.readUTF();
            writeMsg(msg);
        }

        }catch(IOException io){System.out.println("IOEXCEPTION CLTS RUN");}
    }
    void writeMsg(String msg)throws IOException{
        for(int i =1;i<Server.k;i++){
            final int k = i;
            
            new Thread(){
                public void run(){
                    try{
                    Server.clt.get(k).dout.writeUTF(msg);
                    Server.clt.get(k).dout.flush();
                    System.out.println(k);
                    }catch(IOException ie){System.out.println("IOEXCEPTION SEND TO CLIENT");}
                }
            }.start();
        }
    }
}


