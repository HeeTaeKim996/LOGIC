package Structual;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class Facade
{
    /*
    ■ Facade 패턴
    다수의 서브 클래스들을 하나로 묶어 관리하는 총괄 클래스를 만들고, 유저는 총괄 클래스를 통해서만
    작업을 처리하는 구조

    드라이브 스루의 접수대와 같은 역할이다. 사용자는 드라이브 스루 매장 내부의 구조를 알 필요 없다.
    접수대에서 주문 및 받기만 하면 된다. 구체적인 내부 패턴은 드라이브 스루 직원들만 알면 된다.

    파사드 패턴은 다수의 서비 클래스들을 하나로 묶어 관리하는 파사드 클래스를 만들어 처리한다.
    유저는 서브 클래스들의 구조를 알 필요가 없고, 파사드 클래스의 호출 함수만 알면 된다
    */

    public static void main(String[] args)
    {
        /*------------------
                EX 1
         ------------------*/
        SmartHomeFacade smartHomeFacade = new SmartHomeFacade();

        smartHomeFacade.wakeUp();
        smartHomeFacade.leaveHome();



        /*------------------
                EX 2
         ------------------*/
        FileSystemFacade fs = new FileSystemFacade();

        // Write to file
        boolean writeSuccess = fs.writeFile(
                "test.txt", "Hello, Facade Pattern!"
        );
        System.out.println("File write success: " + writeSuccess);

        // Read from file
        String content = fs.readFile("test.txt");
        System.out.println("File content: " + content);

        // Delete file
        boolean deleteSuccess = fs.deleteFile("test.txt");
        System.out.println("File delete success: " + deleteSuccess);
    }


    /*-----------------------
    *         EX 1
    * --------------------- */
    public static class Thermostat
    {
        public void setTemperature(int temperature)
        {
            System.out.printf("온도를 [%d]degree 로 세팅합니다\n", temperature);
        }
    }

    public static class Lights
    {
        public void on()
        {
            System.out.printf("불을 켭니다\n");
        }

        public void off()
        {
            System.out.printf("불을 끕니다\n");
        }
    }

    public static class CoffeeMaker
    {
        public void brewCoffee()
        {
            System.out.printf("커피를 주조합니다\n");
        }
    }


    public static class SmartHomeFacade
    {
        private Thermostat thermostat;
        private Lights lights;
        private CoffeeMaker coffeeMaker;

        public SmartHomeFacade()
        {
            thermostat = new Thermostat();
            lights = new Lights();
            coffeeMaker = new CoffeeMaker();
        }

        public void wakeUp()
        {
            System.out.printf("--출근--\n");
            thermostat.setTemperature(22);
            lights.on();
            coffeeMaker.brewCoffee();
        }
        public void leaveHome()
        {
            System.out.printf("--퇴근--\n");
            thermostat.setTemperature(18);
            lights.off();
        }
    }










    /*-----------------------
     *         EX 2
     * --------------------- */
    static class FileReader {
        public String readFile(String filePath)
                throws IOException {
            return new String(Files.readAllBytes(Paths.get(filePath)));
        }
    }

    static class FileWriter {
        public void writeFile(String filePath, String content)
                throws IOException {
            Files.write(Paths.get(filePath), content.getBytes());
        }
    }

    static class FileDeleter {
        public void deleteFile(String filePath)
                throws IOException {
            Files.delete(Paths.get(filePath));
        }
    }


    static class FileSystemFacade {
        private FileReader fileReader;
        private FileWriter fileWriter;
        private FileDeleter fileDeleter;

        public FileSystemFacade() {
            this.fileReader = new FileReader();
            this.fileWriter = new FileWriter();
            this.fileDeleter = new FileDeleter();
        }

        public String readFile(String filePath) {
            try {
                return fileReader.readFile(filePath);
            } catch (IOException e) {
                System.err.println("Error reading file: " + e.getMessage());
                return null;
            }
        }

        public boolean writeFile(String filePath, String content) {
            try {
                fileWriter.writeFile(filePath, content);
                return true;
            } catch (IOException e) {
                System.err.println("Error writing file: " + e.getMessage());
                return false;
            }
        }

        public boolean deleteFile(String filePath) {
            try {
                fileDeleter.deleteFile(filePath);
                return true;
            } catch (IOException e) {
                System.err.println("Error deleting file: " + e.getMessage());
                return false;
            }
        }
    }
}

