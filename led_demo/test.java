public class test {


    public static void main(String[] args){
    

        String candys = "";
        System.out.println(candyCrush(candys));
        
    }
    public static String candyCrush(String candys){
        String str = removeFirstConsecutive(candys);
        while (!str.equals(candys)){
            candys = str;
            str = removeFirstConsecutive(str);
        }
        return str;
    }
    private static String removeFirstConsecutive(String s){
        if(s.equals("")) return "";
        char curr = s.charAt(0);
        int count = 1;
        for(int i = 1; i < s.length(); i++){
            if(s.charAt(i) == curr)
                count++;
            else{
                if(count >=3){
                    return s.substring(0, i-count)+s.substring(i,s.length());
                }else{
                    count = 1;
                    curr = s.charAt(i);
                }
                
            }
                
            
        }
        return s;
    }
}