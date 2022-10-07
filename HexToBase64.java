public class HexToBase64{
    private static String convertString = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 
    private static String hexCode = "0123456789ABCDEF" ; 
    private static byte[] HexStringToByte(String HexString) { 
        if(HexString.length()%2==0) {
            byte b[] = new byte[HexString.length()/2]; 
            int i = 0; 
            while(i*2<HexString.length()){
                b[i] = (byte) (hexCode.indexOf(HexString.charAt(i*2))*16+hexCode.indexOf(HexString.charAt(i*2+1))); 
                i+=1;
            }
            return b;
        }
        return null;
        }
    private static String convertSub(String Hex) {
        byte[] hexb = HexStringToByte (Hex); 
        String s =""; 
        if(hexb!=null) { 
            s = convertString.charAt((hexb[2]&0xFF)%(1<<6))+s; 
            s = convertString.charAt((((hexb[1]<<2)+((hexb[2]>>6)& 0x03))& 0xFF)%(1<<6)) +s;
            s = convertString.charAt((((hexb[0]<<4)+((hexb[1] >>4)& 0x0F))& 0xFF)%(1<<6))+s; 
            System.out.println((hexb[0] & 0xFF));
            s = convertString.charAt((hexb[0]>>2 & 0xFF)%(1<<6))+s;
        }
        return s;
    }
    public static String convert(String Hex){
        String s = "";
        int len = Hex.length();
        while(len>=6){
            s+=convertSub(Hex.substring(Hex.length()-len, Hex.length()-len+6));
            len-=6;
        }
        if(len>=4){
            String s1=Hex.substring(Hex.length()-len, Hex.length()-len+4);
            byte[] b = HexStringToByte(s1);
            s+= convertString.charAt((b[0]>>2&0xFF)%(1<<6));
            s+= convertString.charAt((((b[0]<<4)&0x30) + ((b[1]>>4)&0x0F))%(1<<6));
            s+=convertString.charAt((b[1]<<2&0xFF)%(1<<6));
            s+="=";
        }
        else if(len>=2){
            String s1=Hex.substring(Hex.length()-len, Hex.length()-len+2);
            byte[] b = HexStringToByte(s1);
            s+=convertString.charAt(((b[0]>>2)&0x3F)%(1<<6));
            s+=convertString.charAt(((b[0]<<4)&0x30)%(1<<6));
            s+="==";
        }
        return s;
    }
}