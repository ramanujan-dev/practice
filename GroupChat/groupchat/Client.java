package groupchat;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.Scanner;

public class Client {
    public static void main(String[] args) {
        try{
        Scanner sc = new Scanner(System.in);
        Socket s = new Socket("0.tcp.in.ngrok.io",13680);
        String name = sc.nextLine();

        DataOutputStream dout = new DataOutputStream(s.getOutputStream());
        DataInputStream din = new DataInputStream(s.getInputStream());

        dout.writeUTF(name);
        
        new Thread(){
            public void run(){
                try{
                while(true){
                    System.out.println(din.readUTF());
                }
                }catch(IOException io){System.out.println("IOEXCEPTION READ THREAD CLIENT");}   
            }
        }.start();
        String msg;
        int k = 1;
        while(k!=0){
            System.out.print(name+" : ");
            msg = sc.nextLine();
            dout.writeUTF(msg);
            dout.flush();
        }

        sc.close();
        s.close();
        }catch(IOException io){System.out.println("IOEXCEPTION MAIN CLIENT");}
    }
}
