package megalib;

import java.io.*;
import java.nio.Buffer;
import java.util.function.BiFunction;
import java.util.function.Function;
import java.util.function.Predicate;

public class Helper {

    public static String terminator;
    static {
        char[] terminatorChars = new char[256];
        for (int i = 0; i < terminatorChars.length; i++) {
           terminatorChars[i] = 'e';
        }
        terminator = new String(terminatorChars);
    }
	
    public static String bytesToStringUTF (byte[] bytes) throws IOException {
        ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(bytes);
        DataInputStream dataInputStream = new DataInputStream(byteArrayInputStream);
        return dataInputStream.readUTF();
    }

    public static byte[] stringUTFToBytes (String string) throws IOException {
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        DataOutputStream dataOutputStream = new DataOutputStream(byteArrayOutputStream);
        dataOutputStream.writeUTF(string);
        return byteArrayOutputStream.toByteArray();
    }

    public static int bytesToInt(byte[] bytes) throws IOException {
        ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(bytes);
        DataInputStream dataInputStream = new DataInputStream(byteArrayInputStream);
        return dataInputStream.readInt();
    }
    
    public static byte[] intToBytes(int integer) throws IOException {
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        DataOutputStream dataOutputStream = new DataOutputStream(byteArrayOutputStream);
        dataOutputStream.writeInt(integer);
        return byteArrayOutputStream.toByteArray();
    }

    private static boolean saveRemoteFileInternal(String localBasePath, DataInputStream inputStream, boolean acceptTerminator) throws IOException {
        try {
            String fileName = inputStream.readUTF();
            if (acceptTerminator && fileName.length() > 255)
                return false;
            System.out.println("ricevo: " + fileName);
            FileOutputStream fileOutPutStream = new FileOutputStream(localBasePath + "/" + fileName);
            long fileSize = inputStream.readLong();
            pipeStreams(fileSize, inputStream, fileOutPutStream);
            return true;
        } catch (EOFException e) {
            System.out.println("raggiunto EOF");
            return false;
        }
    }

    public static boolean saveRemoteFile(String localBasePath, DataInputStream inputStream) throws IOException {
        return saveRemoteFileInternal(localBasePath, inputStream, false);
    }

    /* this works only on linux, consigliato utilizzare content-lenght */
    public static boolean saveRemoteFileTerminator(String localBasePath, DataInputStream inputStream) throws IOException {
        return saveRemoteFileInternal(localBasePath, inputStream, true);
    }

    public static void sendFileRemote(String file, DataOutputStream outputStream) throws IOException {
        sendFileRemote(new File(file), outputStream);
    }

    public static void sendFileRemote(File file, DataOutputStream outputStream) throws IOException {
        FileInputStream fileInputStream = new FileInputStream(file.getAbsolutePath());
        outputStream.writeUTF(file.getName());
        outputStream.writeLong(file.length());
        pipeStreams(file.length(), fileInputStream, outputStream);
    }

    public static void sendFileRemoteTerminator(DataOutputStream outputStream) throws IOException {
        outputStream.writeUTF(terminator);
    }

    public static void pipeStreams(long filedim, InputStream src, OutputStream dest) throws IOException {
        try {
            for (int i = 0; i < filedim; i++)
                dest.write(src.read());
            dest.flush();
        } catch (EOFException e) {
            e.printStackTrace();
        }
    }

    public static String readTillTrue(BufferedReader reader, String message, Predicate<String> function) throws IOException {
        String ret;
        do {
            System.out.println(message);
            ret = reader.readLine();
        } while (!function.test(ret));
        return ret;
    }

}