package Behaviour;

import java.util.ArrayList;
import java.util.List;

public class Visitor
{
    /*--------------------------------------------------
    ■ Visitor 패턴
    연산 수행의 대상이 되는 객체 구조는 안정적(변동 X)이면서,
    연산의 종류와 수단이 자주 변경되는 구조에 사용되는 패턴

    ○
    패턴이 고정적이다.
    1) 연산 수행 대상 인터페이스는 Visitor 인터페이스를 accept 하는 함수를 선언하고,
       연산 수행 대상 Concrete 객체들은 각자 accept 함수를
       accept(Visitor visitor){ visitor.visit(this); }
       로 고정되게 구현한다.

    2) 연산 수행을 하는 Visitor 인터페이스는 연산 수행 대상 객체 인터페이스를 상속한 객체들을
       A, B라 할 때, A, B에 대하여 각각
       visit(A a); visit(B b); 를 선언하고,
       Visitor를 상속한 구현 객체들에서 선언한 함수들을 구현한다.
       (구현 객체들은 연산의 종류/수단 이 각각 다르기에, visit 의 내용이 각각 다르다)

    ○
    위 1), 2) 패턴은 고정적이면서, 연산 수행의 대상이 되는 객체가 추가될 때마다 저 작업들을
    반복해야 하기에, 연산 수행의 대상이 되는 객체 구조가 변동적일 경우 적합한 패턴이 아니다.

    하지만 안정적일 경우, 처음 위 패턴을 만들어 놓은 후에는, 이후 연산의 종류와 수단이 추가될 경우
    Visitor 인터페이스를 상속하여 새로 구현하면 되기 때문에, 효율적일 수 있다.

     -------------------------------------------------*/


    public static void main(String[] args)
    {
        /*-----------------------
         *         EX 1
         * --------------------- */
        Shape circle = new Circle(5);
        Shape rec = new Rectangle(4, 6);

        Visitor1 areaVisitor = new AreaVisitor();
        Visitor1 perimeterVisitor = new PerimeterVisitor();

        System.out.println("Calc Area : ");
        circle.accept(areaVisitor);
        rec.accept(areaVisitor);

        System.out.println("Calc perimeter : ");
        circle.accept(perimeterVisitor);
        rec.accept(perimeterVisitor);



        /*-----------------------
         *         EX 2
         * --------------------- */
        System.out.printf("\n\n\n");
        File file1 = new File("file1.txt", 100);
        File file2 = new File("file2.txt", 200);
        File file3 = new File("file3.txt", 300);

        Directory dir1 = new Directory("Folder1");
        dir1.addElement(file1);
        dir1.addElement(file2);

        Directory dir2 = new Directory("Folder2");
        dir2.addElement(file3);

        Directory rootDir = new Directory("Root");
        rootDir.addElement(dir1);
        rootDir.addElement(dir2);

        SizeCalculatorVisitor sizeVisitor = new SizeCalculatorVisitor();
        rootDir.accept(sizeVisitor);
        System.out.printf("Total size of files : %d\n\n", sizeVisitor.getTotalSize());

        FileSearchVisitor searchVisitor = new FileSearchVisitor("file3.txt");
        rootDir.accept(searchVisitor);
        File foundFile = searchVisitor.getFoundFile();
        if(foundFile != null)
        {
            System.out.printf("File Found : %s, Size : %d\n", foundFile.getName(), foundFile.getSize());
        }
        else
        {
            System.out.println("File not found");
        }
    }

    /*-----------------------
     *         EX 1
     * --------------------- */
    static interface Shape
    {
        void accept(Visitor1 visitor);
    }

    static class Circle implements Shape
    {
        private double radius;

        public Circle(double InRadius)
        {
            radius = InRadius;
        }

        public double getRadius()
        {
            return radius;
        }

        @Override
        public void accept(Visitor1 visitor)
        {
            visitor.visit(this);
        }
    }

    static class Rectangle implements Shape
    {
        private double width, height;

        public Rectangle(double InWidth, double InHeight)
        {
            width = InWidth;
            height = InHeight;
        }

        public double getWidth(){ return width;}
        public double getHeight(){ return height;}

        @Override
        public void accept(Visitor1 visitor)
        {
            visitor.visit(this);
        }
    }

    static interface Visitor1
    {
        void visit(Circle circle);
        void visit(Rectangle rectangle);
    }

    static class AreaVisitor implements Visitor1
    {
        @Override
        public void visit(Circle circle)
        {
            double area = circle.getRadius();
            area = area * area * Math.PI;
            System.out.printf("Circle area : %f\n", area);
        }

        @Override
        public void visit(Rectangle rectangle)
        {
            double area = rectangle.getWidth() * rectangle.getHeight();
            System.out.printf("Rec area : %f\n", area);
        }
    }

    static class PerimeterVisitor implements Visitor1
    {
        @Override
        public void visit(Circle circle)
        {
            double perimeter = 2 * Math.PI  * circle.getRadius();
            System.out.printf("Circle perimeter : %f\n", perimeter);
        }

        @Override
        public void visit(Rectangle rectangle)
        {
            double perimeter = 2 * (rectangle.getHeight() + rectangle.getWidth());
            System.out.printf("Rec perimeter : %f\n", perimeter);
        }
    }




    /*-----------------------
     *         EX 2
     * --------------------- */
    static interface FileSystemElement
    {
        void accept(Visitor2 visitor);
    }

    static class File implements FileSystemElement
    {
        private String name;
        private long size;

        public File(String InName, long InSize)
        {
            name = InName;
            size= InSize;
        }
        public String getName() { return name;}
        public long getSize() { return size;}

        @Override
        public void accept(Visitor2 visitor)
        {
            visitor.visit(this);
        }
    }

    static class Directory implements FileSystemElement
    {
        private String name;
        private List<FileSystemElement> elements;

        public Directory(String InName)
        {
            name = InName;
            elements = new ArrayList<>();
        }

        public String getName(){ return name;}

        public void addElement(FileSystemElement element)
        {
            elements.add(element);
        }

        public List<FileSystemElement> getElements() { return elements;}

        @Override
        public void accept(Visitor2 visitor)
        {
            visitor.visit(this);
        }
    }

    static interface Visitor2
    {
        void visit(File file);
        void visit(Directory directory);
    }

    static class SizeCalculatorVisitor implements Visitor2
    {
        private long totalSize = 0;

        @Override
        public void visit(File file)
        {
            totalSize += file.getSize();
        }

        @Override
        public void visit(Directory directory)
        {
            for(FileSystemElement element : directory.getElements())
            {
                element.accept(this);
            }
        }

        public long getTotalSize(){ return totalSize;}
    }

    static class FileSearchVisitor implements Visitor2
    {
        private String searchFileName;
        private File foundFile;

        public FileSearchVisitor(String InSearchFileName)
        {
            searchFileName = InSearchFileName;
        }

        @Override
        public void visit(File file)
        {
            if(file.getName().equals(searchFileName))
            {
                foundFile = file;
            }
        }

        @Override
        public void visit(Directory directory)
        {
            for(FileSystemElement element : directory.getElements())
            {
                element.accept(this);
            }
        }

        public File getFoundFile()
        {
            return foundFile;
        }
    }


}
